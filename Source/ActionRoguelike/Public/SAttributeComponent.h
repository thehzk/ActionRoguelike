// Fill out your copyright notice in the Description page of Project Settings.

//����ʵ�ָ�������
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttributeComponent.generated.h"


// �����ߣ��ؼ�ӵ���ߣ��ı���Ѫ�����仯ֵ
//ί��

//���д���ʹ����DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams��������һ���ಥί�У�������һ�������ĸ��������¼�������ζ�Ŷ�������߿��Զ��Ĵ��¼������ҵ����¼�������ʱ�����м����߶����յ������ĸ�������
//
//AActor* InstigatorActor: ��������¼���Actor�������Ӱ��Ľ�ɫ�����繥���ߣ���
//USAttributeComponent* OwningComp : �����¼������������ӵ���ߡ�
//float NewHealth : �仯����½���ֵ��
//float Delta : ����ֵ�仯�Ĳ�ֵ����ֵ��ʾ��Ѫ����ֵ��ʾ���ˡ�
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
