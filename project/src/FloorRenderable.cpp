#include "./../include/FloorRenderable.hpp"
#include "./../include/gl_helper.hpp"
#include "./../include/log.hpp"
#include "./../include/Utils.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <cmath>

FloorRenderable::FloorRenderable(ShaderProgramPtr shaderProgram, float width, float height, float depth, float r, float g, float b) :
  HierarchicalRenderable(shaderProgram),
  m_vBuffer(0), m_cBuffer(0)
{
    
    
  /*m_positions.push_back( glm::vec3(z-height,z-height,x+width) );
    m_positions.push_back( glm::vec3(z-height,z-height,z-height) );
    m_colors.push_back( randomColor() );
    m_colors.push_back( randomColor() );*/
    
  width /= 2;
  height /= 2;
  depth /= 2;    
  float x=0, y=0, z=0;
    
    
  // Exercice 1: Fill positions and colors to describe a Floor
  m_positions.push_back( glm::vec3(x-width,y-depth,z-height) );
  m_positions.push_back( glm::vec3(x+width,y-depth,z-height) );
  m_positions.push_back( glm::vec3(x+width,y+depth,z-height) );
    
  m_positions.push_back( glm::vec3(x-width,y-depth,z-height) );
  m_positions.push_back( glm::vec3(x-width,y+depth,z-height) );
  m_positions.push_back( glm::vec3(x+width,y+depth,z-height) );
    
  m_positions.push_back( glm::vec3(x-width,y-depth,z+height) );
  m_positions.push_back( glm::vec3(x+width,y-depth,z+height) );
  m_positions.push_back( glm::vec3(x+width,y+depth,z+height) );
    
  m_positions.push_back( glm::vec3(x-width,y-depth,z+height) );
  m_positions.push_back( glm::vec3(x-width,y+depth,z+height) );
  m_positions.push_back( glm::vec3(x+width,y+depth,z+height) );
    
  m_positions.push_back( glm::vec3(x+width,y-depth,z-height) );
  m_positions.push_back( glm::vec3(x-width,y-depth,z-height) );
  m_positions.push_back( glm::vec3(x-width,y-depth,z+height) );
    
  m_positions.push_back( glm::vec3(x-width,y-depth,z+height) );
  m_positions.push_back( glm::vec3(x+width,y-depth,z-height) );
  m_positions.push_back( glm::vec3(x+width,y-depth,z+height) );
    
  m_positions.push_back( glm::vec3(x-width,y+depth,z+height) );
  m_positions.push_back( glm::vec3(x+width,y+depth,z-height) );
  m_positions.push_back( glm::vec3(x+width,y+depth,z+height) );
    
  m_positions.push_back( glm::vec3(x+width,y+depth,z-height) );
  m_positions.push_back( glm::vec3(x-width,y+depth,z-height) );
  m_positions.push_back( glm::vec3(x-width,y+depth,z+height) );
    
  m_positions.push_back( glm::vec3(x+width,y+depth,z-height) );
  m_positions.push_back( glm::vec3(x+width,y-depth,z-height) );
  m_positions.push_back( glm::vec3(x+width,y-depth,z+height) );
    
  m_positions.push_back( glm::vec3(x+width,y-depth,z+height) );
  m_positions.push_back( glm::vec3(x+width,y+depth,z-height) );
  m_positions.push_back( glm::vec3(x+width,y+depth,z+height) );
    
  m_positions.push_back( glm::vec3(x-width,y+depth,z-height) );
  m_positions.push_back( glm::vec3(x-width,y-depth,z-height) );
  m_positions.push_back( glm::vec3(x-width,y-depth,z+height) );
    
  m_positions.push_back( glm::vec3(x-width,y-depth,z+height) );
  m_positions.push_back( glm::vec3(x-width,y+depth,z-height) );
  m_positions.push_back( glm::vec3(x-width,y+depth,z+height) );
    
    
    
  for(int i=0;i<12*3;i++)    
    m_colors.push_back( glm::vec4(r,g,b,1) );


  // Exercice 2: Compute normal per face or normal per vertex
  // Simply store the normals in m_colors;
  // these normals will then be used to color the triangles


  //Generate buffers
  glGenBuffers(1, &m_vBuffer);
  glGenBuffers(1, &m_cBuffer);

  //Activate buffer and send data to the graphics card
  glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_vBuffer));
  glcheck(glBufferData(GL_ARRAY_BUFFER, m_positions.size()*sizeof(glm::vec3), m_positions.data(), GL_STATIC_DRAW));

  glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_cBuffer));
  glcheck(glBufferData(GL_ARRAY_BUFFER, m_colors.size()*sizeof(glm::vec4), m_colors.data(), GL_STATIC_DRAW));
}

void FloorRenderable::addLocalTransformKeyframe( const GeometricTransformation& transformation, float time )
{
  m_localKeyframes.add( transformation, time );
}

void FloorRenderable::addParentTransformKeyframe( const GeometricTransformation& transformation, float time )
{
  m_parentKeyframes.add( transformation, time );
}


void FloorRenderable::do_draw()
{
  //Uniform: Get location and send data to GPU
  int modelLocation = m_shaderProgram->getUniformLocation("modelMat");
  glcheck(glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(m_model)));

  //Attribute: Get location
  int positionLocation = m_shaderProgram->getAttributeLocation("vPosition");
  int colorLocation = m_shaderProgram->getAttributeLocation("vColor");

  //Activate a location, bind a buffer, specify internal format
  glcheck(glEnableVertexAttribArray(positionLocation));
  glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_vBuffer));
  glcheck(glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)0));

  glcheck(glEnableVertexAttribArray(colorLocation));
  glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_cBuffer));
  glcheck(glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, 0, (void*)0));

  //Draw triangles elements
  glcheck(glDrawArrays(GL_TRIANGLES,0, m_positions.size()));

  glcheck(glDisableVertexAttribArray(positionLocation));
  glcheck(glDisableVertexAttribArray(colorLocation));
}

void FloorRenderable::do_animate(float time) {

   if(!m_localKeyframes.empty())
    {
        setLocalTransform( m_localKeyframes.interpolateTransformation( time ) );
    }
    if(!m_parentKeyframes.empty())
    {
        setParentTransform( m_parentKeyframes.interpolateTransformation( time ) );
    }
}

FloorRenderable::~FloorRenderable()
{
  glcheck(glDeleteBuffers(1, &m_vBuffer));
  glcheck(glDeleteBuffers(1, &m_cBuffer));
}
