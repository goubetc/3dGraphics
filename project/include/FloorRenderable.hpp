#ifndef FLOOR_RENDERABLE_HPP
#define FLOOR_RENDERABLE_HPP

#include "HierarchicalRenderable.hpp"
#include "KeyframeCollection.hpp"
#include <vector>
#include <glm/glm.hpp>

class FloorRenderable : public HierarchicalRenderable
{
public:
  ~FloorRenderable();
  FloorRenderable( ShaderProgramPtr program, float width, float height, float depth, float r, float g, float b);

  void addLocalTransformKeyframe( const GeometricTransformation& transformation, float time );
  void addParentTransformKeyframe( const GeometricTransformation& transformation, float time );


private:
  void do_draw();
  void do_animate( float time );

  std::vector< glm::vec3 > m_positions;
  std::vector< glm::vec4 > m_colors;

  unsigned int m_vBuffer;
  unsigned int m_cBuffer;

  KeyframeCollection m_localKeyframes; /*!< A collection of keyframes for the local transformation of renderable. */
    KeyframeCollection m_parentKeyframes; /*!< A collection of keyframes for the parent transformation of renderable. */
};

typedef std::shared_ptr<FloorRenderable> FloorRenderablePtr;

#endif
