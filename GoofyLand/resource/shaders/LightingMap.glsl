// Lighting Map Shader

#type vertex
#version 330 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoord;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;
uniform mat3 u_NormalMat;

out vec3 v_Normal;
out vec3 v_FragPosition;
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
out vec4 color;

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
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
in vec2 v_TexCoord;

uniform vec3 u_ViewPosition;
uniform vec3 u_LightPosition;
uniform Material material;
uniform Light light;
uniform float u_Time;

void main()
{
    // ambient
    vec3 ambient = light.ambient * texture(material.diffuse, v_TexCoord).rgb;
    
    // diffuse
    vec3 normal = normalize(v_Normal);
    vec3 lightDir = normalize(light.position - v_FragPosition);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, v_TexCoord).rgb;

    // specular
    vec3 viewDir = normalize(u_ViewPosition - v_FragPosition);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, v_TexCoord).rgb;

    // emission
    // vec3 emission = texture(material.emission, v_TexCoord).rgb;
    vec3 emission = vec3(0.0);
    if (texture(material.specular, v_TexCoord).rgb == vec3(0.0))   /*rough check for blackbox inside spec texture */
    {
        /*apply emission texture */
        emission = texture(material.emission, v_TexCoord).rgb;
        
        /*some extra fun stuff with "time uniform" */
        emission = texture(material.emission, v_TexCoord + vec2(0.0, u_Time)).rgb;   /*moving */
        //emission = emission * (sin(u_Time) * 0.5 + 0.5) * 2.0;                     /*fading */
    }

    color = vec4(ambient + diffuse + specular + emission, 1.0);
}