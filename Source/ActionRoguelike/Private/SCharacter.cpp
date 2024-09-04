// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SIteractionComponent.h"
#include "SAttributeComponent.h"



// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	//�����������
	InteractionComp = CreateDefaultSubobject<USIteractionComponent>(TEXT("InteractionComp"));

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>(TEXT("AttributeComp"));

	// ������ʹ��Pawn������ת��
	//����رյĻ���꽫�޷�������ת
	SpringArmComp->bUsePawnControlRotation = true;

	// ��ȡ����ɫ�ƶ��������Ȼ����������ת�����˶���
	//����ر��������ô��ɫ�޷�ת��ֻ��һֱ�泯�Ÿս����ķ��򣬵��ǿ���ǰ��������
	GetCharacterMovement()->bOrientRotationToMovement = true;

	//�رա�ʹ�ÿ�������תYaw��
	//��������������ôAD���޷����ƽ�ɫת��ֻ�������Կ���
	bUseControllerRotationYaw = false;

	//�������д����Ŀ���Ƿ�������������������ת�ͽ�ɫ��ת
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
	//�ڶ�����
	PlayerInputComponent->BindAction("BlackHoleAttack", IE_Pressed, this, &ASCharacter::BlackHoleAttack);
}

void ASCharacter::MoveForward(float Value)
{
	FRotator ControlRot = GetControlRotation();
	// ת��ֻ��עˮƽYaw���������0��ֹӰ��
	ControlRot.Roll = 0.0f;
	ControlRot.Pitch = 0.0f;

	AddMovementInput(ControlRot.Vector(), Value);
	
	//AddMovementInput(GetActorForwardVector(), Value);	
}

void ASCharacter::MoveRight(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Roll = 0.0f;
	ControlRot.Pitch = 0.0f;
	// ��ȡ����������������ĳ���ת���Ҳ࣬������������ƶ��������Y��ʾ�Ҳ�
	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);

	AddMovementInput(RightVector, Value);
	//AddMovementInput(GetActorRightVector(), Value);
}

//��ͨ�������ͷ�ħ��Ͷ����
void ASCharacter::PrimaryAttack()
{
	PlayAnimMontage(AttackAnim);
	//����һ����ʱ������0.2����ٴε���PrimaryAttack_TimeElapsed()
	//û�м�ʱ���Ļ���PrimaryAttack_TimeElapsed()�е����ݾ�д������������ӷ�������ﶯ����ͬ��
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::PrimaryAttack_TimeElapsed,0.2f);
}

void ASCharacter::PrimaryAttack_TimeElapsed()
{
	SpawnProjectile(ProjectileClass); 
}

void ASCharacter::BlackHoleAttack()
{
	PlayAnimMontage(AttackAnim);
	//����һ����ʱ������0.2����ٴε���PrimaryAttack_TimeElapsed()
	//û�м�ʱ���Ļ���PrimaryAttack_TimeElapsed()�е����ݾ�д������������ӷ�������ﶯ����ͬ��
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::BlackHoleAttack_TimeElapsed, 0.2f);
}

void ASCharacter::BlackHoleAttack_TimeElapsed()
{
	SpawnProjectile(BlackHoleProjectileClass);
}

//����ʵ�ִӽ�ɫ���з�������
void ASCharacter::SpawnProjectile(TSubclassOf<AActor> ClassToSpawn)
{
	if (ensureAlways(ClassToSpawn))
	{
		FVector HandleLocation = GetMesh()->GetSocketLocation("Muzzle_01");//��ȡ����λ��

		FActorSpawnParameters SpawnParams;
		// �������á�
		// �˴�������ײ������Ϊ����ʹ��ײҲ�������ɣ���Ϊ�����ڽ�ɫ�м����ɱ�Ȼ��ײ 
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = this;

		// ���뾶
		FCollisionShape Shape;
		Shape.SetSphere(20.0f);

		// ��Ҫ����Լ���ɫ
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);

		// ��ײ����
		FCollisionObjectQueryParams ObjParams;
		ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjParams.AddObjectTypesToQuery(ECC_Pawn);

		// ������Ϊ 5000 cm = 50 m
		FVector TraceStart = CameraComp->GetComponentLocation();
		FVector TraceEnd = TraceStart + (GetControlRotation().Vector() * 5000);

		FHitResult Hit;
		if (GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjParams, Shape, Params)) {
			TraceEnd = Hit.ImpactPoint;
		}

		// β���� - ͷ���� = �������� eg�����(0,0) �յ�(1,1)����������Ϊ(1,1)
		FRotator ProjRotation = FRotationMatrix::MakeFromX(TraceEnd - HandleLocation).Rotator();
		// �����⵽����㷽���ڽ�ɫ������λ������
		FTransform SpawnTM = FTransform(ProjRotation, HandleLocation);

		// �����ܷ��û����ɵĶ�����Actor
		GetWorld()->SpawnActor<AActor>(ClassToSpawn, SpawnTM, SpawnParams);
	}
}

void ASCharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	//Ѫ��С��0��Ѫ���仯ҲС��0
	if (NewHealth <= 0.0f && Delta < 0)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		DisableInput(PC);
	}
}

void ASCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComp->OnHealthChanged.AddDynamic(this, &ASCharacter::OnHealthChanged);
}

//����������
void ASCharacter::PrimaryInteract()
{
	if (InteractionComp)
	{
		InteractionComp->PrimaryInteract();//InteractionComp�����е�һ����Ա��������
	}
}
