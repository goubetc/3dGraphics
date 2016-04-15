#include "./../../include/dynamics/OpponentRenderable.hpp"
#include "./../../include/gl_helper.hpp"
#include "./../../include/log.hpp"
#include "./../../include/Utils.hpp"
#include "../../include/FrameRenderable.hpp"


#include "./../../include/FrameRenderable.hpp"
#include "./../../include/CylinderRenderable.hpp"
#include "./../../include/FloorRenderable.hpp"
#include "./../../include/TriangleRenderable.hpp"
#include "../../include/GeometricTransformation.hpp"

//#include "../include/ShaderProgram.hpp"

#include "../../include/dynamics/DampingForceField.hpp"
#include "../../include/dynamics/ConstantForceField.hpp"
#include "../../include/dynamics/SpringForceField.hpp"
#include "../../include/dynamics/EulerExplicitSolver.hpp"

#include "../../include/dynamics/ParticleRenderable.hpp"
#include "../../include/dynamics/ParticleListRenderable.hpp"
#include "../../include/dynamics/ConstantForceFieldRenderable.hpp"
#include "../../include/dynamics/SpringForceFieldRenderable.hpp"
#include "../../include/dynamics/SpringListRenderable.hpp"
#include "../../include/dynamics/ControlledForceFieldRenderable.hpp"
#include "../../include/dynamics/DynamicSystem.hpp"
#include "../../include/dynamics/DynamicSystemRenderable.hpp"
//#include "../include/setup_opponent.hpp"
#include "../../include/HierarchicalSphereRenderable.hpp"
#include "../../include/Viewer.hpp"



#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>


