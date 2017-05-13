#pragma once
#include "Utility.h"

class PlayerState {
	void *frame_buffer;
	unsigned damage;
	unsigned life_loss;
	Vector2 location;

public:
	PlayerState();
	void update_p1(uint8_t *memory_offset);
	void update_p2(uint8_t *memory_offset);
};

class State {
private:
	// Enemy data structure
	PlayerState enemy_state;
	// My data structures
	PlayerState my_state;
	MYBUTTONS enemy_buttons;

public:
	~State();
	State();
	void update(uint8_t * memory_offset, uint32_t * enemy_inputs);
};

