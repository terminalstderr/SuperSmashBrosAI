#include "stdafx.h"
#include "AiEngine.h"


AiEngine::AiEngine()
{
}


AiEngine::~AiEngine()
{
}

void AiEngine::set_frame_buffer(void **buf)
{
	this->frame_buf = *buf;
}

void AiEngine::set_mem_buffer(void **buf)
{
	this->mem_buf = *buf;
}

Action AiEngine::next_action(State & state, Action & action)
{
	// TODO Actual AI step
	return Action();
}

void PlayerState::update_p1()
{
	location.update(GET_FLOAT(N64_DM_PLAYER1_LOCATION_X), GET_FLOAT(N64_DM_PLAYER1_LOCATION_Y));
	damage = GET_UINT(N64_DM_PLAYER1_DAMAGE);
	life_loss = GET_UINT(N64_DM_PLAYER1_DEATHS);
	frame_buffer = NULL;
}

void PlayerState::update_p2()
{
	location.update(GET_FLOAT(N64_DM_PLAYER2_LOCATION_X), GET_FLOAT(N64_DM_PLAYER2_LOCATION_Y));
	damage = GET_UINT(N64_DM_PLAYER2_DAMAGE);
	life_loss = GET_UINT(N64_DM_PLAYER2_DEATHS);
	frame_buffer = NULL;
}

Vector2::Vector2()
{
	this->x = 0.0f;
	this->y = 0.0f;
}

void Vector2::update(float x, float y)
{
	this->x = x;
	this->y = y;
}

State::State() :
	enemy_state(),
	my_state()
{
}

void State::update()
{
	my_state.update_p1();
	enemy_state.update_p2();
}

PlayerState::PlayerState()
{
}
