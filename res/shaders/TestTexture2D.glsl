#shader vertex
#version 330 core

layout(location = 0) in vec2 position;

uniform float u_Scale;

out vec2 v_TexCoord;

void main(){
    v_TexCoord = position * 0.5 + 0.5;
    gl_Position = vec4(position, 0.0, 1 / u_Scale);
    // gl_Position = vec4(position.xy * 10, 0.0, 1.0);
};

#shader fragment
#version 330 core

in vec2 v_TexCoord;

uniform sampler2D u_Texture;

layout(location = 0) out vec4 fragment_color;

void main(){
    fragment_color = texture(u_Texture, v_TexCoord);
    // fragment_color = vec4(1.0, 1.0, 1.0, 1.0);
};
