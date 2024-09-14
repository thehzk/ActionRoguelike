// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"
#include "Math/UnrealMathUtility.h"

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	//MaxHealth = 100.0f; // Ĭ���������ֵ
	//Health = MaxHealth; // ��ʼʱ�Ľ���ֵΪ�������ֵ
	Health = 100.0f;
}


bool USAttributeComponent::ApplyHealthChange(float Delta)
{
	//Health = FMath::Clamp(Health + Delta, 0.0f, MaxHealth);
	Health += Delta;
	OnHealthChanged.Broadcast(nullptr, this, Health, Delta);
	UE_LOG(LogTemp, Warning, TEXT("Health "));
	return true;
}

bool USAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}