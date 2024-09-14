// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTask_RangeAttack.h"
#include "GameFramework/Character.h"
#include "AI/SAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type USBTTask_RangeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* MyController = OwnerComp.GetAIOwner();  // 获取AI控制器
	if (ensure(MyController))  // 检查AI控制器是否有效
	{
		ACharacter* MyPawn = Cast<ACharacter>(MyController->GetPawn());  // 获取控制的Pawn并转换为角色
		if (MyPawn == nullptr)  // 如果Pawn为空，任务失败
		{
			return EBTNodeResult::Failed;  // 返回任务失败
		}

		AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));  // 从黑板中获取目标Actor
		if (TargetActor == nullptr)  // 如果目标Actor为空，任务失败
		{
			return EBTNodeResult::Failed;  // 返回任务失败
		}

		FVector MuzzleLocation = MyPawn->GetMesh()->GetSocketLocation("Muzzle_01");  // 获取角色的武器枪口位置
		// 方向向量=目标位置 - 当前位置
		FVector Direction = TargetActor->GetActorLocation() - MuzzleLocation;  // 计算从枪口到目标的方向向量
		FRotator MuzzleRotation = Direction.Rotation();  // 计算该方向的旋转角

		FActorSpawnParameters params;  // 定义生成Actor的参数
		params.Instigator = MyPawn;  // 设置当前生成的投射物的始作俑者为MyPawn
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;  // 设置碰撞处理方式为始终生成

		ensure(ProjectileClass);  // 确保ProjectileClass有效
		AActor* NewProj = GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, MuzzleRotation);  // 在枪口位置生成投射物
		return NewProj ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;  // 如果生成投射物成功则任务成功，否则失败
	}

	return EBTNodeResult::Failed;  // 如果控制器无效，任务失败
}

