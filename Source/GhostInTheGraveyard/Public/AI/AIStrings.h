#pragma once

#include "Runtime/Core/Public/UObject/NameTypes.h"
#include "Runtime/Core/Public/Containers/UnrealString.h"

namespace FAIPerceptionTags
{
	const static FName FaucetNoise = TEXT("Trap_Noise");
}

namespace FBBKeys
{
	const static FName SelfActor = TEXT("SelfActor");
	const static FName TargetLocation = TEXT("TargetLocation");
	const static FName TargetActor = TEXT("TargetActor");
	const static FName InvestigateState = TEXT("InvestigateState");
	const static FName IsSearching = TEXT("IsSearching");
}

namespace FAIPaths
{
	const static TCHAR* CreatureBehaviorTreePath = TEXT("BehaviorTree'/Game/AI/Creature/Behavior/BT_Creature.BT_Creature'");
}