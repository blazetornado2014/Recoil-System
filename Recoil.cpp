// MyCharacter.cpp

#include "MyRecoilCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "Misc/FileHelper.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"

AMyRecoilCharacter::AMyRecoilCharacter()
{
    PrimaryActorTick.bCanEverTick = true;
    bIsRecoilActive = false;

    // Load recoil data from a file
    LoadRecoilDataFromFile("C:/Blaze/Documents/Recoil.json");

}

void AMyRecoilCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Bind functions to input
    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMyRecoilCharacter::Fire);
}

void AMyRecoilCharacter::LoadRecoilDataFromFile(const FString& FilePath)
{
    // Read file
    FString FileData;
    if (FFileHelper::LoadFileToString(FileData, *FilePath))
    {
        TSharedPtr<FJsonObject> JsonObj;

        // Create Reader
        TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(FileData);

        // Deserialize the JSON data
        if (FJsonSerializer::Deserialize(Reader, JsonObj))
        {
            // Clear existing recoil data
            RecoilData.Empty();

            // Get Recoil Array
            TArray<TSharedPtr<FJsonValue>> RecoilArray = JsonObj->GetArrayField("RecoilData");

            // Parse each element and add it to RecoilData
            for (const TSharedPtr<FJsonValue>& JsonValue : RecoilArray)
            {
                FVector Recoil = FVector::ZeroVector;

                Recoil.X = RecoilArray->GetNumberField("X");
                Recoil.Y = RecoilArray->GetNumberField("Y");
                Recoil.Z = RecoilArray->GetNumberField("Z");

                // Add Recoil Data
                RecoilData.Add(Recoil);
            }
        }
    }
}

void AMyRecoilCharacter::SaveRecoilDataToFile(const FString& FilePath)
{
    // Create JSON object
    TSharedPtr<FJsonObject> JsonObj = MakeShareable(new FJsonObject);

    // Convert each FVector in RecoilData to JSON
    TArray<TSharedPtr<FJsonValue>> RecoilArray;
    for (const FVector& Recoil : RecoilData)
    {
        TSharedPtr<FJsonValueObject> JsonValue = MakeShareable(new FJsonValueObject);
        JsonValue->SetObjectField("Recoil", MakeShareable(new FJsonObject(Recoil)));
        RecoilArray.Add(JsonValue);
    }

    // Set RecoilArray to the JSON object
    JsonObj->SetArrayField("RecoilData", RecoilArray);

    // Convert the JSON object to string
    FString JsonString;
    TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&JsonString);
    FJsonSerializer::Serialize(JsonObj.ToSharedRef(), JsonWriter);

    // Save the JSON string to file
    FFileHelper::SaveStringToFile(JsonString, *FilePath);
}


void AMyRecoilCharacter::Fire()
{
    ApplyRecoil();
}

void AMyRecoilCharacter::ApplyRecoil()
{
    if (GetController() && RecoilData.Num() > 0)
    {
        bIsRecoilActive = true;

        FVector SelectRecoil = RecoilData[RecoilIdx];
        RecoilIdx++;

        // Apply the selected recoil impulse to the character's controller
        GetController()->AddInputVector(SelectRecoil);

        // Reset recoil after a delay
        GetWorldTimerManager().SetTimer(TimerHandle_ResetRecoil, this, &AMyRecoilCharacter::ResetRecoil, 2.0f, false); 
    }
}

void AMyRecoilCharacter::ResetRecoil()
{
    bIsRecoilActive = false;
    RecoilIdx = 0;
}
