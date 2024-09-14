// Fill out your copyright notice in the Description page of Project Settings.


#include "SIteractionComponent.h"
#include "SGameplayInterface.h"
#include <DrawDebugHelpers.h>


// Sets default values for this component's properties
USIteractionComponent::USIteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USIteractionComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...
	
}


// Called every frame
void USIteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void USIteractionComponent::PrimaryInteract()
{
	// ������ײ��ѯ���������ڼ�⶯̬���壨���籦�䣩
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);//���߼�⽫ֻ����붯̬�������ײ

	// ��ȡ���ô˺����Ľ�ɫ��ӵ�и������Actor��
	AActor* MyOwner = GetOwner();

	// ���ڴ洢��ɫ���۾�λ�ã�λ�úͷ���
	FVector EyeLocation;  // ��ɫ�۾���λ��
	FRotator EyeRotation; // ��ɫ�۾��ķ���

	// ��ȡ��ɫ���ӵ㣬���۾���λ�ú����߷���
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	// �����ɫ�۾�λ����ǰ1000��λ���յ㣬�������߼��
	FVector End = EyeLocation + (EyeRotation.Vector() * 1000.0f);

	// ����һ����ײ������������ڴ洢��⵽�Ľ��
	FHitResult Hit;

	// ��������ִ�����߼�⣬�����۾�λ�õ��յ㷶Χ�ڵĶ�̬����
	GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, ObjectQueryParams);

	// �Ӽ�����л�ȡ��ײ����Actor����
	AActor* HitActor = Hit.GetActor();
	if (HitActor)
	{
		// �����⵽��Actor��Ϊ�գ�����Actor�Ƿ�ʵ���� USGamePlayInterface �ӿ�
		if (HitActor->Implements<USGamePlayInterface>())
		{
			// ��ȡ��ɫ��Pawn���ã���������ת��
			APawn* MyPawn = Cast<APawn>(MyOwner);

			// ����ʵ���˽ӿڵ�Actor��Interact�����������ɫ��Pawn��Ϊ����
			ISGamePlayInterface::Execute_Interact(HitActor, MyPawn);

			// ���ڵ��ԣ�������ɫ���߶α�ʾ���ɹ�
			DrawDebugLine(GetWorld(), EyeLocation, End, FColor::Green, false, 3);
		}
	}
	else
	{
		// ���û����ײ���κ����壬���ƺ�ɫ���߶����ڵ��ԣ���ʾ���ʧ��
		DrawDebugLine(GetWorld(), EyeLocation, End, FColor::Red, false, 2.0f, 0.0f, 2.0f);
	}
}
	

//	TArray<FHitResult>Hits;
//
//	float Radius = 30.0f;
//
//	FCollisionShape Shape;
//	Shape.SetSphere(Radius);
//
//	//SweepMultiByObjectType��һ�����ڼ����ײ�ĺ���
//	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);
//
//	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;
//
//	for (FHitResult Hit : Hits)
//	{
//		AActor* HitActor = Hit.GetActor();
//		if (HitActor) {
//			if (HitActor->Implements<USGamePlayInterface>()) {
//				APawn* MyPawn = Cast<APawn>(MyOwner);
//				ISGamePlayInterface::Execute_Interact(HitActor, MyPawn);
//				break;
//			}
//		}
//		DrawDebugSphere(GetWorld(), Hit.ImpactPoint, Radius, 32, LineColor, false, 2.0f);
//	}
//	DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 2.0f, 0.0f, 2.0f);	
