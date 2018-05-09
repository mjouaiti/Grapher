//
//  Shader.cpp
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

#include "Shader.h"

/**
 * Shader Constructor
 * @param vertexSourcePath path of the vertexShader (GLchar*)
 * @param fragmentSourcePath path of the fragmentShader (GLchar*)
 */
Shader::Shader(const GLchar* vertexSourcePath, const GLchar* fragmentSourcePath)
{
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    
    try
    {
        vShaderFile.open(vertexSourcePath);
        fShaderFile.open(fragmentSourcePath);
        std::stringstream vShaderStream, fShaderStream;
        
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        
        vShaderFile.close();
        fShaderFile.close();
        
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    
    const GLchar* vShaderCode = vertexCode.c_str();
    const GLchar* fShaderCode = fragmentCode.c_str();
    
    GLint vertex, fragment;
    GLint success;
    GLchar infolog[512];
    
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infolog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED" << infolog << std::endl;
    }
    
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infolog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED" << infolog << std::endl;
    }
    
    this->m_program = glCreateProgram();
    glAttachShader(this->m_program, vertex);
    glAttachShader(this->m_program, fragment);
    glLinkProgram(this->m_program);
    
    glGetProgramiv(this->m_program, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(this->m_program, 512, NULL, infolog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED" << infolog << std::endl;
    }
    
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

/**
 * Shader Destructor
 */
Shader::~Shader()
{
    glDeleteProgram(m_program);
}

/**
 * Use Shader
 */
void Shader::use() const
{
    glUseProgram(this->m_program);
}
