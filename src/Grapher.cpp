//
//  Grapher.cpp
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

#ifdef __APPLE__
#   define _OSX
#elif defined(__linux__)
#   define _LINUX
#endif
#include "Grapher.h"

#define OVERVIEW

/**
 * Grapher Constructor
 * @see Grapher(const unsigned int nbVariables)
 */
Grapher::Grapher(): m_t(0), m_dt(0.05), m_tMax(-1),
                    m_adaptiveTime(false), m_record(), m_values(),
                    m_VAO(), m_VBO(), m_nbVariables(0)
{
}

/**
 * Grapher Constructor
 * @param nbVariables number of variables that will be recorded (unsigned int)
 * @see Grapher()
 */
Grapher::Grapher(const unsigned int width, const unsigned int height,
                 const double tMax, const double dt, const unsigned int nbVariables): m_t(0), m_dt(dt), m_tMax(tMax),
                                                                     m_adaptiveTime(false),
                                                                     m_record(), m_values(),
                                                                     m_VAO(), m_VBO(), m_nbVariables(nbVariables),
                                                                     m_multipleDisplay(false)
{
    if (!glfwInit())
    {
        printf("glfwInit() fail to initialize. \n");
        glfwTerminate();
        exit(-1);
    }
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    
    _Window = glfwCreateWindow(width, height, "Grapher", 0, 0);
    
    
    if (!_Window)
    {
        printf("Display window fail to create. \n");
        glfwTerminate();
        exit(-1);
    }
    
    glfwMakeContextCurrent(_Window);
    
#ifdef _LINUX
    glewExperimental = GL_TRUE;
    glewInit();
#endif
    
    glfwSetKeyCallback(_Window, key_callback);
    
#ifdef _LINUX
    glViewport(0, 0, width, height);
#else
    glViewport(0, 0, width * 2, height * 2);
#endif
    
    std::cout << glGetString(GL_VERSION) << std::endl;
    
#ifdef _LINUX
    V_WIDTH = width / 2; V_HEIGHT = height / 2;
#else
    V_WIDTH = width; V_HEIGHT = height;
#endif
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    
    if (m_tMax == -1)
    {
	std::cout << "WARNING::No Time limit Set" << std::endl;
        m_adaptiveTime = true;
        m_tMax = 20;
    }
    
    m_boundariesX[0] = 0;
    m_boundariesX[1] = tMax;
    m_boundariesY[0] = -1;
    m_boundariesY[1] = -1;
    m_VAO = std::vector<GLuint>(m_nbVariables, -1);
    m_VBO = std::vector<GLuint>(m_nbVariables, -1);
    m_values.resize(m_nbVariables);
    m_maxValues = std::vector<double>(m_nbVariables, 0.001);
    m_displayVariables = std::vector<std::vector<unsigned int>>(4);
    bindBuffers();
}

/**
 * Grapher Destructor
 */
Grapher::~Grapher()
{
    for(unsigned int i = 0; i < m_nbVariables; i++)
    {
        glDeleteVertexArrays(1, &m_VAO[i]);
        glDeleteBuffers(1, &m_VBO[i]);
    }
    glDeleteFramebuffers(1, &m_FBO);
    
    glfwTerminate();
    
    GLenum errGL;
    while ((errGL = glGetError()) != GL_NO_ERROR)
    {
        cerr << "OpenGL error: " << errGL << endl;
    }
}

/**
 * Set up and Bind the Buffers
 */
void Grapher::bindBuffers()
{
    glGenFramebuffers(1, &m_FBO);
    glGenTextures(1, &m_texture);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, V_WIDTH, V_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_texture, 0);
    GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, DrawBuffers);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    GLenum status = glCheckFramebufferStatus( GL_FRAMEBUFFER );
    switch(status)
    {
        case GL_FRAMEBUFFER_COMPLETE:
            std::cout << "FBO created" << std::endl;
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
            std::cout << "Incomplete attachment" << std::endl;
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            std::cout << "Missing attachment" << std::endl;
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
            std::cout << "Incomplete draw buffer" << std::endl;
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
            std::cout << "Incomplete read buffer" << std::endl;
            break;
        case GL_FRAMEBUFFER_UNSUPPORTED:
            std::cout << "Type is not Supported" << std::endl;
            break;
    }
    
    
    for(unsigned int i = 0; i < m_nbVariables; i++)
    {
        glGenVertexArrays(1, &m_VAO[i]);
        glBindVertexArray(m_VAO[i]);
        glGenBuffers(1, &m_VBO[i]);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO[i]);
        // We're using GL_STREAM_DRAW since each render is different
        glBufferData(GL_ARRAY_BUFFER, 0, &m_values[i][0], GL_STREAM_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_TRUE, 0, 0);
        glEnableVertexAttribArray(0);
    }
}

