#ifndef RAY_CASTING_H
#define RAY_CASTING_H

#include "pch.h"
#include "../../../Core/Camera.h"
#include "../utils/common.h"

glm::vec3 GetDirectionFromCameraCursor(Camera& camera);
int RayCast(Camera& camera, bool isCenter = true);

#endif
