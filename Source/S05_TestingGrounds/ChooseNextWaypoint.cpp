// Fill out your copyright notice in the Description page of Project Settings.

#include "ChooseNextWaypoint.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    UBlackboardComponent* Component = OwnerComp.GetBlackboardComponent();
    int Index = Component->GetValueAsInt(IndexKey.SelectedKeyName);
    
    UE_LOG(LogTemp, Warning, TEXT("Waypoint Index: %i"), Index);
    return EBTNodeResult::Succeeded;
}
