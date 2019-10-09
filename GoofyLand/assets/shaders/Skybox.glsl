// Skybox Shader

#type vertex
#version 330 core

layout (location = 0) in vec3 a_Position;

out vec3 v_TexCoord;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
    v_TexCoord = a_Position;        // the vector from position to position
    gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);
}

//---------------------------------------------------------------------------

#type fragment
#version 330 core

out vec4 color;

in vec3 v_TexCoord;

uniform samplerCube u_SkyboxTexture;

void main()
{
    color = texture(u_SkyboxTexture, v_TexCoord);
}