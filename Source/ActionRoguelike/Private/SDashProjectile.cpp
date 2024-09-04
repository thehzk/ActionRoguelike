// Fill out your copyright notice in the Description page of Project Settings.


#include "SDashProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"



ASDashProjectile::ASDashProjectile()
{
	TeleportDelay = 0.2f;
	DetonateDelay = 0.2f;

	MoveComp->InitialSpeed = 6000.0f;
}


void ASDashProjectile::BeginPlay()
{
	Super::BeginPlay();
	//����ըʱ��󣬴���Explode
	GetWorldTimerManager().SetTimer(TimerHandle_DelayDetonate, this, &ASDashProjectile::Explode, DetonateDelay);
}

void ASDashProjectile::Explode_Implementation()
{
	// �����������ϰ�����ǰ��ը����������Ҫ���Timer��ֹ����������
	GetWorldTimerManager().ClearTimer(TimerHandle_DelayDetonate);
	// ��ը��Ч
	UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());

	// ���ӱ�ը��ر�ϵͳ��ֹͣ�ƶ���ֹͣ��ײ
	EffectComp->DeactivateSystem();
	MoveComp->StopMovementImmediately();
	SetActorEnableCollision(false);

	// ������ʱ��󣬴�������
	FTimerHandle TimerHandle_DelayedTeleport;
	GetWorldTimerManager().SetTimer(TimerHandle_DelayedTeleport, this, &ASDashProjectile::TeleportInstigator, TeleportDelay);

	// ������base�е�Explode�е�destroy������Ϊ����ʱ���ʱ����Ҫһ��ʱ��
	//Super::Explode_Implementation(); 
}

void ASDashProjectile::TeleportInstigator()
{
	AActor* ActorToTeleport = GetInstigator();
	if (ensure(ActorToTeleport))
	{
		// ����ʱ���ֽ�ɫԭʼRotation
		ActorToTeleport->TeleportTo(GetActorLocation(), ActorToTeleport->GetActorRotation(), false, false);
	}
	// ֮ǰû�е��ã���������
	Destroy();
}
