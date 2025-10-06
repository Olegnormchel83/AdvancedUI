// Author: Olegnormchel. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "FrontendTypes/FrontendEnumTypes.h"
#include "FrontendTypes/FrontendStructTypes.h"
#include "ListDataObject_Base.generated.h"

#define LIST_DATA_ACCESSOR(DataType, PropertyName) \
	FORCEINLINE DataType Get##PropertyName() const { return PropertyName;} \
	void Set##PropertyName(DataType In##PropertyName) { PropertyName = In##PropertyName;}

UCLASS(Abstract)
class FRONTENDUI_API UListDataObject_Base : public UObject
{
	GENERATED_BODY()

public:
	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnListDataModifiedDelegate, UListDataObject_Base*,
	                                     EOptionListDataModifyReason);
	FOnListDataModifiedDelegate OnListDataModified;

	FOnListDataModifiedDelegate OnDependencyDataModified;

	LIST_DATA_ACCESSOR(FName, DataID)
	LIST_DATA_ACCESSOR(FText, DataDisplayName)
	LIST_DATA_ACCESSOR(FText, DescriptionRichText)
	LIST_DATA_ACCESSOR(FText, DisabledRichText)
	LIST_DATA_ACCESSOR(TSoftObjectPtr<UTexture2D>, SoftDescriptionImage)
	LIST_DATA_ACCESSOR(UListDataObject_Base*, ParentData)

	void InitDataObject();

	// Empty in the base class. Child class ListDataObject_Collection
	// should override it. The function should return all the child data a tab has
	virtual TArray<UListDataObject_Base*> GetAllChildListData() const { return TArray<UListDataObject_Base*>(); }
	virtual bool HasAnyChildListData() const { return false; }

	void SetShouldApplySettingsImmediately(bool bInShouldApplyRightAway)
	{
		bShouldApplyChangesImmediately = bInShouldApplyRightAway;
	};

	// The child class should override them to provide implementations for resetting the data
	virtual bool HasDefaultValue() const { return false; }
	virtual bool CanResetBackToDefaultValue() const { return false; }
	virtual bool TryResetBackToDefaultValue() { return false; }

	// Gets called from OptionsDataRegistry for adding in edit conditions for the constructed list data objects
	void AddEditCondition(const FOptionsDataEditConditionDescriptor& InEditCondition);

	// Gets called from OptionsDataRegistry to add in dependecy data
	void AddEditDependencyData(UListDataObject_Base* InDependencyData);
	
	bool IsDataCurrentlyEditable();

protected:
	// Empty in base class. The child classes should override it to handle the initialized needed correctly
	virtual void OnDataObjectInitialized();

	virtual void NotifyListDataModified(UListDataObject_Base* ModifiedData,
	                                    EOptionListDataModifyReason ModifyReason =
		                                    EOptionListDataModifyReason::DirectlyModified);

	// The child class should override this to allow the value be set to the forced string value
	virtual bool CanSetToForcedStringValue(const FString& InForcedValue) const { return false; }

	// The child class should override this to specify how to set the current value to the forced value
	virtual void OnSetToForcedStringValue(const FString& InForcedValue)
	{
	}

	// This function will be called when the value of the dependency data has changed.
	// The child class can override this function to handle the custom logic needed.
	// Super call is expected.
	virtual void OnEditDependencyDataModified(UListDataObject_Base* ModifiedDependencyData,  EOptionListDataModifyReason ModifyReason);
	
private:
	FName DataID;
	FText DataDisplayName;
	FText DescriptionRichText;
	FText DisabledRichText;
	TSoftObjectPtr<UTexture2D> SoftDescriptionImage;

	UPROPERTY(Transient)
	UListDataObject_Base* ParentData;

	bool bShouldApplyChangesImmediately = false;

	UPROPERTY(Transient)
	TArray<FOptionsDataEditConditionDescriptor> EditConditionDescArray;
};
