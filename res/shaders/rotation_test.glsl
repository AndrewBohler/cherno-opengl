#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 texCoord;

uniform float u_Time;
uniform uint u_ID;
uniform vec3 u_Color;
uniform mat4 u_MVP;

out vec3 v_Color;
out vec2 v_TexCoord;

vec3 axis;
vec4 rotated;
const float angle = 2.0;
const float PI = 3.141;
const float TWO_THIRDS_PI = 2.0 * PI / 3.0;
const float FOUR_THIRDS_PI = 4.0 * PI / 3.0;

/* Found this in a comment at http://www.neilmendoza.com/glsl-rotation-about-an-arbitrary-axis/ */
mat3 rotAxis(vec3 axis, float a) {
    axis = normalize(axis); // added this to fix distortion, should probably normalize before calling this
    float s = sin(a);
    float c = cos(a);
    float oc = 1.0 - c;
    vec3 as = axis * s;
    mat3 p = mat3(
        axis.x * axis,
        axis.y * axis,
        axis.z * axis
    );

    mat3 q = mat3(
            c, -as.z,  as.y,
         as.z,     c, -as.x,
        -as.y,  as.x,     c
    );
    return p * oc + q;
}

void main(){
    float x = sin(u_Time +              0 + u_ID);
    float y = sin(u_Time +  TWO_THIRDS_PI + u_ID);
    float z = sin(u_Time + FOUR_THIRDS_PI + u_ID);
    axis = vec3(x, y, z) * 4.0;
    rotated = vec4(rotAxis(axis, u_Time * angle) * position, 4.0);

    v_Color = u_Color;
    v_TexCoord = texCoord;
    // v_TexCoord = rotated.xy / 2 + 0.5;

    // float x_offset = u_ID / 10;
    // float y_offset = u_ID % 10;
    // rotated.xy += vec2(
    //     (x_offset < 5.0 ? (x_offset - 5.0) * 3.0 : (x_offset - 4.0) * 3.0),
    //     (y_offset < 5.0 ? (y_offset - 5.0) * 3.0 : (y_offset - 4.0) * 3.0)
    // );

    gl_Position = u_MVP * rotated;
};



#shader fragment
#version 330 core

in vec3 v_Color;
in vec2 v_TexCoord;

uniform sampler2D u_Texture;

layout(location = 0) out vec4 fragment_color;

void main(){
    vec4 texColor = texture(u_Texture, v_TexCoord);

    // fragment_color = vec4(v_Color, 1.0);
    fragment_color = vec4(texColor.rgb * v_Color, 1.0);
};
