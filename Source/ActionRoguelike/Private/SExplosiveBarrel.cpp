// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplosiveBarrel.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
ASExplosiveBarrel::ASExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	ForceComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("ForceComp"));

	RootComponent = MeshComp;
	ForceComp->SetupAttachment(MeshComp);

	//��ģ����������Ϊtrue
	MeshComp->SetSimulatePhysics(true);
	//����ײԤ������Ϊ������ײ
	MeshComp->SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);

	ForceComp->SetAutoActivate(false);//�Զ��������
	ForceComp->Radius = 1000.0f;//��ը��Χ
	ForceComp->ImpulseStrength = 1000.0f;//��ըǿ��
	ForceComp->bImpulseVelChange = true;//����������С

	ForceComp->AddCollisionChannelToAffect(ECC_WorldDynamic);
}

// Called when the game starts or when spawned
void ASExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
	
} 

// Called every frame
void ASExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASExplosiveBarrel::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	MeshComp->OnComponentHit.AddDynamic(this, &ASExplosiveBarrel::OnActionHit);
}

void ASExplosiveBarrel::OnActionHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ForceComp->FireImpulse();
	UE_LOG(LogTemp, Warning, TEXT("BOOM!"));
}

