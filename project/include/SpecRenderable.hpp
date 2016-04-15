#ifndef SPEC_RENDERABLE_HPP
#define SPEC_RENDERABLE_HPP

#include "HierarchicalRenderable.hpp"
#include "KeyframeCollection.hpp"
#include <vector>
#include <glm/glm.hpp>

class GeometricTransformation;

class SpecRenderable : public HierarchicalRenderable
{
public:
  ~SpecRenderable();
  SpecRenderable( ShaderProgramPtr program );
  void addLocalTransformKeyframe( const GeometricTransformation& transformation, float time );
  void addParentTransformKeyframe( const GeometricTransformation& transformation, float time );

private:
  void do_draw();
  void do_animate( float time );

  std::vector< glm::vec3 > m_positions;
  std::vector< glm::vec4 > m_colors;
  float scale;
  
  KeyframeCollection m_localKeyframes; /*!< A collection of keyframes for the local transformation of renderable. */
    KeyframeCollection m_parentKeyframes; /*!< A collection of keyframes for the parent transformation of renderable. */

  unsigned int m_vBuffer;
  unsigned int m_cBuffer;
};

typedef std::shared_ptr<SpecRenderable> SpecRenderablePtr;

#endif
