#include "stdafx.h"
#include "Action.h"


Action::Action()
{
	buttons = { 0 };
	srand(12345);
}


Action::~Action()
{
}

void Action::apply(uint32_t *controller)
{
	*controller = buttons.Value;
}


void Action::Attack()
{
	buttons.A_BUTTON = 1;
}

void Action::Move(Vector2 direction)
{
	buttons.X_AXIS = direction.discrete_x();
	buttons.Y_AXIS = direction.discrete_y();
}

void Action::Jump()
{
	buttons.D_CBUTTON = 1;
}

void Action::Shield()
{
	buttons.Z_TRIG = 1;
}

void Action::update() {
	buttons.Value = rand();
}
