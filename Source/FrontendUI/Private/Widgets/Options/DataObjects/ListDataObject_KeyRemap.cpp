// Author: Olegnormchel. All rights reserved


#include "Widgets/Options/DataObjects/ListDataObject_KeyRemap.h"

#include "CommonInputBaseTypes.h"
#include "CommonInputSubsystem.h"
#include "FrontendDebugHelper.h"

void UListDataObject_KeyRemap::InitKeyRemapData(UEnhancedInputUserSettings* InOwningInputUserSettings,
                                                UEnhancedPlayerMappableKeyProfile* InKeyProfile,
                                                ECommonInputType InDesiredInputKeyType,
                                                const FPlayerKeyMapping& InOwningPlayerKeyMapping)
{
	CachedOwningInputUserSettings = InOwningInputUserSettings;
	CachedOwningKeyProfile = InKeyProfile;
	CachedDesiredInputType = InDesiredInputKeyType;
	CachedOwningMappingName = InOwningPlayerKeyMapping.GetMappingName();
	CachedOwningMappableKeySlot = InOwningPlayerKeyMapping.GetSlot();
}

FSlateBrush UListDataObject_KeyRemap::GetIconFromCurrentKey() const
{
	check(CachedOwningInputUserSettings);

	FSlateBrush FoundBrush;

	UCommonInputSubsystem* CommonInputSubsystem = UCommonInputSubsystem::Get(
		CachedOwningInputUserSettings->GetLocalPlayer());
	check(CommonInputSubsystem);

	const bool bHasFoundBrush = UCommonInputPlatformSettings::Get()->TryGetInputBrush(
		FoundBrush,
		GetOwningKeyRemapping()->GetCurrentKey(),
		CachedDesiredInputType,
		CommonInputSubsystem->GetCurrentGamepadName()
	);

	if (!bHasFoundBrush)
	{
		Debug::Print(
			TEXT("Unable to find an icon for the key: ") +
			GetOwningKeyRemapping()->GetCurrentKey().GetDisplayName().ToString() +
			TEXT(". Empty brush was applied.")
		);
	}

	return FoundBrush;
}

FPlayerKeyMapping* UListDataObject_KeyRemap::GetOwningKeyRemapping() const
{
	check(CachedOwningKeyProfile);

	FMapPlayerKeyArgs KeyArgs;
	KeyArgs.MappingName = CachedOwningMappingName;
	KeyArgs.Slot = CachedOwningMappableKeySlot;

	return CachedOwningKeyProfile->FindKeyMapping(KeyArgs);
}
