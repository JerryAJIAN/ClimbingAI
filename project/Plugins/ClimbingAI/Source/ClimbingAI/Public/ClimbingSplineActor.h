// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ClimbingSplineActor.generated.h"

class USplineComponent;
class UArrowComponent;
class UMyArrowComponent;


UCLASS()
class CLIMBINGAI_API AClimbingSplineActor : public AActor
{
	GENERATED_BODY()

public:

	AClimbingSplineActor();

protected:

	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginPlay() override;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	virtual void PostLoad() override;

private:

	//���ü�ͷ���������
	FORCEINLINE void SetArrowRelativeLocatin(UArrowComponent* ArrowComp, const FVector& TemplateLoc, const FLinearColor& Color);

	//�����Զ�������
	FORCEINLINE void SetCustomProperties();

	//���ҵ�ǰClimbingSplineActor������Pawn��Ԥ���㣩 @TODO inline
	void FindAroundPawns(float DeltaSeconds);

	//����Ƿ�������������� @TODO inline
	void EnterClimbAreaCheck(float DeltaSeconds);

protected:

	UPROPERTY(EditDefaultsOnly)
		bool bEnabled = true;

	//����ӳ�ʱ�䣨���ڱ�֤����ClimbingSplineActor����߼���ʱ�����
	float DelayCheckTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USceneComponent* CustomRootComp;

	//��ǽ�����·��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USplineComponent* ClimbLineComp;

	//RootComponent����������
	FVector RootLocation;

	//�ж��Ƿ�������������������ͷ�������ߣ�
	UArrowComponent* LeftEdgeComp;
	UArrowComponent* LeftEdgePassComp;

	//��߼�ͷ����������
	FVector LeftEdgeLocation;

	//�ж��Ƿ�������������������ͷ�ķ�����������ߣ�
	FVector LeftEdgeDirection;
	FVector LeftEdgePassDirection;

	//�ж��Ƿ�������������������ͷ������ұߣ�
	UArrowComponent* RightEdgeComp;
	UArrowComponent* RightEdgePassComp;

	//�ұ߼�ͷ����������
	FVector RightEdgeLocation;

	//�ж��Ƿ����������������������������ұߣ�
	FVector RightEdgeDirection;
	FVector RightEdgePassDirection;

	//����������Ŀ��
	UPROPERTY(EditAnywhere, Category = ArrowInfo)
		float ClimbWidth = 200.f;

	//�������������ȣ�ˮƽ���򣬴�RootComponent�� ClimbLineComp ֮��ľ��룩
	UPROPERTY(EditAnywhere, Category = ArrowInfo)
		float ClimbDepth = 50.f;

	//��ͷ��ɫ
	UPROPERTY(EditAnywhere, Category = ArrowInfo)
		FLinearColor ArrowColor = FLinearColor::Yellow;

	//��߼�ͷ��ģ��transform
	const FVector TempLeftEdgeLoc = FVector(-ClimbWidth / 2, 0.f, 0.f);
	const FRotator TempLeftEdgeRot = FRotator(0.f, 1.f, 0.f);
	const FVector TempLeftEdgePassLoc = FVector(-ClimbWidth / 2, 0.f, 0.f);
	const FRotator TempLeftEdgePassRot = FRotator(0.f, -1.f, 0.f);

	//�ұ߼�ͷ��ģ��transform
	const FVector TempRightEdgeLoc = FVector(ClimbWidth / 2, 0.f, 0.f);
	const FRotator TempRightEdgeRot = FRotator(0.f, 179.f, 0.f);
	const FVector TempRightEdgePassLoc = FVector(ClimbWidth / 2, 0.f, 0.f);
	const FRotator TempRightEdgePassRot = FRotator(0.f, -179.f, 0.f);

	//���������ڵ�Lerp�ƶ�����
	FVector ClimbEnterLerpDirection;
	
	//���������ڵ�Lerp�ƶ�����
	float ClimbEnterLerpDistance;

	//��ǰClimbingSplineActor������Pawn��Ԥ���㣩
	TArray<APawn*> AroundPawns;

	float FindAroundPawnsInterval = 3.f;
	float FindAroundPawnsTime = 0.f;

	float EnterClimbAreaCheckInterval = 1.f;
	float EnterClimbAreaCheckTime = 0.f;
};
