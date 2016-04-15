#ifndef DUST_RENDERABLE_HPP
#define DUST_RENDERABLE_HPP

#include "HierarchicalRenderable.hpp"
#include "./dynamics/KartRenderable.hpp"
#include "SpecRenderable.hpp"
#include "KeyframeCollection.hpp"
#include <vector>
#include <glm/glm.hpp>

class GeometricTransformation;

class DustRenderable : public HierarchicalRenderable
{
public:
  ~DustRenderable();
  DustRenderable( ShaderProgramPtr program, int dust, FloorRenderablePtr forceRenderable, ParticlePtr mobile, ConstantForceFieldPtr force);
  void addLocalTransformKeyframe( const GeometricTransformation& transformation, float time );
  void addParentTransformKeyframe( const GeometricTransformation& transformation, float time );

private:
  void do_draw();
  void do_animate( float time );
  int m_dust;
  std::vector< SpecRenderablePtr > parts;
  ParticlePtr m_particle;

  std::vector< glm::vec3 > m_positions;
  std::vector< glm::vec4 > m_colors;
  float scale;
  ConstantForceFieldPtr m_force;

  float angle;   
  
  KeyframeCollection m_localKeyframes; /*!< A collection of keyframes for the local transformation of renderable. */
    KeyframeCollection m_parentKeyframes; /*!< A collection of keyframes for the parent transformation of renderable. */

  unsigned int m_vBuffer;
  unsigned int m_cBuffer;
};

typedef std::shared_ptr<DustRenderable> DustRenderablePtr;

#endif
