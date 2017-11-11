// Fill out your copyright notice in the Description page of Project Settings.

#include "ChooseNextWaypoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "PatrolRoute.h"

EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    // Get Patrol Route
    APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();
    UPatrolRoute* PatrolRoute = Pawn->FindComponentByClass<UPatrolRoute>();
    if (!ensure(PatrolRoute))
        return EBTNodeResult::Failed;
    
    // Get Patrol Points
    auto PatrolPoints = PatrolRoute->GetPatrolPoints();
    if (PatrolPoints.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("A guard is missing patrol points"));
        return EBTNodeResult::Failed;
    }
    
    // Set Next Waypoint
    UBlackboardComponent* Component = OwnerComp.GetBlackboardComponent();
    int Index = Component->GetValueAsInt(IndexKey.SelectedKeyName);
    Component->SetValueAsObject(WaypointKey.SelectedKeyName, PatrolPoints[Index]);
    
    // Cycle Index
    int NextIndex = (Index + 1) % PatrolPoints.Num();
    Component->SetValueAsInt(IndexKey.SelectedKeyName, NextIndex);
    
    return EBTNodeResult::Succeeded;
}
