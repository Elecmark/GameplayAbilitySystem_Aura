// PU Dianheng


#include "UI/HUD/AuraHUD.h"

#include "UI/Widget/AuraUserWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"

UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		OverlayWidgetController->BindCallbacksToDependencies();
	}
	return OverlayWidgetController;
}

void AAuraHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
    checkf(OverlayWidgetClass, TEXT("覆层类未初始化，请填写BP_AuraHUD"));
	checkf(OverlayWidgetControllerClass, TEXT("覆层控制器类未初始化，请填写BP_AuraHUD"));

    // 创建覆层小部件实例
    UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
    
    // 将创建的小部件转换为 UAuraUserWidget 并赋值给 OverlayWidget
    OverlayWidget = Cast<UAuraUserWidget>(Widget);

    const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);

    // 获取或创建覆层控制器，并传递参数
    UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);

    // 将覆层控制器设置到覆层小部件中
    OverlayWidget->SetWidgetController(WidgetController);

    // 广播初始值
    WidgetController->BroadcastInitialValues();

    Widget->AddToViewport();
}