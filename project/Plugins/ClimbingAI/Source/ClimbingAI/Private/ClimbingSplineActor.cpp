// Fill out your copyright notice in the Description page of Project Settings.

#include "ClimbingSplineActor.h"
#include "Components/SplineComponent.h"
#include "Components/ArrowComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "EngineUtils.h"
#include "GameFramework/PawnMovementComponent.h"

#include "ClimbingAIUtil.h"
#include "ClimbingAIComponent.h"

// Sets default values
AClimbingSplineActor::AClimbingSplineActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DelayCheckTime = FMath::RandRange(0.1f, 0.5f);

	CustomRootComp = CreateDefaultSubobject<USceneComponent>(TEXT("CustomRootComp"));
	SetRootComponent(CustomRootComp);

	ClimbLineComp = CreateDefaultSubobject<USplineComponent>(TEXT("ClimbLineComp"));
	ClimbLineComp->SetupAttachment(CustomRootComp);
	ClimbLineComp->SetRelativeLocation(FVector(0.f, -ClimbDepth, 0.f));
	ClimbLineComp->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));

	/*ClimbLineHorizonComp = CreateDefaultSubobject<USplineComponent>(TEXT("ClimbLineHorizonComp"));
	ClimbLineHorizonComp->SetupAttachment(ClimbLineVerticalComp);*/

	LeftEdgeComp = CreateDefaultSubobject<UArrowComponent>(TEXT("LeftEdgeComp"));
	LeftEdgeComp->SetupAttachment(CustomRootComp);
	LeftEdgeComp->SetRelativeLocation(TempLeftEdgeLoc);
	LeftEdgeComp->SetRelativeRotation(TempLeftEdgeRot);
	LeftEdgeComp->SetArrowColor(ArrowColor);
	LeftEdgeComp->SetHiddenInGame(false);

	LeftEdgePassComp = CreateDefaultSubobject<UArrowComponent>(TEXT("LeftEdgePassComp"));
	LeftEdgePassComp->SetupAttachment(CustomRootComp);
	LeftEdgePassComp->SetRelativeLocation(TempLeftEdgePassLoc);
	LeftEdgePassComp->SetRelativeRotation(TempLeftEdgePassRot);
	LeftEdgePassComp->SetArrowColor(ArrowColor);
	LeftEdgePassComp->SetHiddenInGame(false);

	RightEdgeComp = CreateDefaultSubobject<UArrowComponent>(TEXT("RightEdgeComp"));
	RightEdgeComp->SetupAttachment(CustomRootComp);
	RightEdgeComp->SetRelativeLocation(TempRightEdgeLoc);
	RightEdgeComp->SetRelativeRotation(TempRightEdgeRot);
	RightEdgeComp->SetArrowColor(ArrowColor);
	RightEdgeComp->SetHiddenInGame(false);

	RightEdgePassComp = CreateDefaultSubobject<UArrowComponent>(TEXT("RightEdgePassComp"));
	RightEdgePassComp->SetupAttachment(CustomRootComp);
	RightEdgePassComp->SetRelativeLocation(TempRightEdgePassLoc);
	RightEdgePassComp->SetRelativeRotation(TempRightEdgePassRot);
	RightEdgePassComp->SetArrowColor(ArrowColor);
	RightEdgePassComp->SetHiddenInGame(false);
}

void AClimbingSplineActor::Tick(float DeltaSeconds)
{
	if (!bEnabled)
	{
		return;
	}

	if (DelayCheckTime > 0.f)
	{
		DelayCheckTime -= DeltaSeconds;
	}
	else
	{
		FindAroundPawns(DeltaSeconds);

		EnterClimbAreaCheck(DeltaSeconds);
	}
}

void AClimbingSplineActor::BeginPlay()
{
	Super::BeginPlay();

	//���������ܳ��ȣ����˵���Ծ�׶κͶѵ��׶Σ�
	if (ClimbLineComp)
	{
		int PointNum = ClimbLineComp->GetNumberOfSplinePoints();
		

	}

	if (RootComponent)
	{
		RootLocation = RootComponent->GetComponentLocation();
	}

	//�����ͷ��������
	if (LeftEdgeComp)
	{
		LeftEdgeLocation = LeftEdgeComp->GetComponentLocation();
		LeftEdgeDirection = LeftEdgeComp->GetComponentRotation().Vector().GetSafeNormal();
	}
	if (LeftEdgePassComp)
	{
		LeftEdgePassDirection = LeftEdgePassComp->GetComponentRotation().Vector().GetSafeNormal();
	}
	if (RightEdgeComp)
	{
		RightEdgeLocation = RightEdgeComp->GetComponentLocation();
		RightEdgeDirection = RightEdgeComp->GetComponentRotation().Vector().GetSafeNormal();
	}
	if (RightEdgePassComp)
	{
		RightEdgePassDirection = RightEdgePassComp->GetComponentRotation().Vector().GetSafeNormal();
	}

	//�������������ڵ�Lerp�ƶ�����
	if (LeftEdgeComp && RightEdgeComp && ClimbLineComp)
	{
		FVector Point1 = LeftEdgeComp->GetComponentLocation();
		FVector Point2 = RightEdgeComp->GetComponentLocation();
		FVector OutLinePoint = ClimbLineComp->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::World);

		FVector FootPerpendicular = UKismetMathLibrary::FindClosestPointOnLine(OutLinePoint, Point1, Point1 - Point2);

		FVector Direction = OutLinePoint - FootPerpendicular;
		ClimbEnterLerpDirection = Direction.GetSafeNormal();
		ClimbEnterLerpDistance = Direction.Size();
	}
}

void AClimbingSplineActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (ClimbLineComp)
	{
		ClimbLineComp->SetRelativeLocation(FVector(0.f, -ClimbDepth, 0.f));
	}

	SetCustomProperties();
}

void AClimbingSplineActor::PostLoad()
{
	Super::PostLoad();

	//�������PostLoad������һ�μ�ͷ��RelativeLocation����ArrowComp�ڳ����༭���е���ʾ�����⣬ʹ�õĻ��ǹ��캯���е��������
	SetCustomProperties();
}

void AClimbingSplineActor::SetArrowRelativeLocatin(UArrowComponent* ArrowComp, const FVector& TemplateLoc, const FLinearColor& Color)
{
	if (ArrowComp)
	{
		ArrowComp->SetRelativeLocation(TemplateLoc);
		ArrowComp->SetArrowColor(Color);
	}
}

void AClimbingSplineActor::SetCustomProperties()
{
	SetArrowRelativeLocatin(LeftEdgeComp, FVector(-ClimbWidth / 2, TempLeftEdgeLoc.Y, TempLeftEdgeLoc.Z), ArrowColor);
	SetArrowRelativeLocatin(LeftEdgePassComp, FVector(-ClimbWidth / 2, TempLeftEdgePassLoc.Y, TempLeftEdgePassLoc.Z), ArrowColor);
	SetArrowRelativeLocatin(RightEdgeComp, FVector(ClimbWidth / 2, TempRightEdgeLoc.Y, TempRightEdgeLoc.Z), ArrowColor);
	SetArrowRelativeLocatin(RightEdgePassComp, FVector(ClimbWidth / 2, TempRightEdgePassLoc.Y, TempRightEdgePassLoc.Z), ArrowColor);
}

void AClimbingSplineActor::FindAroundPawns(float DeltaSeconds)
{
	FindAroundPawnsTime += DeltaSeconds;

	if (FindAroundPawnsTime > FindAroundPawnsInterval)
	{
		FindAroundPawnsTime = 0.f;

		float CheckDist = ClimbWidth * 2;

		AroundPawns.Reset();

		for (TActorIterator<APawn> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{
			if (FVector::Dist(ActorItr->GetActorLocation(), RootLocation) < CheckDist)
			{
				AroundPawns.Add(*ActorItr);
			}
		}
	}
}

void AClimbingSplineActor::EnterClimbAreaCheck(float DeltaSeconds)
{
	EnterClimbAreaCheckTime += DeltaSeconds;

	if (EnterClimbAreaCheckTime > EnterClimbAreaCheckInterval)
	{
		EnterClimbAreaCheckTime = 0.f;

		for (APawn* Pawn : AroundPawns)
		{
			if (Pawn->IsValidLowLevelFast())
			{
				if (UClimbingAIComponent* Comp = UClimbingAIUtil::GetClimbingAIComponent(Pawn))
				{
					if (1/*Comp->GetState() == EClimbState::ECS_NotArrive*/)
					{
						if (FVector::Dist(RootLocation, Pawn->GetActorLocation()) <= ClimbWidth)
						{
							//��⣺��ͷ��㵽��ɫ�������������������ͷ�����ļнǣ������ LeftEdgePass �ļн�С�� LeftEdge �ļнǣ���˵����������������
							FVector LeftArrowToPawn = (Pawn->GetActorLocation() - LeftEdgeLocation).GetSafeNormal();

							float test1 = LeftEdgeDirection | LeftArrowToPawn;
							float test2 = LeftEdgePassDirection | LeftArrowToPawn;
							float LeftRadian = FMath::RadiansToDegrees(acosf(test1));
							float LeftRadianPass = FMath::RadiansToDegrees(acosf(test2));

							bool IsEnter = false;

							//�����߼��û�н����������򣬱�������ټ��һ���ұ߼�ͷ
							if (LeftRadian < LeftRadianPass)
							{
								FVector RightArrowToPawn = Pawn->GetActorLocation() - RightEdgeLocation;
								float RightRadian = acosf(RightEdgeDirection | RightArrowToPawn);
								float RightRadianPass = acosf(RightEdgePassDirection | RightArrowToPawn);
								if (RightRadian >= RightRadianPass)
								{
									IsEnter = true;
								}
							}
							else
							{
								IsEnter = true;
							}

							if (IsEnter)
							{
								if (LeftEdgeComp && RightEdgeComp)
								{
									Comp->CalcClimbStartPoint(ClimbEnterLerpDirection, ClimbEnterLerpDistance, LeftEdgeComp->GetComponentLocation(), RightEdgeComp->GetComponentLocation());
									Comp->SetState(EClimbState::ECS_Arrived);
									Comp->EnablePawnCollision(false);
									if (UPawnMovementComponent* MovementComp = Pawn->GetMovementComponent())
									{
										MovementComp->StopMovementImmediately();
									}
								}
							}
						}
					}
				}
			}
		}
	}
}