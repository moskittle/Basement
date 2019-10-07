// Material Shader

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
    int shininess;
};

struct Light
{
    vec3 lightColor;
    vec3 lightPosition;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;

in vec3 v_Normal;
in vec3 v_FragPosition;

uniform vec3 u_ViewPosition;

uniform vec3 u_ObjectColor;

uniform vec3 u_LightColor;
uniform vec3 u_LightPosition;

void main()
{
    // ambient
    vec3 ambient = material.ambient * u_LightColor;
    
    // diffuse
    vec3 normal = normalize(v_Normal);
    vec3 lightDir = normalize(u_LightPosition - v_FragPosition);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = u_LightColor * (diff * material.diffuse);

    // specular
    vec3 viewDir = normalize(u_ViewPosition - v_FragPosition);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = u_LightColor * (spec * material.specular);

    // intensity
    ambient *= light.ambient;
    diffuse *= light.diffuse;
    specular *= light.specular;

    vec3 result = ambient + diffuse + specular;
    color = vec4(result, 1.0);
}