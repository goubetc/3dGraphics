#ifndef SETUP_KART_HPP
#define SETUP_KART_HPP

#include "../include/Viewer.hpp"
#include "../include/dynamics/DynamicSystem.hpp"
#include "../include/dynamics/DynamicSystemRenderable.hpp"

void initialize_scene( Viewer& viewer );

void initialize_walls(Viewer& viewer, DynamicSystemPtr& system, DynamicSystemRenderablePtr &systemRenderable);

void initialize_billboard(Viewer& viewer, DynamicSystemPtr& system, DynamicSystemRenderablePtr &systemRenderable);

#endif
