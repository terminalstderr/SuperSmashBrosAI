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
	return &m_current_action;
}

Action *AiEngine::next_action(State &state, Action &action)
{
	m_state = state;
	m_current_action = action;
	// Each turn perform a movement
	//m_current_action.Move();
	// Each turn perform choose to perform one, two, or three of attack, jump, or shield
	m_current_action.Shield();
	m_current_action.Attack();
	m_current_action.Jump();

	//
	//m_next_action.update();
	return &m_current_action;
}
