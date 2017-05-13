#include "stdafx.h"
#include "AiEngine.h"
#include "Utility.h"


AiEngine::AiEngine()
{
}


AiEngine::~AiEngine()
{
}

Action * AiEngine::get_next_action()
{
	return &m_next_action;
}

Action *AiEngine::next_action(State &state, Action &action)
{
	m_state = state;
	m_current_action = action;
	m_next_action.update();
	return &m_next_action;
}
