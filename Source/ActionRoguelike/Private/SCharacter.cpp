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

	//创建交互组件
	InteractionComp = CreateDefaultSubobject<USIteractionComponent>(TEXT("InteractionComp"));

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>(TEXT("AttributeComp"));

	// 开启“使用Pawn控制旋转”
	//如果关闭的话鼠标将无法控制旋转
	SpringArmComp->bUsePawnControlRotation = true;

	// 获取“角色移动”组件，然后开启“将旋转朝向运动”
	//如果关闭了这个那么角色无法转向，只会一直面朝着刚进来的方向，但是可以前后左右走
	GetCharacterMovement()->bOrientRotationToMovement = true;

	//关闭“使用控制器旋转Yaw”
	//如果开启了这个那么AD键无法控制角色转向，只有鼠标可以控制
	bUseControllerRotationYaw = false;

	//以上三行代码的目的是分离控制相机的鼠标控制旋转和角色旋转
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
	//攻击
	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ASCharacter::PrimaryAttack);
	//交互
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ASCharacter::PrimaryInteract);
	//黑洞攻击
	PlayerInputComponent->BindAction("BlackHoleAttack", IE_Pressed, this, &ASCharacter::BlackHoleAttack);
}

void ASCharacter::MoveForward(float Value)
{
	FRotator ControlRot = GetControlRotation();
	// 转向只关注水平Yaw方向，因此置0防止影响
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
	// 获取相机（鼠标控制器）的朝向，转向右侧，并朝这个方向移动；传入的Y表示右侧
	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);

	AddMovementInput(RightVector, Value);
	//AddMovementInput(GetActorRightVector(), Value);
}

//普通攻击，释放魔法投掷物
void ASCharacter::PrimaryAttack()
{
	PlayAnimMontage(AttackAnim);
	//设置一个计时器，在0.2秒后再次调用PrimaryAttack_TimeElapsed()
	//没有计时器的话，PrimaryAttack_TimeElapsed()中的内容就写在这里，但是粒子发射和人物动作不同步
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::PrimaryAttack_TimeElapsed,0.2f);
}

void ASCharacter::PrimaryAttack_TimeElapsed()
{
	SpawnProjectile(ProjectileClass); 
}

void ASCharacter::BlackHoleAttack()
{
	PlayAnimMontage(AttackAnim);
	//设置一个计时器，在0.2秒后再次调用PrimaryAttack_TimeElapsed()
	//没有计时器的话，PrimaryAttack_TimeElapsed()中的内容就写在这里，但是粒子发射和人物动作不同步
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::BlackHoleAttack_TimeElapsed, 0.2f);
}

void ASCharacter::BlackHoleAttack_TimeElapsed()
{
	SpawnProjectile(BlackHoleProjectileClass);
}

//具体实现从角色手中发射物体
void ASCharacter::SpawnProjectile(TSubclassOf<AActor> ClassToSpawn)
{
	if (ensureAlways(ClassToSpawn))
	{
		FVector HandleLocation = GetMesh()->GetSocketLocation("Muzzle_01");//获取发射位置

		FActorSpawnParameters SpawnParams;
		// 参数设置。
		// 此处设置碰撞检测规则为：即使碰撞也总是生成，因为粒子在角色中间生成必然碰撞 
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = this;

		// 检测半径
		FCollisionShape Shape;
		Shape.SetSphere(20.0f);

		// 不要检测自己角色
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);

		// 碰撞设置
		FCollisionObjectQueryParams ObjParams;
		ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjParams.AddObjectTypesToQuery(ECC_Pawn);

		// 检测距离为 5000 cm = 50 m
		FVector TraceStart = CameraComp->GetComponentLocation();
		FVector TraceEnd = TraceStart + (GetControlRotation().Vector() * 5000);

		FHitResult Hit;
		if (GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjParams, Shape, Params)) {
			TraceEnd = Hit.ImpactPoint;
		}

		// 尾向量 - 头向量 = 方向向量 eg：起点(0,0) 终点(1,1)，方向向量为(1,1)
		FRotator ProjRotation = FRotationMatrix::MakeFromX(TraceEnd - HandleLocation).Rotator();
		// 朝向检测到的落点方向，在角色的右手位置生成
		FTransform SpawnTM = FTransform(ProjRotation, HandleLocation);

		// 所有能放置或生成的对象都是Actor
		GetWorld()->SpawnActor<AActor>(ClassToSpawn, SpawnTM, SpawnParams);
	}
}

void ASCharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	//血量小于0且血量变化也小于0
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

//处理交互函数
void ASCharacter::PrimaryInteract()
{
	if (InteractionComp)
	{
		InteractionComp->PrimaryInteract();//InteractionComp对象中的一个成员函数调用
	}
}
