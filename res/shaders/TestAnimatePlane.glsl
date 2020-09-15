#shader vertex
#version 330 core

layout(location = 0) in vec2 position;

uniform bool  u_Animate;
uniform float u_Magnitude;
uniform float u_Scale;
uniform float u_Time;

out vec2 v_TexCoord;

void main(){
    v_TexCoord = position * 0.5 + 0.5;
    
    vec2 shifted = vec2(
        sin(gl_VertexID + 0.000 + u_Time),
        sin(gl_VertexID + 2.094 + u_Time)
    ) * u_Magnitude;

    vec2 pos = position.xy;

    if (u_Animate) pos += shifted;

    gl_Position = vec4(pos, 0.0, 1 / u_Scale);
}

#shader fragment
#version 330 core

in vec2 v_TexCoord;

uniform sampler2D u_Texture;

layout(location = 0) out vec4 fragment_color;

void main(){
    fragment_color = texture(u_Texture, v_TexCoord);
};
