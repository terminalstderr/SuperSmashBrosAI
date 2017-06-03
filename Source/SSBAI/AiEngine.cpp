#include "stdafx.h"
#include "AiEngine.h"
#include "Utility.h"


AiEngine::AiEngine()
{
}


AiEngine::~AiEngine()
{
}

ActionSharedPtr AiEngine::predict(const StateSharedPtr state)
{
	// TODO
	ActionSharedPtr a = ActionSharedPtr(new Action());
	a->Attack();
	return a;
}

void AiEngine::adjustWeights(const ExperienceSharedPtr exp)
{
	// TODO
}
