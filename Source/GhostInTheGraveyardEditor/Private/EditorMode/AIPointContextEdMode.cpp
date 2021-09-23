
#include "EditorMode/AIPointContextEdMode.h"
#include "EditorMode/AIPointContextEdModeToolkit.h"
#include "Editor/UnrealEd/Public/Toolkits/ToolkitManager.h"
#include "ScopedTransaction.h"
#include "Engine/Selection.h"
#include "EditorModeManager.h"
#include "EngineUtils.h"
#include "PrimitiveSceneProxy.h"
#include "EditorMode/AIPointContextEditorObject.h"

/* class for setting up keyboard input */
class FAIPointContextEditorCommands : public TCommands<FAIPointContextEditorCommands>
{
public:

	FAIPointContextEditorCommands()
		: TCommands<FAIPointContextEditorCommands>("AIPointContextEditor",
			FText::FromString(TEXT("AI Point Context Editor")), NAME_None, FEditorStyle::GetStyleSetName())
	{
	}

#define LOCTEXT_NAMESPACE ""

	virtual void RegisterCommands() override 
	{
		UI_COMMAND(DeletePoint, "Delete Point", "Delete the currently selected point.", 
			EUserInterfaceActionType::Button, FInputGesture(EKeys::Delete));
	}

#undef LOCTEXT_NAMESPACE

	TSharedPtr<FUICommandInfo> DeletePoint;
};

FAIPointContextEdMode::FAIPointContextEdMode()
{
	FAIPointContextEditorCommands::Register();
	AIPointContextEdModeActions = MakeShareable(new FUICommandList);

	UISettings = NewObject<UAIPointContextEditorObject>(GetTransientPackage(), TEXT("UISettings"), RF_Transactional);
	UISettings->SetParent(this);
}

FAIPointContextEdMode::~FAIPointContextEdMode()
{
	FAIPointContextEditorCommands::Unregister();
}

const FEditorModeID FAIPointContextEdMode::EM_AIPointContext(TEXT("EM_AIPointContext"));

IMPLEMENT_HIT_PROXY(HAIPointContextProxy, HHitProxy)

void FAIPointContextEdMode::Enter()
{
	FEdMode::Enter();

	if(!Toolkit.IsValid())
	{
		Toolkit = MakeShareable(new FAIPointContextEdModeToolkit);
		Toolkit->Init(Owner->GetToolkitHost());
	}

	// reset target
	CurrentSelectedIndex = -1;
	CurrentSelectedTarget.Reset();

	// automatically selects the first manager found
	AAIPointContextManager* Manager = nullptr;
	for (TActorIterator<AAIPointContextManager> It(GetWorld()); It; ++It)
	{
		AAIPointContextManager* Actor = (*It);
		if (Actor)
		{
			Manager = Actor;
			break;
		}
	}

	if (Manager)
	{
		SelectPoint(Manager, Manager->SearchPoints.Num() - 1);
		CurrentSelectedTarget = Manager;
	}

	// map the UI/keyboard commands
	MapCommands();
}

void FAIPointContextEdMode::Exit()
{
	FToolkitManager::Get().CloseToolkit(Toolkit.ToSharedRef());
	Toolkit.Reset();

	FEdMode::Exit();
}

void FAIPointContextEdMode::Render(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI)
{
	const FColor SelectedColor(255, 255, 255);

	UWorld* World = GetWorld();
	for (TActorIterator<AAIPointContextManager> It(World); It; ++It)
	{
		AAIPointContextManager* Actor = (*It);
		if(Actor)
		{
			FVector ActorLoc = Actor->GetTargetLocation();

			if (!bHiddenSearchPoints)
			{
				for (int i = 0; i < Actor->SearchPoints.Num(); ++i)
				{
					bool bSelected = (Actor == CurrentSelectedTarget && i == CurrentSelectedIndex);
					const FColor& Color = bSelected ? SelectedColor : SphereColors[0];

					PDI->SetHitProxy(new HAIPointContextProxy(Actor, i));
					DrawWireSphere(PDI, FTransform(Actor->SearchPoints[i]), Color, DebugSphereRadius, 10, SDPG_Foreground);
					PDI->SetHitProxy(NULL);
				}
			}
		}
	}
	FEdMode::Render(View, Viewport, PDI);
}

