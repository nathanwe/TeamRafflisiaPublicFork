#pragma once

class Routine
{
public:
	Routine(uint32_t ent) : entityID(ent) {}
	virtual void Update(float deltaTime, InputManager* InputSys) = 0;


protected:
	uint32_t entityID;


};
