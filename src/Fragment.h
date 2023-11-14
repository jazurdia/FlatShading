#pragma once
#include <SDL2/SDL.h>
#include <glm/vec3.hpp>
#include "Color.h"

struct Fragment {
    // X and Y positions
    glm::vec3 position;
    Color color;
    float intensity;
};
