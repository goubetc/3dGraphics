#include "./../include/DustRenderable.hpp"
#include "./../include/gl_helper.hpp"
#include "./../include/log.hpp"
#include "./../include/Utils.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>

#include "stdlib.h"
#include "time.h"

DustRenderable::DustRenderable(ShaderProgramPtr shaderProgram, int dust, FloorRenderablePtr forceRenderable, ParticlePtr mobile, ConstantForceFieldPtr force) :
	HierarchicalRenderable(shaderProgram),
	m_vBuffer(0), m_cBuffer(0)
{
	
	m_dust = dust;
	m_particle = mobile;
	m_force = force;

	for(int i=0;i<dust;i++)
	{
		parts.push_back(std::make_shared<SpecRenderable>(shaderProgram));
	}
	for(auto const& p : parts)
	{
		HierarchicalRenderable::addChild(forceRenderable, p);
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


void DustRenderable::addLocalTransformKeyframe( const GeometricTransformation& transformation, float time )
{
	m_localKeyframes.add( transformation, time );
}

void DustRenderable::addParentTransformKeyframe( const GeometricTransformation& transformation, float time )
{
	m_parentKeyframes.add( transformation, time );
}


void DustRenderable::do_draw()
{
	for(auto const& p : parts)
	{
		p->setLocalTransform(glm::translate(glm::mat4(1.0), glm::vec3(rand()%10 - 5,rand()%10 - 5,rand()%10 - 5)));
	}

	const float& pRadius = m_particle->getRadius();
    const glm::vec3& pPosition = m_particle->getPosition();
    glm::mat4 scale = glm::scale(glm::mat4(1.0), glm::vec3(0.15, 0.15, 0.15));
    glm::mat4 translate = glm::translate(glm::mat4(1.0), glm::vec3(pPosition));
    // float dot = glm::dot(glm::normalize(m_force->getForce()), glm::normalize(glm::vec3(1.0,0.0,0.0)));
    // glm::vec3 cross = glm::cross(m_force->getForce(), glm::vec3(1.0,0.0,0.0));
    
    // if(!(dot != dot || acos(dot) != acos(dot)))  //check if dot is not NaN
    //   angle = acos(dot);
    // glm::mat4 rotate;

    
    // if(glm::dot(cross, glm::vec3(0.0,0.0,1.0)) > 0) angle = -angle;
    //std::cout<<m_force->getForce()[0]<<" "<<m_force->getForce()[1]<<" "<<m_force->getForce()[2]<<"\n";
    //if(m_force->getForce()[0] + m_force->getForce()[0] + m_force->getForce()[0] == 0) angle = 0;
    //if((int)m_back) angle = angle + M_PI;
    //std::cout<<angle<<std::endl;
    // rotate = glm::rotate(glm::mat4(1.0), (float)(angle - M_PI/2), glm::vec3(0.0,0.0,1.0));
    // glm::mat4 rotate2 = glm::rotate(glm::mat4(1.0), (float)(M_PI/2), glm::normalize(glm::vec3(1.0,0.0,0.0)));

    for(auto const& p : parts)
	{
		p->setParentTransform(scale);
	}





}



void DustRenderable::do_animate(float time) {
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

DustRenderable::~DustRenderable()
{
	glcheck(glDeleteBuffers(1, &m_vBuffer));
	glcheck(glDeleteBuffers(1, &m_cBuffer));
}
