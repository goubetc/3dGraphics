#include "./../include/SpecRenderable.hpp"
#include "./../include/gl_helper.hpp"
#include "./../include/log.hpp"
#include "./../include/Utils.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <cmath>
#include "stdlib.h"
#include "time.h"

SpecRenderable::SpecRenderable(ShaderProgramPtr shaderProgram ) :
  HierarchicalRenderable(shaderProgram),
  m_vBuffer(0), m_cBuffer(0)
{
  srand(time(NULL));
  float randomVal1;
  float randomVal2;
  float randomVal3;
      randomVal1 = 0;
      randomVal2 = 0;
      randomVal3 = 35;
      std::cout<<randomVal1;

      m_positions.push_back( glm::vec3(randomVal1,randomVal2,randomVal3) );
      m_positions.push_back( glm::vec3(randomVal1,randomVal2,randomVal3 + 0.7) );
      m_positions.push_back( glm::vec3(randomVal1+ 0.7,randomVal2 ,randomVal3) );
      m_colors.push_back( glm::vec4(0,0,0,1) );
      m_colors.push_back( glm::vec4(0,0,0,1) );
      m_colors.push_back( glm::vec4(0,0,0,1) );

      // m_positions.push_back( glm::vec3(randomVal1+3,randomVal2-2,randomVal3) );
      // m_positions.push_back( glm::vec3(randomVal1-2,randomVal2,randomVal3 - 5) );
      // m_positions.push_back( glm::vec3(randomVal1,randomVal2 + 5,randomVal3+4) );
      // m_colors.push_back( glm::vec4(0,0,0,1) );
      // m_colors.push_back( glm::vec4(0,0,0,1) );
      // m_colors.push_back( glm::vec4(0,0,0,1) );

      // m_positions.push_back( glm::vec3(randomVal1,randomVal2,randomVal3) );
      // m_positions.push_back( glm::vec3(randomVal1,randomVal2,randomVal3 + 5) );
      // m_positions.push_back( glm::vec3(randomVal1 + 5,randomVal2,randomVal3) );
      // m_colors.push_back( glm::vec4(0,0,0,1) );
      // m_colors.push_back( glm::vec4(0,0,0,1) );
      // m_colors.push_back( glm::vec4(0,0,0,1) );


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


void SpecRenderable::addLocalTransformKeyframe( const GeometricTransformation& transformation, float time )
{
  m_localKeyframes.add( transformation, time );
}

void SpecRenderable::addParentTransformKeyframe( const GeometricTransformation& transformation, float time )
{
  m_parentKeyframes.add( transformation, time );
}


void SpecRenderable::do_draw()
{


  // setLocalTransform()
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



void SpecRenderable::do_animate(float time) {
//Assign the interpolated transformations from the keyframes to the local/parent transformations.
    if(!m_localKeyframes.empty())
    {
        setLocalTransform( m_localKeyframes.interpolateTransformation( time ) );
    }
    if(!m_parentKeyframes.empty())
    {
        setParentTransform( m_parentKeyframes.interpolateTransformation( time ) );
    }

}

SpecRenderable::~SpecRenderable()
{
  glcheck(glDeleteBuffers(1, &m_vBuffer));
  glcheck(glDeleteBuffers(1, &m_cBuffer));
}
