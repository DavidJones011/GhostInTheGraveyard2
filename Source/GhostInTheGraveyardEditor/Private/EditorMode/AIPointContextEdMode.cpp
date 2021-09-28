
#include "EditorMode/AIPointContextEdMode.h"
#include "EditorMode/AIPointContextEdModeToolkit.h"
#include "Editor/UnrealEd/Public/Toolkits/ToolkitManager.h"
#include "ScopedTransaction.h"
#include "Engine/Selection.h"
#include "EditorModeManager.h"
#include "EngineUtils.h"
#include "PrimitiveSceneProxy.h"
#include "EditorMode/AIPointContextEditorObject.h"
#include "EditorMode/AIPointContextEditor.h"

FAIPointContextEdMode::FAIPointContextEdMode()
: FEdMode()
, UISettings(nullptr)
, bIsMultiSelecting(false)
, bDataChanged(false)
{
	FAIPointContextEditorCommands::Register();
	AIPointContextEdModeActions = MakeShareable(new FUICommandList);

	//FAIPointContextEditor& AIContextPointModule = FModuleManager::GetModuleChecked<FAIPointContextEditor>("AI Point Context Editor");

	UISettings = NewObject<UAIPointContextEditorObject>(GetTransientPackage(), TEXT("UISettings_AIPointContext"), RF_Transactional);
	UISettings->SetParent(this);
}

FAIPointContextEdMode::~FAIPointContextEdMode()
{
	Selection.Empty();
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

	Selection.Empty();

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

// 	if (Manager)
// 	{
// 		//SelectPoint(Manager, Manager->SearchPoints.Num() - 1, EPointType::Search);
// 		//CurrentSelectedTarget = Manager;
// 	}
}

void FAIPointContextEdMode::Exit()
{
	if (Toolkit.IsValid())
	{
		FToolkitManager::Get().CloseToolkit(Toolkit.ToSharedRef());
		Toolkit.Reset();
	}
	Selection.Empty();
	GEditor->SelectNone(false, true);

	// Redraw one last time to remove any landscape editor stuff from view
	GEditor->RedrawLevelEditingViewports();

	FEdMode::Exit();
}

void FAIPointContextEdMode::AddReferencedObjects(FReferenceCollector& Collector)
{
	FEdMode::AddReferencedObjects(Collector);
	Collector.AddReferencedObject(UISettings);
}

