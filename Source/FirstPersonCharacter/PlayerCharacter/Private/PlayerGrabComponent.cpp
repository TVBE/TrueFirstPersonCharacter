// Copyright (c) 2022-present Barrelhouse. All rights reserved.
// Written by Nino Saglia & Tim Verberne.

#include "PlayerGrabComponent.h"
#include "KineticActorComponent.h"
#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStaticsTypes.h"
#include "DrawDebugHelpers.h"
#include "EngineDefines.h"
#include "Components/PrimitiveComponent.h"
#include "PhysicsPublic.h"
#include "Physics/PhysicsInterfaceCore.h"
#include "Chaos/PBDJointConstraintTypes.h"
#include "Chaos/PBDJointConstraintData.h"
#include "ChaosCheck.h"

DEFINE_LOG_CATEGORY_CLASS(UPlayerGrabComponent, LogGrabComponent)

UPlayerGrabComponent::UPlayerGrabComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.TickGroup = TG_PrePhysics;
}

void UPlayerGrabComponent::OnRegister()
{
	Super::OnRegister();

	if (const APlayerCharacter* PlayerCharacter {Cast<APlayerCharacter>(GetOwner())})
	{
		Camera = PlayerCharacter->GetCamera();
		Movement = PlayerCharacter->GetPlayerCharacterMovement();
	}
}

void UPlayerGrabComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPlayerGrabComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GrabbedComponent && Configuration)
	{
		UpdateTargetLocationWithRotation(DeltaTime);

		if (CurrentZoomLevel != PreviousZoomLevel)
		{
			UpdatePhysicsHandle();
		}
		PreviousZoomLevel = CurrentZoomLevel;

		if (!IsPrimingThrow)
		{
			/** Check if the distance between the location and target location is too big, let the object go. */
			if (Configuration->LetGoDistance <= FVector::Distance(GrabbedComponent->GetComponentLocation(), TargetLocation))
			{
				ReleaseObject();
			}
			
		}
		else
		{
			UpdateThrowTimer(DeltaTime);
		}
	}
}

void UPlayerGrabComponent::UpdatePhysicsHandle()
{
	const float Alpha {static_cast<float>(FMath::GetMappedRangeValueClamped
		(FVector2D(Configuration->MinZoomLevel, Configuration->MaxZoomLevel), FVector2D(0,1), CurrentZoomLevel))};

	
	LinearDamping = FMath::Lerp(Configuration->MinZoomLinearDamping, Configuration->MaxZoomLinearDamping, Alpha);
	LinearStiffness = FMath::Lerp(Configuration->MinZoomLinearStiffness, Configuration->MaxZoomLinearStiffness, Alpha);
	AngularDamping = FMath::Lerp(Configuration->MinZoomAngularDamping, Configuration->MaxZoomAngularDamping, Alpha);
	AngularStiffness = FMath::Lerp(Configuration->MinZoomAngularStiffness, Configuration->MaxZoomAngularStiffness, Alpha);
	InterpolationSpeed = FMath::Lerp(Configuration->MinZoomInterpolationSpeed, Configuration->MaxZoomInterpolationSpeed, Alpha);

	/** Update the constrainthandle. */
	if (ConstraintHandle.IsValid() && ConstraintHandle.Constraint->IsType(Chaos::EConstraintType::JointConstraintType))
	{
		FPhysicsCommand::ExecuteWrite(ConstraintHandle, [&](const FPhysicsConstraintHandle& InConstraintHandle)
		{
			if (Chaos::FJointConstraint* Constraint {static_cast<Chaos::FJointConstraint*>(ConstraintHandle.Constraint)})
			{
				Constraint->SetLinearDriveStiffness(Chaos::FVec3(LinearStiffness));
				Constraint->SetLinearDriveDamping(Chaos::FVec3(LinearDamping));

				Constraint->SetAngularDriveStiffness(Chaos::FVec3(AngularStiffness));
				Constraint->SetAngularDriveDamping(Chaos::FVec3(AngularDamping));
			}
		});
	}
}

