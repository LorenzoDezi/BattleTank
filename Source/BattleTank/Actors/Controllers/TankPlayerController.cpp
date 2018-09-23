#include "Actors/Controllers/TankPlayerController.h"
#include "Actors/Machine.h"
#include "BattleTankGameModeBase.h"
#include "Components/MachineAimingComponent.h"
#include "Components/TankMovementComponent.h"
#include "Engine/World.h"


void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();
	auto possessedTank = Cast<AMachine>(GetPawn());
	if (!possessedTank) return;
	possessedTank->SetMaxHealth(MaxHealth);
	auto aimingComponent = possessedTank->FindComponentByClass<UMachineAimingComponent>();
	auto movComponent = possessedTank->FindComponentByClass<UTankMovementComponent>();
	if (!aimingComponent || !movComponent) return;
	aimingComponent->SetTimeToReload(TimeToReloadInSeconds);
	FoundComponents.Broadcast(aimingComponent, movComponent);
	aimingComponent->SetMaxAmmo(MaxAmmo);
	EndedSetup();
}

void ATankPlayerController::Tick(float Deltaseconds) {
	Super::Tick(Deltaseconds);
	AimAtCrosshair();
}

void ATankPlayerController::AimAtCrosshair() {

	FVector HitLocation;
	auto ControlledTank = GetPawn();
	if (!ControlledTank) return;
	auto AimingComponent = ControlledTank->FindComponentByClass<UMachineAimingComponent>();
	if (!AimingComponent)
		return; 
	if (GetCrosshairLocation(HitLocation)) {
		AimingComponent->AimAt(HitLocation);
	}

}

bool ATankPlayerController::GetCrosshairLocation(FVector & OutHitLocation) const
{
	int32 ViewportSizeX, ViewportSizeY;
	GetViewportSize(ViewportSizeX, ViewportSizeY);
	FVector ViewportWorldLocation, ViewportWorldDirection;
	if (DeprojectScreenPositionToWorld(
		ViewportSizeX * CrosshairRelativeLocationX, 
		ViewportSizeY * CrosshairRelativeLocationY, 
		ViewportWorldLocation, 
		ViewportWorldDirection)) {
		FHitResult hit;
		if (GetWorld()->LineTraceSingleByChannel(
			OUT hit, ViewportWorldLocation, ViewportWorldLocation + (ViewportWorldDirection * CrosshairMaxRange),
			ECollisionChannel::ECC_Visibility,
			FCollisionQueryParams(FName(TEXT("")), false, GetOwner())
		)) {
			OutHitLocation = hit.Location;
			return true;
		}
	}
	OutHitLocation = FVector(0);
	return false;
}

void ATankPlayerController::SetPawn(APawn * InPawn)
{
	Super::SetPawn(InPawn);
	if (!InPawn) return;
	auto possessedTank = Cast<AMachine>(InPawn);
	if (!ensure(possessedTank)) return;
	possessedTank->OnDeathDelegate.AddUniqueDynamic(this, &ATankPlayerController::OnTankDeath);

}

void ATankPlayerController::OnTankDeath()
{
	StartSpectatingOnly();
	auto gameMode = GetWorld()->GetAuthGameMode();
	if (gameMode && gameMode->IsA<ABattleTankGameModeBase>()) {
		Cast<ABattleTankGameModeBase>(gameMode)->PlayerDefeated();
		UE_LOG(LogTemp, Warning, TEXT("Player defeated"));
	}
}
