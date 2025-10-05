// Author: Olegnormchel. All rights reserved


#include "Widgets/Options/DataObjects/ListDataObject_Base.h"
#include "FrontendSettings/FrontendGameUserSettings.h"

void UListDataObject_Base::InitDataObject()
{
	OnDataObjectInitialized();
}

void UListDataObject_Base::OnDataObjectInitialized()
{
}

void UListDataObject_Base::NotifyListDataModified(UListDataObject_Base* ModifiedData,
	EOptionListDataModifyReason ModifyReason)
{
	OnListDataModified.Broadcast(ModifiedData, ModifyReason);

	if (bShouldApplyChangesImmediately)
	{
		UFrontendGameUserSettings::Get()->ApplySettings(true);
	}
}
