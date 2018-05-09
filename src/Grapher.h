//
//  Grapher.h
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
    Grapher(const double tMax = 130, const unsigned int nbVariables = 0);
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
    double m_tMax;                                          /**< maximum time */
    std::vector<float> m_record;                            /**< recorded values */
    unsigned int m_nbVariables;                             /**< number of variables to be recorded */
    std::vector<GLuint> m_VAO;                              /**< vector of VAOs */
    std::vector<GLuint> m_VBO;                              /**< vector of VBOs */
    std::vector<std::vector<glm::vec2> > m_values;          /**< values in a rendering format */
    std::vector<double> m_maxValues;                        /**< maximum values */
    std::vector<std::vector<unsigned int>> m_displayVariables;               /**< variables to be displayed */
};

#endif /* Grapher_h */