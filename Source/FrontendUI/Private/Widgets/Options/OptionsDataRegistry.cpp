// Author: Olegnormchel. All rights reserved


#include "Widgets/Options/OptionsDataRegistry.h"

#include "FrontendSettings/FrontendGameUserSettings.h"
#include "Widgets/Options/OptionsDataInteractionHelper.h"
#include "Widgets/Options/DataObjects/ListDataObject_Collection.h"
#include "Widgets/Options/DataObjects/ListDataObject_String.h"
#include "FrontendFunctionLibrary.h"
#include "FrontendGameplayTags.h"
#include "CompGeom/FitOrientedBox3.h"
#include "Widgets/Options/DataObjects/ListDataObject_Scalar.h"

#define MAKE_OPTIONS_DATA_CONTROL(SetterOrGetterFuncName) \
	MakeShared<FOptionsDataInteractionHelper>(GET_FUNCTION_NAME_STRING_CHECKED(UFrontendGameUserSettings, SetterOrGetterFuncName))

void UOptionsDataRegistry::InitOptionsDataRegistry(ULocalPlayer* InOwningLocalPlayer)
{
	InitGameplayCollectionTab();
	InitAudioCollectionTab();
	InitVideoCollectionTab();
	InitControlCollectionTab();
}

TArray<UListDataObject_Base*> UOptionsDataRegistry::GetListSourceItemsBySelectedTabID(
	const FName& InSelectedTabID) const
{
	UListDataObject_Collection* const* FoundTabCollectionPtr = RegisteredOptionsTabCollections.FindByPredicate(
		[InSelectedTabID](UListDataObject_Collection* AvailableTabCollection)-> bool
		{
			return AvailableTabCollection->GetDataID() == InSelectedTabID;
		}
	);

	checkf(FoundTabCollectionPtr, TEXT("No valid tab found under the ID %s"), *InSelectedTabID.ToString());

	UListDataObject_Collection* FoundTabCollection = *FoundTabCollectionPtr;

	TArray<UListDataObject_Base*> AllChildListItems;

	for (UListDataObject_Base* ChildListData : FoundTabCollection->GetAllChildListData())
	{
		if (!ChildListData)
		{
			continue;
		}

		AllChildListItems.Add(ChildListData);

		if (ChildListData->HasAnyChildListData())
		{
			FindChildListDataRecursively(ChildListData, AllChildListItems);
		}
	}

	return AllChildListItems;
}

void UOptionsDataRegistry::FindChildListDataRecursively(UListDataObject_Base* InParentData,
	TArray<UListDataObject_Base*>& OutFoundChildListData) const
{
	if (!InParentData || !InParentData->HasAnyChildListData())
	{
		return;
	}

	for (UListDataObject_Base* SubChildListData : InParentData->GetAllChildListData())
	{
		if (!SubChildListData)
		{
			continue;
		}

		OutFoundChildListData.Add(SubChildListData);

		if (SubChildListData->HasAnyChildListData())
		{
			FindChildListDataRecursively(SubChildListData, OutFoundChildListData);
		}
	}
}

void UOptionsDataRegistry::InitGameplayCollectionTab()
{
	UListDataObject_Collection* GameplayTabCollection = NewObject<UListDataObject_Collection>();
	GameplayTabCollection->SetDataID(FName("GameplayTabCollection"));
	GameplayTabCollection->SetDataDisplayName(FText::FromString("Gameplay"));

	// This is full code for construct data interaction helper
	/*TSharedPtr<FOptionsDataInteractionHelper> ConstructedHelper =
		MakeShared<FOptionsDataInteractionHelper>(
			GET_FUNCTION_NAME_STRING_CHECKED(UFrontendGameUserSettings, GetCurrentGameDifficulty)
			);*/


	// Game Difficulty
	{
		UListDataObject_String* GameDifficulty = NewObject<UListDataObject_String>();
		GameDifficulty->SetDataID(FName("GameDifficulty"));
		GameDifficulty->SetDataDisplayName(FText::FromString(TEXT("Difficulty")));
		GameDifficulty->SetDescriptionRichText(FText::FromString("Adjust the difficulty of the game experience.\n\n"
			"<Bold>Easy: </>Focuses on the story experience. Provides the most relaxing combat.\n\n"
			"<Bold>Normal: </>Offers slightly harder combat experience.\n\n"
			"<Bold>Hard: </>Offers a much more challenging combat experience.\n\n"
			"<Bold>Very Hard: </>Provides the most challenging combat experience. Not recommended for first play through."));
		GameDifficulty->AddDynamicOption(TEXT("Easy"), FText::FromString(TEXT("Easy")));
		GameDifficulty->AddDynamicOption(TEXT("Normal"), FText::FromString(TEXT("Normal")));
		GameDifficulty->AddDynamicOption(TEXT("Hard"), FText::FromString(TEXT("Hard")));
		GameDifficulty->AddDynamicOption(TEXT("Very Hard"), FText::FromString(TEXT("Very Hard")));
		GameDifficulty->SetDefaultValueFromString(TEXT("Normal"));
		GameDifficulty->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetCurrentGameDifficulty));
		GameDifficulty->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetCurrentGameDifficulty));
		GameDifficulty->SetShouldApplySettingsImmediately(true);

		GameplayTabCollection->AddChildListData(GameDifficulty);
	}

	// Test Item
	{
		UListDataObject_String* TestItem = NewObject<UListDataObject_String>();
		TestItem->SetDataID(FName("TestItem"));
		TestItem->SetDataDisplayName(FText::FromString("Test Image Item"));
		TestItem->SetSoftDescriptionImage(
			UFrontendFunctionLibrary::GetOptionsSoftImageByTag(FrontendGameplayTags::Frontend_Image_TestImage)
		);
		TestItem->SetDescriptionRichText(FText::FromString("The image to display can be specified in the project settings. "
													 "It can be anything the developer assigned in there."));
		
		GameplayTabCollection->AddChildListData(TestItem);
	}

	RegisteredOptionsTabCollections.Add(GameplayTabCollection);
}

