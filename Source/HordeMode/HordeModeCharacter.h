// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"


#include "Components/TimelineComponent.h"
#include "GameFramework/Character.h"
#include "HordeModeCharacter.generated.h"

UENUM(BlueprintType)

enum EActionState
{
	Idle UMETA(DisplayName = "Idle"),
	Running UMETA(DisplayName = "Running"),
	Jumping UMETA(DisplayName = "Jumping")
};

UENUM(BlueprintType)
enum EAimingState
{
	Aiming UMETA(Display = "Aiming"),
	NotAiming UMETA(Display = "NotAiming")
};

class UTimelineComponent;
class AGunBase;
class FOnTimelineFloat;
UCLASS(config=Game)
class AHordeModeCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	
	TEnumAsByte<EActionState> LocomotionState;
	TEnumAsByte<EAimingState> AimingState;
	
	// ADS TIMELINE
	UTimelineComponent* ResetCameraTimeline;
	FOnTimelineFloat ResetCameraTimelineFunction;
	UPROPERTY(EditAnywhere)
	UCurveFloat* ResetCameraCurve;
	UFUNCTION()
	void ADSCamera();
	void ADSPress();
	void ADSRelease();
	void ADSSetCamera(float Target, bool IsAiming);

	float ADSZoomIn = 100;
	float ADSDefault;
	float ADSTarget;
	

	// COMBAT
	void Attack();
	void ReleaseAttack();
	bool bIsHit;

	
	AGunBase* CurrentGun;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AGunBase> StartingGun;

public:
	AHordeModeCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

protected:
	void Tick(float DeltaSeconds) override;

	void BeginPlay() override;
	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);



protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(BlueprintPure)
	bool IsCharacterAiming();
	bool bIsAiming = false;
};

