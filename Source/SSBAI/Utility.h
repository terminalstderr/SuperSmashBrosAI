#pragma once
#include <cstdint>

// Camera Datastructure -- if we set the value to around 10000.00 it will be pretty far away
//#define N64_DM_CAMERA_ZOOM			0x339d08ac//0x3c4214ac
// Player locations information is around 
// Player 1

#define N64_DM_PLAYER1_LOCATION_X	0x0026b528//0x000a7ac8//0x35f11848//0x33b0a928//0x3c55b528
#define N64_DM_PLAYER1_LOCATION_Y	0x0026b52c//0x000a7acc//0x35f1184c//0x33b0a92c//0x3c55b52c
#define N64_DM_PLAYER1_DAMAGE		0x000a4d74//0x35f0eaf4//0x33b0a8d4//0x3c55b4d4
#define N64_DM_PLAYER1_DEATHS		0x000a4d38// OR a4d58 //0x35d48378//0x33944158//0x3c394d58
// Player 2
#define N64_DM_PLAYER2_LOCATION_X	0x0026c078//0x35fae1a4//0x33b09f84//0x3c55ab84
#define N64_DM_PLAYER2_LOCATION_Y	0x0026c07c//0x35fae1a8//0x33b09f88//0x3c55ab88
#define N64_DM_PLAYER2_DAMAGE		0x000a4de8//0x35f0f644//0x33b0b424//0x3c55c024
#define N64_DM_PLAYER2_DEATHS		0x000a4dac//0x35d483cc//0x339441ac//0x3c394dac
// Need to add 15DD0c00 in total -- what is the memory offset? 
// Memory offset -- 0x067b00c0
//                  0x067200c0
// We are off by F620B40

// Reasonable assumption that the game won't have damage higher than 500 
#define MAX_DAMAGE 500
// These are the min and max locations for Fox's level
#define MAX_LOCATION_RADIUS_X 14500
#define MAX_LOCATION_RADIUS_Y 11000

#define GET_FLOAT(x) *((float*)x)
#define GET_UINT(x) *((unsigned*)x)
class State;
class Action;


class Vector2 {
public:
	float x, y;

	Vector2();
	void update(float x, float y);

	Vector2 operator-(const Vector2 &rhs);
	Vector2 operator/(const double rhs);

	uint8_t discrete_x();
	uint8_t discrete_y();
};

typedef union
{
	uint32_t Value;
	struct
	{
		unsigned R_DPAD : 1;
		unsigned L_DPAD : 1;
		unsigned D_DPAD : 1;
		unsigned U_DPAD : 1;
		unsigned START_BUTTON : 1;
		unsigned Z_TRIG : 1;
		unsigned B_BUTTON : 1;
		unsigned A_BUTTON : 1;

		unsigned R_CBUTTON : 1;
		unsigned L_CBUTTON : 1;
		unsigned D_CBUTTON : 1;
		unsigned U_CBUTTON : 1;
		unsigned R_TRIG : 1;
		unsigned L_TRIG : 1;
		unsigned Reserved1 : 1;
		unsigned Reserved2 : 1;

		signed   Y_AXIS : 8;

		signed   X_AXIS : 8;
	};
} MYBUTTONS;
