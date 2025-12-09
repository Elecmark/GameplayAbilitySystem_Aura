// PU Dianheng


#include "Player/AuraPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit()) return;

	LastActor = ThisActor;
	ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());

	/**
	* 从光标位置进行线性追踪。有以下几种情况：
	*	A. LastActor为空 && ThisActor为空
	*		- 不执行任何操作
	*	B. LastActor为空 && ThisActor有效
	*		- 高亮显示ThisActor
	*	C. LastActor有效 && ThisActor为空
	*		- 取消高亮LastActor
	*	D. 两个Actor都有效，但LastActor != ThisActor
	*		- 取消高亮LastActor，并高亮显示ThisActor
	*	E. 两个Actor都有效，且是同一个Actor
	*		- 不执行任何操作
	*/
	
	if (LastActor != nullptr && LastActor != ThisActor)
	{
		// C. LastActor有效 && ThisActor为空
		// D. 两个Actor都有效，但LastActor != ThisActor
		LastActor->UnHighlightActor();
	}

	if (ThisActor != nullptr && ThisActor != LastActor)
	{
		// B. LastActor为空 && ThisActor有效
		// D. 两个Actor都有效，但LastActor != ThisActor
		ThisActor->HighlightActor();
	}
	// A. LastActor为空 && ThisActor为空
	// E. 两个Actor都有效，且是同一个Actor
	// 这两种情况不需要任何操作
	
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(AuraContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);
	Subsystem -> AddMappingContext(AuraContext, 0);

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent -> BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation =GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn -> AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn -> AddMovementInput(RightDirection, InputAxisVector.X);
	}
}
