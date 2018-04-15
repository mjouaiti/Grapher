//
//  Grapher.h
//  Handshaking
//
//  Created by Melanie Jouaiti on 29/09/2017.
//  Copyright © 2017 Melanie Jouaiti. All rights reserved.
//

#ifndef Grapher_h
#define Grapher_h

#include <iostream>
#include <vector>
#include "Shader.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>

class Grapher
{
public:
    Grapher();
    Grapher(const unsigned int nbVariables);
    ~Grapher();
    void update(std::vector<double> data);
    void render0(const Shader& shader) const;
    void render1(const Shader& shader) const;
    void render2(const Shader& shader) const;
    void render3(const Shader& shader) const;
    
    
private:
    void bindBuffers();
    void updateBuffers();
    
    float m_t;                                              /**< current time in seconds */
    float m_dt;                                             /**< time step */
    std::vector<float> m_record;                            /**< recorded values */
    unsigned int m_nbVariables;                             /**< number of variables to be recorded */
    std::vector<GLuint> m_VAO;                              /**< vector of VAOs */
    std::vector<GLuint> m_VBO;                              /**< vector of VBOs */
    std::vector<std::vector<glm::vec2> > m_values;          /**< values in a rendering format */
    std::vector<double> m_maxValues;                        /**< maximum values */
};

#endif /* Grapher_h */
