// Screen Quad Shader

#type vertex
#version 330 core

layout (location = 0) in vec2 a_Position;
layout (location = 1) in vec2 a_TexCoord;

out vec2 v_TexCoord;

void main()
{
    v_TexCoord = a_TexCoord;
    gl_Position = vec4(a_Position.x, a_Position.y, 0.0, 1.0);
}

//---------------------------------------------------------------------------

#type fragment
#version 330 core
out vec4 color;

in vec2 v_TexCoord;

uniform sampler2D u_ScreenTexture;

void main()
{
    color = vec4(texture(u_ScreenTexture, v_TexCoord).rgb, 1.0);
}