// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTask_RangeAttack.h"
#include "GameFramework/Character.h"
#include "AI/SAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type USBTTask_RangeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* MyController = OwnerComp.GetAIOwner();  // ��ȡAI������
	if (ensure(MyController))  // ���AI�������Ƿ���Ч
	{
		ACharacter* MyPawn = Cast<ACharacter>(MyController->GetPawn());  // ��ȡ���Ƶ�Pawn��ת��Ϊ��ɫ
		if (MyPawn == nullptr)  // ���PawnΪ�գ�����ʧ��
		{
			return EBTNodeResult::Failed;  // ��������ʧ��
		}

		AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));  // �Ӻڰ��л�ȡĿ��Actor
		if (TargetActor == nullptr)  // ���Ŀ��ActorΪ�գ�����ʧ��
		{
			return EBTNodeResult::Failed;  // ��������ʧ��
		}

		FVector MuzzleLocation = MyPawn->GetMesh()->GetSocketLocation("Muzzle_01");  // ��ȡ��ɫ������ǹ��λ��
		// ��������=Ŀ��λ�� - ��ǰλ��
		FVector Direction = TargetActor->GetActorLocation() - MuzzleLocation;  // �����ǹ�ڵ�Ŀ��ķ�������
		FRotator MuzzleRotation = Direction.Rotation();  // ����÷������ת��

		FActorSpawnParameters params;  // ��������Actor�Ĳ���
		params.Instigator = MyPawn;  // ���õ�ǰ���ɵ�Ͷ�����ʼ��ٸ��ΪMyPawn
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;  // ������ײ����ʽΪʼ������

		ensure(ProjectileClass);  // ȷ��ProjectileClass��Ч
		AActor* NewProj = GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, MuzzleRotation);  // ��ǹ��λ������Ͷ����
		return NewProj ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;  // �������Ͷ����ɹ�������ɹ�������ʧ��
	}

	return EBTNodeResult::Failed;  // �����������Ч������ʧ��
}

