// SSBAI.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <iostream>
#include "SSBAI.h"


namespace ssbai
{
	// Global SSBAI objects
	AI ai_engine;
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
		// TODO: ai_engine.predict();
		// TODO: ai_engine.update();
	}


	AI::AI() {
	}

	void AI::set_frame_buffer(void ** buf)
	{
		this->frame_buf = *buf;
	}

	void AI::set_mem_buffer(void ** buf)
	{
		this->mem_buf = *buf;
	}

}

