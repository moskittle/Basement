// NanoSuit Shader

#type vertex
#version 330 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoord;

uniform mat4 u_Model;
uniform mat3 u_NormalMat;

layout (std140) uniform CameraMat
{
    mat4 u_View;
    mat4 u_Projection;
};

out vec3 v_FragPosition;
out vec3 v_Normal;
out vec2 v_TexCoord;

void main()
{
    v_FragPosition = vec3(u_Model * vec4(a_Position, 1.0));     // Fragment position in world coord
    v_Normal = u_NormalMat * a_Normal;                          // Calculate normal vector
    v_TexCoord = a_TexCoord;
    gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);
}

//---------------------------------------------------------------------------

#type fragment
#version 330 core
out vec4 FragColor;

in vec3 v_FragPosition;
in vec3 v_Normal;
in vec2 v_TexCoord;

struct Material
{
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;

    vec3 ambient_color;
    vec3 diffuse_color;
    vec3 specular_color;
    float shininess;
};
uniform Material u_Material;

struct Light
{
    vec3 position;

    vec3 ambient_power;
    vec3 diffuse_power;
    vec3 specular_power;
};
uniform Light u_Light;

uniform vec3 u_ViewPosition;

void main()
{
    // ambient
    vec3 ambient = u_Light.ambient_power * texture(u_Material.texture_diffuse1, v_TexCoord).rgb;

    // diffuse
    vec3 normal = normalize(v_Normal);
    vec3 lightDir = normalize(u_Light.position - v_FragPosition);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = u_Light.diffuse_power * diff * texture(u_Material.texture_diffuse1, v_TexCoord).rgb;

    // specualr
    vec3 viewDir = normalize(u_ViewPosition - v_FragPosition);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
    vec3 specular = u_Light.specular_power * spec * texture(u_Material.texture_specular1, v_TexCoord).rgb;

    FragColor = vec4(ambient + diffuse + specular, 1.0);
}