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
#include "../include/texturing/MipMapCubeRenderable.hpp"
#include "../include/texturing/TexturedCubeRenderable.hpp"
#include "../include/ShaderProgram.hpp"
#include "../include/texturing/TexturedLightedMeshRenderable.hpp"
#include "../include/FrameRenderable.hpp"
#include "../include/DustRenderable.hpp"
#include "../include/dynamics/OpponentRenderable.hpp"


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

//Setup the textures in the scene
setup_textures(viewer, system, systemRenderable);

//Setup the kart in the scene²
  std::vector<ParticlePtr> vParticle;

 setup_kart(viewer, system, systemRenderable, flatShader, vParticle);
 setup_opponent_kart(viewer, system, systemRenderable, flatShader, vParticle);
viewer.addRenderable(systemRenderable);


// TODO init obstacles before

//Activate collision and set the restitution coefficient to 1.0
system->setCollisionsDetection(true);
system->setRestitution(1.0f);
  //Setup a Billboard in the scene
  setup_billboard(viewer, system, systemRenderable, flatShader) ;

  

 //hierarchical_kart( viewer, system, systemRenderable );
  std::shared_ptr<HierarchicalMeshRenderable> road = std::make_shared<HierarchicalMeshRenderable>(flatShader, "./../meshes/road.obj");

  road->setLocalTransform( GeometricTransformation( glm::vec3{0,0,-20.56},
						    glm::angleAxis( float(M_PI/2), glm::normalize(glm::vec3( 1,0,0)) ),
						    glm::vec3{180,180,180}).toMatrix() );
 
  viewer.addRenderable(road); 
  //Finally activate animation

  //setup walls for collision: DOES NOT WORK
  //setup_collision_walls(viewer, system, systemRenderable, flatShader);
  //Initialize a plane from 3 points and add it to the system as an obstacle
    glm::mat4 translate = glm::translate( glm::mat4(1.0), glm::vec3( -11,195, 5.0 ) );
    glm::mat4 rotate = glm::rotate(glm::mat4(1.0), (float)(M_PI), glm::normalize(glm::vec3(0.0,0.0,1.0)));
    glm::vec3 p5(-1.0,0.0,-1.0),p6(1.0,0.0,-1.0), p7(1.0,0.0,1.0), p8(-1.0,0.0,1.0);
    PlanePtr plane = std::make_shared<Plane>(p5, p6, p7, -411, 389, 195, "x");
    system->addPlaneObstacle(plane);

    //Create a plane renderable to display the obstacle
    PlaneRenderablePtr planeRenderable = std::make_shared<QuadRenderable>(flatShader, p5,p6,p7,p8);
    //-11,195
    planeRenderable->setParentTransform(translate*rotate);
    HierarchicalRenderable::addChild( systemRenderable, planeRenderable );

 viewer.startAnimation();
 viewer.setAnimationLoop(true, 29);
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

void setup_kart(Viewer& viewer, DynamicSystemPtr& system, DynamicSystemRenderablePtr& systemRenderable, ShaderProgramPtr &flatShader, std::vector<ParticlePtr> vParticle){
  //Initialize Kart with position, velocity, mass and radius and add it to the system
  glm::vec3 px(20.0,180.0,1.0),pv(0.0,0.0,0.0);
  float pm=1.0, pr=1.0;
  //px = glm::vec3(20.0,20.0,1.0);

  //Position the camera
  viewer.getCamera().setViewMatrix( glm::lookAt( px-glm::vec3(15,0,-5), px, glm::vec3(0,0,1)));

  ParticlePtr mobile = std::make_shared<Particle>( px, pv, pm, pr);

  //Initialize a force field that apply only to the mobile particle
  glm::vec3 nullForce(0.0,0.0,0.0);
  vParticle.push_back(mobile);
  ConstantForceFieldPtr force = std::make_shared<ConstantForceField>(vParticle, nullForce);
  system->addForceField( force );
  ControlledForceFieldRenderablePtr forceRenderable = std::make_shared<ControlledForceFieldRenderable>( flatShader, force);
  KartRenderablePtr kart = std::make_shared<KartRenderable>(flatShader, mobile, viewer, force, forceRenderable, 0,0,200 );

  //Initialize a renderable for the force field applied on the mobile particle.
  //This renderable allows to modify the attribute of the force by key/mouse events
  //Add this renderable to the systemRenderable.
  //Add a damping force field to the mobile.
  DampingForceFieldPtr dampingForceField = std::make_shared<DampingForceField>(vParticle, 0.9);
  system->addForceField( dampingForceField );
  system->addParticle( mobile );
  // HierarchicalRenderable::addChild(kart, kart->root);
  HierarchicalRenderable::addChild(systemRenderable, kart->root);
  HierarchicalRenderable::addChild(systemRenderable, forceRenderable);

  DustRenderablePtr dust = std::make_shared<DustRenderable>(flatShader, 100, kart->root, mobile, force);
  viewer.addRenderable(dust);
  // HierarchicalRenderable::addChild(forceRenderable, kart->root);

  HierarchicalRenderable::addChild(forceRenderable, kart->root);
  viewer.addRenderable(kart);

}

