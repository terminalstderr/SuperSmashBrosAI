#include "stdafx.h"
#include "State.h"
#include "Utility.h"
#include <algorithm>
#include <omp.h>


void PlayerState::update_p1(uint8_t *offset, double delta_time)
{
	// For some reason, we have to compute the addresses in their own expression -- this probably has something to do with the usage of the 'GET_XXX' macros.
	// If you do try GET_FLOAT(offset + N64_XXX) then you will see "Access violation reading location XXXX" in the Output window of Visual Studio
	uint8_t *life_loss_addr = offset + N64_DM_PLAYER1_DEATHS;
	uint8_t *damage_addr = offset + N64_DM_PLAYER1_DAMAGE;
	uint8_t *location_x_addr = offset + N64_DM_PLAYER1_LOCATION_X;
	uint8_t *location_y_addr = offset + N64_DM_PLAYER1_LOCATION_Y;
	update(life_loss_addr, damage_addr, location_x_addr, location_y_addr, delta_time);
}

void PlayerState::update_p2(uint8_t *offset, double delta_time)
{
	// For some reason, we have to compute the addresses in their own expression -- this probably has something to do with the usage of the 'GET_XXX' macros.
	// If you do try GET_FLOAT(offset + N64_XXX) then you will see "Access violation reading location XXXX" in the Output window of Visual Studio
	uint8_t *life_loss_addr = offset + N64_DM_PLAYER2_DEATHS;
	uint8_t *damage_addr = offset + N64_DM_PLAYER2_DAMAGE;
	uint8_t *location_x_addr = offset + N64_DM_PLAYER2_LOCATION_X;
	uint8_t *location_y_addr = offset + N64_DM_PLAYER2_LOCATION_Y;
	update(life_loss_addr, damage_addr, location_x_addr, location_y_addr, delta_time);
}

void PlayerState::update(uint8_t * life_loss_addr, uint8_t * damage_addr, uint8_t * location_x_addr, uint8_t * location_y_addr, double delta_time)
{
	// 
	location.update(GET_FLOAT(location_x_addr), GET_FLOAT(location_y_addr));
	velocity = (location - last_location) / delta_time;
	acceleration = (velocity - last_velocity) / delta_time;
	damage = GET_UINT(damage_addr);
	life_loss = GET_UINT(life_loss_addr);
	// Store these for euler differentiated derived metrics (velocity, acceleration
	last_location = location;
	last_velocity = velocity;
}

State::~State()
{
}

State::State() :
	enemy_state(),
	my_state()
{
}

void State::update(uint8_t *memory_offset, uint32_t *enemy_inputs)
{
	double this_frame_time = omp_get_wtime();;
	double delta = this_frame_time - last_frame_time;
	my_state.update_p1(memory_offset, delta);
	enemy_state.update_p2(memory_offset, delta);
	enemy_buttons.Value = *enemy_inputs;
	last_frame_time = this_frame_time;
}

void State::copy(StateSharedPtr other)
{
	this->my_state = other->my_state;
	this->enemy_buttons = other->enemy_buttons;
	this->enemy_state = other->enemy_state;
}

float State::get_reward()
{
	// Reward is 1/20th of the damage ratio plus the enemy life loss
	return 0.05 * (enemy_state.damage_delta / my_state.damage_delta) + enemy_state.life_loss;
}

std::shared_ptr<std::vector<float>> State::get_buttons()
{
	std::shared_ptr<std::vector<float>> ret = std::shared_ptr<std::vector<float>>(new std::vector<float>());
	
	// For simple buttons, we simple cast them to float (they will be either 0.0 or 1.0)
	ret->push_back(static_cast<float>(enemy_buttons.A_BUTTON));
	ret->push_back(static_cast<float>(enemy_buttons.B_BUTTON));
	ret->push_back(static_cast<float>(enemy_buttons.Z_TRIG));
	ret->push_back(static_cast<float>(enemy_buttons.R_TRIG));
	ret->push_back(static_cast<float>(enemy_buttons.L_TRIG));
	
	// For 'or' over buttons, we get the max of "all of the buttons summed" against "1.0"
	// Notice: Usage of extra paranthesis in (std::max) to ensure it does not get expanded to the macro defined in windows.h
	ret->push_back((std::min)(static_cast<float>(enemy_buttons.D_CBUTTON + enemy_buttons.L_CBUTTON + enemy_buttons.R_CBUTTON + enemy_buttons.U_CBUTTON), 1.0f));
	
	// For analog-stick, we normalize from the range of -128, 128 -> 0.0, 1.0
	ret->push_back((enemy_buttons.X_AXIS + 128) / 256.0f);
	ret->push_back((enemy_buttons.Y_AXIS + 128) / 256.0f);
	return ret;
}