void UPlayerGrabComponent::UpdateRotatedHandOffset(FRotator& Rotation, FVector& HandOffset)
{
	/** Get the camera's world rotation. */
	CameraRotation = Camera->GetComponentRotation();
	CameraQuat = Camera->GetComponentQuat();

	const FVector MinValues = FVector(-Configuration->ThrowingShakeSize,-Configuration->ThrowingShakeSize,-Configuration->ThrowingShakeSize);
	const FVector MaxValues = FVector(Configuration->ThrowingShakeSize,Configuration->ThrowingShakeSize,Configuration->ThrowingShakeSize);
	const FVector ThrowingShake = FMath::RandPointInBox(FBox(MinValues, MaxValues));
	const FVector ThrowingVector = (ThrowingShake + Configuration->ThrowingBackupVector)*FMath::Clamp((ThrowingTimeLine),0.0,1.0);
	
	/** Rotate the hand offset vector using the camera's world rotation. */
	RotatedHandOffset =CameraRotation.RotateVector(Configuration->RelativeHoldingHandLocation + ThrowingVector);

	const float HandOffsetScalar {static_cast<float>(FMath::Clamp((((Configuration->BeginHandOffsetDistance)
		- CurrentZoomLevel) / (Configuration->BeginHandOffsetDistance + Configuration->BeginHandOffsetDistance * GrabbedComponentSize)), 0.0, 1000.0))};

	
	FVector NormalizedRotatedHandOffset = RotatedHandOffset.GetSafeNormal();


	float OffsetScalar = HandOffsetScalar * GrabbedComponentSize;


	FVector RotatedScaledHandOffset = OffsetScalar * (RotatedHandOffset + NormalizedRotatedHandOffset * GrabbedComponentSize);


	RotatedHandOffset = Camera->GetComponentLocation() + RotatedScaledHandOffset;
}

/** The looping function that updates the target location and rotation of the currently grabbed object*/
void UPlayerGrabComponent::UpdateTargetLocationWithRotation(float DeltaTime)
{
	if (!GrabbedComponent || !Configuration) { return; }
	AActor* CompOwner = this->GetOwner();
	
	if (CompOwner)
	{
		if (Movement && Movement->GetIsSprinting())
		{
			CurrentZoomLevel = CurrentZoomLevel - Configuration->WalkingRetunZoomSpeed * DeltaTime;
		}
		else
		{
			CurrentZoomLevel = CurrentZoomLevel + CurrentZoomAxisValue * Configuration->ZoomSpeed * DeltaTime;
		}
		
		CurrentZoomLevel = FMath::Clamp(CurrentZoomLevel, Configuration->MinZoomLevel, Configuration->MaxZoomLevel);
	}
	
	if (Camera)
	{
		UpdateRotatedHandOffset(CameraRotation, RotatedHandOffset);
		TargetLocation = RotatedHandOffset + (CurrentZoomLevel * Camera->GetForwardVector());

		FRotator TargetRotation{FRotator(CameraQuat * CameraRelativeRotation)};
		if (CurrentZoomLevel > Configuration->BeginHandOffsetDistance)
		{
			//TODO; Slerp the rotation to a surface.
		}
		SetTargetLocationAndRotation(TargetLocation, TargetRotation);
	}
}

/** Updates on tick when you are manually rotating the object.*/
void UPlayerGrabComponent::UpdateMouseImputRotation(FVector2d MouseInputDelta)
{
	if(RotationMode)
	{
		/** Make rotations based on mouse movement and scroll wheel in the perspective of the world.*/
		FQuat MouseInputQuatX{FQuat(1,0,0,CurrentRotationZoomAxisValue*0.07)};
		FQuat MouseInPutQuatY{FQuat(0,1,0,-MouseInputDelta.Y*0.01)};
		FQuat MouseInPutQuatZ{FQuat(0,0,1,MouseInputDelta.X*0.01)};
		FQuat DeltaRotation = MouseInputQuatX * MouseInPutQuatY * MouseInPutQuatZ;
		
		/**Normalize this rotation and apply it to the relative rotation of the object.*/
		CameraRelativeRotation = DeltaRotation.GetNormalized() * CameraRelativeRotation;
	}
	
}

void UPlayerGrabComponent::UpdateThrowTimer(float DeltaTime)
{
    /** Preview the target location*/
	PerformThrow(1);
	if (PrePrimingThrowTimer <= Configuration->PrePrimingThrowDelayTime)
	{
		PrePrimingThrowTimer += DeltaTime;
	}
	else
	{
		WillThrowOnRelease = true;
			
		if (CurrentZoomLevel != Configuration->ThrowingZoomLevel)
		{
			CurrentZoomLevel += Configuration->ToThrowingZoomSpeed * (Configuration->ThrowingZoomLevel - CurrentZoomLevel) *0.001;
		}
			
		if (PrePrimingThrowTimer <= 1.0)
		{
			/**The timeline to be used by various on tick functions that update location.
			*We normalize the value here since it otherwise would be need to be normalised multiple times later.
			*/
			ThrowingTimeLine += FMath::Clamp(DeltaTime/Configuration->ThrowChargeTime, 0.0,1.0);
		}
	}
}

