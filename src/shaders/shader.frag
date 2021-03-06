//
//  shader.frag
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

#version 330 core

layout(location = 0) out vec3 color;

uniform int c;

void main()
{
    switch (c)
    {
        case 0:
            color = vec3(1.0f, 0.0f, 0.0f);
            break;
        case 1:
            color = vec3(0.0f, 0.0f, 1.0f);
            break;
        case 2:
            color = vec3(0.0f, 1.0f, 0.0f);
            break;
        case 3:
            color = vec3(1.0f, 0.0f, 1.0f);
            break;
        default:
            color = vec3(0.0f, 0.0f, 0.0f);
            break;
    }
}
