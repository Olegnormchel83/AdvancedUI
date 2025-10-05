// Author: Olegnormchel. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "FrontendGameUserSettings.generated.h"

UCLASS()
class FRONTENDUI_API UFrontendGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()

public:
	UFrontendGameUserSettings();

	static UFrontendGameUserSettings* Get();

	//***** Gameplay Collection Tab *****//
	UFUNCTION()
	FString GetCurrentGameDifficulty() const { return CurrentGameDifficulty; };

	UFUNCTION()
	void SetCurrentGameDifficulty(const FString& InNewGameDifficulty) { CurrentGameDifficulty = InNewGameDifficulty; };
	//***** Gameplay Collection Tab *****//

	//***** Audio Collection Tab *****//
	UFUNCTION()
	float GetOverallVolume() const { return OverallVolume; };

	UFUNCTION()
	void SetOverallVolume(float InVolume);

	UFUNCTION()
	float GetMusicVolume() const { return MusicVolume; };

	UFUNCTION()
	void SetMusicVolume(float InVolume);
	//***** Audio Collection Tab *****//

private:
	//***** Gameplay Collection Tab *****//
	UPROPERTY(Config)
	FString CurrentGameDifficulty;
	//***** Gameplay Collection Tab *****//

	//***** Audio Collection Tab *****//
	UPROPERTY(Config)
	float OverallVolume;

	UPROPERTY(Config)
	float MusicVolume;
	//***** Audio Collection Tab *****//
};
