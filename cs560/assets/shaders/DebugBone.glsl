// Floor Shader

#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;
layout (location = 3) in ivec4 a_BoneIds;
layout (location = 4) in vec4 a_BoneWeights;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

const int MAX_BONES = 100;
uniform mat4 u_FinalBoneMatrices[MAX_BONES];

void main()
{
    vec4 jointPosition = u_FinalBoneMatrices[a_BoneIds[0]] * vec4(a_Position, 1.0);

	gl_Position = u_Projection * u_View * u_Model * jointPosition;
}

//---------------------------------------------------------------------------

#type fragment
#version 330 core

out vec4 color;

void main()
{
    color = vec4(1.0, 1.0, 0.0, 1.0);
}