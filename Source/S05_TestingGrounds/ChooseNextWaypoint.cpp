// Fill out your copyright notice in the Description page of Project Settings.

#include "ChooseNextWaypoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "PatrollingGuard.h"

EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    // Get Patrol Points
    AAIController* Controller = OwnerComp.GetAIOwner();
    APawn* Pawn = Controller->GetPawn();
    APatrollingGuard* Guard = Cast<APatrollingGuard>(Pawn);
    TArray<AActor*> PatrolPoints = Guard->PatrolPointsCpp;
    
    // Set Next Waypoint
    UBlackboardComponent* Component = OwnerComp.GetBlackboardComponent();
    int Index = Component->GetValueAsInt(IndexKey.SelectedKeyName);
    Component->SetValueAsObject(WaypointKey.SelectedKeyName, PatrolPoints[Index]);
    
    // Cycle Index
    int NextIndex = (Index + 1) % PatrolPoints.Num();
    Component->SetValueAsInt(IndexKey.SelectedKeyName, NextIndex);
    
    return EBTNodeResult::Succeeded;
}
