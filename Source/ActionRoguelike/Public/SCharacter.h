// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class USIteractionComponent;
class UAnimMontage;

UCLASS()
class ACTIONROGUELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()
protected:
	/*TSubclassOf<AActor> ProjectileClass; 是一个模板类型，用于指定某种 AActor 类及其子类的引用。
	它的作用是在运行时允许你选择一个特定的子类（例如，某种类型的弹药或特效），并且在代码中使用该子类来生成对象。
	作用:
	1. 类型安全性：TSubclassOf 确保了你只能选择 AActor 的子类作为 ProjectileClass 的类型，
	从而保证了类型安全。它防止你选择不相关的类作为投射物类，从而避免潜在的错误。
	2. 编辑器中的灵活性：在编辑器中，你可以通过下拉菜单选择一个特定的 AActor 子类。
	这对于设计师来说非常方便，可以不用修改代码就能改变某个投射物的类型。
	3. 代码的可扩展性：如果你以后想添加更多类型的投射物或其他行为，只需在编辑器中指定新的子类，
	而不需要修改代码。这使得系统更加模块化和可扩展。*/
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor>ProjectileClass;
	//UClass* ProjectileClass;
	//上下两行的区别在于使用TSubclassOf<AActor>时仅限于AAtor类，而使用UClass*时可以是任何类


	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim;

	FTimerHandle TimerHandle_PrimaryAttack;
public:
	// Sets default values for this character's properties
	ASCharacter();

protected:
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere)
	USIteractionComponent* InteractionComp;

	void MoveForward(float Value);
	void MoveRight(float Value);

	void PrimaryAttack();
	void PrimaryInteract();
	
	void PrimaryAttack_TimeElapsed();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
