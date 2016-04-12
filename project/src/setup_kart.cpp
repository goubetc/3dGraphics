#include "../include/Viewer.hpp"
#include "../include/log.hpp"
#include "./../include/ShaderProgram.hpp"
#include "./../include/FrameRenderable.hpp"
#include "./../include/CylinderRenderable.hpp"
#include "./../include/FloorRenderable.hpp"
#include "./../include/TriangleRenderable.hpp"
#include "../include/GeometricTransformation.hpp"

#include "../include/ShaderProgram.hpp"

#include "../include/QuadRenderable.hpp"
#include "../include/FrameRenderable.hpp"

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

#include "../include/texturing/TexturedPlaneRenderable.hpp"
#include "../include/texturing/TexturedCubeRenderable.hpp"
#include "../include/texturing/MultiTexturedCubeRenderable.hpp"
#include "../include/texturing/MipMapCubeRenderable.hpp"
#include "../include/texturing/TexturedLightedMeshRenderable.hpp"
#include "../include/lighting/DirectionalLightRenderable.hpp"

#include <iostream>
#include <string>
#include <fstream>

void initialize_scene( Viewer& viewer ){
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
    
initialize_walls( viewer, system, systemRenderable );

initialize_billboard(viewer, system, systemRenderable);
    
//Finally activate animation
viewer.startAnimation();
}

void initialize_walls(Viewer& viewer, DynamicSystemPtr& system, DynamicSystemRenderablePtr &systemRenderable){

//Position the camera
viewer.getCamera().setViewMatrix( glm::lookAt( glm::vec3(0,0,10), glm::vec3(0,0,0), glm::vec3(0,1,0)));

ShaderProgramPtr flatShader = std::make_shared<ShaderProgram>("../shaders/defaultVertex.glsl", "../shaders/defaultFragment.glsl");
viewer.addShaderProgram( flatShader );

 //Initialize four planes to create walls arround the particles
 glm::vec3 planeNormal, planePoint;
 planeNormal = glm::vec3(-1,0,0);
 planePoint = glm::vec3(10,0,0);
 PlanePtr p0 = std::make_shared<Plane>( planeNormal, planePoint);
 system->addPlaneObstacle( p0 );

 planeNormal = glm::vec3(1,0,0);
 planePoint = glm::vec3(-10,0,0);
 PlanePtr p1 = std::make_shared<Plane>( planeNormal, planePoint);
 system->addPlaneObstacle( p1 );

 planeNormal = glm::vec3(0,-1,0);
 planePoint = glm::vec3(0,10,0);
 PlanePtr p2 = std::make_shared<Plane>( planeNormal, planePoint);
 system->addPlaneObstacle( p2 );

 planeNormal = glm::vec3(0,1,0);
 planePoint = glm::vec3(0,-10,0);
 PlanePtr p3 = std::make_shared<Plane>( planeNormal, planePoint);
 system->addPlaneObstacle( p3 );

 planeNormal = glm::vec3(0,0,1);
 planePoint = glm::vec3(0,0,0);
 PlanePtr floorPlane = std::make_shared<Plane>( planeNormal, planePoint);
 system->addPlaneObstacle( floorPlane );

 //Create  plane renderables to display each obstacle
 //Add them to the system renderable
 glm::vec3 x1, x2, x3, x4;
 glm::vec4 color;
 x1 = glm::vec3( 10, 10,5);
 x2 = glm::vec3( 10, 10,0);
 x3 = glm::vec3( 10,-10,0);
 x4 = glm::vec3( 10,-10,5);
 color = glm::vec4( 0.4, 0.2, 0.2, 1.0);
 PlaneRenderablePtr p1Renderable = std::make_shared<QuadRenderable>( flatShader, x1, x2, x3, x4, color);
 HierarchicalRenderable::addChild(systemRenderable, p1Renderable);

 x1 = glm::vec3( -10, 10,5);
 x2 = glm::vec3( -10, 10,0);
 x3 = glm::vec3( 10, 10,0);
 x4 = glm::vec3( 10, 10,5);
 color = glm::vec4( 0.4, 0.2, 0.2, 1.0);
 PlaneRenderablePtr p2Renderable = std::make_shared<QuadRenderable>( flatShader, x1, x2, x3, x4, color);
 HierarchicalRenderable::addChild(systemRenderable, p2Renderable);

 x1 = glm::vec3( -10, -10,5);
 x2 = glm::vec3( -10, -10,0);
 x3 = glm::vec3( -10,10,0);
 x4 = glm::vec3( -10,10,5);
 color = glm::vec4( 0.2, 0.4, 0.4, 1.0 );
 PlaneRenderablePtr p3Renderable = std::make_shared<QuadRenderable>( flatShader, x1, x2, x3, x4, color);
 HierarchicalRenderable::addChild(systemRenderable, p3Renderable);

 x1 = glm::vec3( 10, -10,5);
 x2 = glm::vec3( 10, -10,0);
 x3 = glm::vec3( -10,-10,0);
 x4 = glm::vec3( -10,-10,5);
 color = glm::vec4(0.2, 0.4, 0.4, 1.0);
 PlaneRenderablePtr p4Renderable = std::make_shared<QuadRenderable>( flatShader, x1, x2, x3, x4, color);
 HierarchicalRenderable::addChild(systemRenderable, p4Renderable);

 //Activate collision and set the restitution coefficient to 1.0
 system->setCollisionsDetection(true);
 system->setRestitution(1.0f);
  
}