void UOptionsDataRegistry::InitAudioCollectionTab()
{
	UListDataObject_Collection* AudioTabCollection = NewObject<UListDataObject_Collection>();
	AudioTabCollection->SetDataID(FName("AudioTabCollection"));
	AudioTabCollection->SetDataDisplayName(FText::FromString("Audio"));

	// Volume Category
	{
		UListDataObject_Collection* VolumeCategoryCollection = NewObject<UListDataObject_Collection>();
		VolumeCategoryCollection->SetDataID(FName("VolumeCategoryCollection"));
		VolumeCategoryCollection->SetDataDisplayName(FText::FromString(TEXT("Volume")));

		AudioTabCollection->AddChildListData(VolumeCategoryCollection);

		// Overall Volume
		{
			UListDataObject_Scalar* OverallVolume = NewObject<UListDataObject_Scalar>();
			OverallVolume->SetDataID(FName("OverallVolume"));
			OverallVolume->SetDataDisplayName(FText::FromString(TEXT("Overall Volume")));
			OverallVolume->SetDescriptionRichText(FText::FromString(TEXT("This is description for overall volume")));
			OverallVolume->SetDisplayValueRange(TRange<float>(0.f, 1.f));
			OverallVolume->SetOutputValueRange(TRange<float>(0.f, 2.f));
			OverallVolume->SetSliderStepSize(0.01f);
			OverallVolume->SetDefaultValueFromString(LexToString(1.f));
			OverallVolume->SetDisplayNumericType(ECommonNumericType::Percentage);
			OverallVolume->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal()); //No Decimal: 50% //One Decimal: 50.5%
			OverallVolume->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetOverallVolume));
			OverallVolume->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetOverallVolume));
			OverallVolume->SetShouldApplySettingsImmediately(true);

			VolumeCategoryCollection->AddChildListData(OverallVolume);
		}

		// Test Item
		{
			UListDataObject_String* TestItem = NewObject<UListDataObject_String>();
			TestItem->SetDataID(FName("TestItem"));
			TestItem->SetDataDisplayName(FText::FromString("Test Image Item"));
			TestItem->SetSoftDescriptionImage(
				UFrontendFunctionLibrary::GetOptionsSoftImageByTag(FrontendGameplayTags::Frontend_Image_TestImage)
			);
			TestItem->SetDescriptionRichText(FText::FromString("The image to display can be specified in the project settings. "
														 "It can be anything the developer assigned in there."));
		
			VolumeCategoryCollection->AddChildListData(TestItem);
		}
	}
	
	RegisteredOptionsTabCollections.Add(AudioTabCollection);
}

void UOptionsDataRegistry::InitVideoCollectionTab()
{
	UListDataObject_Collection* VideoTabCollection = NewObject<UListDataObject_Collection>();
	VideoTabCollection->SetDataID(FName("VideoTabCollection"));
	VideoTabCollection->SetDataDisplayName(FText::FromString("Video"));

	RegisteredOptionsTabCollections.Add(VideoTabCollection);
}

void UOptionsDataRegistry::InitControlCollectionTab()
{
	UListDataObject_Collection* ControlTabCollection = NewObject<UListDataObject_Collection>();
	ControlTabCollection->SetDataID(FName("ControlTabCollection"));
	ControlTabCollection->SetDataDisplayName(FText::FromString("Control"));

	RegisteredOptionsTabCollections.Add(ControlTabCollection);
}
