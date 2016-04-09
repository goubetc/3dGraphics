#include "./../../include/dynamics/SpringForceField.hpp"

SpringForceField::SpringForceField(const ParticlePtr p1, const ParticlePtr p2, float stiffness, float equilibriumLength, float damping) :
  m_p1(p1),
  m_p2(p2),
  m_stiffness(stiffness),
  m_equilibriumLength(equilibriumLength),
  m_damping(damping)
{}

void SpringForceField::do_addForce()
{

  
  //TODO: Implement a damped spring
  //Functions to use:
  //glm::length( vec3 ): Return the length of a vector
  //glm::normalize( vec3 ): Return the normalization of a vector
  //Particle::getVelocity(), Particle::getForce(), Particle::setForce()
  //Nb:   Compute force ONLY IF the displacement length is above std::numeric_limits<float>::epsilon()
  //      Otherwise the computation is useless
  int length1_2 = glm::length((m_p1->getPosition() - m_p2->getPosition()));
  if (length1_2 > std::numeric_limits<float>::epsilon()){
    m_p1->setForce(-m_stiffness*( length1_2 - m_equilibriumLength) * glm::normalize(((m_p1->getPosition() - m_p2->getPosition()))));
    m_p2->setForce(-m_p2->getForce());
  }
}

ParticlePtr SpringForceField::getParticle1() const
{
  return m_p1;
}

ParticlePtr SpringForceField::getParticle2() const
{
  return m_p2;
}
