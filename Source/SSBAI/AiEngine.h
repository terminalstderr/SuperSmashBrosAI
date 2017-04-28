#pragma once
// This is going to be a frame, the player locations, the velocities

#define N64_MEMORY_OFFSET			0x00000000
// Camera Datastructure -- if we set the value to around 10000.00 it will be pretty far away
#define N64_DM_CAMERA_ZOOM			0x3c4214ac
// Player locations information is around 
// Player 1
#define N64_DM_PLAYER1_LOCATION_X	0x3c55b528
#define N64_DM_PLAYER1_LOCATION_Y	0x3c55b52c
#define N64_DM_PLAYER1_DAMAGE		0x3c55b4d4
#define N64_DM_PLAYER1_DEATHS		0x3c394d58
// Player 2
#define N64_DM_PLAYER2_LOCATION_X	0x3c55ab84
#define N64_DM_PLAYER2_LOCATION_Y	0x3c55ab88
#define N64_DM_PLAYER2_DAMAGE		0x3c55c024
#define N64_DM_PLAYER2_DEATHS		0x3c394dac

#define GET_FLOAT(x) *((float*)x)
#define GET_UINT(x) *((unsigned*)x)


class Vector2 {
private:
	float x, y;

public:
	Vector2();
	void update(float x, float y);
};

class PlayerState {
	void *frame_buffer;
	unsigned damage;
	unsigned life_loss;
	Vector2 location;

public: 
	PlayerState();
	void update_p1();
	void update_p2();
};

class State {
private:
	// Enemy data structure
	PlayerState enemy_state;
	// My data structures
	PlayerState my_state;

public:
	State();
	void update();
};
// Actions are essentially joystick movements and button presses
class Action {};
class Reward {};


class AiEngine
{
private:
	void *frame_buf;
	void *mem_buf;
	 
public:
	AiEngine();
	~AiEngine();
	void set_frame_buffer(void **buf);
	void set_mem_buffer(void **buf);
	Action next_action(State &state, Action &action);
};

