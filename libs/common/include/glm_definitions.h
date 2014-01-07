#pragma once
#ifndef __GLM_DEFINITIONS_H__
#define __GLM_DEFINITIONS_H__

//#define GLM_MESSAGES
#define GLM_SWIZZLE GLM_SWIZZLE_XYZW
#include <glm/glm.hpp>
#include <glm/core/type_vec.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/projection.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>

// axes
const glm::vec3 X_AXIS(1.0f, 0.0f, 0.0f);
const glm::vec3 Y_AXIS(0.0f, 1.0f, 0.0f);
const glm::vec3 Z_AXIS(0.0f, 0.0f, 1.0f);

#endif // __GLM_DEFINITIONS_H__