void setup_opponent_kart(Viewer& viewer, DynamicSystemPtr& system, DynamicSystemRenderablePtr& systemRenderable, ShaderProgramPtr &flatShader, std::vector<ParticlePtr> vParticle){
  //Initialize Kart with position, velocity, mass and radius and add it to the system
  // glm::vec3 px(10.0,180.0,1.0),pv(0.0,0.0,0.0);
  // float pm=1.0, pr=1.0;
  

  // ParticlePtr opp_mobile = std::make_shared<Particle>( px, pv, pm, pr);

  // //Initialize a force field that apply only to the mobile particle
  // vParticle.push_back(opp_mobile);
  OpponentRenderablePtr opp_kart = std::make_shared<OpponentRenderable>(flatShader, viewer, 0,0,200 );

  
  // system->addParticle( opp_mobile );
  

  viewer.addRenderable(opp_kart->root);
  viewer.addRenderable(opp_kart);

}

void setup_collision_walls(Viewer& viewer, DynamicSystemPtr& system, DynamicSystemRenderablePtr &systemRenderable, ShaderProgramPtr &flatShader){
    //Initialize a plane from 3 points and add it to the system as an obstacle
    glm::mat4 translate = glm::translate( glm::mat4(1.0), glm::vec3( -11,195, 5.0 ) );
    glm::mat4 rotate = glm::rotate(glm::mat4(1.0), (float)(M_PI), glm::normalize(glm::vec3(0.0,0.0,1.0)));
    glm::vec3 p5(-1.0,0.0,-1.0),p6(1.0,0.0,-1.0), p7(1.0,0.0,1.0), p8(-1.0,0.0,1.0);
    PlanePtr plane = std::make_shared<Plane>(p5, p6, p7, -411, 389, 195, "x");
    system->addPlaneObstacle(plane);

    //Create a plane renderable to display the obstacle
    PlaneRenderablePtr planeRenderable = std::make_shared<QuadRenderable>(flatShader, p5,p6,p7,p8);
    //-11,195
    planeRenderable->setParentTransform(translate*rotate);
    HierarchicalRenderable::addChild( systemRenderable, planeRenderable );

    //Initialize a plane from 3 points and add it to the system as an obstacle
    translate = glm::translate( glm::mat4(1.0), glm::vec3( 188,18, 5.0 ) );
    rotate = glm::rotate(glm::mat4(1.0), (float)(M_PI/2), glm::normalize(glm::vec3(0.0,0.0,1.0)));
    PlanePtr plane2 = std::make_shared<Plane>(p5, p6, p7, -348, 384, 188, "y");
    system->addPlaneObstacle(plane2);

    //Create a plane renderable to display the obstacle
    PlaneRenderablePtr planeRenderable2 = std::make_shared<QuadRenderable>(flatShader, p5,p6,p7,p8);
    //-11,195
    planeRenderable2->setParentTransform(translate*rotate);
    HierarchicalRenderable::addChild( systemRenderable, planeRenderable2 );

    //Initialize a plane from 3 points and add it to the system as an obstacle
    translate = glm::translate( glm::mat4(1.0), glm::vec3( -210,-4, 5.0 ) );
    rotate = glm::rotate(glm::mat4(1.0), (float)(M_PI/2), glm::normalize(glm::vec3(0.0,0.0,1.0)));
    PlanePtr plane3 = std::make_shared<Plane>(p5, p6, p7, -402, 394, -210, "y");
    system->addPlaneObstacle(plane3);

    //Create a plane renderable to display the obstacle
    PlaneRenderablePtr planeRenderable3 = std::make_shared<QuadRenderable>(flatShader, p5,p6,p7,p8);
    //-11,195
    planeRenderable3->setParentTransform(translate*rotate);
    HierarchicalRenderable::addChild( systemRenderable, planeRenderable3 );

    //Initialize a plane from 3 points and add it to the system as an obstacle
    translate = glm::translate( glm::mat4(1.0), glm::vec3( -30,-205, 5.0 ) );
    rotate = glm::rotate(glm::mat4(1.0), (float)(M_PI), glm::normalize(glm::vec3(0.0,0.0,1.0)));
    PlanePtr plane4 = std::make_shared<Plane>(p5, p6, p7, -395, 335, -205, "x");
    system->addPlaneObstacle(plane4);

    //Create a plane renderable to display the obstacle
    PlaneRenderablePtr planeRenderable4 = std::make_shared<QuadRenderable>(flatShader, p5,p6,p7,p8);
    //-11,195
    planeRenderable4->setParentTransform(translate*rotate);
    HierarchicalRenderable::addChild( systemRenderable, planeRenderable4 );

    //Initialize a plane from 3 points and add it to the system as an obstacle
    translate = glm::translate( glm::mat4(1.0), glm::vec3( 136,17.5, 5.0 ) );
    rotate = glm::rotate(glm::mat4(1.0), (float)(M_PI/2), glm::normalize(glm::vec3(0.0,0.0,1.0)));
    PlanePtr plane5 = std::make_shared<Plane>(p5, p6, p7, -240.5, 275.5, 136, "y");
    system->addPlaneObstacle(plane5);

    //Create a plane renderable to display the obstacle
    PlaneRenderablePtr planeRenderable5 = std::make_shared<QuadRenderable>(flatShader, p5,p6,p7,p8);
    //-11,195
    planeRenderable5->setParentTransform(translate*rotate);
    HierarchicalRenderable::addChild( systemRenderable, planeRenderable5 );

    //Initialize a plane from 3 points and add it to the system as an obstacle
    translate = glm::translate( glm::mat4(1.0), glm::vec3( -30.5,-156, 5.0 ) );
    rotate = glm::rotate(glm::mat4(1.0), (float)(M_PI), glm::normalize(glm::vec3(0.0,0.0,1.0)));
    PlanePtr plane6 = std::make_shared<Plane>(p5, p6, p7, -288.5, 227.5, -156, "x");
    system->addPlaneObstacle(plane6);

    //Create a plane renderable to display the obstacle
    PlaneRenderablePtr planeRenderable6 = std::make_shared<QuadRenderable>(flatShader, p5,p6,p7,p8);
    //-11,195
    planeRenderable6->setParentTransform(translate*rotate);
    HierarchicalRenderable::addChild( systemRenderable, planeRenderable6 );

    //Initialize a plane from 3 points and add it to the system as an obstacle
    translate = glm::translate( glm::mat4(1.0), glm::vec3( -12,143, 5.0 ) );
    rotate = glm::rotate(glm::mat4(1.0), (float)(M_PI), glm::normalize(glm::vec3(0.0,0.0,1.0)));
    PlanePtr plane7 = std::make_shared<Plane>(p5, p6, p7, -307, 283, 143, "x");
    system->addPlaneObstacle(plane7);

    //Create a plane renderable to display the obstacle
    PlaneRenderablePtr planeRenderable7 = std::make_shared<QuadRenderable>(flatShader, p5,p6,p7,p8);
    //-11,195
    planeRenderable7->setParentTransform(translate*rotate);
    HierarchicalRenderable::addChild( systemRenderable, planeRenderable7 );

    //Initialize a plane from 3 points and add it to the system as an obstacle
    translate = glm::translate( glm::mat4(1.0), glm::vec3( -160,-6, 5.0 ) );
    rotate = glm::rotate(glm::mat4(1.0), (float)(M_PI/2), glm::normalize(glm::vec3(0.0,0.0,1.0)));
    PlanePtr plane8 = std::make_shared<Plane>(p5, p6, p7, -160-300, -160+300, -160, "y");
    system->addPlaneObstacle(plane8);

    //Create a plane renderable to display the obstacle
    PlaneRenderablePtr planeRenderable8 = std::make_shared<QuadRenderable>(flatShader, p5,p6,p7,p8);
    //-11,195
    planeRenderable8->setParentTransform(translate*rotate);
    HierarchicalRenderable::addChild( systemRenderable, planeRenderable8 );

    //Initialize a plane from 3 points and add it to the system as an obstacle
    translate = glm::translate( glm::mat4(1.0), glm::vec3( 120,-110, 5.0 ) );
    rotate = glm::rotate(glm::mat4(1.0), (float)(M_PI), glm::normalize(glm::vec3(0.0,0.0,1.0)));
    PlanePtr plane9 = std::make_shared<Plane>(p5, p6, p7, 120-40, 120+40, -110, "x");
    system->addPlaneObstacle(plane9);

    //Create a plane renderable to display the obstacle
    PlaneRenderablePtr planeRenderable9 = std::make_shared<QuadRenderable>(flatShader, p5,p6,p7,p8);
    //-11,195
    planeRenderable9->setParentTransform(translate*rotate);
    HierarchicalRenderable::addChild( systemRenderable, planeRenderable9 );

    //Initialize a plane from 3 points and add it to the system as an obstacle
    translate = glm::translate( glm::mat4(1.0), glm::vec3( 174,-170, 5.0 ) );
    rotate = glm::rotate(glm::mat4(1.0), (float)(M_PI), glm::normalize(glm::vec3(0.0,0.0,1.0)));
    PlanePtr plane10 = std::make_shared<Plane>(p5, p6, p7, 174-40, 174+40, -170, "x");
    system->addPlaneObstacle(plane10);

    //Create a plane renderable to display the obstacle
    PlaneRenderablePtr planeRenderable10 = std::make_shared<QuadRenderable>(flatShader, p5,p6,p7,p8);
    //-11,195
    planeRenderable10->setParentTransform(translate*rotate);
    HierarchicalRenderable::addChild( systemRenderable, planeRenderable10 );

    //Initialize a plane from 3 points and add it to the system as an obstacle
    translate = glm::translate( glm::mat4(1.0), glm::vec3( 154,-190, 5.0 ) );
    rotate = glm::rotate(glm::mat4(1.0), (float)(M_PI/2), glm::normalize(glm::vec3(0.0,0.0,1.0)));
    PlanePtr plane11 = std::make_shared<Plane>(p5, p6, p7, -190-40, 190+40, 154, "y");
    system->addPlaneObstacle(plane11);

    //Create a plane renderable to display the obstacle
    PlaneRenderablePtr planeRenderable11 = std::make_shared<QuadRenderable>(flatShader, p5,p6,p7,p8);
    //-11,195
    planeRenderable11->setParentTransform(translate*rotate);
    HierarchicalRenderable::addChild( systemRenderable, planeRenderable11 );

    //Initialize a plane from 3 points and add it to the system as an obstacle
    translate = glm::translate( glm::mat4(1.0), glm::vec3( 100,-132, 5.0 ) );
    rotate = glm::rotate(glm::mat4(1.0), (float)(M_PI/2), glm::normalize(glm::vec3(0.0,0.0,1.0)));
    PlanePtr plane12 = std::make_shared<Plane>(p5, p6, p7, -132-50, 132+50, 100, "y");
    system->addPlaneObstacle(plane12);

    //Create a plane renderable to display the obstacle
    PlaneRenderablePtr planeRenderable12 = std::make_shared<QuadRenderable>(flatShader, p5,p6,p7,p8);
    //-11,195
    planeRenderable12->setParentTransform(translate*rotate);
    HierarchicalRenderable::addChild( systemRenderable, planeRenderable12 );



}

