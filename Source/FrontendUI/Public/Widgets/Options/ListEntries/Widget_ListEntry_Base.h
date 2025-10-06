// Author: Olegnormchel. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "FrontendTypes/FrontendEnumTypes.h"
#include "Widget_ListEntry_Base.generated.h"

class UListDataObject_Base;
class UCommonTextBlock;

UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class FRONTENDUI_API UWidget_ListEntry_Base : public UCommonUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On List Entry Widget Hovered"))
	void BP_OnListEntryWidgetHovered(bool bWasHovered, bool bIsEntryWidgetStillSelected);
	void NativeOnListEntryWidgetHovered(bool bWasHovered);

protected:
	// The child blueprint should override this function for gamepad interaction to function properly
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Get Widget To Focud For Gamepad"))
	UWidget* BP_GetWidgetToFocudForGamepad() const;
	
	//~ Begin IUserObjectListEntry Interface
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual void NativeOnEntryReleased() override;
	//~ End IUserObjectListEntry Interface

	//~ Begin UUserWidget Interface
	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;
	//~ End UUserWidget Interface
	
	// The child class should override this function to handle the initialization needed. Super call is expected
	virtual void OnOwningListDataObjectSet(UListDataObject_Base* InOwningListDataObject);

	// The child class should override this function to update the UI values after the data object has been modified.
	// Super call is not needed.
	virtual void OnOwningListDataObjectModified(UListDataObject_Base* OwningModifiedData, EOptionListDataModifyReason ModifyReason);

	// The child should override this to change editable state of the widgets it owns.
	//  Super call is expected
	virtual void OnToggleEditableState(bool bIsEditable);
	
	void SelectThisEntryWidget();
	
private:
	//***** Bound Widgets *****//
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = "true"))
	UCommonTextBlock* CommonText_SettingDisplayName;
	//***** Bound Widgets *****//
};
