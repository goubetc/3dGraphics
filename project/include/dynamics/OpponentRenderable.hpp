#ifndef OPPONENT_RENDERABLE_HPP
#define OPPONENT_RENDERABLE_HPP

#include "../HierarchicalRenderable.hpp"
#include "./ConstantForceField.hpp"
#include "../KeyframeCollection.hpp"
#include "Particle.hpp"
#include "DynamicSystem.hpp"
#include "./../../include/CylinderRenderable.hpp"
#include "DynamicSystemRenderable.hpp"
#include "ParticleRenderable.hpp"
#include "../FloorRenderable.hpp"
#include "./../../include/dynamics/ControlledForceFieldRenderable.hpp"
#include <vector>
#include <glm/glm.hpp>
#include "../Viewer.hpp"




/**@brief Render a particle to the screen.
 *
 * Render a particle to the screen. Since a particle is modeled by
 * a ball, this renderable simply render the corresponding ball. If
 * you have more than one renderable, have a look to ParticleListRenderable.
 */

class OpponentRenderable : public HierarchicalRenderable
{
public:
std::shared_ptr<FloorRenderable> root; 
  ~OpponentRenderable();
  /**@brief Build a particle renderable.
   *
   * Build a renderable to render a particle.
   * @param program The shader program used to render the particle.
   * @param particle The particle to render.
   */
  OpponentRenderable( ShaderProgramPtr program, Viewer& viewer, float r, float g, float b );
  //glm::vec3 OpponentRenderable::getPosition() const;

private:
  void do_draw();
  void do_animate( float time );
  
  bool m_back;
  std::shared_ptr<CylinderRenderable> wheel_br;
  std::shared_ptr<CylinderRenderable> wheel_bl;
  std::shared_ptr<CylinderRenderable> wheel_fr;
  std::shared_ptr<CylinderRenderable> wheel_fl;
  
  ControlledForceFieldRenderablePtr m_forceRend;

    time_t time1;
  time_t time2;


  ParticlePtr m_particle;
  ConstantForceFieldPtr m_force;
std::vector< glm::vec3 > m_positions;
  std::vector< glm::vec4 > m_colors;
  std::vector< glm::vec3 > m_normals;

  unsigned int m_pBuffer;
  unsigned int m_cBuffer;
  unsigned int m_nBuffer;
  Viewer& m_viewer;
  float angle;
  float wheelSpeed;
};

typedef std::shared_ptr<OpponentRenderable> OpponentRenderablePtr;

#endif //OPPONENT_RENDERABLE_HPP
