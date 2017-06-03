#include "stdafx.h"
#include "State.h"
#include "Utility.h"


void PlayerState::update_p1(uint8_t *offset)
{
	// For some reason, we have to compute the addresses in their own expression -- this probably has something to do with the usage of the 'GET_XXX' macros.
	// If you do try GET_FLOAT(offset + N64_XXX) then you will see "Access violation reading location XXXX" in the Output window of Visual Studio
	uint8_t *life_loss_addr = offset + N64_DM_PLAYER1_DEATHS;
	uint8_t *damage_addr = offset + N64_DM_PLAYER1_DAMAGE;
	uint8_t *location_x_addr = offset + N64_DM_PLAYER1_LOCATION_X;
	uint8_t *location_y_addr = offset + N64_DM_PLAYER1_LOCATION_Y;
	location.update(GET_FLOAT(location_x_addr), GET_FLOAT(location_y_addr));
	damage = GET_UINT(damage_addr);
	life_loss = GET_UINT(life_loss_addr);
}

void PlayerState::update_p2(uint8_t *offset)
{
	// For some reason, we have to compute the addresses in their own expression -- this probably has something to do with the usage of the 'GET_XXX' macros.
	// If you do try GET_FLOAT(offset + N64_XXX) then you will see "Access violation reading location XXXX" in the Output window of Visual Studio
	uint8_t *life_loss_addr = offset + N64_DM_PLAYER2_DEATHS;
	uint8_t *damage_addr = offset + N64_DM_PLAYER2_DAMAGE;
	uint8_t *location_x_addr = offset + N64_DM_PLAYER2_LOCATION_X;
	uint8_t *location_y_addr = offset + N64_DM_PLAYER2_LOCATION_Y;
	location.update(GET_FLOAT(location_x_addr), GET_FLOAT(location_y_addr));
	damage = GET_UINT(damage_addr);
	life_loss = GET_UINT(life_loss_addr);
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
	my_state.update_p1(memory_offset);
	enemy_state.update_p2(memory_offset);
	enemy_buttons.Value = *enemy_inputs;

}

void State::copy(StateSharedPtr other)
{
	this->my_state = other->my_state;
	this->enemy_buttons = other->enemy_buttons;
	this->enemy_state = other->enemy_state;
}

PlayerState::PlayerState()
{
}