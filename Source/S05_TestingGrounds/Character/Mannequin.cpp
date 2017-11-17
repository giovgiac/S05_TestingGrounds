// Fill out your copyright notice in the Description page of Project Settings.

#include "Mannequin.h"
#include "../Weapons/Gun.h"

#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
AMannequin::AMannequin()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(RootComponent);
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetCastShadow(false);
	Mesh1P->bCastDynamicShadow = false;
}

// Called when the game starts or when spawned
void AMannequin::BeginPlay()
{
	Super::BeginPlay();

	if (GunBlueprint == NULL)
	{
		UE_LOG(LogTemp, Warning, TEXT("Gun Blueprint Missing."));
		return;
	}
	
	// Spawn and attach gun
	Gun = GetWorld()->SpawnActor<AGun>(GunBlueprint);
	Gun->AnimInstance = Mesh1P->GetAnimInstance();
	Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
}

// Called every frame
void AMannequin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMannequin::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMannequin::Fire()
{
	if (Gun != NULL)
	{
		Gun->OnFire();
	}
}
