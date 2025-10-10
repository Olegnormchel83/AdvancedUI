// Author: Olegnormchel. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "FrontendLoadingScreenSubsystem.generated.h"

UCLASS()
class FRONTENDUI_API UFrontendLoadingScreenSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	//~ Begin USubsystem Interface
	virtual bool ShouldCreateSubsystem(UObject* Outer) const;
	virtual void Initialize(FSubsystemCollectionBase& Collection);
	virtual void Deinitialize();
	//~ End USubsystem Interface

private:
	UFUNCTION()
	void OnMapPreLoaded(const FWorldContext& WorldContext, const FString& MapName);

	UFUNCTION()
	void OnMapPostLoaded(UWorld* LoadedWorld);
};
