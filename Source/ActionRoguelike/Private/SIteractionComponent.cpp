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
	// 创建碰撞查询参数，用于检测动态物体（例如宝箱）
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);//射线检测将只检查与动态物体的碰撞

	// 获取调用此函数的角色（拥有该组件的Actor）
	AActor* MyOwner = GetOwner();

	// 用于存储角色的眼睛位置（位置和方向）
	FVector EyeLocation;  // 角色眼睛的位置
	FRotator EyeRotation; // 角色眼睛的方向

	// 获取角色的视点，即眼睛的位置和视线方向
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	// 计算角色眼睛位置向前1000单位的终点，用于射线检测
	FVector End = EyeLocation + (EyeRotation.Vector() * 1000.0f);

	// 创建一个碰撞检测结果对象，用于存储检测到的结果
	FHitResult Hit;

	// 在世界中执行射线检测，检测从眼睛位置到终点范围内的动态物体
	GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, ObjectQueryParams);

	// 从检测结果中获取碰撞到的Actor对象
	AActor* HitActor = Hit.GetActor();
	if (HitActor)
	{
		// 如果检测到的Actor不为空，检查该Actor是否实现了 USGamePlayInterface 接口
		if (HitActor->Implements<USGamePlayInterface>())
		{
			// 获取角色的Pawn引用，进行类型转换
			APawn* MyPawn = Cast<APawn>(MyOwner);

			// 调用实现了接口的Actor的Interact函数，传入角色的Pawn作为参数
			ISGamePlayInterface::Execute_Interact(HitActor, MyPawn);

			// 用于调试，绘制绿色的线段表示检测成功
			DrawDebugLine(GetWorld(), EyeLocation, End, FColor::Green, false, 3);
		}
	}
	else
	{
		// 如果没有碰撞到任何物体，绘制红色的线段用于调试，表示检测失败
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
