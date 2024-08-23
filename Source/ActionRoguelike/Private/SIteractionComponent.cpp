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

	AActor* MyOwner = GetOwner();//获取角色

	FVector EyeLocation;//角色眼睛位置
	FRotator EyeRotation;//角色眼睛方向
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);//获取角色眼睛位置和方向

	FVector End = EyeLocation + (EyeRotation.Vector() * 1000.0f);//角色眼睛位置向前1000距离的位置

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
//	//SweepMultiByObjectType是一个用于检测碰撞的函数
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