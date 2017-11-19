// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

USTRUCT()
struct FSpawnPosition
{
	GENERATED_USTRUCT_BODY()

	FVector Location;
	float Rotation;
	float Scale;
};

UCLASS()
class S05_TESTINGGROUNDS_API ATile : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY()
	class UActorPool* Pool;
	
	class AActor* NavMeshBoundsVolume;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Navigation")
	FVector NavigationBoundsOffset;

	UPROPERTY(EditDefaultsOnly, Category="Spawning")
	FVector MinimumExtent;

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	FVector MaximumExtent;

public:	
	// Sets default values for this actor's properties
	ATile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category="Spawning")
	void PlaceActors(TSubclassOf<AActor> ToSpawn, int32 MinSpawn = 1, int32 MaxSpawn = 1, float Radius = 500.0f, float MinScale = 1.0f, float MaxScale = 1.0f);

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void PlaceAIPawns(TSubclassOf<APawn> ToSpawn, int32 MinSpawn = 1, int32 MaxSpawn = 1, float Radius = 500.0f);

	UFUNCTION(BlueprintCallable, Category="Pool")
	void SetPool(class UActorPool* InPool);

private:
	bool CanSpawnAtLocation(FVector Location, float Radius);

	bool FindEmptyLocation(FVector& OutLocation, float Radius);

	template<typename T>
	void RandomlyPlaceActors(TSubclassOf<T> ToSpawn, int32 MinSpawn = 1, int32 MaxSpawn = 1, float Radius = 500.0f, float MinScale = 1.0f, float MaxScale = 1.0f);

	void PlaceActor(TSubclassOf<AActor> ToSpawn, FSpawnPosition& SpawnPosition);

	void PlaceActor(TSubclassOf<APawn> ToSpawn, FSpawnPosition& SpawnPosition);
	
	void PositionNavMeshBoundsVolume();

};
