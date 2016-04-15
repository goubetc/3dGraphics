#include "./../include/BillboardRenderable.hpp"
#include "./../include/gl_helper.hpp"
#include "./../include/log.hpp"
#include "./../include/Utils.hpp"


#include "./../include/FrameRenderable.hpp"
#include "./../include/CylinderRenderable.hpp"
#include "./../include/FloorRenderable.hpp"
#include "./../include/TriangleRenderable.hpp"
#include "../include/GeometricTransformation.hpp"

//#include "../include/ShaderProgram.hpp"

#include "../include/dynamics/DampingForceField.hpp"
#include "../include/dynamics/ConstantForceField.hpp"
#include "../include/dynamics/SpringForceField.hpp"
#include "../include/dynamics/EulerExplicitSolver.hpp"

#include "../include/dynamics/ParticleRenderable.hpp"
#include "../include/dynamics/ParticleListRenderable.hpp"
#include "../include/dynamics/ConstantForceFieldRenderable.hpp"
#include "../include/dynamics/SpringForceFieldRenderable.hpp"
#include "../include/dynamics/SpringListRenderable.hpp"
#include "../include/dynamics/ControlledForceFieldRenderable.hpp"
#include "../include/dynamics/DynamicSystem.hpp"
#include "../include/dynamics/DynamicSystemRenderable.hpp"
//#include "../include/setup_kart.hpp"
#include "../include/HierarchicalSphereRenderable.hpp"

#include "../include/texturing/TexturedPlaneRenderable.hpp"
#include "../include/texturing/TexturedCubeRenderable.hpp"
#include "../include/texturing/MultiTexturedCubeRenderable.hpp"
#include "../include/texturing/MipMapCubeRenderable.hpp"
#include "../include/texturing/TexturedLightedMeshRenderable.hpp"
#include "../include/lighting/DirectionalLightRenderable.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>


BillboardRenderable::BillboardRenderable(ShaderProgramPtr flatShader, ParticlePtr mobile, std::string filename) :
  
  HierarchicalRenderable(flatShader),
  m_particle(mobile),
  m_pBuffer(0),
  m_cBuffer(0),
  m_nBuffer(0),
  m_fileName(filename)
{


  //Temporary variables
  glm::mat4 parentTransformation(1.0), localTransformation(1.0);
  MaterialPtr pearl = Material::Pearl();

  texShader = std::make_shared<ShaderProgram>("../shaders/textureVertex.glsl","../shaders/textureFragment.glsl");
     
  //variables
  float poleRadius = 0.05;
  float poleHeight = 5;
  float poleX = 0.0;
  float poleY = 0.0;
  float poleZ = 0.0;
  float billboardWidth = 0.2;
  float billboardHeight = 8;
  float billboardThickness = 4;
  float billboardX = 0.0;
  float billboardY = 0.0;
  float billboardZ = 3.0;

  //pole of the billboard
  std::shared_ptr<CylinderRenderable> pole = std::make_shared<CylinderRenderable>(flatShader, 20, poleHeight, poleRadius, 102,51,0);
  pole->setLocalTransform( glm::translate(glm::mat4(1.0), glm::vec3(poleX,poleY,poleZ)));

  //front of the billboard
  std::shared_ptr<FloorRenderable> billboard = std::make_shared<FloorRenderable>(flatShader, billboardWidth, billboardThickness, billboardHeight, 100,0,100);
  pole->setLocalTransform( glm::translate(glm::mat4(1.0), glm::vec3(billboardX,billboardY,billboardZ)));

  //HierarchicalRenderable::addChild(master, pole);
  HierarchicalRenderable::addChild(pole, billboard);

  //Textured plane
  texPlane = std::make_shared<TexturedPlaneRenderable>(texShader, m_fileName);
    
  texPlane->setParentTransform( GeometricTransformation( glm::vec3{billboardX+0.101,billboardY,0},
  							 glm::angleAxis( float(M_PI/2), glm::normalize(glm::vec3( 0,1,0)) ),
  							 glm::vec3{4,8,4}).toMatrix() );
  texPlane->setLocalTransform(glm::rotate(glm::mat4(1.0), (float)(3*M_PI/2), glm::vec3(0.0,0.0,1.0)));
    
  
  texPlane->setMaterial(pearl);

// choice of wrap option
  
texPlane->setWrapOption(0);

texPlane->updateTextureOption();
  
  
  HierarchicalRenderable::addChild( pole, texPlane );
  master = pole;    
}
void BillboardRenderable::do_draw(){}



void BillboardRenderable::do_animate(float time) {}

BillboardRenderable::~BillboardRenderable()
{
  glcheck(glDeleteBuffers(1, &m_pBuffer));
  glcheck(glDeleteBuffers(1, &m_cBuffer));
  glcheck(glDeleteBuffers(1, &m_nBuffer));
}
