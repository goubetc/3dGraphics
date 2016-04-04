#include "./../include/CubeRenderable.hpp"
#include "./../include/gl_helper.hpp"
#include "./../include/log.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

CubeRenderable::CubeRenderable(ShaderProgramPtr shaderProgram) :
  Renderable(shaderProgram), m_vBuffer(0)
{
}

void CubeRenderable::do_draw()
{
}

void CubeRenderable::do_animate(float time) {}

CubeRenderable::~CubeRenderable()
{
}