/**
 * Updates the Buffers with the new Values
 * @see update(std::vector<double> data)
 */
void Grapher::updateBuffers()
{
    for(unsigned int i = 0; i < m_nbVariables; i++)
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO[i]);
        glBufferData(GL_ARRAY_BUFFER, m_values[i].size() * sizeof(glm::vec2), &m_values[i][0], GL_STREAM_DRAW);
    }
}

/**
 * Updates the Buffers with the new Values
 * @param data the new data sent by the simulation to be added (std::vector<double>)
 * @see updateBuffers()
 */
void Grapher::update(std::vector<double> data)
{
    if(data.size() != m_nbVariables)
    {
	std::cout << "WARNING:: Updating variable number to " << (int)data.size() << std::endl;
        m_nbVariables = (int)data.size();
        m_VAO = std::vector<GLuint>(m_nbVariables, -1);
        m_VBO = std::vector<GLuint>(m_nbVariables, -1);
        m_values.resize(m_nbVariables);
        m_maxValues = std::vector<double>(m_nbVariables, 0.01);
        bindBuffers();
    }
    std::vector<double> scale(m_nbVariables);
    std::vector<unsigned int> indices;
    for(unsigned int i = 1; i < m_nbVariables; i++)
    {
        if(std::abs(data[i]) > m_maxValues[i])
        {
            scale[i] = std::abs(data[i]);
            indices.push_back(i);
        }
        else
            scale[i] = m_maxValues[i];
        
        if(!m_adaptiveTime)
        {
            if(m_boundariesY[0] == -1)
                m_values[i].push_back(glm::vec2(2 * (data[0] - m_boundariesX[0]) / (m_boundariesX[1] - m_boundariesX[0]) - 1.0, data[i]/ m_maxValues[i]));
            else
                m_values[i].push_back(glm::vec2(2 * (data[0] - m_boundariesX[0]) / (m_boundariesX[1] - m_boundariesX[0]) - 1.0, 2 * (data[i] - m_boundariesY[0]) / (m_boundariesY[1] - m_boundariesY[0]) - 1.0));
        }
        else
        {
            if(m_boundariesY[0] == -1)
                m_values[i].push_back(glm::vec2(data[0] - 1.0, data[i]/ m_maxValues[i]));
            else if(m_t > m_boundariesX[1])
            {
                for(unsigned int i = 1; i < m_nbVariables; i++)
                {
                    // For space and efficiency's sake, the values are progressively erased so that only the ones displayed are stored.
                    m_values[i].erase(m_values[i].begin(), m_values[i].begin() + 1);
                    // All values are moved one step to the left. This is the moving motion.
                    for(unsigned long v = m_values[i].size(); v > 0; v--)
                        m_values[i][v] -= glm::vec2(2 * m_dt / (m_boundariesX[1] - m_boundariesX[0]), 0);
                }
                m_values[i].push_back(glm::vec2(2 * (data[0] - m_boundariesX[0] - (m_t - m_boundariesX[1])) / (m_boundariesX[1] - m_boundariesX[0]) - 1.0, 2 * (data[i] - m_boundariesY[0]) / (m_boundariesY[1] - m_boundariesY[0]) - 1.0));
            }
            else
                m_values[i].push_back(glm::vec2(2 * (data[0] - m_boundariesX[0]) / (m_boundariesX[1] - m_boundariesX[0]) - 1.0, 2 * (data[i] - m_boundariesY[0]) / (m_boundariesY[1] - m_boundariesY[0]) - 1.0));
                //m_values[i].push_back(glm::vec2(data[0] - 1.0, 2 * (data[i] - m_boundariesY[0]) / (m_boundariesY[1] - m_boundariesY[0]) - 1.0));
        }

        if(m_t > 0)
            m_values[i].push_back(m_values[i].back());
        // Each value (except for the first) is stored twice so that we can render lines. This is not necessary if you only wish to render points.
        m_record.push_back(data[i]);
    }
    // Once the curve fills the three quarters of the screen, it moves to the left with time so that it keeps beeing displayed online.
//    if(m_adaptiveTime)
//    {
//        if(m_t > m_boundariesX[1])
//        {
//            for(unsigned int i = 1; i < m_nbVariables; i++)
//            {
//                // For space and efficiency's sake, the values are progressively erased so that only the ones displayed are stored.
//                //if(m_values[i].size() > 2 * m_boundariesX[1] / m_dt)
//                    //m_values[i].erase(m_values[i].begin(), m_values[i].begin() + 1);
//                // All values are moved one step to the left. This is the moving motion.
//                for(unsigned long v = m_values[i].size(); v > 0; v--)
//                    m_values[i][v] -= glm::vec2(m_dt, 0);
//            }
//        }
//    }
    if(m_boundariesY[0] == -1)
    {
        for(unsigned int i: indices)
        {
            for(unsigned int j = 0; j < m_values[i].size(); j++)
                m_values[i][j].y *= m_maxValues[i] / scale[i];
        }
    }
    m_maxValues = scale;
    m_t += m_dt;
    updateBuffers();
}

