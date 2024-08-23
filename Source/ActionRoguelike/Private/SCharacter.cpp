// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SIteractionComponent.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	//�����������
	InteractionComp = CreateDefaultSubobject<USIteractionComponent>(TEXT("InteractionComp"));

	GetCharacterMovement()->bOrientRotationToMovement = true;

	bUseControllerRotationYaw = false;
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	//����
	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ASCharacter::PrimaryAttack);
	//����
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ASCharacter::PrimaryInteract);
}

void ASCharacter::MoveForward(float Value)
{
	FRotator ControlRot = GetControlRotation();
	// ת��ֻ��עˮƽYaw���������0��ֹӰ��
	ControlRot.Roll = 0.0f;
	ControlRot.Pitch = 0.0f;

	AddMovementInput(ControlRot.Vector(), Value);

}

void ASCharacter::MoveRight(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Roll = 0.0f;
	ControlRot.Pitch = 0.0f;
	// ��ȡ����������������ĳ���ת���Ҳ࣬������������ƶ��������Y��ʾ�Ҳ�
	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);

	AddMovementInput(RightVector, Value);
}

void ASCharacter::PrimaryAttack()
{
	PlayAnimMontage(AttackAnim);
	//����һ����ʱ������0.2����ٴε���PrimaryAttack_TimeElapsed()
	//û�м�ʱ���Ļ���PrimaryAttack_TimeElapsed()�е����ݾ�д������������ӷ�������ﶯ����ͬ��
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::PrimaryAttack_TimeElapsed,0.2f);
}

void ASCharacter::PrimaryAttack_TimeElapsed()
{
	FVector HandleLocation = GetMesh()->GetSocketLocation("Muzzle_01");//��ȡ����λ��

	FTransform SpawnTM = FTransform(GetControlRotation(), HandleLocation);//���÷���λ�úͳ���
	FActorSpawnParameters SpawnParams;
	// �������á�
	// �˴�������ײ������Ϊ����ʹ��ײҲ�������ɣ���Ϊ�����ڽ�ɫ�м����ɱ�Ȼ��ײ 
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;
	// �����ܷ��û����ɵĶ�����Actor
	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
}

//����������
void ASCharacter::PrimaryInteract()
{
	if (InteractionComp)
	{
		InteractionComp->PrimaryInteract();//InteractionComp�����е�һ����Ա��������
	}
}
