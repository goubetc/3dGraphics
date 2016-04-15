#ifndef BILLBOARD_RENDERABLE_HPP
#define BILLBOARD_RENDERABLE_HPP

#include "HierarchicalRenderable.hpp"
#include "dynamics/Particle.hpp"
#include "dynamics/DynamicSystem.hpp"
#include "dynamics/DynamicSystemRenderable.hpp"
#include "./CylinderRenderable.hpp"
#include "texturing/TexturedPlaneRenderable.hpp"
//#include "FloorRenderable.hpp"
#include <vector>
#include <glm/glm.hpp>




/**@brief Render a particle to the screen.
 *
 * Render a particle to the screen. Since a particle is modeled by
 * a ball, this renderable simply render the corresponding ball. If
 * you have more than one renderable, have a look to ParticleListRenderable.
 */

class BillboardRenderable : public HierarchicalRenderable
{
public:
  std::shared_ptr<CylinderRenderable> master;        
  ShaderProgramPtr texShader;
  TexturedPlaneRenderablePtr texPlane;
  ~BillboardRenderable();
  /**@brief Build a particle renderable.
   *
   * Build a renderable to render a particle.
   * @param program The shader program used to render the particle.
   * @param particle The particle to render.
   */
  BillboardRenderable( ShaderProgramPtr program, ParticlePtr mobile, std::string filename );
  

private:
  void do_draw();
  void do_animate( float time );



  ParticlePtr m_particle;
  std::vector< glm::vec3 > m_positions;
  std::vector< glm::vec4 > m_colors;
  std::vector< glm::vec3 > m_normals;

  unsigned int m_pBuffer;
  unsigned int m_cBuffer;
  unsigned int m_nBuffer;
  std::string m_fileName;
};

typedef std::shared_ptr<BillboardRenderable> BillboardRenderablePtr;

#endif
