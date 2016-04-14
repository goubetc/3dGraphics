#include "../include/Viewer.hpp"
#include "../include/log.hpp"
#include "./../include/ShaderProgram.hpp"
#include "./../include/FrameRenderable.hpp"
// #include "./../include/CylinderRenderable.hpp"
// #include "./../include/FloorRenderable.hpp"
// #include "./../include/TriangleRenderable.hpp"
#include "../include/GeometricTransformation.hpp"

#include "../include/ShaderProgram.hpp"

#include "../include/QuadRenderable.hpp"

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
#include "../include/setup_scene.hpp"
#include "../include/HierarchicalSphereRenderable.hpp"
#include "../include/dynamics/KartRenderable.hpp"
#include "../include/texturing/TexturedPlaneRenderable.hpp"

#include "../include/lighting/LightedMeshRenderable.hpp"
#include "../include/lighting/LightedCubeRenderable.hpp"
#include "../include/lighting/LightedCylinderRenderable.hpp"
#include "../include/lighting/DirectionalLightRenderable.hpp"
#include "../include/lighting/PointLightRenderable.hpp"
#include "../include/lighting/SpotLightRenderable.hpp"

#include "../include/ShaderProgram.hpp"
#include "../include/FrameRenderable.hpp"

#include "../include/BillboardRenderable.hpp"

#include "./../include/HierarchicalMeshRenderable.hpp"

#include <iostream>
#include <string>
#include <fstream>

void initialize_scene( Viewer& viewer ){
//Set up a shader and add a 3D frame.
ShaderProgramPtr flatShader = std::make_shared<ShaderProgram>("../shaders/defaultVertex.glsl", "../shaders/defaultFragment.glsl");  
viewer.addShaderProgram( flatShader );
FrameRenderablePtr frame = std::make_shared<FrameRenderable>(flatShader);
viewer.addRenderable(frame);

//Set up the lights in the scene
setup_lights(viewer, flatShader);

//Initialize a dynamic system (Solver, Time step, Restitution coefficient)
DynamicSystemPtr system = std::make_shared<DynamicSystem>();
EulerExplicitSolverPtr solver = std::make_shared<EulerExplicitSolver>();
system->setSolver(solver);
system->setDt(0.01);

//Create a renderable associated to the dynamic system
//This renderable is responsible for calling DynamicSystem::computeSimulationStep() in the animate() function
//It is also responsible for some of the key/mouse events
DynamicSystemRenderablePtr systemRenderable = std::make_shared<DynamicSystemRenderable>(system);
std::cout << "address systemRenderable : " << &systemRenderable << std::endl;
viewer.addRenderable(systemRenderable);

//Position the camera
viewer.getCamera().setViewMatrix( glm::lookAt( glm::vec3(5,20,10), glm::vec3(20,20,2), glm::vec3(0,0,1)));

//Setup the textures in the scene
setup_textures(viewer, system, systemRenderable);

//Setup the kart in the scene²
 setup_kart(viewer, system, systemRenderable, flatShader);

// TODO init obstacles before

//Activate collision and set the restitution coefficient to 1.0
system->setCollisionsDetection(true);
system->setRestitution(1.0f);

// viewer.addRenderable(kart);

  //Setup a Billboard in the scene
  setup_billboard(viewer, system, systemRenderable, flatShader);

  
 //Setup mountains in the scene
  //setup_mountains(viewer, flatShader);


  //Finally activate animation
 viewer.startAnimation();
}

void setup_mountains(Viewer &viewer, ShaderProgramPtr &flatShader){
  std::shared_ptr<HierarchicalMeshRenderable> mountains = std::make_shared<HierarchicalMeshRenderable>(flatShader, "./../meshes/track.obj");

  mountains->setLocalTransform( GeometricTransformation( glm::vec3{0,0,19},
                glm::angleAxis( float(M_PI/2), glm::normalize(glm::vec3( 1,0,0)) ),
                glm::vec3{20,20,20}).toMatrix() );
 
  viewer.addRenderable(mountains);
}

