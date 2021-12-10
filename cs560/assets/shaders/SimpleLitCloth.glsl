// Phong Lighting Shader for Cloth

#type vertex
#version 330 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 3) in ivec4 a_BoneIds;   // vertex index is store in a_BoneIds[0]

uniform vec3 u_OffsetPositions[1000];
uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

out vec3 v_Normal;
out vec3 v_WorldPosition;

void main()
{
    vec3 position = a_Position + u_OffsetPositions[a_BoneIds[0]];
    v_WorldPosition = vec3(u_Model * vec4(position, 1.0));     // Fragment position in world coord
    v_Normal = a_Normal;

    gl_Position = u_Projection * u_View * u_Model * vec4(position, 1.0);
}

//---------------------------------------------------------------------------

#type fragment
#version 330 core
out vec4 color;

in vec3 v_Normal;
in vec3 v_WorldPosition;

uniform vec3 u_BaseColor;
uniform float u_AmbientIntensity;
// uniform float u_SpecularIntensity;
// uniform int u_Shininess;
// uniform vec3 u_ViewPosition;

uniform vec3 u_LightColor;
uniform vec3 u_LightPosition;

void main()
{
    // ambient
    vec3 ambient = u_AmbientIntensity * u_LightColor;
    
    // diffuse
    vec3 normal = normalize(v_Normal);
    vec3 lightDir = normalize(u_LightPosition - v_WorldPosition);
    float diffuseStrength = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diffuseStrength * u_LightColor;

    // // specular
    // vec3 viewDir = normalize(u_ViewPosition - v_WorldPosition);
    // vec3 reflectDir = reflect(-lightDir, normal);
    // vec3 specular = u_SpecularIntensity * pow(max(dot(viewDir, reflectDir), 0.0), u_Shininess) * u_LightColor;

    vec3 result = (ambient + diffuse) * u_BaseColor;
    color = vec4(result, 1.0);
}