#include "./../../include/dynamics/KartRenderable.hpp"
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
//#include "../include/setup_kart.hpp"
#include "../../include/HierarchicalSphereRenderable.hpp"
#include "../../include/Viewer.hpp"



#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>


KartRenderable::KartRenderable(ShaderProgramPtr flatShader, ParticlePtr mobile, Viewer& viewer, ConstantForceFieldPtr force, ControlledForceFieldRenderablePtr forceRenderable, float r, float g, float b) :
  
  HierarchicalRenderable(flatShader),
  m_particle(mobile),
  m_pBuffer(0),
  m_cBuffer(0),
  m_nBuffer(0),
  m_viewer(viewer)
  {

  m_force = force;
  m_forceRend = forceRenderable;

  ////////////////// kart parts //////////////////////////

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


////////////////////////////// Creating Kart ////////////////////////////

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
    


  ////////////////////////////// Linking Driver and Kart /////////////


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

void KartRenderable::do_draw(){

  //Update the parent and local transform matrix to position the geometric data according to the particle's data.
    const float& pRadius = m_particle->getRadius();
    const glm::vec3& pPosition = m_particle->getPosition();
    glm::mat4 scale = glm::scale(glm::mat4(1.0), glm::vec3(pRadius));
    glm::mat4 translate = glm::translate(glm::mat4(1.0), glm::vec3(pPosition));
    float dot = glm::dot(glm::normalize(m_force->getForce()), glm::normalize(glm::vec3(1.0,0.0,0.0)));
    glm::vec3 cross = glm::cross(m_force->getForce(), glm::vec3(1.0,0.0,0.0));
    
    if(!(dot != dot || acos(dot) != acos(dot)))  //check if dot is not NaN
      angle = acos(dot);
    glm::mat4 rotate;

    
    if(glm::dot(cross, glm::vec3(0.0,0.0,1.0)) > 0) angle = -angle;
    //std::cout<<m_force->getForce()[0]<<" "<<m_force->getForce()[1]<<" "<<m_force->getForce()[2]<<"\n";
    //if(m_force->getForce()[0] + m_force->getForce()[0] + m_force->getForce()[0] == 0) angle = 0;
    //if((int)m_back) angle = angle + M_PI;
    //std::cout<<angle<<std::endl;
    rotate = glm::rotate(glm::mat4(1.0), (float)(angle - M_PI/2), glm::vec3(0.0,0.0,1.0));
    glm::mat4 rotate2 = glm::rotate(glm::mat4(1.0), (float)(M_PI/2), glm::normalize(glm::vec3(1.0,0.0,0.0)));
    
    root->setParentTransform(translate*scale*rotate*rotate2);


    /////////////////////////   TURNING    ///////////////////////////

    int wheel_turn_right = (int)m_forceRend->m_status.turning_right;
    int wheel_turn_left = (int)m_forceRend->m_status.turning_left;
    if(wheel_turn_right)
    {
      wheel_br->setLocalTransform(glm::rotate(glm::mat4(1.0), (float)(-wheel_turn_right * M_PI/6), glm::normalize(glm::vec3(0.0,1.0,0.0))));
      wheel_bl->setLocalTransform(glm::rotate(glm::mat4(1.0), (float)(-wheel_turn_right * M_PI/6), glm::normalize(glm::vec3(0.0,1.0,0.0))));
    }

    
    else if(wheel_turn_left)
    {
      wheel_br->setLocalTransform(glm::rotate(glm::mat4(1.0), (float)(wheel_turn_left * M_PI/6), glm::normalize(glm::vec3(0.0,1.0,0.0))));
      wheel_bl->setLocalTransform(glm::rotate(glm::mat4(1.0), (float)(wheel_turn_left * M_PI/6), glm::normalize(glm::vec3(0.0,1.0,0.0))));
    }

    else
    {
      wheel_br->setLocalTransform(glm::rotate(glm::mat4(1.0), (float)0.0, glm::normalize(glm::vec3(0.0,1.0,0.0))));
      wheel_bl->setLocalTransform(glm::rotate(glm::mat4(1.0), (float)0.0, glm::normalize(glm::vec3(0.0,1.0,0.0))));
    }


    /////////////////////// CAMERA ///////////////////////////////////////////////////
    //Compute normalized mouse position between [-1,1]
    float x = 15 * cos((float)(angle));
    float y = 15  * sin((float)(angle));  
    glm::vec3 cameraPos = pPosition-glm::vec3(x,y,-10);
    m_viewer.getCamera().setViewMatrix( glm::lookAt( cameraPos, pPosition, glm::vec3(0,0,1)));
}

void KartRenderable::do_animate(float time) {
  const glm::vec3& pPosition = m_particle->getPosition();
  //Position the camera
  //m_viewer.getCamera().setViewMatrix( glm::lookAt( glm::vec3(5,20,10), glm::vec3(0,0,0), glm::vec3(0,0,1)));
  //m_viewer.getCamera();
}

KartRenderable::~KartRenderable()
{
  glcheck(glDeleteBuffers(1, &m_pBuffer));
  glcheck(glDeleteBuffers(1, &m_cBuffer));
  glcheck(glDeleteBuffers(1, &m_nBuffer));
}

/*glm::vec3 KartRenderable::getPosition() const{
    return m_positions;
}*/