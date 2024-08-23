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
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	AActor* MyOwner = GetOwner();//��ȡ��ɫ

	FVector EyeLocation;//��ɫ�۾�λ��
	FRotator EyeRotation;//��ɫ�۾�����
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);//��ȡ��ɫ�۾�λ�úͷ���

	FVector End = EyeLocation + (EyeRotation.Vector() * 1000.0f);//��ɫ�۾�λ����ǰ1000�����λ��

	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, ObjectQueryParams);

	AActor* HitActor = Hit.GetActor();
	if (HitActor) {
		if (HitActor->Implements<USGamePlayInterface>()) {
			APawn* MyPawn = Cast<APawn>(MyOwner);
			ISGamePlayInterface::Execute_Interact(HitActor, MyPawn);
			DrawDebugLine(GetWorld(), EyeLocation, End, FColor::Green, false, 3);
		}
		else
		{
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
}