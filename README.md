IMPORTANT:

This code has been tested on Mac OSX and Ubuntu. We do NOT support Windows. If you endeavour to run it on Windows, you're on your own.

REQUIRED DEPENDENCIES:
   
    OpenGL https://www.opengl.org
   
    GLFW http://www.glfw.org
   
    GLEW (Linux ONLY) http://glew.sourceforge.net
   
    V-REP http://www.coppeliarobotics.com

COMPILING

First, make sure that all the required dependencies (see above) are installed.

For Mac, you can use the .xcodefile. However, you will have to change the V-REP Header search path in Build Settings and the vrep files in the Build Phases.

You can also use cmake and follow these instructions which are the same for Ubuntu:

    cd wherever_the_code_is/build

    cmake -D VREP_PATH=/your/vrep/path/ ..

    make
    
RUNNING

    ./Graphical

Before running ./Graphical, open the VREP file mico.ttt in the V-REP file.
