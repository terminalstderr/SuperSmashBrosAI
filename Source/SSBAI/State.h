#pragma once
#include "Utility.h"

class PlayerState {
	void *frame_buffer;
	unsigned damage;
	unsigned life_loss;
	Vector2 location;

public:
	PlayerState();
	void update_p1(void *memory_offset);
	void update_p2(void *memory_offset);
};

class State {
private:
	// Enemy data structure
	PlayerState enemy_state;
	// My data structures
	PlayerState my_state;

public:
	~State();
	State();
	void update(void *memory_offset);
};

