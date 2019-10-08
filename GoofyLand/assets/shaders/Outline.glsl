// Stencil Test Shader

#type vertex
#version 330 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_TexCoord;

out vec2 v_TexCoord;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
    v_TexCoord = a_TexCoord;
    gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);
}

//---------------------------------------------------------------------------

#type fragment
#version 330 core

out vec4 color;

void main()
{
    color = vec4(0.04, 0.28, 0.26, 1.0);
}