void FAIPointContextEdMode::Render(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI)
{
	const FColor SelectedColor(255, 255, 255);
	float DebugDrawDistSquared = DebugRenderDistance * DebugRenderDistance;

	UWorld* World = GetWorld();
	for (TActorIterator<AAIPointContextManager> It(World); It; ++It)
	{
		AAIPointContextManager* Actor = (*It);
		if(Actor)
		{
			FVector ActorLoc = Actor->GetTargetLocation();

// 			if (!bHiddenSearchPoints)
// 			{
// 				for (int i = 0; i < Actor->SearchPoints.Num(); ++i)
// 				{
// 					bool bSelected = (Actor == CurrentSelectedTarget && i == CurrentSelectedIndex);
// 					const FColor& Color = bSelected ? SelectedColor : SphereColors[0];
// 					FVector& Point = Actor->SearchPoints[i];				
// 					if (FVector::DistSquared(Point, View->ViewLocation) <= DebugDrawDistSquared)
// 					{
// 						PDI->SetHitProxy(new HAIPointContextProxy(Actor, i, EPointType::Search, -1));
// 						DrawWireSphere(PDI, FTransform(Point), Color, DebugSphereRadius, 10, SDPG_Foreground);
// 						PDI->SetHitProxy(NULL);
// 					}
// 				}
// 			}

			if (!bHiddenPatrolPoints)
			{
				int32 SectionNum = Actor->GetPatrolSectionNum();
				for (int32 Section = 0; Section < SectionNum; Section++)
				{
					if (bDataChanged) // data has changed render next frame
					{
						bDataChanged = false;
						return;
					}

					const TArray<FPatrolPointData>* PatrolData = Actor->GetPatrolPointData(Section);
					TArray<bool> Marked;
					Marked.AddZeroed(PatrolData->Num());
					TArray<int32> TraversalStack;
					for (int32 Index = 0; Index < PatrolData->Num(); Index++)
					{
						if (bDataChanged) // data has changed render next frame
							return;

						if(Marked[Index]) // already marked
							continue;

						TraversalStack.Push(Index);

						while (TraversalStack.Num() > 0)
						{
							if (bDataChanged) // data has changed render next frame
							{
								bDataChanged = false;
								return;
							}

							int32 Top = TraversalStack.Pop();
							const FPatrolPointData& Data = (*PatrolData)[Top];
							Marked[Top] = true;

							//bool bSelected = (Actor == CurrentSelectedTarget && Data.Index == CurrentSelectedIndex
								//&& Data.SectionId == CurrentPatrolSection && CurrentSelectedPointType == EPointType::Patrol);

							bool bSelected = false;
							const FColor& Color = bSelected ? SelectedColor : FColor::Magenta;
							const FVector& Point = Data.Location;

							if (Data.PriorLinkIndex != -1)
							{
								FPatrolPointData PriorData;
								if (!Marked[Data.PriorLinkIndex] && Actor->TryGetPatrolPointData(Data.PriorLinkIndex, Data.SectionId, PriorData))
								{						
									TraversalStack.Push(Data.PriorLinkIndex);
									PDI->DrawLine(Data.Location, PriorData.Location, Color, SDPG_Foreground);
									//DrawConnectedArrow(PDI, FTransform(PriorData.Location - Data.Location).ToMatrixNoScale(), Color, 50.0F, 50.0F, SDPG_Foreground);
								}
							}

							if (Data.NextLinkIndex != -1)
							{								
								FPatrolPointData NextData;
								if (!Marked[Data.NextLinkIndex] && Actor->TryGetPatrolPointData(Data.NextLinkIndex, Data.SectionId, NextData))
								{
									TraversalStack.Push(Data.NextLinkIndex);
									PDI->DrawLine(Data.Location, NextData.Location, Color, SDPG_Foreground);
									//DrawConnectedArrow(PDI, FTransform(NextData.Location - Data.Location).ToMatrixNoScale(), Color, 50.0F, 50.0F, SDPG_Foreground);
								}
							}

							FPlane ViewPlane = View->Project(Point);
							if (ViewPlane.X >= -1 && ViewPlane.X <= 1 && ViewPlane.Y >= -1 && ViewPlane.Y <= 1 && FVector::DistSquared(Point, View->ViewLocation) <= DebugDrawDistSquared)
							{
								PDI->SetHitProxy(new HAIPointContextProxy(Actor, Data.Index, EPointType::Patrol, Data.SectionId));
								DrawWireSphere(PDI, FTransform(Point), Color, DebugSphereRadius, 10, SDPG_Foreground);
								PDI->SetHitProxy(NULL);
							}
						}
					}
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
			AAIPointContextManager* Manager = Cast<AAIPointContextManager>(Proxy->RefObject);
			int32 Index = Proxy->Index;
			EPointType PointType = Proxy->PointType;

			int32 ArrayNum = 0;
			if (Manager)
			{
				switch (PointType)
				{
				case EPointType::Patrol:
					ArrayNum = Manager->GetPatrolSectionPatrolNum(Proxy->PatrolSection);
					break;
				case EPointType::Search:
					ArrayNum = Manager->SearchPoints.Num();
					break;
				case EPointType::SoundTransfer:
					break;
				}
			}

			if (Index >= 0 && Index < ArrayNum)
			{
				SelectPoint(Manager, Index, PointType, Proxy->PatrolSection);
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
			for (const SelectionData& SelectData : Selection)
			{
				SelectData.CurrentSelectedTarget->Modify();

				switch (SelectData.CurrentSelectedPointType)
				{
				case EPointType::Search:
					SelectData.CurrentSelectedTarget->SearchPoints[SelectData.CurrentSelectedIndex] += InDrag;
					break;
				case EPointType::Patrol:
					*SelectData.CurrentSelectedTarget->GetPatrolPointVectorPtr(SelectData.Section, SelectData.CurrentSelectedIndex) += InDrag;
					break;
				default:
					break;
				}
			}
		}
		return true;
	}

	return false;
}

FVector FAIPointContextEdMode::GetWidgetLocation() const
{
	if(Selection.Num() == 0)
		return FVector::ZeroVector;
	
	SelectionData LastSelection = Selection[Selection.Num()-1];

	if (HasValidSelection())
	{
		switch (LastSelection.CurrentSelectedPointType)
		{
		case EPointType::Search:
			return LastSelection.CurrentSelectedTarget->SearchPoints[LastSelection.CurrentSelectedIndex];
			break;
		case EPointType::SoundTransfer:
			break;
		case EPointType::Patrol:
		{
			FVector Loc = *LastSelection.CurrentSelectedTarget->GetPatrolPointVectorPtr(LastSelection.Section, LastSelection.CurrentSelectedIndex);
			return Loc;
		}
		break;
		default:
			break;
		}
	}
	return FEdMode::GetWidgetLocation();
}

bool FAIPointContextEdMode::InputKey(FEditorViewportClient* ViewportClient, FViewport* Viewport, FKey Key, EInputEvent Event)
{
	bool bHandled = false;

	if (!bHandled && Event == IE_Pressed)
	{
		bHandled = AIPointContextEdModeActions->ProcessCommandBindings(Key, FSlateApplication::Get().GetModifierKeys(), false);
	}

	// Multiselect with the shift key
	if ((Key == EKeys::LeftShift || Key == EKeys::RightShift) && Event == IE_Pressed)
	{
		bIsMultiSelecting = true;
	}
	else if ((Key == EKeys::LeftShift || Key == EKeys::RightShift) && Event == IE_Released)
	{
		bIsMultiSelecting = false;
	}

	return bHandled;
}

TSharedPtr<SWidget> FAIPointContextEdMode::GenerateContextMenu(FEditorViewportClient* InViewportClient) const
{
	FMenuBuilder MenuBuilder(true, NULL);

	/*MenuBuilder.PushCommandList(AIPointContextEdModeActions.ToSharedRef());
	MenuBuilder.BeginSection("AIPointContext Section");
	if (HasValidSelection())
	{
		TSharedRef<SWidget> LabelWidget =
			SNew(STextBlock)
			.Text(FText::FromString(FString::FromInt(CurrentSelectedIndex)))
			.ColorAndOpacity(FLinearColor::Green);
		MenuBuilder.AddWidget(LabelWidget, FText::FromString(TEXT("Point Index: ")));
		MenuBuilder.AddMenuSeparator();
		MenuBuilder.AddMenuEntry(FAIPointContextEditorCommands::Get().AddPoint);
	}
	MenuBuilder.EndSection();
	MenuBuilder.PopCommandList();*/

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
		

		int32 SelectDataSection = -1;
		if (Selection.Num() > 0)
		{
			SelectDataSection = Selection[Selection.Num() - 1].Section;
		}

		switch (PointType)
		{
			case EPointType::Patrol:
				if (SelectDataSection != -1)
				{				
					int32 Index = Manager->AddPatrolPointToSection(NewPoint, SelectDataSection);
					SelectPoint(Manager, Index, PointType, SelectDataSection);
				}
				else
				{
					//CurrentPatrolSection = Manager->CreatePatrolSection(NewPoint);
					SelectPoint(Manager, 0, PointType, Manager->CreatePatrolSection(NewPoint));
				}
				break;
			case EPointType::Search:
				Manager->SearchPoints.Add(NewPoint);
				SelectPoint(Manager, Manager->SearchPoints.Num() - 1, PointType);
				break;
			case EPointType::SoundTransfer:
				// TO BE IMPLEMENTED
				break;
		}

		Manager->Modify();
		bDataChanged = true;
	}
}

void FAIPointContextEdMode::AddPointAtLocation(FVector Location, EPointType PointType)
{
}

bool FAIPointContextEdMode::CanAddPoint(EPointType PointType) const
{
	return (GetSelectedTargetPointActor() != nullptr);
}

void FAIPointContextEdMode::RemovePoints()
{
	AAIPointContextManager* Manager = GetSelectedTargetPointActor();
	if (Manager)
	{
		const FScopedTransaction Transaction(FText::FromString("Remove Point"));
		for (const SelectionData& SelectData : Selection)
		{
			Manager->RemovePatrolPointFromSection(SelectData.CurrentSelectedIndex, SelectData.Section);
		}
		Manager->Modify();
		Selection.Empty();
		bDataChanged = true;
	}
}

bool FAIPointContextEdMode::CanRemovePoints() const
{
	if (Selection.Num() == 0 || !HasValidSelection())
		return false;

	return true;
}

bool FAIPointContextEdMode::HasValidSelection() const
{
	for (const SelectionData& SelectData : Selection)
	{
		const bool bValidTarget = SelectData.CurrentSelectedTarget.IsValid();
		if (bValidTarget)
		{
			switch (SelectData.CurrentSelectedPointType)
			{
			case EPointType::Search:
				return SelectData.CurrentSelectedIndex >= 0 && SelectData.CurrentSelectedIndex < SelectData.CurrentSelectedTarget->SearchPoints.Num();
			case EPointType::SoundTransfer:
				return false;
			case EPointType::Patrol:
				return SelectData.CurrentSelectedTarget->IsValid(SelectData.Section, SelectData.CurrentSelectedIndex);
			default:
				break;
			}
		}
	}
	return false;
}

void FAIPointContextEdMode::SelectPoint(AAIPointContextManager* Actor, int32 Index, EPointType PointType, int32 Section /*=-1*/ )
{
	SelectionData SelectData;
	SelectData.CurrentSelectedIndex = Index;
	SelectData.CurrentSelectedTarget = Actor;
	SelectData.CurrentSelectedPointType = PointType;
	SelectData.Section = Section;

	if (Selection.Num() > 1 && !bIsMultiSelecting)
	{
		Selection.Empty();
	}

	int32 OutIndex = 0;
	if (IsAlreadySelected(SelectData, OutIndex))
	{
		return;
	}

	if (Selection.Num() == 1 && !bIsMultiSelecting)
	{
		Selection[0] = MoveTemp(SelectData);
	}
	else
	{
		Selection.Push(MoveTemp(SelectData));
	}


	// select this actor only
	TWeakObjectPtr<AAIPointContextManager> SelectedActor = Selection[Selection.Num() - 1].CurrentSelectedTarget;
	if (SelectedActor.IsValid())
	{
		GEditor->SelectNone(true, true);
		GEditor->SelectActor(SelectedActor.Get(), true, true);
	}
}

void FAIPointContextEdMode::CreateLink()
{
	const FScopedTransaction Transaction(FText::FromString("Link Points"));

	AAIPointContextManager* Manager = GetSelectedTargetPointActor();

	if (Manager && SelectionNum() > 1)
	{
		SelectionData* PriorData = &Selection[0];
		for (int32 Index = 1; Index < Selection.Num(); Index++)
		{
			SelectionData* SelectData = &Selection[Index];
			Manager->LinkPatrolPoints(PriorData->CurrentSelectedIndex, SelectData->CurrentSelectedIndex, SelectData->Section, true);
			PriorData = SelectData;
		}
		Manager->Modify();
		bDataChanged = true;
	}
}

bool FAIPointContextEdMode::CanCreateLink() const
{
	if (SelectionNum() < 2 || !HasValidSelection())
		return false;

	return SelectionSharesSame(true);
}

void FAIPointContextEdMode::ClearLinks()
{
	AAIPointContextManager* Manager = GetSelectedTargetPointActor();
	if (Manager)
	{
		const FScopedTransaction Transaction(FText::FromString("Clear Links"));
		for (const SelectionData& SelectData : Selection)
		{
			Manager->RemoveLink(SelectData.CurrentSelectedIndex, SelectData.Section, Unlink_Both);
		}
		Manager->Modify();
		bDataChanged = true;
	}
}

bool FAIPointContextEdMode::CanClearLinks() const
{
	if(Selection.Num() == 0 || !HasValidSelection())
		return false;

	return true;
}

void FAIPointContextEdMode::SetDebugSphereRadius(float Radius)
{
	DebugSphereRadius = Radius;
}

void FAIPointContextEdMode::SetDebugDrawDistance(float Distance)
{
	DebugRenderDistance = Distance;
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
		Manager->SearchPoints.Empty();
		bDataChanged = true;
		SelectPoint(nullptr, -1, EPointType::Search);
		Manager->Modify();
	}
}

bool FAIPointContextEdMode::SelectionSharesSame(bool bCompareSection /*= false*/) const
{
	if(GetSelectedTargetPointActor() == nullptr || Selection.Num() == 0)
		return false;

	if(Selection.Num() == 1)
		return true;

	EPointType FoundType = Selection[0].CurrentSelectedPointType;
	AAIPointContextManager* FoundTarget = Selection[0].CurrentSelectedTarget.Get();
	int32 FoundSection = Selection[0].Section;
	for (int32 i = 1; i < Selection.Num(); i++)
	{
		const SelectionData& Data = Selection[i];
		if((Data.CurrentSelectedPointType != FoundType) || 
			(Data.CurrentSelectedTarget != FoundTarget) ||
			(bCompareSection && (Data.Section != FoundSection)))
			return false;
	}

	return true;
}

bool FAIPointContextEdMode::IsAlreadySelected(const SelectionData& InData, int32& OutIndex)
{
	bool bExists = false;
	OutIndex = -1;
	int32 Index = 0;
	for (const SelectionData& Data : Selection)
	{
		if (InData.CurrentSelectedIndex == Data.CurrentSelectedIndex &&
			InData.CurrentSelectedPointType == Data.CurrentSelectedPointType &&
			InData.CurrentSelectedTarget == Data.CurrentSelectedTarget &&
			InData.Section == Data.Section)
		{
			bExists = true;
			OutIndex = Index;
		}

		Index++;
	}
	return bExists;
}

TSharedRef<FUICommandList> FAIPointContextEdMode::GetUICommandList() const
{
	check(Toolkit.IsValid());
	return Toolkit->GetToolkitCommands();
}
