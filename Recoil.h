// MyCharacter.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyRecoilCharacter.generated.h"

UCLASS()
class RECOIL_API AMyRecoilCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AMyRecoilCharacter();

    FTimerHandle TimerHandle_ResetRecoil;

    // Array of RecoilData, editable in the Blueprint
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
    TArray<FVector> RecoilData;

    // Function to save RecoilData to a JSON file
    UFUNCTION(BlueprintCallable, Category = "Recoil")
    void SaveRecoilDataToFile(const FString& FilePath);

protected:
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UFUNCTION()
    void Fire();

private:
    int RecoilIdx = 0;
    bool bIsRecoilActive;

    void ApplyRecoil();
    void ResetRecoil();
    void LoadRecoilDataFromFile(const FString& FilePath);
};