OpponentRenderable::OpponentRenderable(ShaderProgramPtr flatShader, Viewer& viewer, float r, float g, float b) :
  
  HierarchicalRenderable(flatShader),
  m_pBuffer(0),
  m_cBuffer(0),
  m_nBuffer(0),
  m_viewer(viewer)
  {

  wheelSpeed = 0;

    time1 = time(0);
  time2 = time(0);

  ////////////////// opponent parts //////////////////////////

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


////////////////////////////// Creating Opponent ////////////////////////////

// master = std::make_shared<ParticleRenderable>( flatShader, mobile, force, m_back );


// need to not print master.
//  master->setLocalTransform(glm::scale( glm::mat4(1.), glm::vec3(0,0,0)));

  std::shared_ptr<CylinderRenderable> seat = std::make_shared<CylinderRenderable>(flatShader, 1, 1, 1, 0,0,0);
  seat->setLocalTransform(GeometricTransformation( glm::vec3{}, glm::quat(), glm::vec3{}).toMatrix());
  
  std::shared_ptr<FloorRenderable> seat_top = std::make_shared<FloorRenderable>(flatShader, vSeat.width, vSeat.thickness, vSeat.height, 0, 0, 0);
  std::shared_ptr<FloorRenderable> seat_bottom = std::make_shared<FloorRenderable>(flatShader, vSeat.width, vSeat.length, vSeat.thickness, 0, 0, 0);
    
  seat_top->setLocalTransform(glm::translate(glm::mat4(1.0), glm::vec3(0.0,vSeat.height/2 + vSeat.thickness/2, vSeat.length/2))); 
    
  seat->setParentTransform(glm::translate(glm::mat4(1.0), glm::vec3(0.0,0.25,2.0)));         //SEAT LOCATION
    
    
  root = std::make_shared<FloorRenderable>(flatShader, 5, 10, 0.3, r,g,b);
  wheel_fl = std::make_shared<CylinderRenderable>(flatShader, 20, 0.5, 1, 0, 0, 0);
  wheel_fr = std::make_shared<CylinderRenderable>(flatShader, 20, 0.5, 1, 0, 0, 0);
  wheel_bl = std::make_shared<CylinderRenderable>(flatShader, 20, 0.5, 1, 0, 0, 0);
  wheel_br = std::make_shared<CylinderRenderable>(flatShader, 20, 0.5, 1, 0, 0, 0);
  

  //HOOD
  std::shared_ptr<TriangleRenderable> hood = std::make_shared<TriangleRenderable>(flatShader, vFloor.width, vHood.height, vHood.length, r,g,b);
    
  hood->setParentTransform(glm::translate(glm::mat4(1.0), glm::vec3(0.0,vFloor.height/2 + vHood.height/2,-vFloor.length/2 + vHood.length/2)));   

  hood->setLocalTransform(glm::rotate(glm::mat4(1.0), (float)(-M_PI/2.0), glm::vec3(1.0,0.0,0.0)));   

  //BACK
  std::shared_ptr<FloorRenderable> back = std::make_shared<FloorRenderable>(flatShader, vFloor.width, vBack.height, vBack.length, r,g,b);
  back->setLocalTransform(glm::translate(glm::mat4(1.0), glm::vec3(0.0,vFloor.height/2 + vBack.height/2,vFloor.length/2 - vBack.length/2)));   

  
    
  //wheel_fl->setParentTransform(GeometricTransformation( glm::vec3{5,5,0}, glm::quat(), glm::vec3{1,1,1}).toMatrix());   
  //wheel_fl->setLocalTransform(glm::rotate(glm::mat4(1.0), (float)(M_PI/2.0), glm::vec3(0.0,1.0,0.0)));   
  wheel_fl->setParentTransform(glm::translate(glm::mat4(1.0), glm::vec3(-2.75,0.0,3.5)) * glm::rotate(glm::mat4(1.0), (float)(M_PI/2.0), glm::vec3(0.0,1.0,0.0)));
    
  //wheel_fr->setLocalTransform(glm::rotate(glm::mat4(1.0), (float)(-M_PI/2.0), glm::vec3(0.0,1.0,0.0)));   
  wheel_fr->setParentTransform(glm::translate(glm::mat4(1.0), glm::vec3(2.75,0.0,3.5)) * glm::rotate(glm::mat4(1.0), (float)(-M_PI/2.0), glm::vec3(0.0,1.0,0.0)));  
    
  //wheel_bl->setParentTransform(glm::rotate(glm::mat4(1.0), (float)(M_PI/2.0), glm::vec3(0.0,1.0,0.0)));   
  wheel_bl->setParentTransform(glm::translate(glm::mat4(1.0), glm::vec3(-2.75,0.0,-3.5)) * glm::rotate(glm::mat4(1.0), (float)(M_PI/2.0), glm::vec3(0.0,1.0,0.0)));  
    
  //wheel_br->setParentTransform(glm::rotate(glm::mat4(1.0), (float)(-M_PI/2.0), glm::vec3(0.0,1.0,0.0)));   
  wheel_br->setParentTransform(glm::translate(glm::mat4(1.0), glm::vec3(2.75,0.0,-3.5)) * glm::rotate(glm::mat4(1.0), (float)(-M_PI/2.0), glm::vec3(0.0,1.0,0.0)));  
  
   // rightWheelRenderable->setParentTransform(glm::translate(glm::mat4(1.0), glm::vec3(2.75,0.0,-3.5)));
   // rightWheelRenderable->setLocalTransform(glm::translate(glm::mat4(1.0), glm::vec3(2.75,0.0,-3.5)));
   // leftWheelRenderable->setParentTransform(glm::translate(glm::mat4(1.0), glm::vec3(-2.75,0.0,-3.5)));
   // leftWheelRenderable->setLocalTransform(glm::translate(glm::mat4(1.0), glm::vec3(-2.75,0.0,-3.5)));
    
  // // For each element of the hierarchy,
  // // Set local transform and parent transform
  // glm::mat4 rootParentTransform;
  // root->setParentTransform(rootParentTransform);


  // Define parent/children relationships
  HierarchicalRenderable::addChild(root, wheel_fl);
  HierarchicalRenderable::addChild(root, wheel_fr);
  HierarchicalRenderable::addChild(root, wheel_bl);
  HierarchicalRenderable::addChild(root, wheel_br);
  
  // HierarchicalRenderable::addChild(root, leftWheelRenderable);
  // HierarchicalRenderable::addChild(root, rightWheelRenderable);
  
  // HierarchicalRenderable::addChild(leftWheelRenderable, wheel_bl);
  // HierarchicalRenderable::addChild(rightWheelRenderable, wheel_br);
  HierarchicalRenderable::addChild(seat, seat_top);
  HierarchicalRenderable::addChild(seat, seat_bottom);
  HierarchicalRenderable::addChild(root, seat);
  HierarchicalRenderable::addChild(root, hood);
  HierarchicalRenderable::addChild(root, back);
  
  

  //HierarchicalRenderable::addChild(mobile, root);

  ///////////////////////// Creating Driver //////////////////////////////

	
  // Create renderables
  std::shared_ptr<CylinderRenderable> person = std::make_shared<CylinderRenderable>(flatShader, 1, 1, 1, 255, 0, 0);
  person->setLocalTransform(GeometricTransformation( glm::vec3{}, glm::quat(), glm::vec3{}).toMatrix());

  //PERSON
  std::shared_ptr<HierarchicalSphereRenderable> head = std::make_shared<HierarchicalSphereRenderable>(flatShader, 255, 0, 0);
  std::shared_ptr<FloorRenderable> body = std::make_shared<FloorRenderable>(flatShader, vPerson.width, vPerson.height, vPerson.length, 0, 100, 50);

  body->setLocalTransform(glm::rotate(glm::mat4(1.0), (float)(M_PI/2.0), glm::vec3(1.0,0.0,0.0)));   
  body->setParentTransform(glm::translate(glm::mat4(1.0), glm::vec3(0.0,vPerson.height/2,0.0)));

  head->setParentTransform(glm::translate(glm::mat4(1.0), glm::vec3(0.0,vPerson.height + vPerson.neck_length + 1.0,0.0)));


  std::shared_ptr<CylinderRenderable> arm_r = std::make_shared<CylinderRenderable>(flatShader, vPerson.faces, vPerson.limb_length, vPerson.limb_diameter, 255, 0, 0);
  std::shared_ptr<CylinderRenderable> arm_l = std::make_shared<CylinderRenderable>(flatShader, vPerson.faces, vPerson.limb_length, vPerson.limb_diameter, 255, 0, 0);
  std::shared_ptr<CylinderRenderable> leg_r = std::make_shared<CylinderRenderable>(flatShader, vPerson.faces, vPerson.limb_length, vPerson.limb_diameter, 255, 0, 0);
  std::shared_ptr<CylinderRenderable> leg_l = std::make_shared<CylinderRenderable>(flatShader, vPerson.faces, vPerson.limb_length, vPerson.limb_diameter, 255, 0, 0);

  std::shared_ptr<CylinderRenderable> neck = std::make_shared<CylinderRenderable>(flatShader, vPerson.faces, vPerson.neck_length*3, vPerson.limb_diameter, 255, 0, 0);

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
    


  ////////////////////////////// Linking Driver and Opponent /////////////

  root->setParentTransform(glm::translate(glm::mat4(1.0), glm::vec3(0.0,0.0,1)) * glm::rotate(glm::mat4(1.0), (float)(M_PI/2), glm::normalize(glm::vec3(0.0,0.0,1.0))) * glm::rotate(glm::mat4(1.0), (float)(M_PI/2), glm::normalize(glm::vec3(1.0,0.0,0.0))));
  HierarchicalRenderable::addChild(root, person);
        
  //  HierarchicalRenderable::addChild(forceRenderable, master);

  // HierarchicalRenderable::addChild(master, root);
  
  // root->setParentTransform(glm::rotate(glm::mat4(1.0), (float)(M_PI/2.0), glm::vec3(0.0,1.0,0.0)));



  //Create buffers
  glGenBuffers(1, &m_pBuffer); //vertices
  glGenBuffers(1, &m_cBuffer); //colors
  glGenBuffers(1, &m_nBuffer); //normals

  //Activate buffer and send data to the graphics card
  glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_pBuffer));
  glcheck(glBufferData(GL_ARRAY_BUFFER, m_positions.size()*sizeof(glm::vec3), m_positions.data(), GL_STATIC_DRAW));
  glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_cBuffer));
  glcheck(glBufferData(GL_ARRAY_BUFFER, m_colors.size()*sizeof(glm::vec4), m_colors.data(), GL_STATIC_DRAW));
  glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_nBuffer));
  glcheck(glBufferData(GL_ARRAY_BUFFER, m_normals.size()*sizeof(glm::vec3), m_normals.data(), GL_STATIC_DRAW));
}

