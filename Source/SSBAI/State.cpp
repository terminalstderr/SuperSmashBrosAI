#include "stdafx.h"
#include "State.h"
#include "Utility.h"


void PlayerState::update_p1(void *memory_offset)
{
	long offset = GET_UINT(memory_offset);
	long life_loss_addr = offset + N64_DM_PLAYER1_DEATHS;
	long damage_addr = offset + N64_DM_PLAYER1_DAMAGE;
	long location_x_addr = offset + N64_DM_PLAYER1_LOCATION_X;
	long location_y_addr = offset + N64_DM_PLAYER1_LOCATION_Y;
	location.update(GET_FLOAT(location_x_addr), GET_FLOAT(location_y_addr));
	damage = GET_UINT(damage_addr);
	life_loss = GET_UINT(life_loss_addr);
}

void PlayerState::update_p2(void *memory_offset)
{
	long offset = GET_UINT(memory_offset);
	long life_loss_addr = offset + N64_DM_PLAYER2_DEATHS;
	long damage_addr = offset + N64_DM_PLAYER2_DAMAGE;
	long location_x_addr = offset + N64_DM_PLAYER2_LOCATION_X;
	long location_y_addr = offset + N64_DM_PLAYER2_LOCATION_Y;
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

void State::update(void *memory_offset)
{
	my_state.update_p1(memory_offset);
	enemy_state.update_p2(memory_offset);
}

PlayerState::PlayerState()
{
}