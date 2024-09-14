// Fill out your copyright notice in the Description page of Project Settings.

//接口类

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SGamePlayInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USGamePlayInterface : public UInterface
{
	GENERATED_BODY()
};


// 在这个类中添加接口函数。这个类将被继承以实现该接口。
class ACTIONROGUELIKE_API ISGamePlayInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// 声明一个蓝图可实现的事件函数，用于与其他对象交互
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Interact(APawn* InstigatorPawn);// 传入调用者。为了使不能双足行走的角色能正确调用，定义为Pawn而不是Character
	//APawn* InstigatorPawn：这个参数表示发起交互的角色的指针。通过传递这个指针，函数可以知道是谁发起了这次交互，从而可以根据不同的发起者执行不同的逻辑。

};