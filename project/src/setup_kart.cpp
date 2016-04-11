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
    
hierarchical_kart( viewer, system, systemRenderable );
    
//Finally activate animation
viewer.startAnimation();
}

void hierarchical_kart(Viewer& viewer, DynamicSystemPtr& system, DynamicSystemRenderablePtr &systemRenderable){

//Position the camera
viewer.getCamera().setViewMatrix( glm::lookAt( glm::vec3(0,0,10), glm::vec3(0,0,0), glm::vec3(0,1,0)));

ShaderProgramPtr flatShader = std::make_shared<ShaderProgram>("../shaders/defaultVertex.glsl", "../shaders/defaultFragment.glsl");
// create programs (you can use the same program for both the child and parent)
//ShaderProgramPtr parentProg = std::make_shared<ShaderProgram>(  ) ;
//ShaderProgramPtr childProg = std::make_shared<ShaderProgram>(  ) ;    
viewer.addShaderProgram( flatShader );
// FrameRenderablePtr frame = std::make_shared<FrameRenderable>(flatShader);
// std::cout << "address frame : " << &frame << std::endl;
// viewer.addRenderable(frame);

//Initialize particles with position, velocity, mass and radius and add it to the system
glm::vec3 px(0.0,0.0,0.0),pv(0.0,0.0,0.0);
float pm=1.0, pr=1.0;
px = glm::vec3(0.0,0.0,1.0);
ParticlePtr mobile = std::make_shared<Particle>( px, pv, pm, pr);
system->addParticle( mobile );
	
struct sSeat{
float width = 2;
float length = 2;
float height = 3;
float thickness = 0.5;
};
	
struct sSeat vSeat;
  struct sFloor
  {
    float width = 5;
    float length = 10;
    float height = 0.3;
  };

  struct sFloor vFloor;


  struct sHood
  {
    float length = 5;
    float height = 1.5;    
  };

  struct sHood vHood;



  struct sBack
  {
    float length = 2;
    float height = 1;    
  };

  struct sBack vBack;

/* COORDINATES:	
   (width, height, length)	
   (sideways, up, forwards)	
   THIS ORDER 	
*/
	
// Create renderables
std::shared_ptr<ParticleRenderable> master = std::make_shared<ParticleRenderable>( flatShader, mobile);

master->setLocalTransform(glm::scale( glm::mat4(1.), glm::vec3(0,0,0)));

  std::shared_ptr<CylinderRenderable> seat = std::make_shared<CylinderRenderable>(flatShader, 1, 1, 1);
  seat->setLocalTransform(GeometricTransformation( glm::vec3{}, glm::quat(), glm::vec3{}).toMatrix());
  
    
std::shared_ptr<FloorRenderable> seat_top = std::make_shared<FloorRenderable>(flatShader, vSeat.width, vSeat.thickness, vSeat.height);
std::shared_ptr<FloorRenderable> seat_bottom = std::make_shared<FloorRenderable>(flatShader, vSeat.width, vSeat.length, vSeat.thickness);
    
seat_top->setLocalTransform(glm::translate(glm::mat4(1.0), glm::vec3(0.0,vSeat.height/2 + vSeat.thickness/2, vSeat.length/2))); 
    
seat->setParentTransform(glm::translate(glm::mat4(1.0), glm::vec3(0.0,0.25,2.0)));         //SEAT LOCATION
    
    
std::shared_ptr<FloorRenderable> root = std::make_shared<FloorRenderable>(flatShader, 5, 10, 0.3);
std::shared_ptr<CylinderRenderable> wheel_fl = std::make_shared<CylinderRenderable>(flatShader, 20, 0.5, 1);
std::shared_ptr<CylinderRenderable> wheel_fr = std::make_shared<CylinderRenderable>(flatShader, 20, 0.5, 1);
std::shared_ptr<CylinderRenderable> wheel_bl = std::make_shared<CylinderRenderable>(flatShader, 20, 0.5, 1);
std::shared_ptr<CylinderRenderable> wheel_br = std::make_shared<CylinderRenderable>(flatShader, 20, 0.5, 1);
  

    //HOOD
    std::shared_ptr<TriangleRenderable> hood = std::make_shared<TriangleRenderable>(flatShader, vFloor.width, vHood.height, vHood.length);
    
    hood->setParentTransform(glm::translate(glm::mat4(1.0), glm::vec3(0.0,vFloor.height/2 + vHood.height/2,-vFloor.length/2 + vHood.length/2)));   

    hood->setLocalTransform(glm::rotate(glm::mat4(1.0), (float)(-M_PI/2.0), glm::vec3(1.0,0.0,0.0)));   

    //BACK
    std::shared_ptr<FloorRenderable> back = std::make_shared<FloorRenderable>(flatShader, vFloor.width, vBack.height, vBack.length);
    back->setLocalTransform(glm::translate(glm::mat4(1.0), glm::vec3(0.0,vFloor.height/2 + vBack.height/2,vFloor.length/2 - vBack.length/2)));   

  
    
//wheel_fl->setParentTransform(GeometricTransformation( glm::vec3{5,5,0}, glm::quat(), glm::vec3{1,1,1}).toMatrix());   
 wheel_fl->setLocalTransform(glm::rotate(glm::mat4(1.0), (float)(M_PI/2.0), glm::vec3(0.0,1.0,0.0)));   
 wheel_fl->setParentTransform(glm::translate(glm::mat4(1.0), glm::vec3(-2.75,0.0,3.5)));   
    
 wheel_fr->setLocalTransform(glm::rotate(glm::mat4(1.0), (float)(-M_PI/2.0), glm::vec3(0.0,1.0,0.0)));   
 wheel_fr->setParentTransform(glm::translate(glm::mat4(1.0), glm::vec3(2.75,0.0,3.5)));  
    
 wheel_bl->setLocalTransform(glm::rotate(glm::mat4(1.0), (float)(M_PI/2.0), glm::vec3(0.0,1.0,0.0)));   
 wheel_bl->setParentTransform(glm::translate(glm::mat4(1.0), glm::vec3(-2.75,0.0,-3.5)));  
    
 wheel_br->setLocalTransform(glm::rotate(glm::mat4(1.0), (float)(-M_PI/2.0), glm::vec3(0.0,1.0,0.0)));   
 wheel_br->setParentTransform(glm::translate(glm::mat4(1.0), glm::vec3(2.75,0.0,-3.5)));  
    
 // For each element of the hierarchy,
 // Set local transform and parent transform
 glm::mat4 rootParentTransform;
 root->setParentTransform(rootParentTransform);


 // Define parent/children relationships
 HierarchicalRenderable::addChild(root, wheel_fl);
 HierarchicalRenderable::addChild(root, wheel_fr);
 HierarchicalRenderable::addChild(root, wheel_bl);
 HierarchicalRenderable::addChild(root, wheel_br);
 HierarchicalRenderable::addChild(seat, seat_top);
 HierarchicalRenderable::addChild(seat, seat_bottom);
 HierarchicalRenderable::addChild(root, seat);
 HierarchicalRenderable::addChild(root, hood);
 HierarchicalRenderable::addChild(root, back);

 //HierarchicalRenderable::addChild(mobile, root);

        
 // Add the root of the hierarchy to the viewer
 //viewer.addRenderable(root);

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
 HierarchicalRenderable::addChild(systemRenderable, forceRenderable);

 //Add a damping force field to the mobile.
 DampingForceFieldPtr dampingForceField = std::make_shared<DampingForceField>(vParticle, 0.9);
 system->addForceField( dampingForceField );

 //Activate collision and set the restitution coefficient to 1.0
 system->setCollisionsDetection(true);
 system->setRestitution(1.0f);



 ///////////////////// Person ///////////////////////

 struct sPerson
 {
   float length = 0.5;
   float height = 2.5;  
   float width = 1.5;  
   float limb_diameter = 0.15;
   float limb_length = 2.0;
   int faces = 15;
   float neck_length = 0.2;
 }; struct sPerson vPerson;




 /*
   COORDINATES:
	
   (width, height, length)
	
   (sideways, up, forwards)
	
   THIS ORDER 
	
 */
	
	
 // Create renderables
 std::shared_ptr<CylinderRenderable> person = std::make_shared<CylinderRenderable>(flatShader, 1, 1, 1);
 person->setLocalTransform(GeometricTransformation( glm::vec3{}, glm::quat(), glm::vec3{}).toMatrix());

 //PERSON
 std::shared_ptr<HierarchicalSphereRenderable> head = std::make_shared<HierarchicalSphereRenderable>(flatShader);
 std::shared_ptr<FloorRenderable> body = std::make_shared<FloorRenderable>(flatShader, vPerson.width, vPerson.height, vPerson.length);

 body->setLocalTransform(glm::rotate(glm::mat4(1.0), (float)(M_PI/2.0), glm::vec3(1.0,0.0,0.0)));   
 body->setParentTransform(glm::translate(glm::mat4(1.0), glm::vec3(0.0,vPerson.height/2,0.0)));

 head->setParentTransform(glm::translate(glm::mat4(1.0), glm::vec3(0.0,vPerson.height + vPerson.neck_length + 1.0,0.0)));


 std::shared_ptr<CylinderRenderable> arm_r = std::make_shared<CylinderRenderable>(flatShader, vPerson.faces, vPerson.limb_length, vPerson.limb_diameter);
 std::shared_ptr<CylinderRenderable> arm_l = std::make_shared<CylinderRenderable>(flatShader, vPerson.faces, vPerson.limb_length, vPerson.limb_diameter);
 std::shared_ptr<CylinderRenderable> leg_r = std::make_shared<CylinderRenderable>(flatShader, vPerson.faces, vPerson.limb_length, vPerson.limb_diameter);
 std::shared_ptr<CylinderRenderable> leg_l = std::make_shared<CylinderRenderable>(flatShader, vPerson.faces, vPerson.limb_length, vPerson.limb_diameter);

 std::shared_ptr<CylinderRenderable> neck = std::make_shared<CylinderRenderable>(flatShader, vPerson.faces, vPerson.neck_length*3, vPerson.limb_diameter);

 arm_r->setLocalTransform(glm::rotate(glm::mat4(1.0), (float)(0.25), glm::vec3(1.0,0.0,0.0)));
 arm_r->setParentTransform(glm::translate(glm::mat4(1.0), glm::vec3(vPerson.width/2,vPerson.height - vPerson.limb_diameter*2,-vPerson.limb_length/2)));  

 arm_l->setLocalTransform(glm::rotate(glm::mat4(1.0), (float)(0.25), glm::vec3(1.0,0.0,0.0)));   
 arm_l->setParentTransform(glm::translate(glm::mat4(1.0), glm::vec3(-vPerson.width/2,vPerson.height - vPerson.limb_diameter*2,-vPerson.limb_length/2)));  

 // leg_r->setLocalTransform(glm::rotate(glm::mat4(1.0), (float)(0.25), glm::vec3(1.0,0.0,0.0)));   
 leg_r->setParentTransform(glm::translate(glm::mat4(1.0), glm::vec3(vPerson.width/2,vPerson.limb_diameter,-vPerson.limb_length/2)));  

 // leg_l->setLocalTransform(glm::rotate(glm::mat4(1.0), (float)(0.25), glm::vec3(1.0,0.0,0.0)));   
 leg_l->setParentTransform(glm::translate(glm::mat4(1.0), glm::vec3(-vPerson.width/2,vPerson.limb_diameter,-vPerson.limb_length/2)));  

neck->setLocalTransform(glm::rotate(glm::mat4(1.0), (float)(M_PI/2.0), glm::vec3(1.0,0.0,0.0)));   
 neck->setParentTransform(glm::translate(glm::mat4(1.0), glm::vec3(0.0,vPerson.height + vPerson.neck_length,0.0))); 

 
 person->setLocalTransform(GeometricTransformation( glm::vec3(0.0,vSeat.thickness,2.5),   glm::angleAxis( float(M_PI), glm::normalize(glm::vec3( 0,1,0)) ), glm::vec3(1,1,1)).toMatrix());  
 person->setParentTransform(glm::translate(glm::mat4(1.0), glm::vec3(0.0,vSeat.thickness,2.5)));
 // person->setParentTransform(glm::translate(glm::mat4(1.0), glm::vec3(0.0,vPerson.height + vPerson.neck_length,0.0)));





    

   


 // Define parent/children relationships
 HierarchicalRenderable::addChild(person, body);
 HierarchicalRenderable::addChild(person, arm_r);
 HierarchicalRenderable::addChild(person, arm_l);
 HierarchicalRenderable::addChild(person, leg_r);
 HierarchicalRenderable::addChild(person, leg_l);
 HierarchicalRenderable::addChild(person, neck);
 HierarchicalRenderable::addChild(person, head);
    



 HierarchicalRenderable::addChild(root, person);
        
 HierarchicalRenderable::addChild(forceRenderable, master);

 HierarchicalRenderable::addChild(master, root);
  
 root->setParentTransform(glm::rotate(glm::mat4(1.0), (float)(M_PI/2.0), glm::vec3(1.0,0.0,0.0)));

 // root->computeTotalParentTransform();
 // root->updateModelMatrix();
// hood->setParentTransform(glm::translate(glm::mat4(1.0), glm::vec3(0.0,vFloor.height/2 + vHood.height/2,-vFloor.length/2 + vHood.length/2)));   

//     hood->setLocalTransform(glm::rotate(glm::mat4(1.0), (float)(-M_PI/2.0), glm::vec3(1.0,0.0,0.0)));   
  
}

