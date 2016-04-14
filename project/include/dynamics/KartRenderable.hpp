#ifndef KART_RENDERABLE_HPP
#define KART_RENDERABLE_HPP

#include "../HierarchicalRenderable.hpp"
#include "./ConstantForceField.hpp"
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

class KartRenderable : public HierarchicalRenderable
{
public:
std::shared_ptr<FloorRenderable> root; 
  ~KartRenderable();
  /**@brief Build a particle renderable.
   *
   * Build a renderable to render a particle.
   * @param program The shader program used to render the particle.
   * @param particle The particle to render.
   */
  KartRenderable( ShaderProgramPtr program, ParticlePtr mobile, Viewer& viewer, ConstantForceFieldPtr force, ControlledForceFieldRenderablePtr forceRenderable, float r, float g, float b );
  //glm::vec3 KartRenderable::getPosition() const;

private:
  void do_draw();
  void do_animate( float time );
  
  bool m_back;
  std::shared_ptr<CylinderRenderable> wheel_br;
  std::shared_ptr<CylinderRenderable> wheel_bl;
  std::shared_ptr<CylinderRenderable> wheel_fr;
  std::shared_ptr<CylinderRenderable> wheel_fl;
  
  ControlledForceFieldRenderablePtr m_forceRend;


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
};

typedef std::shared_ptr<KartRenderable> KartRenderablePtr;

#endif //KART_RENDERABLE_HPP
