#pragma once
#include <GL/glew.h>
#include "camera.h"

class PainterState {
public:
	PainterState(Camera camera) : camera(camera)
	{}

	Camera camera;
};