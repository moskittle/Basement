// Floor Shader

#type vertex
#version 330 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_TexCoord;

out vec2 v_TexCoord;


uniform mat4 u_Model;
layout (std140) uniform CameraMat
{
    mat4 u_View;
    mat4 u_Projection;
};

void main()
{
    v_TexCoord = a_TexCoord;
    gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);
}

//---------------------------------------------------------------------------

#type fragment
#version 330 core

in vec2 v_TexCoord;

out vec4 color;

uniform sampler2D u_Texture;

void main()
{
    color = texture(u_Texture, v_TexCoord);
}