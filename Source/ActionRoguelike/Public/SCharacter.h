// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class USIteractionComponent;
class UAnimMontage;
class USAttributeComponent;

UCLASS()
class ACTIONROGUELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()
protected:


	UPROPERTY(Editanywhere, Category = "Attack")
	TSubclassOf<AActor> BlackHoleProjectileClass;


	void BlackHoleAttack();
	void BlackHoleAttack_TimeElapsed();

public:
	// Sets default values for this character's properties
	ASCharacter();


protected:
#pragma region 
	//�ƶ����������
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	void MoveForward(float Value);
	void MoveRight(float Value);
#pragma endregion


#pragma region
	//����SIteractionComponent�����ڴ�����
	UPROPERTY(VisibleAnywhere)
	USIteractionComponent* InteractionComp;

	//�󶨰������� E
	void PrimaryInteract();
#pragma endregion

	//��ͨ����
	void PrimaryAttack();

#pragma region
	//�������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USAttributeComponent* AttributeComp;

	/*TSubclassOf<AActor> ProjectileClass; ��һ��ģ�����ͣ�����ָ��ĳ�� AActor �༰����������á�
����������������ʱ������ѡ��һ���ض������ࣨ���磬ĳ�����͵ĵ�ҩ����Ч���������ڴ�����ʹ�ø����������ɶ���
����:
1. ���Ͱ�ȫ�ԣ�TSubclassOf ȷ������ֻ��ѡ�� AActor ��������Ϊ ProjectileClass �����ͣ�
�Ӷ���֤�����Ͱ�ȫ������ֹ��ѡ����ص�����ΪͶ�����࣬�Ӷ�����Ǳ�ڵĴ���
2. �༭���е�����ԣ��ڱ༭���У������ͨ�������˵�ѡ��һ���ض��� AActor ���ࡣ
��������ʦ��˵�ǳ����㣬���Բ����޸Ĵ�����ܸı�ĳ��Ͷ��������͡�
3. ����Ŀ���չ�ԣ�������Ժ�����Ӹ������͵�Ͷ�����������Ϊ��ֻ���ڱ༭����ָ���µ����࣬
������Ҫ�޸Ĵ��롣��ʹ��ϵͳ����ģ�黯�Ϳ���չ��*/
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor>ProjectileClass;
	//UClass* ProjectileClass;
	//�������е���������ʹ��TSubclassOf<AActor>ʱ������AAtor�࣬��ʹ��UClass*ʱ�������κ���


	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim;

	FTimerHandle TimerHandle_PrimaryAttack;

	void PrimaryAttack_TimeElapsed();
#pragma endregion


	void SpawnProjectile(TSubclassOf<AActor> ClassToSpawn);

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

	virtual void PostInitializeComponents() override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
