#include "./../include/Renderable.hpp"
#include "./../include/gl_helper.hpp"
#include <glm/gtc/type_ptr.hpp>

Renderable::~Renderable(){}

Renderable::Renderable()
  : m_shaderProgram{ nullptr },
    m_model{} // the default constructor of a mat4 loads the identity
{}

Renderable::Renderable(ShaderProgramPtr program)
  : m_shaderProgram{ program },
    m_model{} // the default constructor of a mat4 loads the identity
{}

void Renderable::bindShaderProgram()
{
    m_shaderProgram->bind();
}

void Renderable::unbindShaderProgram()
{
    m_shaderProgram->unbind();
}

int Renderable::projectionLocation()
{
    return m_shaderProgram->getUniformLocation("projMat");
}

int Renderable::viewLocation()
{
    return m_shaderProgram->getUniformLocation("viewMat");
}

void Renderable::draw()
{
  do_draw();
}

void Renderable::animate( float time )
{
  do_animate( time );
}

void Renderable::keyPressedEvent(sf::Event& e)
{
  do_keyPressedEvent( e );
}

void Renderable::keyReleasedEvent(sf::Event& e)
{
  do_keyReleasedEvent( e );
}

void Renderable::mousePressEvent(sf::Event& e)
{
  do_mousePressEvent( e );
}

void Renderable::mouseReleaseEvent(sf::Event& e)
{
  do_mouseReleaseEvent( e );
}

void Renderable::mouseWheelEvent(sf::Event& e)
{
  do_mouseWheelEvent( e );
}

void Renderable::mouseMoveEvent(sf::Event& e)
{
  do_mouseMoveEvent( e );
}

void Renderable::do_keyPressedEvent(sf::Event& e)
{}

void Renderable::do_keyReleasedEvent(sf::Event& e)
{}

void Renderable::do_mousePressEvent(sf::Event& e)
{}

void Renderable::do_mouseReleaseEvent(sf::Event& e)
{}

void Renderable::do_mouseWheelEvent(sf::Event& e)
{}

void Renderable::do_mouseMoveEvent(sf::Event& e)
{}


void Renderable::setModelMatrix( const glm::mat4& model )
{
  m_model = model;
}

const glm::mat4& Renderable::getModelMatrix() const
{
  return m_model;
}

void Renderable::setShaderProgram( ShaderProgramPtr prog )
{
  m_shaderProgram = prog;
}
