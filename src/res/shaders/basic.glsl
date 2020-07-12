#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
uniform float id;
uniform float offset;
vec4 offset_vector;
// mat4 rot = {
//     { cos(offset), -sin(offset),  sin(offset), 0},
//     { sin(offset),  cos(offset), -sin(offset), 0},
//     {-sin(offset),  sin(offset),  cos(offset), 0},
//     {           0,            0,            0, 1}
// }


out vec4 vertColor;
void main()
{
    offset_vector = sin(
        vec4(
            offset * 5 + (id/10),
            offset * 5 + (id/10) + 2.0866,
            offset * 5 + (id/10) + 4.1733,
            0
        ) + id
    ) / 2;

    offset_vector.xy += position.xy * (sin(vec2(offset * 5 + id, offset * 5 + id + 3.141)) / 3 + 0.5);
    offset_vector.w = 4;
    
    gl_Position = offset_vector;

    vertColor.r = sin(offset_vector.x * 3.141 + offset + id) / 2 + 0.5;
    vertColor.g = sin(offset_vector.y * 3.141 + offset + id + 2.0866) / 2 + 0.5;
    vertColor.b = sin(offset_vector.z * 3.141 + offset + id + 4.1733) / 2 + 0.5;
    vertColor.a = 1;
};

#shader fragment
#version 330 core

in vec4 vertColor;
int color_depth = 4;
layout(location = 0) out vec4 fragColor;

void main()
{
    fragColor.r = int(vertColor.r * color_depth);
    fragColor.g = int(vertColor.g * color_depth);
    fragColor.b = int(vertColor.b * color_depth);

    fragColor /= color_depth;
};
