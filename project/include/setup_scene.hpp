#ifndef SETUP_KART_HPP
#define SETUP_KART_HPP

#include "Viewer.hpp"
#include "dynamics/DynamicSystem.hpp"
#include "dynamics/DynamicSystemRenderable.hpp"

void initialize_scene( Viewer& viewer );

void setup_mountains(Viewer &viewer, ShaderProgramPtr &flatShader);

void setup_lights(Viewer& viewer);

void setup_kart(Viewer& viewer, DynamicSystemPtr& system, DynamicSystemRenderablePtr &systemRenderable);

void setup_billboard(Viewer& viewer, DynamicSystemPtr& system, DynamicSystemRenderablePtr &systemRenderable);

void setup_textures(Viewer& viewer, DynamicSystemPtr& system, DynamicSystemRenderablePtr &systemRenderable);
#endif