void setup_lights(Viewer& viewer, ShaderProgramPtr &flatShader){
  //Define a shader that encode an illumination model
  ShaderProgramPtr phongShader = std::make_shared<ShaderProgram>("../shaders/phongVertex.glsl", "../shaders/phongFragment.glsl");
  viewer.addShaderProgram( phongShader );

  glm::mat4 localTransformation;

  //Add a 3D frame to the viewer
  FrameRenderablePtr frame = std::make_shared<FrameRenderable>(flatShader);
  viewer.addRenderable(frame);

  //Define a directional light for the whole scene
  glm::vec3 d_direction = glm::normalize(glm::vec3(0.0,0.0,-1.0));
  glm::vec3 d_ambient(1.0,1.0,1.0), d_diffuse(1.0,1.0,0.8), d_specular(1.0,1.0,1.0);
  //glm::vec3 d_ambient(0.0,0.0,0.0), d_diffuse(0.0,0.0,0.0), d_specular(0.0,0.0,0.0);
  DirectionalLightPtr directionalLight = std::make_shared<DirectionalLight>(d_direction, d_ambient, d_diffuse, d_specular);
  //Add a renderable to display the light and control it via mouse/key event
  glm::vec3 lightPosition(0.0,5.0,8.0);
  DirectionalLightRenderablePtr directionalLightRenderable = std::make_shared<DirectionalLightRenderable>(flatShader, directionalLight, lightPosition);
  localTransformation = glm::scale(glm::mat4(1.0), glm::vec3(1,1,1));
  directionalLightRenderable->setLocalTransform(localTransformation);
  viewer.setDirectionalLight(directionalLight);
  viewer.addRenderable(directionalLightRenderable);

  //Define a point light
  //TODO: PUT POINT LIGHT ABOVE BILLBOARDS AND OTHER STATIC OBJECTS
  glm::vec3 p_position(0.0,0.0,0.0), p_ambient(0.0,0.0,0.0), p_diffuse(0.0,0.0,0.0), p_specular(0.0,0.0,0.0);
  float p_constant=0.0, p_linear=0.0, p_quadratic=0.0;

  p_position = glm::vec3(-8, 5.0, 5.0);
  p_ambient = glm::vec3(0.0,0.0,0.0);
  p_diffuse = glm::vec3(1.0,0.0,0.0);
  p_specular = glm::vec3(1.0,0.0,0.0);
  p_constant=1.0;
  p_linear=5e-1;
  p_quadratic=0;
  PointLightPtr pointLight1 = std::make_shared<PointLight>(p_position, p_ambient, p_diffuse, p_specular, p_constant, p_linear, p_quadratic);
  PointLightRenderablePtr pointLightRenderable1 = std::make_shared<PointLightRenderable>(flatShader, pointLight1);
  localTransformation = glm::scale(glm::mat4(1.0), glm::vec3(0.5,0.5,0.5));
  pointLightRenderable1->setLocalTransform(localTransformation);
  viewer.addPointLight(pointLight1);
  viewer.addRenderable(pointLightRenderable1);

  /*p_position = glm::vec3(8, 5.0, 5.0);
  p_ambient = glm::vec3(0.0,0.0,0.0);
  p_diffuse = glm::vec3(0.0,0.0,1.0);
  p_specular = glm::vec3(0.0,0.0,1.0);
  p_constant=1.0;
  p_linear=5e-1;
  p_quadratic=0;
  PointLightPtr pointLight2 = std::make_shared<PointLight>(p_position, p_ambient, p_diffuse, p_specular, p_constant, p_linear, p_quadratic);
  PointLightRenderablePtr pointLightRenderable2 = std::make_shared<PointLightRenderable>(flatShader, pointLight2);
  localTransformation = glm::scale(glm::mat4(1.0), glm::vec3(0.5,0.5,0.5));
  pointLightRenderable2->setLocalTransform(localTransformation);
  viewer.addPointLight(pointLight2);
  viewer.addRenderable(pointLightRenderable2);*/
}