void initialize_billboard(Viewer& viewer, DynamicSystemPtr& system, DynamicSystemRenderablePtr &systemRenderable){

    //Temporary variables
    glm::mat4 parentTransformation(1.0), localTransformation(1.0);
    MaterialPtr pearl = Material::Pearl();
    std::string filename = "./../textures/ensimag.jpg";

  ShaderProgramPtr flatShader = std::make_shared<ShaderProgram>("../shaders/defaultVertex.glsl", "../shaders/defaultFragment.glsl");
  viewer.addShaderProgram( flatShader );
  ShaderProgramPtr texShader = std::make_shared<ShaderProgram>("../shaders/textureVertex.glsl","../shaders/textureFragment.glsl");
  viewer.addShaderProgram( texShader );
  
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
  
  //root
  std::shared_ptr<CylinderRenderable> root = std::make_shared<CylinderRenderable>( flatShader, 1, 1, 1);
  root->setLocalTransform(glm::scale( glm::mat4(1.), glm::vec3(0,0,0)));

  //variables
  float poleRadius = 0.05;
  float poleHeight = 4;
  float poleX = 0.0;
  float poleY = 0.0;
  float poleZ = 0.0;
  float billboardWidth = 0.2;
  float billboardHeight = 4;
  float billboardThickness = 2;
  float billboardX = 0.0;
  float billboardY = 0.0;
  float billboardZ = 3.0;

  //pole of the billboard
  std::shared_ptr<CylinderRenderable> pole = std::make_shared<CylinderRenderable>(flatShader, 20, poleHeight, poleRadius);
  pole->setLocalTransform( glm::translate(glm::mat4(1.0), glm::vec3(poleX,poleY,poleZ)));

  //front of the billboard
  std::shared_ptr<FloorRenderable> billboard = std::make_shared<FloorRenderable>(flatShader, billboardWidth, billboardThickness, billboardHeight);
  pole->setLocalTransform( glm::translate(glm::mat4(1.0), glm::vec3(billboardX,billboardY,billboardZ)));

  HierarchicalRenderable::addChild(root, pole);
  HierarchicalRenderable::addChild(root, billboard);

  //Textured plane
    TexturedPlaneRenderablePtr texPlane = std::make_shared<TexturedPlaneRenderable>(texShader, filename);
    //parentTransformation = glm::scale(glm::mat4(1.0), glm::vec3(2,4,2));
    //texPlane->setLocalTransform(glm::rotate(glm::mat4(1.0), (float)(M_PI/2), glm::vec3(0.0,1.0,0.0)));
    //texPlane->setParentTransform(parentTransformation);
    //texPlane->setMaterial(pearl);
    //texBil->setParentTransform( glm::translate(glm::mat4(1.0), glm::vec3(billboardX+2,billboardY+2,billboardZ+2)));
    //viewer.addRenderable(texPlane);
    
    texPlane->setParentTransform( GeometricTransformation( glm::vec3{billboardX+0.101,billboardY,0},
        glm::angleAxis( float(M_PI/2), glm::normalize(glm::vec3( 0,1,0)) ),
        glm::vec3{2,4,2}).toMatrix() );
        
    texPlane->setLocalTransform(glm::rotate(glm::mat4(1.0), (float)(3*M_PI/2), glm::vec3(0.0,0.0,1.0)));
    
    texPlane->setMaterial(pearl);
    HierarchicalRenderable::addChild( root, texPlane );
    
    //Rotate the root
    //root->setParentTransform( glm::translate(glm::mat4(1.0), glm::vec3(0,0,6)));
    root->setParentTransform(glm::rotate(glm::mat4(1.0), (float)(M_PI), glm::vec3(0.0,1.0,0.0)));
    
    auto roadTurn = std::make_shared<TexturedPlaneRenderable>(texShader, filename);
    
    //HierarchicalRenderable::addChild(systemRenderable, root);
    HierarchicalRenderable::addChild(systemRenderable, roadTurn);
}
