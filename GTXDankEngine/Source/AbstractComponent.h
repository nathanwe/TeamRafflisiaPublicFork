#ifndef ABSTRACTCOMPONENT_H
#define ABSTRACTCOMPONENT_H

#include <chrono>
#include "Enums.h"

class AbstractComponent
{
public:
	AbstractComponent();
	virtual ~AbstractComponent();
	virtual void Update(std::chrono::milliseconds dt) = 0;
	virtual void HandleEvent(Event& event) = 0;
	virtual ComponentType GetType() = 0;

private:
};


#endif