void setup_kart(Viewer& viewer, DynamicSystemPtr& system, DynamicSystemRenderablePtr &systemRenderable, ShaderProgramPtr &flatShader){
  //Initialize Kart with position, velocity, mass and radius and add it to the system
  glm::vec3 px(20.0,20.0,1.0),pv(0.0,0.0,0.0);
  float pm=1.0, pr=1.0;
  //px = glm::vec3(20.0,20.0,1.0);

  ParticlePtr mobile = std::make_shared<Particle>( px, pv, pm, pr);

  //Initialize a force field that apply only to the mobile particle
  glm::vec3 nullForce(0.0,0.0,0.0);
  std::vector<ParticlePtr> vParticle;
  vParticle.push_back(mobile);
  ConstantForceFieldPtr force = std::make_shared<ConstantForceField>(vParticle, nullForce);
  system->addForceField( force );
  ControlledForceFieldRenderablePtr forceRenderable = std::make_shared<ControlledForceFieldRenderable>( flatShader, force);
  KartRenderablePtr kart = std::make_shared<KartRenderable>(flatShader, mobile, force, forceRenderable, 0,0,200 );

  //Initialize a renderable for the force field applied on the mobile particle.
  //This renderable allows to modify the attribute of the force by key/mouse events
  //Add this renderable to the systemRenderable.
  //Add a damping force field to the mobile.
  DampingForceFieldPtr dampingForceField = std::make_shared<DampingForceField>(vParticle, 0.9);
  system->addForceField( dampingForceField );
  system->addParticle( mobile );
  HierarchicalRenderable::addChild(systemRenderable, kart->root);
  HierarchicalRenderable::addChild(systemRenderable, forceRenderable);
  HierarchicalRenderable::addChild(forceRenderable, kart->root);

}

void setup_billboard(Viewer& viewer, DynamicSystemPtr& system, DynamicSystemRenderablePtr &systemRenderable, ShaderProgramPtr &flatShader){
  glm::vec3 px(0.0,0.0,0.0),pv(0.0,0.0,0.0);
  float pm=1.0, pr=1.0;
  px = glm::vec3(0.0,0.0,1.0);
  ParticlePtr notMobile = std::make_shared<Particle>( px, pv, pm, pr);
  BillboardRenderable billboard = BillboardRenderable(flatShader, notMobile);
  viewer.addShaderProgram( billboard.texShader );
  billboard.master->setParentTransform( GeometricTransformation( glm::vec3{5,5,5.5},
                glm::angleAxis( float(M_PI), glm::normalize(glm::vec3( 0,1,0)) ),
                glm::vec3{1,1,1}).toMatrix() );



  HierarchicalRenderable::addChild(systemRenderable, billboard.master); 
  //  HierarchicalRenderable::addChild(kart->master, kart->root );
}

void setup_textures(Viewer& viewer, DynamicSystemPtr& system, DynamicSystemRenderablePtr &systemRenderable){
  // texturing plane

MaterialPtr pearl = Material::Emerald();
//Textured shader
ShaderProgramPtr texShader = std::make_shared<ShaderProgram>("../shaders/textureVertex.glsl","../shaders/textureFragment.glsl");
 viewer.addShaderProgram( texShader );

 ShaderProgramPtr multiTexShader = std::make_shared<ShaderProgram>("../shaders/multiTextureVertex.glsl","../shaders/multiTextureFragment.glsl");
 viewer.addShaderProgram( multiTexShader );

glm::mat4 parentTransformation;

 //Textured plane
 std::string filename = "./../textures/grass_texture.png";
 TexturedPlaneRenderablePtr texPlane = std::make_shared<TexturedPlaneRenderable>(texShader, filename);
 parentTransformation = glm::scale(glm::mat4(1.0), glm::vec3(100.0,100.0,100.0));
 texPlane->setParentTransform(parentTransformation);
 texPlane->setMaterial(pearl);

 viewer.addRenderable(texPlane);
 HierarchicalRenderable::addChild(systemRenderable, texPlane);
}
