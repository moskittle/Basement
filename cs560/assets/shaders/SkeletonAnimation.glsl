// Skeleton Shader

#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;
layout (location = 3) in ivec4 a_BoneIds;
layout (location = 4) in vec4 a_BoneWeights;

// MVP
uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

// Bones
const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 u_FinalBoneMatrices[MAX_BONES];

out vec3 v_Normal;
out vec2 v_TexCoord;

void main()
{
    // calculate position with bone offset
    vec4 finalPosition = vec4(0.0);

    for(int i = 0; i < MAX_BONE_INFLUENCE; ++i)
    {
        int boneId = int(a_BoneIds[i]);
        float weight = a_BoneWeights[i];

        if(boneId == -1) { continue; }
        if(boneId >= MAX_BONES) { finalPosition = vec4(a_Position, 1.0); break; }

        vec4 localPosition = u_FinalBoneMatrices[boneId] * vec4(a_Position, 1.0);
        finalPosition += localPosition * weight;
        vec3 localNormal = mat3(u_FinalBoneMatrices[boneId]) * a_Normal;
    }

	gl_Position = u_Projection * u_View * u_Model * finalPosition;

	v_TexCoord = a_TexCoord;
	v_Normal = a_Normal;
}

//---------------------------------------------------------------------------

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec3 v_Normal;
in vec2 v_TexCoord;

uniform sampler2D u_Texture;

void main()
{
	color = texture(u_Texture, v_TexCoord);
	// color = vec4(1.0, 1.0, 0.0, 1.0);
	// color = vec4(v_TexCoord, 0.0, 1.0);
	// color = vec4(v_Normal, 1.0);
}