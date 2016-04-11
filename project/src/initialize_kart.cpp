#include "../include/Viewer.hpp"
#include "../include/log.hpp"
#include "./../include/ShaderProgram.hpp"
#include "./../include/FrameRenderable.hpp"
#include "./../include/CylinderRenderable.hpp"
#include "./../include/FloorRenderable.hpp"
#include "./../include/TriangleRenderable.hpp"
#include "../include/GeometricTransformation.hpp"


#include <iostream>
#include <string>
#include <fstream>



static void initialize_hierarchical_scene(Viewer& viewer)
{
  ShaderProgramPtr flatShader = std::make_shared<ShaderProgram>("../shaders/defaultVertex.glsl", "../shaders/defaultFragment.glsl");
 // create programs (you can use the same program for both the child and parent)
    //ShaderProgramPtr parentProg = std::make_shared<ShaderProgram>(  ) ;
    //ShaderProgramPtr childProg = std::make_shared<ShaderProgram>(  ) ;    
    viewer.addShaderProgram( flatShader );
           
     FrameRenderablePtr frame = std::make_shared<FrameRenderable>(flatShader);
	viewer.addRenderable(frame);
	
	struct sSeat
	{
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

  struct sWheel
  {
    float diameter = 1;
    float height = 0.5;
    int faces = 20;
    float spread  = 3.5;
    
  };

  struct sWheel vWheel;

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




	/*
	COORDINATES:
	
	(width, height, length)
	
	(sideways, up, forwards)
	
	THIS ORDER 
	
	*/
	
	
    // Create renderables
    std::shared_ptr<CylinderRenderable> root = std::make_shared<CylinderRenderable>(flatShader, 1, 1, 1);
    root->setLocalTransform(GeometricTransformation( glm::vec3{}, glm::quat(), glm::vec3{}).toMatrix());
    
    std::shared_ptr<CylinderRenderable> seat = std::make_shared<CylinderRenderable>(flatShader, 1, 1, 1);
    seat->setLocalTransform(GeometricTransformation( glm::vec3{}, glm::quat(), glm::vec3{}).toMatrix());
    
    //SEAT
    std::shared_ptr<FloorRenderable> seat_top = std::make_shared<FloorRenderable>(flatShader, vSeat.width, vSeat.thickness, vSeat.height);
    std::shared_ptr<FloorRenderable> seat_bottom = std::make_shared<FloorRenderable>(flatShader, vSeat.width, vSeat.length, vSeat.thickness);
    
    seat_top->setLocalTransform(glm::translate(glm::mat4(1.0), glm::vec3(0.0,vSeat.height/2 + vSeat.thickness/2, vSeat.length/2))); 
    
    seat->setParentTransform(glm::translate(glm::mat4(1.0), glm::vec3(0.0,0.25,2.0)));         //SEAT LOCATION


    //HOOD
    std::shared_ptr<TriangleRenderable> hood = std::make_shared<TriangleRenderable>(flatShader, vFloor.width, vHood.height, vHood.length);
    
    hood->setParentTransform(glm::translate(glm::mat4(1.0), glm::vec3(0.0,vFloor.height/2 + vHood.height/2,-vFloor.length/2 + vHood.length/2)));   

    hood->setLocalTransform(glm::rotate(glm::mat4(1.0), (float)(-M_PI/2.0), glm::vec3(1.0,0.0,0.0)));   

    //BACK
    std::shared_ptr<FloorRenderable> back = std::make_shared<FloorRenderable>(flatShader, vFloor.width, vBack.height, vBack.length);
    back->setLocalTransform(glm::translate(glm::mat4(1.0), glm::vec3(0.0,vFloor.height/2 + vBack.height/2,vFloor.length/2 - vBack.length/2)));   
   
    
    
    //WHEELS
    std::shared_ptr<FloorRenderable> kart_floor = std::make_shared<FloorRenderable>(flatShader, vFloor.width, vFloor.height, vFloor.length);
    std::shared_ptr<CylinderRenderable> wheel_fl = std::make_shared<CylinderRenderable>(flatShader, vWheel.faces, vWheel.height, vWheel.diameter);
    std::shared_ptr<CylinderRenderable> wheel_fr = std::make_shared<CylinderRenderable>(flatShader, vWheel.faces, vWheel.height, vWheel.diameter);
    std::shared_ptr<CylinderRenderable> wheel_bl = std::make_shared<CylinderRenderable>(flatShader, vWheel.faces, vWheel.height, vWheel.diameter);
    std::shared_ptr<CylinderRenderable> wheel_br = std::make_shared<CylinderRenderable>(flatShader, vWheel.faces, vWheel.height, vWheel.diameter);
    
    
    kart_floor->setLocalTransform(glm::rotate(glm::mat4(1.0), (float)(M_PI/2.0), glm::vec3(1.0,0.0,0.0)));   
    wheel_fl->setLocalTransform(glm::rotate(glm::mat4(1.0), (float)(M_PI/2.0), glm::vec3(0.0,1.0,0.0)));   
    wheel_fl->setParentTransform(glm::translate(glm::mat4(1.0), glm::vec3(-vFloor.width/2 - vWheel.height/2,0.0,vWheel.spread)));   
    
    wheel_fr->setLocalTransform(glm::rotate(glm::mat4(1.0), (float)(-M_PI/2.0), glm::vec3(0.0,1.0,0.0)));   
    wheel_fr->setParentTransform(glm::translate(glm::mat4(1.0), glm::vec3(vFloor.width/2 + vWheel.height/2,0.0,vWheel.spread)));  
    
    wheel_bl->setLocalTransform(glm::rotate(glm::mat4(1.0), (float)(M_PI/2.0), glm::vec3(0.0,1.0,0.0)));   
    wheel_bl->setParentTransform(glm::translate(glm::mat4(1.0), glm::vec3(-vFloor.width/2 - vWheel.height/2,0.0,-vWheel.spread)));  
    
    wheel_br->setLocalTransform(glm::rotate(glm::mat4(1.0), (float)(-M_PI/2.0), glm::vec3(0.0,1.0,0.0)));   
    wheel_br->setParentTransform(glm::translate(glm::mat4(1.0), glm::vec3(vFloor.width/2 + vWheel.height/2,0.0,-vWheel.spread)));  
    
    // For each element of the hierarchy,
    // Set local transform and parent transform
    glm::mat4 rootParentTransform;
    root->setParentTransform(rootParentTransform);

   


    // Define parent/children relationships
    HierarchicalRenderable::addChild(root, kart_floor);
    HierarchicalRenderable::addChild(root, wheel_fl);
    HierarchicalRenderable::addChild(root, wheel_fr);
    HierarchicalRenderable::addChild(root, wheel_bl);
    HierarchicalRenderable::addChild(root, wheel_br);
    HierarchicalRenderable::addChild(seat, seat_top);
    HierarchicalRenderable::addChild(seat, seat_bottom);
    HierarchicalRenderable::addChild(root, seat);
    HierarchicalRenderable::addChild(root, hood);
    HierarchicalRenderable::addChild(root, back);



        
    // Add the root of the hierarchy to the viewer
    viewer.addRenderable(root);






   

}