/** The update loop that scales the zoomaxis value from the mouse input */
void UPlayerGrabComponent::UpdateZoomAxisValue(float ZoomAxis)
{
	if(!RotationMode)
	{
		CurrentZoomAxisValue = FMath::Clamp(((CurrentZoomAxisValue + 0.1 * ZoomAxis) * 0.9),-2.0,2.0);
	}
	else
	{
		CurrentRotationZoomAxisValue = ZoomAxis;
	}
	
}

/** Grab the object pass it to the physicshandle and capture the relative object rotation*/
void UPlayerGrabComponent::GrabActor(AActor* ActorToGrab)
{
	/** check if there's a reference and cast to static mesh component to get a ref to the first static mesh. */
	if (!ActorToGrab || GrabbedComponent)
		{
			UE_LOG(LogGrabComponent, Warning, TEXT("No actor references"));
			return;
		}
	if (UStaticMeshComponent* StaticMeshComponent {Cast<UStaticMeshComponent>(ActorToGrab->GetComponentByClass(UStaticMeshComponent::StaticClass()))})
	{
		CurrentZoomLevel = FVector::Distance(Camera->GetComponentLocation(), StaticMeshComponent->GetCenterOfMass());
		GrabComponentAtLocationWithRotation(StaticMeshComponent, NAME_None,StaticMeshComponent->GetCenterOfMass(),StaticMeshComponent->GetComponentRotation());

		/** Get the GrabbedComponent rotation relative to the camera. */
		
		 CameraRelativeRotation = Camera->GetComponentQuat().Inverse() * GrabbedComponent->GetComponentQuat();
	
		/** Start the tick function so that the update for the target location can start updating. */
		SetComponentTickEnabled(true);
	}
	else
	{
		UE_LOG(LogGrabComponent,Warning,TEXT("NoStaticmeshFound"))
	}


	/** Check if the actor already has a kinetic component. If this is the case, call HandleOnOwnerGrabbed on the component.
	 *	If not, add the component to the grabbed actor. */
	if (UKineticActorComponent* KineticComponent {Cast<UKineticActorComponent>(ActorToGrab->GetComponentByClass(UKineticActorComponent::StaticClass()))})
	{
		KineticComponent->HandleOnOwnerGrabbed();
	}
	else
	{
		ActorToGrab->AddComponentByClass(UKineticActorComponent::StaticClass(), false, FTransform(), false);
	}
	
	FBox BoundingBox {GrabbedComponent->Bounds.GetBox()};
	GrabbedComponentSize = FVector::Distance(BoundingBox.Min, BoundingBox.Max)/2;
}

void UPlayerGrabComponent::ReleaseObject()
{
	if(GrabbedComponent)
	{
		SetComponentTickEnabled(false);

		if (UKineticActorComponent* KineticComponent {Cast<UKineticActorComponent>(GrabbedComponent->GetOwner()->GetComponentByClass(UKineticActorComponent::StaticClass()))})
		{
			KineticComponent->HandleOnOwnerReleased();
		}
		
		if(WillThrowOnRelease)
		{
		
		}
		else
		{
			/** If the object is not thrown, clamp the velocity.*/
			FVector Velocity = GrabbedComponent->GetComponentVelocity();
			GrabbedComponent->SetPhysicsLinearVelocity(Velocity.GetClampedToSize(0.0, 500.0));
		}
		WillThrowOnRelease = false;
		ReleaseComponent();
		UE_LOG(LogGrabComponent, VeryVerbose, TEXT("Released Object."))
		StopPrimingThrow();
	}
}


void UPlayerGrabComponent::BeginPrimingThrow()
{
	IsPrimingThrow = true;
	PrePrimingThrowTimer = 0.0;
	ThrowingTimeLine = 0.0f;
	UE_LOG(LogGrabComponent, VeryVerbose, TEXT("Started Priming Throw."))
}

void UPlayerGrabComponent::StopPrimingThrow()
{
	IsPrimingThrow = false;
	WillThrowOnRelease = false;
	PrePrimingThrowTimer = 0.0;
	ThrowingTimeLine = 0.0f;
	UE_LOG(LogGrabComponent, VeryVerbose, TEXT("Stopped Priming Throw."))
}

