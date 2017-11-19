// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"
#include "ActorPool.h"
#include "AI/Navigation/NavigationSystem.h"
#include "Engine/World.h"

// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set default navigation bounds offset.
	NavigationBoundsOffset = FVector(2000.0f, 0.0f, 0.0f);

	// Set default spawning extents.
	MinimumExtent = FVector(0.0f, -2000.0f, 0.0f);
	MaximumExtent = FVector(4000.0f, 2000.0f, 0.0f);
}

void ATile::SetPool(UActorPool* InPool)
{
	UE_LOG(LogTemp, Warning, TEXT("Setting Pool %s"), *InPool->GetName());
	Pool = InPool;
	PositionNavMeshBoundsVolume();
}

void ATile::PositionNavMeshBoundsVolume()
{
	NavMeshBoundsVolume = Pool->Checkout();

	if (NavMeshBoundsVolume == NULL)
	{
		UE_LOG(LogTemp, Error, TEXT("[%s] Not Enough Actors In Pool."), *GetName());
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("[%s] Checked Out: {%s}"), *GetName(), *NavMeshBoundsVolume->GetName());
	NavMeshBoundsVolume->SetActorLocation(GetActorLocation() + NavigationBoundsOffset);
	GetWorld()->GetNavigationSystem()->Build();
}

void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, int32 MinSpawn, int32 MaxSpawn, float Radius, float MinScale, float MaxScale)
{
	TArray<FSpawnPosition> SpawnPositions = RandomSpawnPositions(MinSpawn, MaxSpawn, Radius, MinScale, MaxScale);

	for (FSpawnPosition& SpawnPosition : SpawnPositions)
	{
		PlaceActor(ToSpawn, SpawnPosition);
	}
}

void ATile::PlaceAIPawns(TSubclassOf<APawn> ToSpawn, int32 MinSpawn, int32 MaxSpawn, float Radius)
{
	TArray<FSpawnPosition> SpawnPositions = RandomSpawnPositions(MinSpawn, MaxSpawn, Radius, 1.0f, 1.0f);

	for (FSpawnPosition& SpawnPosition : SpawnPositions)
	{
		PlaceAIPawn(ToSpawn, SpawnPosition);
	}
}

bool ATile::FindEmptyLocation(FVector& OutLocation, float Radius)
{
	FBox Bounds(MinimumExtent, MaximumExtent);

	const int32 MAX_ATTEMPTS = 100;
	for (size_t i = 0; i < MAX_ATTEMPTS; i++)
	{
		FVector CandidatePoint = FMath::RandPointInBox(Bounds);
		if (CanSpawnAtLocation(CandidatePoint, Radius))
		{
			OutLocation = CandidatePoint;
			return true;
		}
	}

	return false;
}

TArray<FSpawnPosition> ATile::RandomSpawnPositions(int32 MinSpawn, int32 MaxSpawn, float Radius, float MinScale, float MaxScale)
{
	int32 NumberToSpawn = FMath::RandRange(MinSpawn, MaxSpawn);
	TArray<FSpawnPosition> SpawnPositions;

	for (size_t i = 0; i < NumberToSpawn; i++)
	{
		FSpawnPosition SpawnPosition;

		// Randomize Rotation and Scale
		SpawnPosition.Rotation = FMath::RandRange(-180.0f, 180.0f);
		SpawnPosition.Scale = FMath::RandRange(MinScale, MaxScale);

		if (FindEmptyLocation(SpawnPosition.Location, Radius * SpawnPosition.Scale))
		{
			SpawnPositions.Add(SpawnPosition);
		}
	}

	return SpawnPositions;
}

void ATile::PlaceActor(TSubclassOf<AActor> ToSpawn, FSpawnPosition& SpawnPosition)
{
	AActor* Spawned = GetWorld()->SpawnActor<AActor>(ToSpawn);
	Spawned->SetActorRelativeLocation(SpawnPosition.Location);
	Spawned->SetActorRotation(FRotator(0.0f, SpawnPosition.Rotation, 0.0f));
	Spawned->SetActorScale3D(FVector(SpawnPosition.Scale));
	Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
}

void ATile::PlaceAIPawn(TSubclassOf<APawn> ToSpawn, FSpawnPosition& SpawnPosition)
{
	APawn* Pawn = GetWorld()->SpawnActor<APawn>(ToSpawn);
	Pawn->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	Pawn->SetActorRelativeLocation(SpawnPosition.Location);
	Pawn->SetActorRotation(FRotator(0.0f, SpawnPosition.Rotation, 0.0f));
	Pawn->SpawnDefaultController();
	Pawn->Tags.Add(TEXT("Enemy"));
}

bool ATile::CanSpawnAtLocation(FVector Location, float Radius)
{
	FHitResult HitResult;
	FVector GlobalLocation = ActorToWorld().TransformPosition(Location);

	bool HasHit = GetWorld()->SweepSingleByChannel(
		HitResult, 
		GlobalLocation,
		GlobalLocation,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(Radius));

	return !HasHit;
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();

}

void ATile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Pool->Return(NavMeshBoundsVolume);
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
