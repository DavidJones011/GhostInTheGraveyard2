#pragma once

#include "Runtime/Core/Public/UObject/NameTypes.h"
#include "Runtime/Core/Public/Containers/UnrealString.h"

namespace FAIPerceptionTags
{
	const static FName DistinctNoiseTag = TEXT("Distinct_Noise");
	const static FName NoiseTag = TEXT("Noise");
}

namespace FBBKeys
{
	const static FName SelfActor       = TEXT("SelfActor");
	const static FName TargetLocation  = TEXT("TargetLocation");
	const static FName TargetActor     = TEXT("TargetActor");
	const static FName PlayerSeen      = TEXT("PlayerSeen");
	const static FName ActiveState     = TEXT("ActiveState");
	const static FName FoundBreakable  = TEXT("FoundBreakable");
}

namespace FAIPaths
{
	const static TCHAR* CreatureBehaviorTreePath = TEXT("BehaviorTree'/Game/AI/Creature/Behavior/BT_Creature.BT_Creature'");
}

namespace FAITags
{
	const static FName BreakableTag = TEXT("Breakable_Obstacle");
}