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
    root->setLocalTransform(GeometricTransformation( glm::vec3{}, glm::quat(), glm::vec3{}).toMatrix());

    //PERSON
    std::shared_ptr<SphereRenderable> head = std::make_shared<FloorRenderable>(flatShader);
    std::shared_ptr<FloorRenderable> body = std::make_shared<FloorRenderable>(flatShader, vPerson.width, vPerson.height, vPerson.length);

    std::shared_ptr<CylinderRenderable> arm_r = std::make_shared<CylinderRenderable>(flatShader, vPerson.faces, vPerson.length, vPerson.limb_diameter);
    std::shared_ptr<CylinderRenderable> arm_l = std::make_shared<CylinderRenderable>(flatShader, vPerson.faces, vPerson.length, vPerson.limb_diameter);
    std::shared_ptr<CylinderRenderable> leg_r = std::make_shared<CylinderRenderable>(flatShader, vPerson.faces, vPerson.length, vPerson.limb_diameter);
    std::shared_ptr<CylinderRenderable> leg_l = std::make_shared<CylinderRenderable>(flatShader, vPerson.faces, vPerson.length, vPerson.limb_diameter);

    std::shared_ptr<CylinderRenderable> neck = std::make_shared<CylinderRenderable>(flatShader, vPerson.faces, vPerson.neck_length, vPerson.limb_diameter);

    arm_r->setLocalTransform(glm::rotate(glm::mat4(1.0), (float)(0.25 + M_PI/2.0), glm::vec3(1.0,0.0,0.0)));   
    arm_r->setParentTransform(glm::translate(glm::mat4(1.0), glm::vec3(vPerson.width/2,vPerson.height/2 - vPerson.limb_diameter,vPerson.limb_length/2)));  

    arm_l->setLocalTransform(glm::rotate(glm::mat4(1.0), (float)(0.25 + M_PI/2.0), glm::vec3(1.0,0.0,0.0)));   
    arm_l->setParentTransform(glm::translate(glm::mat4(1.0), glm::vec3(-vPerson.width/2,vPerson.height/2 - vPerson.limb_diameter,vPerson.limb_length/2)));  

    leg_r->setLocalTransform(glm::rotate(glm::mat4(1.0), (float)(0.25 + M_PI/2.0), glm::vec3(1.0,0.0,0.0)));   
    leg_r->setParentTransform(glm::translate(glm::mat4(1.0), glm::vec3(vPerson.width/2,-vPerson.height/2 + vPerson.limb_diameter,vPerson.limb_length/2)));  

    leg_l->setLocalTransform(glm::rotate(glm::mat4(1.0), (float)(0.25 + M_PI/2.0), glm::vec3(1.0,0.0,0.0)));   
    leg_l->setParentTransform(glm::translate(glm::mat4(1.0), glm::vec3(-vPerson.width/2,-vPerson.height/2 + vPerson.limb_diameter,vPerson.limb_length/2)));  

    neck->setParentTransform(glm::translate(glm::mat4(1.0), glm::vec3(0.0,vPerson.height/2 + vPerson.neck_length,0.0))); 




    

   


    // Define parent/children relationships
    HierarchicalRenderable::addChild(person, body);
    HierarchicalRenderable::addChild(person, arm_r);
    HierarchicalRenderable::addChild(person, arm_l);
    HierarchicalRenderable::addChild(person, leg_r);
    HierarchicalRenderable::addChild(person, leg_l);
    HierarchicalRenderable::addChild(person, neck);
    HierarchicalRenderable::addChild(person, head);
    



    HierarchicalRenderable::addChild(root, person);
        
    // Add the root of the hierarchy to the viewer
    viewer.addRenderable(root);
   

}