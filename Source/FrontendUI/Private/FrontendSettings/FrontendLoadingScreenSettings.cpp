// Author: Olegnormchel. All rights reserved


#include "FrontendSettings/FrontendLoadingScreenSettings.h"
#include "Blueprint/UserWidget.h"

TSubclassOf<UUserWidget> UFrontendLoadingScreenSettings::GetLoadingScreenWidgetClassChecked() const
{
	checkf(!SoftLoadingScreenWisgetClass.IsNull(),
		TEXT("Forgot to assign a valid widget blueprint is the project settings as loading screen"));

	TSubclassOf<UUserWidget> LoadedLoadingScreenWidget =  SoftLoadingScreenWisgetClass.LoadSynchronous();

	return LoadedLoadingScreenWidget;
}
