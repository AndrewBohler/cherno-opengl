#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
vec3 axis;
vec4 rotated;
const float angle = 2.0;
uniform float u_Time;
uniform uint u_ID;
uniform vec3 u_Color;
out vec3 vertex_color;

/* Found this in a comment at http://www.neilmendoza.com/glsl-rotation-about-an-arbitrary-axis/ */
mat3 rotAxis(vec3 axis, float a) {
    axis = normalize(axis);
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
    float x = sin(u_Time * 2.101 + u_ID);
    float y = sin(u_Time * 2.202 + u_ID);
    float z = sin(u_Time * 2.303 + u_ID);
    axis = vec3(x, y, z) * 4;
    // vec4 trans = vec4(x, y, z, 0) * 3;
    rotated = vec4(rotAxis(axis, u_Time * angle) * position, 4.0);
    // rotated += trans;
    // rotated = vec4(position, 10);
    // rotated = rotationMatrix(axis, offset * angle) * rotated;
    // vertex_color = color / 2 + 0.5;
    
    // vertex_color = position;

    // vec3 base_color = vec3(position / 2 + 0.5);
    // vertex_color = base_color + -(rotated.z * abs(rotated.z) / 2);

    vertex_color = u_Color;

    gl_Position = rotated;
};



#shader fragment
#version 330 core

in vec3 vertex_color;
layout(location = 0) out vec4 fragment_color;

void main(){
    fragment_color = vec4(vertex_color / 2.0 + 0.5, 1.0);
    // fragment_color = vec4(vertex_color, 1);
};
