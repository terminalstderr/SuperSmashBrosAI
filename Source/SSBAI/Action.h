#pragma once

#include "Utility.h"
#include <memory>


class Action
{
private:
	// My 
	MYBUTTONS buttons;

public:
	Action();
	~Action();
	void apply(uint32_t *controller);
	// We have two options as far as I see -- we can have very discrete actions then we make a function for each -- i.e. void punch(), void kick(), etc...'
	// Alternately, we might just update ut
	// We have a directionality action for  every time step
	// We have an attack action for every time step
	
	// ---- Minimal Set ----
	// Sprint (left,right)
	// Jump (left, up, right) -- can only jump 6 times in sequence
	// Smash (left, right, up, down)
	// Attack 
	// Sprint (2)
	// Walk (2)
	// Smash (4)
	// Special (2)
	// Attack (4)
	// Jump

	// Attack
	// Move
	// Jump
	// Sheild
	void Attack();
	void Move(Vector2 direction);
	void Jump();
	void Shield();



	void update();
};

typedef std::shared_ptr<Action> ActionSharedPtr;