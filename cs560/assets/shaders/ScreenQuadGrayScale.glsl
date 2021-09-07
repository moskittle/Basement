// Screen Quad Gray Scale Shader

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
    // inversion
    //color = vec4(vec3(1.0 - texture(u_ScreenTexture, v_TexCoord).rgb), 1.0);

    // weighted gray scale
    color = texture(u_ScreenTexture, v_TexCoord);
    float average = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
    color = vec4(average, average, average, 1.0);
}