void setup_billboard(Viewer& viewer, DynamicSystemPtr& system, DynamicSystemRenderablePtr &systemRenderable, ShaderProgramPtr &flatShader){
  glm::vec3 px(10.0,10.0,0.0),pv(0.0,0.0,0.0);
  float pm=1.0, pr=1.0;
  px = glm::vec3(0.0,0.0,1.0);
  ParticlePtr notMobile = std::make_shared<Particle>( px, pv, pm, pr);
  BillboardRenderable billboard = BillboardRenderable(flatShader, notMobile, "./../textures/adele.jpg");
  viewer.addShaderProgram( billboard.texShader );
  billboard.master->setParentTransform( GeometricTransformation( glm::vec3{50,200,5.5},
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

 //GRASS
 std::string filename = "./../textures/grass_texture.png";
 TexturedPlaneRenderablePtr texPlane = std::make_shared<TexturedPlaneRenderable>(texShader, filename);
 parentTransformation = glm::scale(glm::mat4(1.0), glm::vec3(600.0,600.0,600.0));
 texPlane->setParentTransform(parentTransformation);
 texPlane->setMaterial(pearl);
 viewer.addRenderable(texPlane);

//  // middle mountain
// TexturedLightedMeshRenderablePtr mountain = std::make_shared<TexturedLightedMeshRenderable>( texShader, "../meshes/mountain.obj", "../textures/mountain.png");
//     mountain->setMaterial(pearl);
//     parentTransformation = glm::translate( glm::mat4(1.0), glm::vec3( 0, 4, 1.0 ) );
//     parentTransformation = glm::rotate( parentTransformation, float(M_PI_2), glm::vec3(1,0,0) );
//     parentTransformation = glm::scale( parentTransformation, glm::vec3(2,2,2));
//     mountain->setParentTransform( parentTransformation );
//     viewer.addRenderable(mountain);



// Border cube
    filename = "./../textures/brick.jpg";
    TexturedCubeRenderablePtr texCube = std::make_shared<TexturedCubeRenderable>(texShader, filename);
    parentTransformation = glm::translate(glm::mat4(1.0), glm::vec3(2,0.0,0.5));
    texCube->setParentTransform( GeometricTransformation( glm::vec3{-11,195,0},
                glm::angleAxis( float(M_PI), glm::normalize(glm::vec3( 0,1,0)) ),
                glm::vec3{400,3,3}).toMatrix() );

// texCube->setParentTransform(parentTransformation);
     texCube->setMaterial(pearl);
     // texCube->setWrapOption(0);
     viewer.addRenderable(texCube);


TexturedCubeRenderablePtr texCube2 = std::make_shared<TexturedCubeRenderable>(texShader, filename);
    parentTransformation = glm::translate(glm::mat4(1.0), glm::vec3(2,0.0,0.5));
    texCube2->setParentTransform( GeometricTransformation( glm::vec3{188,18,0},
                glm::angleAxis( float(M_PI), glm::normalize(glm::vec3( 0,1,0)) ),
                glm::vec3{3,366,3}).toMatrix() );

// texCube->setParentTransform(parentTransformation);
     texCube2->setMaterial(pearl);
     viewer.addRenderable(texCube2);

TexturedCubeRenderablePtr texCube3 = std::make_shared<TexturedCubeRenderable>(texShader, filename);
    parentTransformation = glm::translate(glm::mat4(1.0), glm::vec3(2,0.0,0.5));
    texCube3->setParentTransform( GeometricTransformation( glm::vec3{-210,-4,0},
                glm::angleAxis( float(M_PI), glm::normalize(glm::vec3( 0,1,0)) ),
                glm::vec3{3,398,3}).toMatrix() );

// texCube->setParentTransform(parentTransformation);
     texCube3->setMaterial(pearl);
     viewer.addRenderable(texCube3);


    TexturedCubeRenderablePtr texCube4 = std::make_shared<TexturedCubeRenderable>(texShader, filename);
    parentTransformation = glm::translate(glm::mat4(1.0), glm::vec3(2,0.0,0.5));
    texCube4->setParentTransform( GeometricTransformation( glm::vec3{-30,-205,0},
                glm::angleAxis( float(M_PI), glm::normalize(glm::vec3( 0,1,0)) ),
                glm::vec3{365,3,3}).toMatrix() );

// texCube->setParentTransform(parentTransformation);
     texCube->setMaterial(pearl);
     viewer.addRenderable(texCube4);

    TexturedCubeRenderablePtr texCube5 = std::make_shared<TexturedCubeRenderable>(texShader, filename);
    parentTransformation = glm::translate(glm::mat4(1.0), glm::vec3(2,0.0,0.5));
    texCube5->setParentTransform( GeometricTransformation( glm::vec3{-30.5,-156,0},
                glm::angleAxis( float(M_PI), glm::normalize(glm::vec3( 0,1,0)) ),
                glm::vec3{258,3,3}).toMatrix() );

// texCube->setParentTransform(parentTransformation);
     texCube->setMaterial(pearl);
     viewer.addRenderable(texCube5);

    TexturedCubeRenderablePtr texCube6 = std::make_shared<TexturedCubeRenderable>(texShader, filename);
    parentTransformation = glm::translate(glm::mat4(1.0), glm::vec3(2,0.0,0.5));
    texCube6->setParentTransform( GeometricTransformation( glm::vec3{136,17.5,0},
                glm::angleAxis( float(M_PI), glm::normalize(glm::vec3( 0,1,0)) ),
							   glm::vec3{3,258,3}).toMatrix() );

// texCube->setParentTransform(parentTransformation);
     texCube->setMaterial(pearl);
     viewer.addRenderable(texCube6);

TexturedCubeRenderablePtr texCube7 = std::make_shared<TexturedCubeRenderable>(texShader, filename);
    parentTransformation = glm::translate(glm::mat4(1.0), glm::vec3(2,0.0,0.5));
    texCube7->setParentTransform( GeometricTransformation( glm::vec3{-12,143,0},
                glm::angleAxis( float(M_PI), glm::normalize(glm::vec3( 0,1,0)) ),
                glm::vec3{295,3,3}).toMatrix() );

// texCube->setParentTransform(parentTransformation);
     texCube->setMaterial(pearl);
     viewer.addRenderable(texCube7);

TexturedCubeRenderablePtr texCube8 = std::make_shared<TexturedCubeRenderable>(texShader, filename);
    parentTransformation = glm::translate(glm::mat4(1.0), glm::vec3(2,0.0,0.5));
    texCube8->setParentTransform( GeometricTransformation( glm::vec3{-160,-6,0},
                glm::angleAxis( float(M_PI), glm::normalize(glm::vec3( 0,1,0)) ),
                glm::vec3{3,300,3}).toMatrix() );


      texCube->setMaterial(pearl);
      viewer.addRenderable(texCube8);

TexturedCubeRenderablePtr texCube9 = std::make_shared<TexturedCubeRenderable>(texShader, filename);
    parentTransformation = glm::translate(glm::mat4(1.0), glm::vec3(2,0.0,0.5));
    texCube9->setParentTransform( GeometricTransformation( glm::vec3{120,-110,0},
                glm::angleAxis( float(M_PI), glm::normalize(glm::vec3( 0,1,0)) ),
                glm::vec3{40,3,3}).toMatrix() );


      texCube->setMaterial(pearl);
      viewer.addRenderable(texCube9);

TexturedCubeRenderablePtr texCube10 = std::make_shared<TexturedCubeRenderable>(texShader, filename);
    parentTransformation = glm::translate(glm::mat4(1.0), glm::vec3(2,0.0,0.5));
    texCube10->setParentTransform( GeometricTransformation( glm::vec3{174,-170,0},
                glm::angleAxis( float(M_PI), glm::normalize(glm::vec3( 0,1,0)) ),
                glm::vec3{40,3,3}).toMatrix() );


      texCube10->setMaterial(pearl);
      viewer.addRenderable(texCube10);

TexturedCubeRenderablePtr texCube11 = std::make_shared<TexturedCubeRenderable>(texShader, filename);
    parentTransformation = glm::translate(glm::mat4(1.0), glm::vec3(2,0.0,0.5));
    texCube11->setParentTransform( GeometricTransformation( glm::vec3{154,-190,0},
                glm::angleAxis( float(M_PI), glm::normalize(glm::vec3( 0,1,0)) ),
                glm::vec3{3,40,3}).toMatrix() );


      texCube11->setMaterial(pearl);
      viewer.addRenderable(texCube11);

TexturedCubeRenderablePtr texCube12 = std::make_shared<TexturedCubeRenderable>(texShader, filename);
    parentTransformation = glm::translate(glm::mat4(1.0), glm::vec3(2,0.0,0.5));
    texCube12->setParentTransform( GeometricTransformation( glm::vec3{100,-132,0},
                glm::angleAxis( float(M_PI), glm::normalize(glm::vec3( 0,1,0)) ),
                glm::vec3{3,50,3}).toMatrix() );


      texCube12->setMaterial(pearl);
      viewer.addRenderable(texCube12);
  
  //Finish line
 filename = "./../textures/finish.jpg";
 TexturedPlaneRenderablePtr finish = std::make_shared<TexturedPlaneRenderable>(texShader, filename);
 glm::mat4 scale = glm::scale(glm::mat4(1.0), glm::vec3(40.0,13.0,600.0));
 glm::mat4 translatefinish = glm::translate(glm::mat4(1.0), glm::vec3(20,171,0.2));
 glm::mat4 rotatefinish = glm::rotate(glm::mat4(1.0), (float)(M_PI/2), glm::normalize(glm::vec3(0.0,0.0,1.0)));
 finish->setParentTransform(translatefinish*rotatefinish*scale);
 //finish->setMaterial(pearl);
 //finish->setWrapOption(0);
 finish->updateTextureOption();
 //viewer.addRenderable(finish);
 


 HierarchicalRenderable::addChild(systemRenderable, texPlane);
 HierarchicalRenderable::addChild(systemRenderable, finish);
}
