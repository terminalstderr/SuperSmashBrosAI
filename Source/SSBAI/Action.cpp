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


void Action::update() {
	buttons.Value = rand();
}