void Grapher::setDisplayedVariables(const unsigned int screen, std::vector<unsigned int> var)
{
    m_displayVariables[screen] = var;
    if(screen > 0)
        m_multipleDisplay = true;
}

void savePPM(std::string filename, unsigned char* pixels)
{
    std::ofstream out(filename.c_str(), std::ios::out);
    if(!out)
    {
        std::cerr << "ERROR::CANNOT::OPEN::FILE" << std::endl;
        exit(1);
    }
    
    out << "P3\n";
    out << 1024 << " " << 768 << std::endl;
    out << 3 << std::endl;
    int nbData = 0;
    for(unsigned int y = 0; y < 1024 * 768 * 3; y++)
    {
        out << (int)pixels[y] << " ";
        nbData++;
        if(nbData % 21 == 0)
                out << "\n";
    }
    out.close();
}

void Grapher::step(std::vector<double> values, const Shader& shader)
{
    glfwPollEvents();
    glClear(GL_COLOR_BUFFER_BIT);
    
    update(values);
    
    if(m_multipleDisplay)
    {
        glViewport(0, 0, V_WIDTH, V_HEIGHT);
        render0(shader);
        
        glViewport(0, V_HEIGHT, V_WIDTH, V_HEIGHT);
        render1(shader);
        
        glViewport(V_WIDTH, V_HEIGHT, V_WIDTH, V_HEIGHT);
        render2(shader);
        
        glViewport(V_WIDTH, 0, V_WIDTH, V_HEIGHT);
        render3(shader);
    }
    else
        render0(shader);
    
    glfwSwapBuffers(_Window);
}

int countF = 0;

void Grapher::renderToFramebuffer(const std::string& path, std::vector<double> values, const Shader& shader)
{
    glfwPollEvents();
    
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
    glViewport(0, 0, V_WIDTH, V_HEIGHT);
    glClear(GL_COLOR_BUFFER_BIT);
    
    update(values);
    
    if(m_multipleDisplay)
    {
        glViewport(0, 0, V_WIDTH, V_HEIGHT);
        render0(shader);
        
        glViewport(0, V_HEIGHT, V_WIDTH, V_HEIGHT);
        render1(shader);
        
        glViewport(V_WIDTH, V_HEIGHT, V_WIDTH, V_HEIGHT);
        render2(shader);
        
        glViewport(V_WIDTH, 0, V_WIDTH, V_HEIGHT);
        render3(shader);
    }
    else
        render0(shader);
    
    unsigned char* pixels = new unsigned char[V_WIDTH * V_HEIGHT * 3];
    memset(pixels, 0, V_WIDTH * V_HEIGHT * 3);
    glReadPixels(0, 0, V_WIDTH, V_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, pixels);
    
    savePPM(path + std::to_string(countF++) + ".ppm", pixels);
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    
    //glfwSwapBuffers(_Window);
}


