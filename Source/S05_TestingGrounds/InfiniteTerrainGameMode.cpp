// Fill out your copyright notice in the Description page of Project Settings.

#include "InfiniteTerrainGameMode.h"
#include "ActorPool.h"

#include "AI/Navigation/NavMeshBoundsVolume.h"
#include "EngineUtils.h"

AInfiniteTerrainGameMode::AInfiniteTerrainGameMode()
{
	NavMeshBoundsVolumePool = CreateDefaultSubobject<UActorPool>(TEXT("NavMeshBoundsVolumePool"));
}

void AInfiniteTerrainGameMode::AddToPool(ANavMeshBoundsVolume* VolumeToAdd)
{
	UE_LOG(LogTemp, Warning, TEXT("Found Volume: %s"), *VolumeToAdd->GetName());
}

void AInfiniteTerrainGameMode::PopulateBoundsVolumePool()
{
	TActorRange<ANavMeshBoundsVolume> VolumeIterator(GetWorld());
	for (ANavMeshBoundsVolume* Volume : VolumeIterator)
	{
		AddToPool(Volume);
	}
}