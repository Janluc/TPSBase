// Copyright Epic Games, Inc. All Rights Reserved.

#include "HordeModeCharacter.h"
#include "GunBase.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Logging/TokenizedMessage.h"

//////////////////////////////////////////////////////////////////////////
// AHordeModeCharacter


AHordeModeCharacter::AHordeModeCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	ResetCameraTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Reset Camera"));
	ResetCameraTimelineFunction.BindUFunction(this, "ADSCamera");
}

//////////////////////////////////////////////////////////////////////////
// Input

void AHordeModeCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AHordeModeCharacter::JumpAction);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("ADS", IE_Pressed, this, &AHordeModeCharacter::ADSPress);
	PlayerInputComponent->BindAction("ADS", IE_Released, this, &AHordeModeCharacter::ADSRelease);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AHordeModeCharacter::Attack);
	PlayerInputComponent->BindAction("Attack", IE_Released, this, &AHordeModeCharacter::ReleaseAttack);

	PlayerInputComponent->BindAxis("MoveForward", this, &AHordeModeCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AHordeModeCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
}

TEnumAsByte<EActionState> AHordeModeCharacter::GetActionState()
{
	return ActionState;
}


TEnumAsByte<EAimingState> AHordeModeCharacter::GetAimingState()
{
	return  AimingState;
}

bool AHordeModeCharacter::JumpInputPressed()
{
	return bJumpInput;
}

bool AHordeModeCharacter::IsCharacterAiming()
{
	return bIsAiming;
}


void AHordeModeCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	UE_LOG(LogTemp, Warning, TEXT("Player State: %s"), *UEnum::GetValueAsString<EActionState>(ActionState));
	if (GetCharacterMovement()->IsFalling())
	{
		if(!InAir)
		{
			InAir = true;
			ActionState = Jumping;
			bJumpInput = false;
			OnGround = false;
		}
	}
	else
	{
		if(!OnGround)
		{
			OnGround = true;
			ActionState = Idle;
			InAir = false;
		}
	}
}

void AHordeModeCharacter::BeginPlay()
{
	Super::BeginPlay();
	ADSDefault = CameraBoom->TargetArmLength;
	ResetCameraTimeline->AddInterpFloat(ResetCameraCurve, ResetCameraTimelineFunction);
	CurrentGun = GetWorld()->SpawnActor<AGunBase>(StartingGun);
	CurrentGun->SetOwner(this);
}

void AHordeModeCharacter::MoveForward(float Value)
{
	ForwardInput = Value;
	switch (ActionState)
	{
		case Idle: case Running: case Jumping:
		if ((Controller != NULL) && (Value != 0.0f))
		{
			// find out which way is forward
			
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get forward vector
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			AddMovementInput(Direction, Value);
			if (GetCharacterMovement()->IsFalling())
			{
				ActionState = Jumping;
				return;
			}
			ActionState = Running;
		}
		else if (RightInput == 0.0f && !GetCharacterMovement()->IsFalling())
			ActionState = Idle;
		break;

		default:
			break;
	}
}

void AHordeModeCharacter::MoveRight(float Value)
{
	RightInput = Value;
	switch (ActionState)
	{
	case Idle: case Running: case Jumping:
	    if ((Controller != NULL) && (Value != 0.0f))
	    {
    		// find out which way is forward
    		const FRotator Rotation = Controller->GetControlRotation();
    		const FRotator YawRotation(0, Rotation.Yaw, 0);

    		// get forward vector
    		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
    		AddMovementInput(Direction, Value);
	    	if (GetCharacterMovement()->IsFalling())
	    	{
	    		ActionState = Jumping;
	    		return;
	    	}
	    	ActionState = Running;
	    }
		else if (ForwardInput == 0.0f && !GetCharacterMovement()->IsFalling())
			ActionState = Idle;
		break;

	default:
		break;
	}
}


void AHordeModeCharacter::JumpAction()
{
	switch (ActionState)
	{
		case Idle: case Running:
			bJumpInput = true;
			Jump();
			break;
		
		default:
			break;
		
	}
	
}

void AHordeModeCharacter::Attack()
{
	if(AimingState == Aiming)
	{
		CurrentGun->PullTrigger();
	}
		
}


void AHordeModeCharacter::ReleaseAttack()
{
	CurrentGun->ReleaseTrigger();
}

// ADS Timeline Function
void AHordeModeCharacter::ADSCamera()
{
	CameraBoom->TargetArmLength = FMath::FInterpTo(CameraBoom->TargetArmLength, ADSTarget, GetWorld()->GetDeltaSeconds(), 15);
	
}

void AHordeModeCharacter::ADSPress()
{
	UE_LOG(LogTemp, Warning, TEXT("Press"))
	AimingState = Aiming;
	ADSSetCamera(ADSZoomIn, true);
}

void AHordeModeCharacter::ADSRelease()
{
	AimingState = NotAiming;
	ADSSetCamera(ADSDefault, false);
}

void AHordeModeCharacter::ADSSetCamera(float Target, bool IsAiming)
{
	ResetCameraTimeline->Stop();
	ADSTarget = Target;
	bUseControllerRotationYaw = IsAiming;
	GetCharacterMovement()->bOrientRotationToMovement = !IsAiming;
	CameraBoom->bEnableCameraLag = !IsAiming;
	bIsAiming = IsAiming;
	ResetCameraTimeline->PlayFromStart();
}
