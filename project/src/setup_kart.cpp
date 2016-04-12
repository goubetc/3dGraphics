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
#include "../include/setup_kart.hpp"
#include "../include/HierarchicalSphereRenderable.hpp"
#include "../include/dynamics/KartRenderable.hpp"
#include "../include/texturing/TexturedPlaneRenderable.hpp"
#include "../include/lighting/DirectionalLightRenderable.hpp"

#include "../include/ShaderProgram.hpp"
#include "../include/FrameRenderable.hpp"

#include "../include/BillboardRenderable.hpp"

#include <iostream>
#include <string>
#include <fstream>

void initialize_kart( Viewer& viewer ){
//Set up a shader and add a 3D frame.
ShaderProgramPtr flatShader = std::make_shared<ShaderProgram>("../shaders/defaultVertex.glsl", "../shaders/defaultFragment.glsl");  
viewer.addShaderProgram( flatShader );
FrameRenderablePtr frame = std::make_shared<FrameRenderable>(flatShader);
std::cout << "address frame : " << &frame << std::endl;
viewer.addRenderable(frame);

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
viewer.getCamera().setViewMatrix( glm::lookAt( glm::vec3(0,0,10), glm::vec3(0,0,0), glm::vec3(0,1,0)));








///////////////////////////////////////////////////////////////////
glm::mat4 parentTransformation(1.0), localTransformation(1.0);
//Define a directional light for the whole scene
glm::vec3 d_direction = glm::normalize(glm::vec3(0.0,0.0,-1.0));
glm::vec3 d_ambient(1.0,1.0,1.0), d_diffuse(1.0,1.0,0.8), d_specular(1.0,1.0,1.0);
DirectionalLightPtr directionalLight = std::make_shared<DirectionalLight>(d_direction, d_ambient, d_diffuse, d_specular);
//Add a renderable to display the light and control it via mouse/key event
glm::vec3 lightPosition(0.0,0.0,5.0);
DirectionalLightRenderablePtr directionalLightRenderable = std::make_shared<DirectionalLightRenderable>(flatShader, directionalLight, lightPosition);
localTransformation = glm::scale(glm::mat4(1.0), glm::vec3(0.5,0.5,0.5));
directionalLightRenderable->setLocalTransform(localTransformation);
viewer.setDirectionalLight(directionalLight);
viewer.addRenderable(directionalLightRenderable);

// texturing plane

MaterialPtr pearl = Material::Pearl();
//Textured shader
ShaderProgramPtr texShader = std::make_shared<ShaderProgram>("../shaders/textureVertex.glsl","../shaders/textureFragment.glsl");
 viewer.addShaderProgram( texShader );

 ShaderProgramPtr multiTexShader = std::make_shared<ShaderProgram>("../shaders/multiTextureVertex.glsl","../shaders/multiTextureFragment.glsl");
 viewer.addShaderProgram( multiTexShader );


 //Textured plane
 std::string filename = "./../textures/grass_texture.png";
 TexturedPlaneRenderablePtr texPlane = std::make_shared<TexturedPlaneRenderable>(texShader, filename);
 parentTransformation = glm::scale(glm::mat4(1.0), glm::vec3(100.0,100.0,100.0));
 texPlane->setParentTransform(parentTransformation);
 texPlane->setMaterial(pearl);

 viewer.addRenderable(texPlane);


//Initialize Kart with position, velocity, mass and radius and add it to the system
 
glm::vec3 px(0.0,0.0,0.0),pv(0.0,0.0,0.0);
float pm=1.0, pr=1.0;
px = glm::vec3(0.0,0.0,1.0);

ParticlePtr mobile = std::make_shared<Particle>( px, pv, pm, pr);
KartRenderablePtr kart = std::make_shared<KartRenderable>(flatShader, mobile);
  

//Initialize a force field that apply only to the mobile particle
glm::vec3 nullForce(0.0,0.0,0.0);
std::vector<ParticlePtr> vParticle;
vParticle.push_back(mobile);
ConstantForceFieldPtr force = std::make_shared<ConstantForceField>(vParticle, nullForce);
system->addForceField( force );

//Initialize a renderable for the force field applied on the mobile particle.
//This renderable allows to modify the attribute of the force by key/mouse events
//Add this renderable to the systemRenderable.
ControlledForceFieldRenderablePtr forceRenderable = std::make_shared<ControlledForceFieldRenderable>( flatShader, force );
  
//Add a damping force field to the mobile.
DampingForceFieldPtr dampingForceField = std::make_shared<DampingForceField>(vParticle, 0.9);
system->addForceField( dampingForceField );



// TODO init obstacles before

//Activate collision and set the restitution coefficient to 1.0
system->setCollisionsDetection(true);
system->setRestitution(1.0f);

// viewer.addRenderable(kart);

// create Billboard
ParticlePtr notMobile = std::make_shared<Particle>( px, pv, pm, pr);
 BillboardRenderable billboard = BillboardRenderable(flatShader, notMobile);
 viewer.addShaderProgram( billboard.texShader );
 billboard.master->setParentTransform( GeometricTransformation( glm::vec3{5,5,5.5},
								glm::angleAxis( float(M_PI), glm::normalize(glm::vec3( 0,1,0)) ),
								glm::vec3{1,1,1}).toMatrix() );

 

 HierarchicalRenderable::addChild(systemRenderable, texPlane);
 HierarchicalRenderable::addChild(systemRenderable, forceRenderable);
 HierarchicalRenderable::addChild(systemRenderable, kart->master);
 HierarchicalRenderable::addChild(systemRenderable, billboard.master); 
 HierarchicalRenderable::addChild(forceRenderable, kart->master);
 //  HierarchicalRenderable::addChild(kart->master, kart->root );
  

  system->addParticle( mobile );
 //hierarchical_kart( viewer, system, systemRenderable );

    
 //Finally activate animation
 viewer.startAnimation();
}

