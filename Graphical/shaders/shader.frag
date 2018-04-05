#version 330 core

out vec4 color;

uniform int c;

void main()
{
    switch (c)
    {
        case 0:
            color = vec4(1.0f, 0.0f, 0.0f, 0.0f);
            break;
        case 1:
            color = vec4(0.0f, 0.0f, 1.0f, 0.0f);
            break;
        case 2:
            color = vec4(0.0f, 1.0f, 0.0f, 0.0f);
            break;
        case 3:
            color = vec4(1.0f, 0.0f, 1.0f, 0.0f);
            break;
        default:
            color = vec4(0.0f, 0.0f, 0.0f, 0.0f);
            break;
    }
}
