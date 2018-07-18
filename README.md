The Grapher Library is designed to continously display values of a system. Just feed the system values every timestep to the Grapher. 

IMPORTANT:

This code has been tested on Mac OSX and Ubuntu. We do NOT support Windows. If you endeavour to run it on Windows, you're on your own.

REQUIRED DEPENDENCIES:
   
    OpenGL https://www.opengl.org
   
    GLFW http://www.glfw.org
   
    GLEW (Linux ONLY) http://glew.sourceforge.net

COMPILING

First, make sure that all the required dependencies (see above) are installed.

For Mac, you can use the .xcodefile. However, you may have to change some library search path.

You can also use cmake and follow these instructions which are the same for Ubuntu:

    cd wherever_the_code_is/build

    cmake ..

    make
    
This will create the static library libGrapher.a in the build folder. You need to link this file in order to use the library.

Please see https://github.com/mjouaiti/Code_Frontiers for an example.
