#ifndef SETUP_KART_HPP
#define SETUP_KART_HPP

#include "Viewer.hpp"
#include "dynamics/DynamicSystem.hpp"
#include "dynamics/DynamicSystemRenderable.hpp"

void initialize_kart( Viewer& viewer );

void hierarchical_kart(Viewer& viewer, DynamicSystemPtr& system, DynamicSystemRenderablePtr &systemRenderable);

void setup_lights(Viewer& viewer);

#endif