void OpponentRenderable::do_draw(){

  




    /////////////////////////   TURNING    ///////////////////////////

    glm::mat4 rot;
    glm::mat4 wSpeedL;
    glm::mat4 wSpeedR;
    wheelSpeed += 20/2000;
    wSpeedR = glm::rotate(glm::mat4(1.0), wheelSpeed, glm::normalize(glm::vec3(0.0,0.0,1.0)));
    wSpeedL = glm::rotate(glm::mat4(1.0), -wheelSpeed, glm::normalize(glm::vec3(0.0,0.0,1.0)));



    
      wheel_br->setLocalTransform(wSpeedR);
      wheel_bl->setLocalTransform(wSpeedL);
    

    wheel_fr->setLocalTransform(wSpeedR);
    wheel_fl->setLocalTransform(wSpeedL);

  //   time2 = time(0);
  // while(time2 - time1 < 3) time2 = time(0);

}

void OpponentRenderable::do_animate(float time) {

    float z = 0.5;
    float x = 3;


    root->addParentTransformKeyframe( GeometricTransformation( glm::vec3{20.0,170.0,1.0}, glm::quat(glm::vec3((float)(M_PI/2), 0, (float)(-M_PI/2)))), 0.0);
    root->addParentTransformKeyframe( GeometricTransformation( glm::vec3{20.0,170.0,1.0}, glm::quat(glm::vec3((float)(M_PI/2), 0, (float)(-M_PI/2)))), 0.0*z + x );
    root->addParentTransformKeyframe( GeometricTransformation( glm::vec3{120.0,180.0,1.0}, glm::quat(glm::vec3((float)(M_PI/2), 0, (float)(-M_PI/2)))), 3.0*z + x );
    root->addParentTransformKeyframe( GeometricTransformation( glm::vec3{170.0,180.0,1.0}, glm::quat(glm::vec3((float)(M_PI/2), 0, (float)(M_PI-0.5)))), 4.5*z + x );
    root->addParentTransformKeyframe( GeometricTransformation( glm::vec3{170.0,140.0,1.0}, glm::quat(glm::vec3((float)(M_PI/2), 0, (float)(M_PI)))), 6*z + x );
    root->addParentTransformKeyframe( GeometricTransformation( glm::vec3{170.0,-90.0,1.0}, glm::quat(glm::vec3((float)(M_PI/2), 0, (float)(M_PI)))), 9*z + x );
    root->addParentTransformKeyframe( GeometricTransformation( glm::vec3{170.0,-140.0,1.0}, glm::quat(glm::vec3((float)(M_PI/2), 0, (float)(M_PI/2 - 0.5)))), 10*z + x );
    root->addParentTransformKeyframe( GeometricTransformation( glm::vec3{125.0,-140.0,1.0}, glm::quat(glm::vec3((float)(M_PI/2), 0, (float)(M_PI)))), 12*z + x );
    root->addParentTransformKeyframe( GeometricTransformation( glm::vec3{125.0,-180.0,1.0}, glm::quat(glm::vec3((float)(M_PI/2), 0, (float)(M_PI/2+0.5)))), 14*z + x );
    root->addParentTransformKeyframe( GeometricTransformation( glm::vec3{100.0,-180.0,1.0}, glm::quat(glm::vec3((float)(M_PI/2), 0, (float)(M_PI/2)))), 15*z + x );
    root->addParentTransformKeyframe( GeometricTransformation( glm::vec3{-140.0,-180.0,1.0}, glm::quat(glm::vec3((float)(M_PI/2), 0, (float)(M_PI/2)))), 18*z + x );
    root->addParentTransformKeyframe( GeometricTransformation( glm::vec3{-180.0,-180.0,1.0}, glm::quat(glm::vec3((float)(M_PI/2), 0, (float)(2*M_PI+0.5)))), 19*z + x );
    root->addParentTransformKeyframe( GeometricTransformation( glm::vec3{-180.0,-140.0,1.0}, glm::quat(glm::vec3((float)(M_PI/2), 0, (float)(2*M_PI)))), 20*z + x );
    root->addParentTransformKeyframe( GeometricTransformation( glm::vec3{-180.0,140.0,1.0}, glm::quat(glm::vec3((float)(M_PI/2), 0, (float)(2*M_PI)))), 24*z + x );
    root->addParentTransformKeyframe( GeometricTransformation( glm::vec3{-145.0,180.0,1.0}, glm::quat(glm::vec3((float)(M_PI/2), 0, (float)(-M_PI/2 - 0.5)))), 25*z + x );
    root->addParentTransformKeyframe( GeometricTransformation( glm::vec3{-120.0,180.0,1.0}, glm::quat(glm::vec3((float)(M_PI/2), 0, (float)(-M_PI/2)))), 26*z + x );
    root->addParentTransformKeyframe( GeometricTransformation( glm::vec3{20.0,180.0,1.0}, glm::quat(glm::vec3((float)(M_PI/2), 0, (float)(-M_PI/2)))), 29*z + x );
}

OpponentRenderable::~OpponentRenderable()
{
  glcheck(glDeleteBuffers(1, &m_pBuffer));
  glcheck(glDeleteBuffers(1, &m_cBuffer));
  glcheck(glDeleteBuffers(1, &m_nBuffer));
}

/*glm::vec3 OpponentRenderable::getPosition() const{
    return m_positions;
}*/