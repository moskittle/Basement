// Lighting Shader

#type vertex
#version 330 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

out vec3 v_Normal;
out vec3 v_Position;

void main()
{
    gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);
    v_Normal = a_Normal;
}

//---------------------------------------------------------------------------

#type fragment
#version 330 core
out vec4 color;

in vec3 v_Normal;
in vec3 v_Position;

uniform float u_AmbientStrength;
uniform vec3 u_ObjectColor;
uniform vec3 u_LightColor;
uniform vec3 u_LightPosition;

void main()
{
    // ambient
    vec3 ambient = u_AmbientStrength * u_LightColor;
    
    // diffuse
    vec3 normal = normalize(v_Normal);
    vec3 lightDir = normalize(u_LightPosition - v_Position);
    float diffuseStrength = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diffuseStrength * u_LightColor;

    vec3 result = (ambient + diffuse) * u_ObjectColor;
    color = vec4(result, 1.0);
}