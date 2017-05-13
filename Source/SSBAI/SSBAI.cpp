// SSBAI.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <iostream>
#include "SSBAI.h"
#include "AiEngine.h"

#define SKIP_FRAME_MODULO 4

namespace ssbai
{
	// Global SSBAI objects
	AiEngine ai_engine;
	State current_state;
	Action *action;
	unsigned long frame_counter = 0;

	void Hooks::frame_update(uint8_t *memory, uint32_t *controller1, uint32_t *controller2)
	{
		if (frame_counter == 0) {
			action = ai_engine.get_next_action();
		}
		// Apply the action to the game
		// We might want to enable next action every frame -- otherwise the controlls will be spastic
		action->apply(controller1);

		// We only want to update action every couple of frames, not every frame.
		frame_counter++;
		if (frame_counter % SKIP_FRAME_MODULO != 0)
			return;

		// Update the current state (just getting information from memory space of video game)
		current_state.update(memory, controller2);

		
		// need to retain the action from the last step...
		action = ai_engine.next_action(current_state, *action);
		

		// On every frame, we will do 
		// ai_engine.step()
		// When training -- 
		// 1. the step will perform a prediction based on the state
		// 2. the step will perform an update based on 
		// TODO: ai_engine.predict();
		// TODO: ai_engine.update();
	}
}

