#pragma once
#include "Utility.h"
#include <memory>

class State;
typedef std::shared_ptr<State> StateSharedPtr;

class PlayerState {
public:
	unsigned damage;
	unsigned damage_delta;
	unsigned life_loss;
	Vector2 location;

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
	void copy(StateSharedPtr other);
	float get_reward();
};