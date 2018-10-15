// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ClimbingAIComponent.generated.h"


UENUM(BlueprintType)
enum class EClimbState : uint8
{
	//δ������������
	ECS_NotArrive,
	//�ѵ�����������
	ECS_Arrived,
	//Ԥ������
	ECS_PreClimb,
	//����
	ECS_Climb,
	//��ǽ��½
	ECS_Landing,
	//�ﵽǽ��
	ESC_IdleOnWall,
};


/**
 * 
 */
UCLASS()
class CLIMBINGAI_API UClimbingAIComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UClimbingAIComponent();

	void SetState(EClimbState ClimbState) { State = ClimbState; }

	EClimbState GetState() { return State; }

	void CalcClimbStartPoint(const FVector& Direction, float Distance, const FVector& LineStart, const FVector& LineEnd);

	void EnablePawnCollision(bool bIsEnable);

	void SetEnable(bool bEnabled) { IsEnable = bEnabled; }
	
protected:

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	//@TODO inline
	//״̬�߼�Tick�����0.3��ִ�У�
	void StateMachineTick(float DeltaSeconds);

	//״ִ̬���߼�Tick��ÿִ֡�У�
	void StateFrameTick(float DeltaSeconds);

	//��ǽת����ǽʱ��
	void RotateToWall(float DeltaSeconds);

	//�����˶���½����ǽʱ��2018-09-22�޸�
	void ClimbLandingParabola(float DeltaSeconds);

	//δ�������������
	void SMProcNotArrive(float DeltaSeconds);

	//������������
	void SMProcArrive(float DeltaSeconds);

protected:

	bool IsEnable = false;

	EClimbState State = EClimbState::ECS_NotArrive;

	//��������ʼ��
	FVector ClimbStartPoint;

	//ECS_Arrived ״̬�µ�Lerpʱ��
	float ArriveLerpTime = 0.f;
	float ArriveLerpDuration = 1.f;

	//ECS_Arrived ״̬�µ�Lerpʱ��
	FVector LerpStartLocation;

	//FVector TestFootPerpendicular;
};
