#pragma once
#include "Utility.h"
#include <memory>
#include <vector>
#include <omp.h>

class State;
typedef std::shared_ptr<State> StateSharedPtr;

class PlayerState {
public:
	unsigned damage;
	unsigned damage_delta;
	unsigned life_loss;
	Vector2 last_location;
	Vector2 location;
	Vector2 last_velocity;
	Vector2 velocity;
	Vector2 acceleration;

	PlayerState();
	void update_p1(uint8_t *memory_offset, double delta_time);
	void update_p2(uint8_t *memory_offset, double delta_time);
private:
	// This is a helper that takes all of the raw memory addresses and actually updates the internal state of this struct
	void update(uint8_t *life_loss_addr, uint8_t *damage_addr, uint8_t *location_x_addr, uint8_t *location_y_addr, double delta_time);
};

class State {
private:
	// Enemy data structure
	PlayerState enemy_state;
	// My data structures
	PlayerState my_state;
	MYBUTTONS enemy_buttons;
	double last_frame_time = omp_get_wtime();;

public:
	~State();
	State();
	void update(uint8_t * memory_offset, uint32_t * enemy_inputs);
	void copy(StateSharedPtr other);
	float get_reward();
	std::shared_ptr<std::vector<float>> get_buttons();
	std::shared_ptr<std::vector<float>> get_locations();
	std::shared_ptr<std::vector<float>> get_velocities();
	std::shared_ptr<std::vector<float>> get_accelerations();
	std::shared_ptr<std::vector<float>> get_player_distance();
	std::shared_ptr<std::vector<float>> get_damages();
};