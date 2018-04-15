//
//  Shader.h
//  Handshaking
//
//  Created by Melanie Jouaiti on 29/09/2017.
//  Copyright © 2017 Melanie Jouaiti. All rights reserved.
//

#ifndef _Shader_h
#define _Shader_h

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

#if defined(__linux__)
#include <GL/glew.h>
#endif
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#endif
#include <GLFW/glfw3.h>

class Shader
{
public:
    Shader(const GLchar* vertexSourcePath, const GLchar* fragmentSourcePath);
    void use() const;
    ~Shader();
    
    GLuint m_program;               /**< program id */
};

#endif
