#pragma once
#include "State.h"
#include "Action.h"

class AiEngine
{
private:
	Action m_current_action;
	State m_state;
public:
	AiEngine();
	~AiEngine();
	Action *get_next_action();
	Action *next_action(State &state, Action &action);
};