/**
 * Render to the first Viewport
 * @param shader Shader to be used (Shader)
 * @see render1(const Shader& shader)
 * @see render2(const Shader& shader)
 * @see render3(const Shader& shader)
 */
void Grapher::render0(const Shader& shader) const
{
    if(m_nbVariables < 1 || m_displayVariables[0].size() < 1)
        return;
    shader.use();
    
    int c = 1;
//    if (m_values[1].size() > 55)
//        c = 1;
    for(int var: m_displayVariables[0])
    {
        // Sets the curve color (0 is for blue, 1 is for red, 2 for green, 3 for purple)
        glUniform1i(glGetUniformLocation(shader.m_program, "c"), c++);
        glBindVertexArray(m_VAO[var]);
        // This function is broken on Mac so don't even bother playing with it. Works with Linux though
        glPointSize(5);
        glDrawArrays(GL_LINES, 0, int(m_values[var].size()));
        glBindVertexArray(0);
    }
}

/**
 * Render to the second Viewport
 * @param shader Shader to be used (Shader)
 * @see render0(const Shader& shader)
 * @see render2(const Shader& shader)
 * @see render3(const Shader& shader)
 */
void Grapher::render1(const Shader& shader) const
{
    if(m_nbVariables < 1 || m_displayVariables[1].size() < 1)
        return;
    shader.use();
    int c = 0;
    for(int var: m_displayVariables[1])
    {
        // Sets the curve color (0 is for blue, 1 is for red, 2 for green, 3 for purple)
        glUniform1i(glGetUniformLocation(shader.m_program, "c"), c++);
        glBindVertexArray(m_VAO[var]);
        // This function is broken on Mac so don't even bother playing with it. Works with Linux though
        glPointSize(5);
        glDrawArrays(GL_LINES, 0, int(m_values[var].size()));
        glBindVertexArray(0);
    }
}

/**
 * Render to the third Viewport
 * @param shader Shader to be used (Shader)
 * @see render0(const Shader& shader)
 * @see render1(const Shader& shader)
 * @see render3(const Shader& shader)
 */
void Grapher::render2(const Shader& shader) const
{
    if(m_nbVariables < 1 || m_displayVariables[2].size() < 1)
        return;
    shader.use();
    int c = 0;
    for(int var: m_displayVariables[2])
    {
        // Sets the curve color (0 is for blue, 1 is for red, 2 for green, 3 for purple)
        glUniform1i(glGetUniformLocation(shader.m_program, "c"), c++);
        glBindVertexArray(m_VAO[var]);
        // This function is broken on Mac so don't even bother playing with it. Works with Linux though
        glPointSize(5);
        glDrawArrays(GL_LINES, 0, int(m_values[var].size()));
        glBindVertexArray(0);
    }
}

/**
 * Render to the fourth Viewport
 * @param shader Shader to be used (Shader)
 * @see render0(const Shader& shader)
 * @see render1(const Shader& shader)
 * @see render2(const Shader& shader)
 */
void Grapher::render3(const Shader& shader) const
{
    if(m_nbVariables < 1 || m_displayVariables[3].size() < 1)
        return;
    shader.use();
    int c = 0;
    for(int var: m_displayVariables[3])
    {
        // Sets the curve color (0 is for blue, 1 is for red, 2 for green, 3 for purple)
        glUniform1i(glGetUniformLocation(shader.m_program, "c"), c++);
        glBindVertexArray(m_VAO[var]);
        // This function is broken on Mac so don't even bother playing with it. Works with Linux though
        glPointSize(5);
        glDrawArrays(GL_LINES, 0, int(m_values[var].size()));
        glBindVertexArray(0);
    }
}

bool Grapher::shouldClose() const
{
    return glfwWindowShouldClose(_Window);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if(key == GLFW_KEY_Q && action == GLFW_PRESS)
    {
        
    }
}


void Grapher::setBoundariesX(const double xMin, const double xMax)
{
    m_boundariesX[0] = xMin;
    m_boundariesX[1] = xMax;
}

void Grapher::setBoundariesY(const double yMin, const double yMax)
{
    m_boundariesY[0] = yMin;
    m_boundariesY[1] = yMax;
}
