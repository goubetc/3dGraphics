#include "./../include/IndexedCubeRenderable.hpp"
#include "./../include/gl_helper.hpp"
#include "./../include/log.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

IndexedCubeRenderable::IndexedCubeRenderable(ShaderProgramPtr shaderProgram) : Renderable(shaderProgram)
{
}

void IndexedCubeRenderable::do_draw()
{
}

void IndexedCubeRenderable::do_animate(float time) {}

IndexedCubeRenderable::~IndexedCubeRenderable()
{
}
