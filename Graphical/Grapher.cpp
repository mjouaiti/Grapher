//
//  Grapher.cpp
//  Handshaking
//
//  Created by Melanie Jouaiti on 29/09/2017.
//  Copyright © 2017 Melanie Jouaiti. All rights reserved.
//

#include "Grapher.h"
#include "../Simulation/Simulation2.h"

#define TMAX 110
#define OVERVIEW

/**
 * Grapher Constructor
 * @see Grapher(const unsigned int nbVariables)
 */
Grapher::Grapher(): m_t(0), m_dt(0.01), m_record(), m_values(),
                    m_VAO(), m_VBO(), m_nbVariables(-1)
{
}

/**
 * Grapher Constructor
 * @param nbVariables number of variables that will be recorded
 * @see Grapher()
 */
Grapher::Grapher(const unsigned int nbVariables): m_t(0), m_dt(0.01),
                                                m_record(), m_values(),
                                                m_VAO(), m_VBO(), m_nbVariables(nbVariables)
{
    m_VAO = std::vector<GLuint>(m_nbVariables, -1);
    m_VBO = std::vector<GLuint>(m_nbVariables, -1);
    m_values.resize(m_nbVariables);
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
}

/**
 * Set up and Bind the Buffers
 */
void Grapher::bindBuffers()
{
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
 * @param data the new data sent by the simulation to be added
 * @see updateBuffers()
 */
void Grapher::update(std::vector<double> data)
{
    if(data.size() != m_nbVariables)
    {
        std::cerr << "Warning:: Sent " << data.size() << " values but should have received: " << m_nbVariables << std::endl;
        m_nbVariables = (int)data.size();
        m_VAO = std::vector<GLuint>(m_nbVariables, -1);
        m_VBO = std::vector<GLuint>(m_nbVariables, -1);
        m_values.resize(m_nbVariables);
        bindBuffers();
    }
    for(unsigned int i = 1; i < m_nbVariables; i++)
    {
        
#ifdef OVERVIEW
        m_values[i].push_back(glm::vec2(2.0 * data[0] / TMAX - 1.0, data[i] * scale[i]));
#else
        m_values[i].push_back(glm::vec2(data[0]-1, data[i] * scale[i]));
        if(m_t > 1.51)
            m_values[i].back().x -= (m_t - 1.5 - m_dt);
#endif
        if(m_t > 0)
            m_values[i].push_back(m_values[i].back());
        // Each value (except for the first) is stored twice so that we can render lines. This is not necessary if you only wish to render points.
        m_record.push_back(data[i]);
    }
    // Once the curve fills the three quarters of the screen, it moves to the left with time so that it keeps beeing displayed online.
#ifndef OVERVIEW
    if(m_t > 1.5)
    {
        for (unsigned int i = 1; i < m_nbVariables; i++)
        {
            // For space and efficiency's sake, the values are progressively erased so that only the ones displayed are stored.
            if(m_values[i].size() > 400)
                m_values[i].erase(m_values[i].begin(), m_values[i].begin() + 100);
            // All values are moved one step to the left. This is the moving motion.
            for(unsigned int v = m_values[i].size(); v > 0; v--)
                m_values[i][v] -= glm::vec2(m_dt, 0);
        }
    }
#endif
    m_t += m_dt;
    updateBuffers();
}

/**
 * Render to the first Viewport
 * @param shader Shader to be used
 * @see render1(const Shader& shader)
 * @see render2(const Shader& shader)
 * @see render3(const Shader& shader)
 */
void Grapher::render0(const Shader& shader) const
{
    if(m_nbVariables < 1)
        return;
    shader.use();
    // Sets the curve color (0 is for blue)
    glUniform1i(glGetUniformLocation(shader.m_program, "c"), 0);
    glBindVertexArray(m_VAO[_sigma_s2E]);
    // This function is broken on Mac so don't even bother playing with it. Works with Linux though
    glPointSize(5);
    glDrawArrays(GL_LINES, 0, m_values[_sigma_s2E].size());
    
    glUniform1i(glGetUniformLocation(shader.m_program, "c"), 1);
    glBindVertexArray(m_VAO[_sigma_s2F]);
    glPointSize(5);
    glDrawArrays(GL_LINES, 0, m_values[_sigma_s2F].size());
    
    glUniform1i(glGetUniformLocation(shader.m_program, "c"), 2);
    glBindVertexArray(m_VAO[_sigma_s3E]);
    glPointSize(5);
    glDrawArrays(GL_LINES, 0, m_values[_sigma_s3E].size());
    glBindVertexArray(0);
    
    glUniform1i(glGetUniformLocation(shader.m_program, "c"), 3);
    glBindVertexArray(m_VAO[_sigma_s3F]);
    // This function is broken on Mac so don't even bother playing with it. Works with Linux though
    glPointSize(5);
    glDrawArrays(GL_LINES, 0, m_values[_sigma_s3F].size());
    glBindVertexArray(0);
}

/**
 * Render to the second Viewport
 * @param shader Shader to be used
 * @see render0(const Shader& shader)
 * @see render2(const Shader& shader)
 * @see render3(const Shader& shader)
 */
void Grapher::render1(const Shader& shader) const
{
    if(m_nbVariables < 1)
        return;
    shader.use();
    // Sets the curve color (0 is for blue)
    glUniform1i(glGetUniformLocation(shader.m_program, "c"), 0);
    glBindVertexArray(m_VAO[_Eps_2E]);
    // This function is broken on Mac so don't even bother playing with it. Works with Linux though
    glPointSize(5);
    glDrawArrays(GL_LINES, 0, m_values[_Eps_2E].size());

    glUniform1i(glGetUniformLocation(shader.m_program, "c"), 1);
    glBindVertexArray(m_VAO[_Eps_2F]);
    glPointSize(5);
    glDrawArrays(GL_LINES, 0, m_values[_Eps_2F].size());

    glUniform1i(glGetUniformLocation(shader.m_program, "c"), 2);
    glBindVertexArray(m_VAO[_Eps_3E]);
    glPointSize(5);
    glDrawArrays(GL_LINES, 0, m_values[_Eps_3E].size());
    glBindVertexArray(0);

    glUniform1i(glGetUniformLocation(shader.m_program, "c"), 3);
    glBindVertexArray(m_VAO[_Eps_3F]);
    // This function is broken on Mac so don't even bother playing with it. Works with Linux though
    glPointSize(5);
    glDrawArrays(GL_LINES, 0, m_values[_Eps_3F].size());
    glBindVertexArray(0);
}

/**
 * Render to the third Viewport
 * @param shader Shader to be used
 * @see render0(const Shader& shader)
 * @see render1(const Shader& shader)
 * @see render3(const Shader& shader)
 */
void Grapher::render2(const Shader& shader) const
{
    if(m_nbVariables < 1)
        return;
    shader.use();
    
    // Sets the curve color (0 is for blue, 1 is for red)
    glUniform1i(glGetUniformLocation(shader.m_program, "c"), 0);
    glBindVertexArray(m_VAO[_s2]);
    glPointSize(5);
    glDrawArrays(GL_LINES, 0, m_values[_s2].size());
    glBindVertexArray(0);
    
    glUniform1i(glGetUniformLocation(shader.m_program, "c"), 2);
    glBindVertexArray(m_VAO[_F2]);
    // This function is broken on Mac so don't even bother playing with it. Works with Linux though
    glPointSize(5);
    glDrawArrays(GL_LINES, 0, m_values[_F2].size());
    glBindVertexArray(0);
}

/**
 * Render to the fourth Viewport
 * @param shader Shader to be used
 * @see render0(const Shader& shader)
 * @see render1(const Shader& shader)
 * @see render2(const Shader& shader)
 */
void Grapher::render3(const Shader& shader) const
{
    if(m_nbVariables < 1)
        return;
    shader.use();
    
    // Sets the curve color (0 is for blue, 1 is for red)
    glUniform1i(glGetUniformLocation(shader.m_program, "c"), 0);
    glBindVertexArray(m_VAO[_s3]);
    glPointSize(5);
    glDrawArrays(GL_LINES, 0, m_values[_s3].size());
    glBindVertexArray(0);
    
    glUniform1i(glGetUniformLocation(shader.m_program, "c"), 1);
    glBindVertexArray(m_VAO[_F3]);
    // This function is broken on Mac so don't even bother playing with it. Works with Linux though
    glPointSize(5);
    glDrawArrays(GL_LINES, 0, m_values[_F3].size());
    glBindVertexArray(0);
}