void hierarchical_kart(Viewer& viewer, DynamicSystemPtr& system, DynamicSystemRenderablePtr &systemRenderable){

  // // FrameRenderablePtr frame = std::make_shared<FrameRenderable>(flatShader);
  // // std::cout << "address frame : " << &frame << std::endl;
  // // viewer.addRenderable(frame);

  // //Initialize particles with position, velocity, mass and radius and add it to the system
  // glm::vec3 px(0.0,0.0,0.0),pv(0.0,0.0,0.0);
  // float pm=1.0, pr=1.0;
  // px = glm::vec3(0.0,0.0,1.0);

  // ParticlePtr mobile = std::make_shared<Particle>( px, pv, pm, pr);
  // KartRenderablePtr kart = std::make_shared<KartRenderable>(flatShader, mobile);

  // system->addParticle( mobile );

  // /* COORDINATES:	
  //    (width, height, length)	
  //    (sideways, up, forwards)	
  //    THIS ORDER 	
  // */
	
  // // Create renderables
  // //Initialize four planes to create walls arround the particles
  // glm::vec3 planeNormal, planePoint;
  // planeNormal = glm::vec3(-1,0,0);
  // planePoint = glm::vec3(10,0,0);
  // PlanePtr p0 = std::make_shared<Plane>( planeNormal, planePoint);
  // system->addPlaneObstacle( p0 );

  // planeNormal = glm::vec3(1,0,0);
  // planePoint = glm::vec3(-10,0,0);
  // PlanePtr p1 = std::make_shared<Plane>( planeNormal, planePoint);
  // system->addPlaneObstacle( p1 );

  // planeNormal = glm::vec3(0,-1,0);
  // planePoint = glm::vec3(0,10,0);
  // PlanePtr p2 = std::make_shared<Plane>( planeNormal, planePoint);
  // system->addPlaneObstacle( p2 );

  // planeNormal = glm::vec3(0,1,0);
  // planePoint = glm::vec3(0,-10,0);
  // PlanePtr p3 = std::make_shared<Plane>( planeNormal, planePoint);
  // system->addPlaneObstacle( p3 );

  // planeNormal = glm::vec3(0,0,1);
  // planePoint = glm::vec3(0,0,0);
  // PlanePtr floorPlane = std::make_shared<Plane>( planeNormal, planePoint);
  // system->addPlaneObstacle( floorPlane );

  // //Create  plane renderables to display each obstacle
  // //Add them to the system renderable
  // glm::vec3 x1, x2, x3, x4;
  // glm::vec4 color;
  // x1 = glm::vec3( 10, 10,5);
  // x2 = glm::vec3( 10, 10,0);
  // x3 = glm::vec3( 10,-10,0);
  // x4 = glm::vec3( 10,-10,5);
  // color = glm::vec4( 0.4, 0.2, 0.2, 1.0);
  // PlaneRenderablePtr p1Renderable = std::make_shared<QuadRenderable>( flatShader, x1, x2, x3, x4, color);
  // HierarchicalRenderable::addChild(systemRenderable, p1Renderable);

  // x1 = glm::vec3( -10, 10,5);
  // x2 = glm::vec3( -10, 10,0);
  // x3 = glm::vec3( 10, 10,0);
  // x4 = glm::vec3( 10, 10,5);
  // color = glm::vec4( 0.4, 0.2, 0.2, 1.0);
  // PlaneRenderablePtr p2Renderable = std::make_shared<QuadRenderable>( flatShader, x1, x2, x3, x4, color);
  // HierarchicalRenderable::addChild(systemRenderable, p2Renderable);

  // x1 = glm::vec3( -10, -10,5);
  // x2 = glm::vec3( -10, -10,0);
  // x3 = glm::vec3( -10,10,0);
  // x4 = glm::vec3( -10,10,5);
  // color = glm::vec4( 0.2, 0.4, 0.4, 1.0 );
  // PlaneRenderablePtr p3Renderable = std::make_shared<QuadRenderable>( flatShader, x1, x2, x3, x4, color);
  // HierarchicalRenderable::addChild(systemRenderable, p3Renderable);

  // x1 = glm::vec3( 10, -10,5);
  // x2 = glm::vec3( 10, -10,0);
  // x3 = glm::vec3( -10,-10,0);
  // x4 = glm::vec3( -10,-10,5);
  // color = glm::vec4(0.2, 0.4, 0.4, 1.0);
  // PlaneRenderablePtr p4Renderable = std::make_shared<QuadRenderable>( flatShader, x1, x2, x3, x4, color);
  // HierarchicalRenderable::addChild(systemRenderable, p4Renderable);
  
}
