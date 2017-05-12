#pragma once
#include "State.h"

// Actions are essentially joystick movements and button presses
class Action {};
class Reward {};


class AiEngine
{
private:
	void *frame_buf;
	void *mem_buf;
	 
public:
	AiEngine();
	~AiEngine();
	Action next_action(State &state, Action &action);
};

