//
//  Shader.h
//
//  Code_Frontiers
//  Copyright (C) 2018  Université de Lorraine - CNRS
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//  Created by Melanie Jouaiti on 29/09/2017.
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
    Shader();
    Shader(const GLchar* vertexSourcePath, const GLchar* fragmentSourcePath);
    void use() const;
    ~Shader();
    
    GLuint m_program;               /**< program id */
};

#endif
