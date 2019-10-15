// NanoSuit Metal Shader
// Environment mapping

#type vertex
#version 330 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;

out vec3 v_FragPosition;
out vec3 v_Normal;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;
uniform mat3 u_NormalMat;

void main()
{
    v_FragPosition = vec3(u_Model * vec4(a_Position, 1.0));     // Fragment position in world coord
    v_Normal = u_NormalMat * a_Normal;                          // Calculate normal vector
    gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);
}

//---------------------------------------------------------------------------

#type fragment
#version 330 core
out vec4 FragColor;

in vec3 v_FragPosition;
in vec3 v_Normal;

uniform vec3 u_ViewPosition;
uniform samplerCube u_Skybox;

void main()
{
    vec3 I = normalize(v_FragPosition - u_ViewPosition);
    vec3 R = reflect(I, normalize(v_Normal));

    FragColor = vec4(texture(u_Skybox, R).rgb, 1.0);
}