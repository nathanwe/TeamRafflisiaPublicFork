#ifndef PATHFINDER_H
#define PATHFINDER_H

#include "pch.h"

class Pathfinder
{
public:
	glm::vec3 GetNextWaypoint(glm::vec3 start, glm::vec3 end);
};

#endif // !PATHFINDER_H

