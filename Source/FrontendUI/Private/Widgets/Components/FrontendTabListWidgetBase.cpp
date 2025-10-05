// Author: Olegnormchel. All rights reserved


#include "Widgets/Components/FrontendTabListWidgetBase.h"
#include "Editor/WidgetCompilerLog.h"
#include "Widgets/Components/FrontendCommonButtonBase.h"

void UFrontendTabListWidgetBase::RequestRegisterTab(const FName& InTabID, const FText& InTabDisplayName)
{
	RegisterTab(InTabID, TabButtonEntryWidgetClass, nullptr);

	if (UFrontendCommonButtonBase* FoundButton = Cast<UFrontendCommonButtonBase>(GetTabButtonBaseByID(InTabID)))
	{
		FoundButton->SetButtonText(InTabDisplayName);
		
	}
}

#if WITH_EDITOR	
void UFrontendTabListWidgetBase::ValidateCompiledDefaults(class IWidgetCompilerLog& CompileLog) const
{
	Super::ValidateCompiledDefaults(CompileLog);

	if (!TabButtonEntryWidgetClass)
	{
		CompileLog.Error(FText::FromString(TEXT("No TabButtonEntryWidgetClass has no valid entry specified") +
			GetClass()->GetName() +
			TEXT(" needs a valid entry widget class to function properly")
			));
	}
}
#endif
