// Fill out your copyright notice in the Description page of Project Settings.

//�ӿ���

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


// �����������ӽӿں���������ཫ���̳���ʵ�ָýӿڡ�
class ACTIONROGUELIKE_API ISGamePlayInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// ����һ����ͼ��ʵ�ֵ��¼��������������������󽻻�
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Interact(APawn* InstigatorPawn);// ��������ߡ�Ϊ��ʹ����˫�����ߵĽ�ɫ����ȷ���ã�����ΪPawn������Character
	//APawn* InstigatorPawn�����������ʾ���𽻻��Ľ�ɫ��ָ�롣ͨ���������ָ�룬��������֪����˭��������ν������Ӷ����Ը��ݲ�ͬ�ķ�����ִ�в�ͬ���߼���

};