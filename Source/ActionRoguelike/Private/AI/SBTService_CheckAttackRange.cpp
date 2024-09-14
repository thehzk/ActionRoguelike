// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_CheckAttackRange.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/SAIController.h"

//���AI��Ŀ��֮��ľ��룬�ж�Ŀ���Ƿ��ڹ�����Χ�ڣ���������洢����Ϊ���ĺڰ���
void USBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    // ���ø����TickNode������ȷ��������߼�Ҳ��ִ��
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    // ��ȡ�ڰ������ȷ�������
    UBlackboardComponent* BlackBoardComp = OwnerComp.GetBlackboardComponent();
    if (ensure(BlackBoardComp))
    {
        // �Ӻڰ��л�ȡĿ��Actor
        AActor* TargetActor = Cast<AActor>(BlackBoardComp->GetValueAsObject("TargetActor"));
        if (TargetActor)
        {
            // ��ȡAI��������ȷ�������
            AAIController* MyController = OwnerComp.GetAIOwner();
            if (ensure(MyController))
            {
                // ��ȡAI�����������Ƶ�Pawn��ȷ�������
                APawn* AIPawn = MyController->GetPawn();
                if (ensure(AIPawn))
                {
                    // ����AI��Ŀ��Actor֮��ľ���
                    float DistanceTo = FVector::Distance(TargetActor->GetActorLocation(), AIPawn->GetActorLocation());
                    // �жϾ����Ƿ���1000.f���ڣ��Զ��幥����Χ��
                    bool bWithinRange = DistanceTo < 1000.f;

                    bool bHasLineSite = false;
                    if (ensure(bWithinRange))
                    {
                        //LineOfSightTo������ԭ�������߼�⣬��Ҫ���赲�Ķ��������ECC_Visiable���͵ġ�������ǰ�ǽ�����ó��������ͣ����߼���ֱ�Ӵ�ǽ����
                        bHasLineSite = MyController->LineOfSightTo(TargetActor);
                    }
                    // ��������Χ�Ĳ���ֵ�洢�ںڰ���
                    BlackBoardComp->SetValueAsBool(AttackRangeKey.SelectedKeyName, bWithinRange && bHasLineSite);
                }
            }
        }
    }
}
