// Stencil Test Shader

#type vertex
#version 330 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;

out vec2 v_TexCoord;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
    gl_Position = u_Projection * u_View * u_Model * vec4(a_Position + a_Normal * 0.05, 1.0);
}

//---------------------------------------------------------------------------

#type fragment
#version 330 core

out vec4 color;

void main()
{
    color = vec4(0.41, 0.75, 0.15, 1.0);
}