// CylinderRenderablePtr initialize_billboard(Viewer& viewer){
//   std::string filename = "./../textures/mipmap1.png";

//   ShaderProgramPtr flatShader = std::make_shared<ShaderProgram>("../shaders/defaultVertex.glsl", "../shaders/defaultFragment.glsl");
//   viewer.addShaderProgram( flatShader );
//   ShaderProgramPtr texShader = std::make_shared<ShaderProgram>("../shaders/textureVertex.glsl","../shaders/textureFragment.glsl");
//   viewer.addShaderProgram( texShader );
  
//   //root
//   std::shared_ptr<CylinderRenderable> root = std::make_shared<CylinderRenderable>( flatShader);
//   root->setLocalTransform(glm::scale( glm::mat4(1.), glm::vec3(0,0,0)));

//   //variables
//   float poleRadius = 2;
//   float poleHeight = 4;
//   float poleX = 0.0;
//   float poleY = 0.0;
//   float poleZ = 0.0;
//   float billboardWidth = 4;
//   float billboardHeight = 0.2;
//   float billboardThickness = 2;
//   float billboardX = 0.0;
//   float billboardY = 0.0;
//   float billboardZ = 3.0;

//   //pole of the billboard
//   std::shared_ptr<CylinderRenderable> pole = std::make_shared<CylinderRenderable>(flatShader, 20, poleHeight, poleRadius);
//   pole->setLocalTransform( glm::translate(glm::mat4(1.0), glm::vec3(poleX,poleY,poleZ)));

//   //front of the billboard
//   std::shared_ptr<FloorRenderable> billboard = std::make_shared<FloorRenderable>(flatShader, billboardWidth, billboardThickness, billboardHeight);
//   pole->setLocalTransform( glm::translate(glm::mat4(1.0), glm::vec3(billboardX,billboardY,billboardZ)));

//   HierarchicalRenderable::addChild(root, pole);
//   HierarchicalRenderable::addChild(root, billboard);

//   //Textured plane
//     TexturedPlaneRenderablePtr texPlane = std::make_shared<TexturedPlaneRenderable>(texShader, filename);
//     texPlane->setLocalTransform(glm::translate(glm::mat4(1.0), glm::vec3(billboardX,billboardY,billboardZ)));
//     //parentTransformation = glm::scale(glm::mat4(1.0), glm::vec3(10.0,10.0,10.0));
//     //texPlane->setParentTransform(parentTransformation);
//     texPlane->setMaterial(pearl);
//     viewer.addRenderable(texPlane);
//     texPlane->setMaterial(pearl);
//     HierarchicalRenderable::addChild( root, texPlane );
//     return root;
// }
