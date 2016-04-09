#include "../include/Viewer.hpp"
#include "../include/log.hpp"

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
}

int main()
{
  check_execution_directory();

  // initialize_scene( viewer );

    std::cout << "Hello World" << std::endl;
    return EXIT_SUCCESS;
}
