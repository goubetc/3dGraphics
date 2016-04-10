#include "./../include/CylinderRenderable.hpp"
#include "./../include/gl_helper.hpp"
#include "./../include/log.hpp"
#include "./../include/Utils.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <cmath>

CylinderRenderable::CylinderRenderable(ShaderProgramPtr shaderProgram, int faces, float height, float radius) :
    HierarchicalRenderable(shaderProgram),
    m_vBuffer(0), m_cBuffer(0)
{

    // Exercice 1: Fill positions and colors to describe a cylinder
     height /= 2;
    
    /*m_positions.push_back( glm::vec3(0.0,0.0,1.0) );
    m_positions.push_back( glm::vec3(0.0,0.0,0.0) );
    m_colors.push_back( randomColor() );
    m_colors.push_back( randomColor() );*/
    float x=0, y=0, z=0;
    
    // Exercice 1: Fill positions and colors to describe a cylinder
    for(int i=0;i<faces;i++)
    {
      float theta = i * 2 * M_PI/faces;
      float theta2 = (i+1) * 2 * M_PI/faces;
      
      m_positions.push_back( glm::vec3(radius*cos(theta)+x,radius*sin(theta)+y,z-height) );
      m_positions.push_back( glm::vec3(radius*cos(theta)+x,radius*sin(theta)+y,z+height) );
      m_positions.push_back( glm::vec3(radius*cos(theta2)+x,radius*sin(theta2)+y,z+height) );
      m_positions.push_back( glm::vec3(radius*cos(theta)+x,radius*sin(theta)+y,z+height) );
      m_positions.push_back( glm::vec3(radius*cos(theta2)+x,radius*sin(theta2)+y,z+height) );
      m_positions.push_back( glm::vec3(0.0+x,0.0+y,z+height) );
      

      m_colors.push_back( randomColor() );
      m_colors.push_back( randomColor() );
      m_colors.push_back( randomColor() );
      m_colors.push_back( randomColor() );
      m_colors.push_back( randomColor() );
      m_colors.push_back( randomColor() );
      
      m_positions.push_back( glm::vec3(radius*cos(theta)+x,radius*sin(theta)+y,z-height) );
      m_positions.push_back( glm::vec3(radius*cos(theta2)+x,radius*sin(theta2)+y,z-height) );
      m_positions.push_back( glm::vec3(radius*cos(theta2)+x,radius*sin(theta2)+y,z+height) );
      m_positions.push_back( glm::vec3(radius*cos(theta)+x,radius*sin(theta)+y,z-height) );
      m_positions.push_back( glm::vec3(radius*cos(theta2)+x,radius*sin(theta2)+y,z-height) );
      m_positions.push_back( glm::vec3(0.0+x,0.0+y,z-height) );

      m_colors.push_back( randomColor() );
      m_colors.push_back( randomColor() );
      m_colors.push_back( randomColor() );
      m_colors.push_back( randomColor() );
      m_colors.push_back( randomColor() );
      m_colors.push_back( randomColor() );
    }


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


void CylinderRenderable::do_draw()
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

void CylinderRenderable::do_animate(float time) {}

CylinderRenderable::~CylinderRenderable()
{
    glcheck(glDeleteBuffers(1, &m_vBuffer));
    glcheck(glDeleteBuffers(1, &m_cBuffer));
}//setLocalTransform
