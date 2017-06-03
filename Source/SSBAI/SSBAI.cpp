// SSBAI.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <iostream>
#include "SSBAI.h"
#include "AiEngine.h"
#include "ReplayMemory.h"

#define SKIP_FRAME_MODULO 4
#define REPLAY_LEARN_COUNT 2

namespace ssbai
{

	unsigned long frame_counter = 0;

	bool enabled_pressed = false;
	bool m_engine_enabled = false;
	bool engine_enabled(MYBUTTONS controller) {
		
		if (!enabled_pressed && controller.L_TRIG == 1) {
			enabled_pressed = true;
			m_engine_enabled = !m_engine_enabled;
		}
		if (enabled_pressed && controller.L_TRIG == 0) {
			enabled_pressed = false;
		}
		return m_engine_enabled;	
	}

	// We must hold onto 'last' state and 'current state'
	// In each frame update, we have to get the environment reward and state. call these r1, s1.
	// The last state we were in was s0, the last action we used was a0.
	// Insert s0,a0,r1,s1 into replay memory
	// Global SSBAI objects
	AiEngine ai_engine;
	ReplayMemory replay_memory;

	// Below is our SARS (state, action, reward, state) set
	StateSharedPtr state(new State);
	ActionSharedPtr action(new Action);
	float reward = 0.0;
	StateSharedPtr last_state(new State);

	void env_pre_update(uint8_t *mem_offset, uint32_t *enemy_inputs)
	{
		last_state->copy(state);
		state->update(mem_offset, enemy_inputs);
		reward = state->get_reward();
	}

	void ai_update()
	{
		// Add the replay memory now that we know how the last frame went
		ExperienceSharedPtr e(new Experience(last_state, action, reward, state));
		replay_memory.addExperience(e);

		// Predict the next action!
		action = ai_engine.predict(state);

		// Teach the AI engine now
		ai_engine.adjustWeights(e);
		for (int i = 0; i<REPLAY_LEARN_COUNT; ++i) {
			e = replay_memory.sampleExperience();
			ai_engine.adjustWeights(e);
		}
	}

	void env_post_update(uint32_t *my_inputs) {
		action->apply(my_inputs);
	}

	// We only need to hold onto 'last' action. (i.e. just 'a')
	//
	// 
	// Then we select an action based on those, a1.
	//  
	void Hooks::frame_update(uint8_t *memory, uint32_t *controller1, uint32_t *controller2)
	{
		MYBUTTONS c2;
		c2.Value = *controller2;
		if (!engine_enabled(c2))
			return;

		// Apply the action to the game
		// We might want to enable next action every frame -- otherwise the controlls will be spastic
		action->apply(controller1);

		// We only want to update action every couple of frames, not every frame.
		frame_counter++;
		if (frame_counter % SKIP_FRAME_MODULO == 0) 
		{
			env_pre_update(memory, controller2);
			ai_update();
		}
		// The env_post_update is essentially just applying an action -- we do this regardless of AI_engine skipped frames
		env_post_update(controller1);
	}


}

