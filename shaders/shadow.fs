#version 330 core
in  vec4  FragPos;
uniform vec3  lightPos;
uniform float farPlane;

void main() {
    // Store linear depth [0,1] so the cubemap holds true distances.
    gl_FragDepth = length(FragPos.xyz - lightPos) / farPlane;
}
