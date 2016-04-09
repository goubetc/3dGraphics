#ifndef TRIANGLE_RENDERABLE_HPP
#define TRIANGLE_RENDERABLE_HPP

#include "HierarchicalRenderable.hpp"
#include <vector>
#include <glm/glm.hpp>

class TriangleRenderable : public HierarchicalRenderable
{
public:
    ~TriangleRenderable();
    TriangleRenderable( ShaderProgramPtr program, float width, float height, float depth);

private:
    void do_draw();
    void do_animate( float time );

    std::vector< glm::vec3 > m_positions;
    std::vector< glm::vec4 > m_colors;

    unsigned int m_vBuffer;
    unsigned int m_cBuffer;
};

typedef std::shared_ptr<TriangleRenderable> TriangleRenderablePtr;

#endif