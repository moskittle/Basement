// Emerald Material Shader

#type vertex
#version 330 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;
uniform mat3 u_NormalMat;

out vec3 v_Normal;
out vec3 v_FragPosition;

void main()
{
    v_FragPosition = vec3(u_Model * vec4(a_Position, 1.0));     // Fragment position in world coord
    v_Normal = u_NormalMat * a_Normal;                          // Calculate normal vector

    gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);
}

//---------------------------------------------------------------------------

#type fragment
#version 330 core
out vec4 color;

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 v_Normal;
in vec3 v_FragPosition;

uniform vec3 u_ViewPosition;

uniform vec3 u_ObjectColor;

// uniform vec3 u_LightColor;
uniform vec3 u_LightPosition;

Material emerald = Material(vec3(0.0215, 0.1745, 0.0215), vec3(0.07568, 0.61424, 0.07568), vec3(0.633, 0.727811, 0.633), 0.6 * 128);
Light light = Light(u_LightPosition, vec3(1.0), vec3(1.0), vec3(1.0));

void main()
{
    // ambient
    vec3 ambient = light.ambient * emerald.ambient;
    
    // diffuse
    vec3 normal = normalize(v_Normal);
    vec3 lightDir = normalize(light.position - v_FragPosition);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * emerald.diffuse);

    // specular
    vec3 viewDir = normalize(u_ViewPosition - v_FragPosition);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), emerald.shininess);
    vec3 specular = light.specular * (spec * emerald.specular);

    vec3 result = ambient + diffuse + specular;
    color = vec4(result, 1.0);
}