bool FAIPointContextEdMode::HandleClick(FEditorViewportClient* InViewportClient, HHitProxy* HitProxy, const FViewportClick& Click)
{
	bool bHandled = false;

	if (HitProxy)
	{
		if (HitProxy->IsA(HAIPointContextProxy::StaticGetType()))
		{
			bHandled = true;
			HAIPointContextProxy* Proxy = (HAIPointContextProxy*)HitProxy;
			AAIPointContextManager* Actor = Cast<AAIPointContextManager>(Proxy->RefObject);
			int32 Index = Proxy->Index;
			if (Actor && Index >= 0 && Index < Actor->SearchPoints.Num())
			{
				SelectPoint(Actor, Index);
			}
		}
	}

	if (HitProxy && bHandled && Click.GetKey() == EKeys::RightMouseButton)
	{
		TSharedPtr<SWidget> MenuWidget = GenerateContextMenu(InViewportClient);
		if (MenuWidget.IsValid())
		{
			FSlateApplication::Get().PushMenu(
				Owner->GetToolkitHost()->GetParentWidget(),
				FWidgetPath(),
				MenuWidget.ToSharedRef(),
				FSlateApplication::Get().GetCursorPos(),
				FPopupTransitionEffect(FPopupTransitionEffect::ContextMenu)
			);
		}
	}

	return bHandled;
}

bool FAIPointContextEdMode::InputDelta(FEditorViewportClient* InViewportClient, FViewport* InViewport, FVector& InDrag, FRotator& InRot, FVector& InScale)
{
	if (InViewportClient->GetCurrentWidgetAxis() == EAxisList::None)
	{
		return false;
	}

	if (HasValidSelection())
	{
		if (!InDrag.IsZero())
		{
			CurrentSelectedTarget->Modify();
			CurrentSelectedTarget->SearchPoints[CurrentSelectedIndex] += InDrag;
		}
		return true;
	}

	return false;
}

FVector FAIPointContextEdMode::GetWidgetLocation() const
{
	if (HasValidSelection())
	{
		return CurrentSelectedTarget->SearchPoints[CurrentSelectedIndex];
	}
	return FEdMode::GetWidgetLocation();
}

void FAIPointContextEdMode::MapCommands()
{
	const auto& Commands = FAIPointContextEditorCommands::Get();

	AIPointContextEdModeActions->MapAction(
		Commands.DeletePoint,
		FExecuteAction::CreateSP(this, &FAIPointContextEdMode::RemovePoint),
		FCanExecuteAction::CreateSP(this, &FAIPointContextEdMode::CanRemovePoint));
}

bool FAIPointContextEdMode::InputKey(FEditorViewportClient* ViewportClient, FViewport* Viewport, FKey Key, EInputEvent Event)
{
	bool bHandled = false;

	if (!bHandled && Event == IE_Pressed)
	{
		bHandled = AIPointContextEdModeActions->ProcessCommandBindings(Key, FSlateApplication::Get().GetModifierKeys(), false);
	}

	return bHandled;
}

TSharedPtr<SWidget> FAIPointContextEdMode::GenerateContextMenu(FEditorViewportClient* InViewportClient) const
{
	FMenuBuilder MenuBuilder(true, NULL);

	MenuBuilder.PushCommandList(AIPointContextEdModeActions.ToSharedRef());
	MenuBuilder.BeginSection("AIPointContext Section");
	if (HasValidSelection())
	{
		TSharedRef<SWidget> LabelWidget =
			SNew(STextBlock)
			.Text(FText::FromString(FString::FromInt(CurrentSelectedIndex)))
			.ColorAndOpacity(FLinearColor::Green);
		MenuBuilder.AddWidget(LabelWidget, FText::FromString(TEXT("Point Index: ")));
		MenuBuilder.AddMenuSeparator();
		MenuBuilder.AddMenuEntry(FAIPointContextEditorCommands::Get().DeletePoint);
	}
	MenuBuilder.EndSection();
	MenuBuilder.PopCommandList();

	TSharedPtr<SWidget> MenuWidget = MenuBuilder.MakeWidget();
	return MenuWidget;
}

AAIPointContextManager* FAIPointContextEdMode::GetSelectedTargetPointActor() const
{
	TArray<UObject*> SelectedObjects;
	GEditor->GetSelectedActors()->GetSelectedObjects(SelectedObjects);
	if (SelectedObjects.Num() == 1)
	{
		return Cast<AAIPointContextManager>(SelectedObjects[0]);
	}
	return nullptr;
}

void FAIPointContextEdMode::AddPoint(EPointType PointType)
{
	AAIPointContextManager* Manager = GetSelectedTargetPointActor();
	if (Manager)
	{
		// this allows us to undo/redo
		const FScopedTransaction Transaction(FText::FromString("Add Point"));
		FEditorViewportClient* Client = (FEditorViewportClient*)GEditor->GetActiveViewport()->GetClient();
		// cast a ray to place sphere on geometry
		const FVector StartLocation = Client->GetViewLocation();
		const FVector EndLocation = Client->GetViewRotation().Vector() * 2000.0F;
		FHitResult HitResult;
		FVector NewPoint;
		const bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECollisionChannel::ECC_WorldStatic);
		if (bHit)
			NewPoint = HitResult.Location + (HitResult.Normal * DebugSphereRadius);
		else
			NewPoint = Client->GetViewLocation() + (Client->GetViewRotation().Vector() * DebugSphereRadius);
		Manager->Modify();
		Manager->SearchPoints.Add(NewPoint);
		SelectPoint(Manager, Manager->SearchPoints.Num() - 1);
	}
}

