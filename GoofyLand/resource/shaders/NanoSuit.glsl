// NanoSuit Shader

#type vertex
#version 330 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoord;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

out vec2 v_TexCoord;

void main()
{
    v_TexCoord = a_TexCoord;

    gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);
}

//---------------------------------------------------------------------------

#type fragment
#version 330 core
out vec4 FragColor;

in vec2 v_TexCoord;

struct Material
{
    sampler2D texture_diffuse1;
    // sampler2D texture_specular1;
    // sampler2D texture_height1;
};
uniform Material material;

void main()
{
    FragColor = texture(material.texture_diffuse1, v_TexCoord);
}