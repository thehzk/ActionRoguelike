// Fill out your copyright notice in the Description page of Project Settings.

//用于实现各种属性
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttributeComponent.generated.h"


// 发起者，控件拥有者，改变后的血量，变化值
//委托

//这行代码使用了DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams宏来声明一个多播委托，定义了一个带有四个参数的事件。这意味着多个监听者可以订阅此事件，并且当该事件被触发时，所有监听者都会收到以下四个参数：
//
//AActor* InstigatorActor: 触发这次事件的Actor，即造成影响的角色（例如攻击者）。
//USAttributeComponent* OwningComp : 触发事件的属性组件的拥有者。
//float NewHealth : 变化后的新健康值。
//float Delta : 健康值变化的差值，正值表示加血，负值表示受伤。
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, InstigatorActor, USAttributeComponent*, OwningComp, float, NewHealth, float, Delta);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USAttributeComponent();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
	float MaxHealth;

	UFUNCTION(BlueprintCallable)
	bool IsAlive() const;

public:	
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyHealthChange(float Delta);
	
	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;
};
