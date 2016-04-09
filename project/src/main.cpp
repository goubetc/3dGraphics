#include "../include/Viewer.hpp"
#include "../include/log.hpp"
#include "./../include/ShaderProgram.hpp"
#include "./../include/FrameRenderable.hpp"
#include "./../include/CylinderRenderable.hpp"
#include "./../include/FloorRenderable.hpp"
#include "../include/GeometricTransformation.hpp"


#include <iostream>
#include <string>
#include <fstream>

static void check_execution_directory()
{
  // There are many ways to do that.
  // This is a nightmare to do it in a portable way, for any platform you might want to use.
  // We do not want to add a dependency to the boost libraries just for that.
  // Thus, we chose a simple hack to warn you if you do not execute the program in the right directory :-}
  std::ifstream font( "../fonts/Anonymous_Pro.ttf");
  std::ifstream text_fragment( "../shaders/text_fragment.glsl");

  if( !font.is_open() || !text_fragment.is_open() )
    {
      LOG( fatal, "could not find ../fonts/Anonymous_Pro.ttf or ../shaders/text_fragment.glsl" );
      LOG( fatal, "are you sure you launched the executable in the build/ directory?");
      exit( EXIT_FAILURE );
    }
}



static void initialize_scene( Viewer& viewer )
{
    //initialize your scene into the viewer
  //Path to the vertex shader glsl code
  std::string vShader = "./../shaders/defaultVertex.glsl";
  //Path to the fragment shader glsl code
  std::string fShader = "./../shaders/defaultFragment.glsl";
  //Compile and link the shaders into a program
  ShaderProgramPtr defaultShader = std::make_shared<ShaderProgram>(vShader, fShader);
  //Add the shader program to the Viewer
  viewer.addShaderProgram(defaultShader);
  
  vShader = "./../shaders/flatVertex.glsl";
  fShader = "./../shaders/flatFragment.glsl";
  ShaderProgramPtr flatShader = std::make_shared<ShaderProgram>(vShader, fShader);

  //Add the shader to the Viewer
  viewer.addShaderProgram(flatShader);

  CylinderRenderablePtr frame = std::make_shared<CylinderRenderable>(defaultShader, 10, 15, 3);
  viewer.addRenderable(frame);
  //CylinderRenderablePtr frame = std::make_shared<CylinderRenderable>(defaultShader, 0, 0, 0, 0, 0, 0);
  //viewer.addRenderable(frame);
  
  
}

int main()
{
  check_execution_directory();

    check_execution_directory();
  int width = 1280;
  int heigth = 720;
  Viewer viewer (width, heigth);

  initialize_kart( viewer );
  while( viewer.isRunning() )
{
    viewer.handleEvent();
    viewer.draw();
    viewer.display();
}

    std::cout << "Hello World" << std::endl;
    return EXIT_SUCCESS;
}
