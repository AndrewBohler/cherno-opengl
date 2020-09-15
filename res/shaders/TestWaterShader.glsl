#shader vertex
#version 330 core

layout(location = 0) in vec2 position;

uniform bool  u_Animate;
uniform float u_Magnitude;
uniform float u_Scale;
uniform float u_Time;

out vec2 v_TexCoord;
out float z_pos;

void main(){
    v_TexCoord = position * 0.5 + 0.5;
    
    vec2 shifted = vec2(
        sin(gl_VertexID + 0.000 + u_Time),
        sin(gl_VertexID + 2.094 + u_Time)
    ) * u_Magnitude;

    vec2 pos = position.xy;
    if (u_Animate) pos += shifted;

    z_pos = sin(pos.x * pos.y * u_Time);

    gl_Position = vec4(pos, z_pos, 1 / u_Scale);
}

#shader fragment
#version 330 core

in vec2 v_TexCoord;
in float z_pos;

uniform vec4 u_SheenColor;
uniform vec4 u_ShadowColor;
uniform float u_SheenHeight;
uniform float u_ShadowHeight;

uniform sampler2D u_Texture;

layout(location = 0) out vec4 fragment_color;

void main(){
    fragment_color = texture(u_Texture, v_TexCoord);

    if (z_pos > u_SheenHeight){
        fragment_color.rgb += (u_SheenColor.rgb * u_SheenColor.a);
    } else if (z_pos < u_ShadowHeight){
        fragment_color.rgb *= (u_ShadowColor.rgb * u_ShadowColor.a);
    }
};
