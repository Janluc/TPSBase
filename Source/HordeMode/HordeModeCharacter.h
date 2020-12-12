// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"


#include "Components/TimelineComponent.h"
#include "GameFramework/Character.h"
#include "HordeModeCharacter.generated.h"

class UTimelineComponent;
class AGunBase;
class FOnTimelineFloat;
class AMeleeWeaponBase;

UENUM(BlueprintType)

enum EActionState
{
	Idle UMETA(DisplayName = "Idle"),
	Running UMETA(DisplayName = "Running"),
	Jumping UMETA(DisplayName = "Jumping"),
	Attacking UMETA(DisplayName = "Attacking"),
	Dashing UMETA(DisplayName = "Dashing")
};

UENUM(BlueprintType)
enum EAimingState
{
	Aiming UMETA(Display = "Aiming"),
	NotAiming UMETA(Display = "NotAiming")
};


UCLASS(config=Game)
class AHordeModeCharacter : public ACharacter
{
	GENERATED_BODY()

// VARIABLES
private:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
	// STATE MACHINE
	TEnumAsByte<EActionState> ActionState = Idle;
	TEnumAsByte<EAimingState> AimingState = NotAiming;
	
	// ADS TIMELINE
	UPROPERTY()
	UTimelineComponent* ResetCameraTimeline;
	FOnTimelineFloat ResetCameraTimelineFunction;
	UPROPERTY(EditAnywhere)
	UCurveFloat* ResetCameraCurve;
	float ADSZoomIn = 100;
	float ADSDefault;
	float ADSTarget;

	// IN AIR
	bool InAir = false;
	bool OnGround = false;
	bool bJumpInput;

	// COMBAT
	bool bIsHit;

	// WEAPONS
	UPROPERTY()
	AGunBase* CurrentGun;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AGunBase> StartingGun;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons", meta = (AllowPrivateAccess = "true"))
	AMeleeWeaponBase* MeleeWeapon;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AMeleeWeaponBase> StartingMeleeWeapon;

public:


	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;
	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	// INPUT
	bool bIsAiming = false;
	float ForwardInput;
	float RightInput;


// FUNCTIONS
private:
	
	// ADS TIMELINE
	UFUNCTION()
	void ADSCamera();
	void ADSPress();
	void ADSRelease();
	void ADSSetCamera(float Target, bool IsAiming);

	// COMBAT
	void Attack();
	void ReleaseAttack();

	// JUMP / IN AIR
	void JumpAction();

protected:
	void Tick(float DeltaSeconds) override;

	void BeginPlay() override;
	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	AHordeModeCharacter();
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	
	// BLUEPRINT INPUT CHECK FUNCTIONS	
	UFUNCTION(BlueprintPure)
	bool JumpInputPressed();
		// STATE MACHINE	
	UFUNCTION(BlueprintPure)
    TEnumAsByte<EActionState> GetActionState();
	UFUNCTION(BlueprintPure)
    TEnumAsByte<EAimingState> GetAimingState();
	

};

