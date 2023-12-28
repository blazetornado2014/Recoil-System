// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "Kistmet/KismetMathLibrary.h"
#include "Curves/CurveVector.h"
#include "InputActionValue.h"
#include "MyProjectCharacter.generated.h"
#include ". ./Weapons/Weapon.h"


UCLASS(config=Game)
class AMyProjectCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

public:
	AMyProjectCharacter();
	

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
			

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UCurveVector* recoilCurve;
	UPROPERTY()
		bool firingClient = false;
	UPROPERTY()
		float fireRate = 0.0f;
	UFUNCTION(BlueprintCallable)
		void RecoilStart();
	UPROPERTY()
		bool recoil;
	UPROPERTY()
		bool recoilRecovery;
	UPROPERTY()
		bool firing;
	UPROPERTY()
		FTimerHandle fireTimer;
	UPROPERTY()
		FTimerHandle recoveryTimer;
	UFUNCTION()
		void RecoilTimerFunction();
	UPROPERTY()
		FRotator recoilStartRot;
	UPROPERTY()
		FRotator recoilDeltaRot;
	UPROPERTY()
		FRotator playerDeltaRot;
	UPROPERTY(BlueprintReadWrite)
		APlayerController* PCRef;
	UFUNCTION(BlueprintCallable)
		void RecoilStop();
	UFUNCTION()
		void RecoveryStart();
	UPROPERTY()
		FRotator del;
	UFUNCTION()
		void RecoveryTimerFunction();
	UPROPERTY(BlueprintReadWrite)
		float recoveryTime = 1.0f;
	UPROPERTY(BlueprintReadWrite)
		float recoverySpeed = 10.0f;
	UPROPERTY(BlueprintReadWrite, Replicated)
		Weapon* Weapon;
	
	// To add mapping context
	virtual void BeginPlay();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

