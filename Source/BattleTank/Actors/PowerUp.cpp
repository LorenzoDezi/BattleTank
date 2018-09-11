// Fill out your copyright notice in the Description page of Project Settings.

#include "PowerUp.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Machine.h"
#include "Engine/World.h"


// Sets default values
APowerUp::APowerUp()
{
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APowerUp::BeginPlay()
{
	Super::BeginPlay();
	UStaticMeshComponent* mesh = FindComponentByClass<UStaticMeshComponent>();
	if(mesh)
		mesh->OnComponentBeginOverlap.AddDynamic(this, &APowerUp::OnOverlap);
}

// Called every frame
void APowerUp::Tick(float DeltaTime)
{
	FRotator rotation = this->GetActorRotation();
	this->SetActorRotation(FRotator(
		rotation.Pitch,
		rotation.Yaw + DeltaTime * RotationDegreePerSecond,
		rotation.Roll 
	));
	if (!enabled && !GetEnabledPercent()) {
		enabled = true;
		SetMaterial(enabledMaterial);
	}
	Super::Tick(DeltaTime);
}

void APowerUp::OnOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, 
	bool bFromSweep, 
	const FHitResult & SweepResult)
{
	if (!enabled) return;
	if (!OtherActor->ActorHasTag(FName("Enemy")) && OtherActor->IsA<AMachine>()) {
		
		auto machine = Cast<AMachine>(OtherActor);
		PowerUp(machine);
		LastTimeHit = GetWorld()->GetTimeSeconds();
		enabled = false;
		SetMaterial(disabledMaterial);
		if(SoundWhenPicked)
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundWhenPicked, GetActorLocation());
	}
	
}

void APowerUp::PowerUp(AMachine * machine)
{
	return;
}

float APowerUp::GetEnabledPercent()
{
	float result =  (float) ((LastTimeHit + TimeToEnable) - GetWorld()->GetTimeSeconds()) / (float) TimeToEnable;
	if (result < 0.f) result = 0.f;
	return result;
}

bool APowerUp::IsEnabled()
{
	return enabled;
}

void APowerUp::SetMaterial(UMaterialInterface * material)
{
	UStaticMeshComponent* mainMesh = FindComponentByClass<UStaticMeshComponent>();
	if (mainMesh && material)
		mainMesh->SetMaterial(0, material);
	TArray<USceneComponent*> meshes = TArray<USceneComponent*>();
	mainMesh->GetChildrenComponents(true, meshes);
	for (auto mesh : meshes) {
		if (mesh->IsA<UStaticMeshComponent>()) {
			auto meshCasted = Cast<UStaticMeshComponent>(mesh);
			if (material)
				meshCasted->SetMaterial(0, material);
		}
	}
}

