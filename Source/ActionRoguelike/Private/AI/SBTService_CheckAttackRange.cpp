// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_CheckAttackRange.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/SAIController.h"

//检查AI与目标之间的距离，判断目标是否在攻击范围内，并将结果存储在行为树的黑板中
void USBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    // 调用父类的TickNode函数，确保父类的逻辑也会执行
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    // 获取黑板组件，确保其存在
    UBlackboardComponent* BlackBoardComp = OwnerComp.GetBlackboardComponent();
    if (ensure(BlackBoardComp))
    {
        // 从黑板中获取目标Actor
        AActor* TargetActor = Cast<AActor>(BlackBoardComp->GetValueAsObject("TargetActor"));
        if (TargetActor)
        {
            // 获取AI控制器，确保其存在
            AAIController* MyController = OwnerComp.GetAIOwner();
            if (ensure(MyController))
            {
                // 获取AI控制器所控制的Pawn，确保其存在
                APawn* AIPawn = MyController->GetPawn();
                if (ensure(AIPawn))
                {
                    // 计算AI与目标Actor之间的距离
                    float DistanceTo = FVector::Distance(TargetActor->GetActorLocation(), AIPawn->GetActorLocation());
                    // 判断距离是否在1000.f以内（自定义攻击范围）
                    bool bWithinRange = DistanceTo < 1000.f;

                    bool bHasLineSite = false;
                    if (ensure(bWithinRange))
                    {
                        //LineOfSightTo函数的原理是射线检测，他要求阻挡的对象必须是ECC_Visiable类型的。如果我们把墙壁设置成其他类型，射线检测会直接穿墙而过
                        bHasLineSite = MyController->LineOfSightTo(TargetActor);
                    }
                    // 将攻击范围的布尔值存储在黑板中
                    BlackBoardComp->SetValueAsBool(AttackRangeKey.SelectedKeyName, bWithinRange && bHasLineSite);
                }
            }
        }
    }
}
