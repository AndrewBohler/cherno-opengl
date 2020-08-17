#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 texCoord;

uniform float u_Time;

out vec4 v_Color;
out vec2 v_TexCoord;

void main()
{
    float offset = 0;
    offset += position.x > 0 ? 1 : 3;
    offset += position.y > 0 ? 1 : 3;
    offset += position.z > 0 ? 1 : 3;

    vec3 position_offset = vec3(
        sin(u_Time + offset),
        sin(u_Time + offset + 2.0),
        sin(u_Time + offset + 4.0)
    );

    v_Color = vec4(position, 1.0);
    v_TexCoord = texCoord;
    
    gl_Position = vec4(position + sin(u_Time), 4.0);
};

#shader fragment
#version 330 core


in vec4 v_Color;
in vec2 v_TexCoord;
in float v_VertexID;

layout(location = 0) out vec4 fragColor;

void main()
{
    vec2 dummy = v_TexCoord;
    fragColor = v_Color;
};
