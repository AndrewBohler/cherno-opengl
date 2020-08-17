#shader vertex
#version 330 core

layout(location = 0) in vec2 posCoord;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

void main(){
    gl_Position = vec4(posCoord, 0.0, 10.0);
    v_TexCoord = texCoord;
}

#shader fragment
#version 330 core

in vec2 v_TexCoord;

uniform sampler2D u_Texture;

layout(location = 0) out vec4 fragColor;

void main(){
    vec4 texColor = texture(u_Texture, v_TexCoord);
    texColor.a = 0.0;
    fragColor = texColor;
}
