#ifndef SETUP_KART_HPP
#define SETUP_KART_HPP

#include "../include/Viewer.hpp"
#include "../include/dynamics/DynamicSystem.hpp"
#include "../include/dynamics/DynamicSystemRenderable.hpp"

void initialize_kart( Viewer& viewer );

void hierarchical_kart(Viewer& viewer, DynamicSystemPtr& system, DynamicSystemRenderablePtr &systemRenderable);

#endif