void FAIPointContextEdMode::AddPointAtLocation(FVector Location, EPointType PointType)
{
}

bool FAIPointContextEdMode::CanAddPoint(EPointType PointType) const
{
	return (GetSelectedTargetPointActor() != nullptr);
}

void FAIPointContextEdMode::RemovePoint()
{
	if (HasValidSelection())
	{
		const FScopedTransaction Transaction(FText::FromString("Remove Point"));

		// remove the point
		CurrentSelectedTarget->Modify();
		CurrentSelectedTarget->SearchPoints.RemoveAt(CurrentSelectedIndex);
		SelectPoint(nullptr, -1);
	}
}

bool FAIPointContextEdMode::CanRemovePoint()
{
	return HasValidSelection();
}

bool FAIPointContextEdMode::HasValidSelection() const
{
	return CurrentSelectedTarget.IsValid() && CurrentSelectedIndex >= 0 && CurrentSelectedIndex < CurrentSelectedTarget->SearchPoints.Num();
}

void FAIPointContextEdMode::SelectPoint(AAIPointContextManager* Actor, int32 Index)
{
	CurrentSelectedTarget = Actor;
	CurrentSelectedIndex = Index;

	// select this actor only
	if (CurrentSelectedTarget.IsValid())
	{
		GEditor->SelectNone(true, true);
		GEditor->SelectActor(CurrentSelectedTarget.Get(), true, true);
	}
}

void FAIPointContextEdMode::SetDebugSphereRadius(float Radius)
{
	DebugSphereRadius = Radius;
}

void FAIPointContextEdMode::SetHiddenDebugSpheresByType(bool Hidden, EPointType PointType)
{
	switch (PointType)
	{
		case EPointType::Search:
			bHiddenSearchPoints = Hidden;
			break;
		case EPointType::SoundTransfer:
			bHiddenSoundTransferPoints = Hidden;
			break;
		case EPointType::Patrol:
			bHiddenPatrolPoints = Hidden;
			break;
		default:
			break;
	}

	if (PointType == CurrentSelectedPointType)
	{
		SelectPoint(nullptr, -1);
	}
}

void FAIPointContextEdMode::GenerateSearchPoints(float Spread, float Extent, float MaxHeight, float SlopeThrehold)
{
	ClearSearchPoints();

	const FVector StartLoc = FVector(-Extent, -Extent, MaxHeight);
	int32 PointsCount = FMath::RoundToInt(Extent / Spread);

	AAIPointContextManager* Manager = GetSelectedTargetPointActor();
	if (Manager)
	{
		const FScopedTransaction Transaction(FText::FromString("Generate Search Points"));
		FEditorViewportClient* Client = (FEditorViewportClient*)GEditor->GetActiveViewport()->GetClient();
		Manager->Modify();

		for (int32 i = 0; i < PointsCount * 2; i++)
		{
			for (int32 j = 0; j < PointsCount * 2; j++)
			{
				FVector RayOrigin = StartLoc + (FVector::RightVector * i * Spread) + (FVector::ForwardVector * j * Spread);
				ECollisionChannel DefaultSightCollisionChannel = ECollisionChannel::ECC_WorldStatic;
				FHitResult HitResult;
				FCollisionQueryParams QueryParams;
				FCollisionObjectQueryParams ObjectParams;
				ObjectParams.AddObjectTypesToQuery(ECC_WorldStatic);
				const bool bHit = GetWorld()->GetWorld()->LineTraceSingleByObjectType(HitResult, RayOrigin,
					RayOrigin + (FVector::DownVector * Extent * 2.0F), ObjectParams, QueryParams);

				if (bHit)
				{
					if ((HitResult.Normal | FVector::UpVector) >= SlopeThrehold && HitResult.IsValidBlockingHit())
					{
						Manager->SearchPoints.Add(HitResult.ImpactPoint);
						//SelectPoint(Manager, Manager->SearchPoints.Num() - 1);
					}
				}
			}
		}
	}
}

void FAIPointContextEdMode::ClearSearchPoints()
{
	AAIPointContextManager* Manager = GetSelectedTargetPointActor();
	if (Manager)
	{
		const FScopedTransaction Transaction(FText::FromString("Clear Search Points"));
		Manager->Modify();
		Manager->SearchPoints.Empty();
	}

	SelectPoint(nullptr, -1);
}

