// SSBAI.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <iostream>
#include "SSBAI.h"
#include "AiEngine.h"


namespace ssbai
{
	// Global SSBAI objects
	AiEngine ai_engine;
	State current_state;
	Action current_action;
	Action next_action;
	bool initialized = false;

	void Hooks::initialize(void** frame_buffer, void **mem_buffer)
	{
		if (initialized) 
		{
			return;
		}
		ai_engine.set_frame_buffer(frame_buffer);
		ai_engine.set_mem_buffer(mem_buffer);
		initialized = true;
	}

	void Hooks::frame_update() 
	{
		// Need to construct the current state
		current_state.update();
		// need to retain the action from the last step...
		next_action = ai_engine.next_action(current_state, current_action);
		// On every frame, we will do 
		// ai_engine.step()
		// When training -- 
		// 1. the step will perform a prediction based on the state
		// 2. the step will perform an update based on 
		// TODO: ai_engine.predict();
		// TODO: ai_engine.update();
	}
}