std::shared_ptr<std::vector<float>> State::get_locations()
{
	std::shared_ptr<std::vector<float>> ret = std::shared_ptr<std::vector<float>>(new std::vector<float>());
	// We have MAX_RADIUS defined which is essentially the maximum amount of space (negative or positive) that a player can move on the screen.
	// On Fox's level this appears to be around -10,000 to 10,000.
	// The math is to normalize the location to the range of (0.0, 1.0) 
	// We may want to do a 'min-clamp' to ensure it doesn't go over 1.0, but this should be fine without
	ret->push_back((enemy_state.location.x + MAX_LOCATION_RADIUS_X) / (2 * MAX_LOCATION_RADIUS_X));
	ret->push_back((enemy_state.location.y + MAX_LOCATION_RADIUS_Y) / (2 * MAX_LOCATION_RADIUS_Y));
	ret->push_back((my_state.location.x + MAX_LOCATION_RADIUS_X) / (2 * MAX_LOCATION_RADIUS_X));
	ret->push_back((my_state.location.y + MAX_LOCATION_RADIUS_Y) / (2 * MAX_LOCATION_RADIUS_Y));
	return ret;
}

std::shared_ptr<std::vector<float>> State::get_velocities()
{
	std::shared_ptr<std::vector<float>> ret = std::shared_ptr<std::vector<float>>(new std::vector<float>());
	// TODO We may want to do a 'min-clamp' to ensure it doesn't go over 1.0 (this metric and Acceleration will be particularly prone to large spikes!)
	ret->push_back((enemy_state.velocity.x + MAX_VELOCITY) / (2 * MAX_VELOCITY));
	ret->push_back((enemy_state.velocity.y + MAX_VELOCITY) / (2 * MAX_VELOCITY));
	ret->push_back((my_state.velocity.x + MAX_VELOCITY) / (2 * MAX_VELOCITY));
	ret->push_back((my_state.velocity.y + MAX_VELOCITY) / (2 * MAX_VELOCITY));
	return ret;
}

std::shared_ptr<std::vector<float>> State::get_accelerations()
{
	std::shared_ptr<std::vector<float>> ret = std::shared_ptr<std::vector<float>>(new std::vector<float>());
	// TODO We may want to do a 'min-clamp' to ensure it doesn't go over 1.0 (this metric and Velocity will be particularly prone to large spikes!)
	ret->push_back((enemy_state.acceleration.x + MAX_ACCELERATION) / (2 * MAX_ACCELERATION));
	ret->push_back((enemy_state.acceleration.y + MAX_ACCELERATION) / (2 * MAX_ACCELERATION));
	ret->push_back((my_state.acceleration.x + MAX_ACCELERATION) / (2 * MAX_ACCELERATION));
	ret->push_back((my_state.acceleration.y + MAX_ACCELERATION) / (2 * MAX_ACCELERATION));
	return ret;
}

std::shared_ptr<std::vector<float>> State::get_player_distance()
{
	std::shared_ptr<std::vector<float>> ret = std::shared_ptr<std::vector<float>>(new std::vector<float>());
	// See note about doing a 'min clamp' in get_locations()
	Vector2 distance = enemy_state.location - my_state.location;
	ret->push_back((distance.x + MAX_LOCATION_RADIUS_X) / (2*MAX_LOCATION_RADIUS_X));
	ret->push_back((distance.y + MAX_LOCATION_RADIUS_Y) / (2*MAX_LOCATION_RADIUS_Y));
	return ret;
}

std::shared_ptr<std::vector<float>> State::get_damages()
{ 
	// This is the overall damage delt to each player
	std::shared_ptr<std::vector<float>> ret = std::shared_ptr<std::vector<float>>(new std::vector<float>());
	
	// We have a MAX_DAMAGE defined which is a reasonable maximum for damage (might want to do a 'min-clamp' to ensure it doesn't go over 1.0, but that is not strictly nessecary)
	ret->push_back(static_cast<float>(enemy_state.damage) / MAX_DAMAGE);
	ret->push_back(static_cast<float>(my_state.damage) / MAX_DAMAGE);

	return ret;
}

PlayerState::PlayerState()
{
}