/** Execute a throw if the throw is priming*/
void UPlayerGrabComponent::PerformThrow(bool OnlyPreviewTrajectory)
{
	if(WillThrowOnRelease)
	{
		
		bool ThrowOverHands{false};
		/** Calculate the throwing strenght using the timeline we updated in the tick.*/
		const float ThrowingStrength{Configuration->ThrowingStrengthCure->GetFloatValue(ThrowingTimeLine)};
		FVector Target {FVector()};
		FVector TraceStart {Camera->GetComponentLocation()};
		FVector TraceEnd {Camera->GetForwardVector() * 10000000 + TraceStart};FHitResult HitResult;
		FCollisionQueryParams CollisionParams;
		if  (this->GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, CollisionParams))
		{
			Target = HitResult.ImpactPoint;
		}
		else
		{
			Target = TraceEnd;
		}
		
		/** Calculate the direction from the player to the target */
		FVector Direction = Target - GrabbedComponent->GetComponentLocation();
		Direction.Normalize();
		
		FVector FinalDirection{0,0,0};
		if(Target != TraceEnd)
		{
			/** Calculate the angle to throw the object using a ballistic trajectory.*/
			//float ThrowAngle = CalculateThrowAngle(ReleaseLocation,Target,ThrowingStrength,ThrowOverHands);
			
			/**Rotate the Direction vector around the cross product of the Direction vector and the world up vector by the angle Theta. */
			FVector RotationAxis = Direction.CrossProduct(Direction,FVector::UpVector);
			RotationAxis.Normalize();
			//FinalDirection = Direction.RotateAngleAxis(ThrowAngle, RotationAxis);
			FinalDirection.Normalize();

			float VectorLength = RotationAxis.Size();
		}
		else
		{
			FinalDirection = Direction;
		}

		/** for now, we won't use the manual calculation for throwing angle if there isn't any solution*/
		FinalDirection = Direction;
		
		ThrowVelocity = FinalDirection * ThrowingStrength;

		FVector TossVelocity;
		if(!UGameplayStatics::SuggestProjectileVelocity(
				this,
				TossVelocity,
				GrabbedComponent->GetComponentLocation(),
				Target,
				ThrowingStrength,
				false,
				0,
				GetWorld()->GetGravityZ(),
				ESuggestProjVelocityTraceOption::DoNotTrace,
				FCollisionResponseParams::DefaultResponseParam,
				TArray<AActor*>{GetOwner(),GrabbedComponent->GetAttachParentActor()},
				false))
		{
			TossVelocity = ThrowVelocity;
		}
		VisualizeProjectilePath(GrabbedComponent->GetOwner(),GrabbedComponent->GetComponentLocation(),TossVelocity);

		if(!OnlyPreviewTrajectory)
		{
			/** Set the physics velocity of the grabbed component to the calculated throwing direction */
			
			GrabbedComponent->SetPhysicsLinearVelocity(TossVelocity);
			/** Release the grabbed component after the throw */
			GrabbedComponent->WakeRigidBody();
			ReleaseObject();
			
		}
	}
}

void UPlayerGrabComponent::VisualizeProjectilePath(AActor* ProjectileActor, FVector StartLocation, FVector LaunchVelocity)
{
	/** Define the parameters for the prediction */
	TArray<FVector> OutPathPositions;
	FPredictProjectilePathResult OutPath;
	FPredictProjectilePathParams PredictParams{};
	PredictParams.StartLocation = StartLocation;
	PredictParams.LaunchVelocity = LaunchVelocity;
	PredictParams.bTraceComplex = false;
	PredictParams.bTraceWithCollision = true;
	PredictParams.DrawDebugType = EDrawDebugTrace::None;
	PredictParams.DrawDebugTime = 2.0f;
	PredictParams.SimFrequency = 30.0f;
	PredictParams.MaxSimTime = 2.0f;
	PredictParams.ActorsToIgnore = TArray<AActor*>{GetOwner(),GrabbedComponent->GetOwner()};

	/** Call the prediction function */
	UGameplayStatics::PredictProjectilePath(ProjectileActor,PredictParams,OutPath);

	if (OutPath.HitResult.bBlockingHit)
	{
		FVector HitLocation = OutPath.HitResult.Location;
		float SphereRadius = 10.0f;
		FColor SphereColor = FColor::Red;
		float SphereLifeTime = 0.0f;
		DrawDebugSphere(GetWorld(), HitLocation, SphereRadius, 32, SphereColor, false, SphereLifeTime);
	}

	
	/** for (int32 i = 0; i < OutPathPositions.Num(); ++i)
	{
		const FVector& PathPosition = OutPathPositions[i];
		const FPredictProjectilePathPointData& PathPointData = OutPath.PathData[i];
	} */
}

void UPlayerGrabComponent::BeginTetriaryInteraction()
{
	RotationMode = true;
}

void UPlayerGrabComponent::EndTetriaryInteraction()
{
	RotationMode = false;
}






