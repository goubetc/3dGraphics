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


  //Initialize Kart with position, velocity, mass and radius and add it to the system
  
  glm::vec3 px(0.0,0.0,0.0),pv(0.0,0.0,0.0);
  float pm=1.0, pr=1.0;
  px = glm::vec3(0.0,0.0,1.0);

  ParticlePtr mobile = std::make_shared<Particle>( px, pv, pm, pr);
  
  

  //Initialize a force field that apply only to the mobile particle
  glm::vec3 nullForce(0.0,0.0,0.0);
  std::vector<ParticlePtr> vParticle;
  vParticle.push_back(mobile);
  ConstantForceFieldPtr force = std::make_shared<ConstantForceField>(vParticle, nullForce);
  system->addForceField( force );
  ControlledForceFieldRenderablePtr forceRenderable = std::make_shared<ControlledForceFieldRenderable>( flatShader, force );
  //ControlledWheelRenderablePtr rightWheelRenderable = std::make_shared<ControlledWheelRenderable>( flatShader );
  //ControlledWheelRenderablePtr leftWheelRenderable = std::make_shared<ControlledWheelRenderable>( flatShader );
  KartRenderablePtr kart = std::make_shared<KartRenderable>(flatShader, mobile, force, forceRenderable->getBack());

  //Initialize a renderable for the force field applied on the mobile particle.
  //This renderable allows to modify the attribute of the force by key/mouse events
  //Add this renderable to the systemRenderable.
  
  
//Add a damping force field to the mobile.
  DampingForceFieldPtr dampingForceField = std::make_shared<DampingForceField>(vParticle, 1);
  system->addForceField( dampingForceField );



  HierarchicalRenderable::addChild(systemRenderable, forceRenderable);
  HierarchicalRenderable::addChild(systemRenderable, kart->root);
  HierarchicalRenderable::addChild(forceRenderable, kart->root);
  //  HierarchicalRenderable::addChild(kart->master, kart->root );
  

  system->addParticle( mobile );
  //viewer.addRenderable(kart->leftWheelRenderable);
  //viewer.addRenderable(kart->rightWheelRenderable);
  //hierarchical_kart( viewer, system, systemRenderable );


  // TODO init obstacles before

  //Activate collision and set the restitution coefficient to 1.0
  system->setCollisionsDetection(true);
  system->setRestitution(1.0f);

   viewer.addRenderable(kart);
    
  //Finally activate animation
  viewer.startAnimation();
   // viewer.setAnimationLoop(true, 3.0);
}

void hierarchical_kart(Viewer& viewer, DynamicSystemPtr& system, DynamicSystemRenderablePtr &systemRenderable){

  
  
}
