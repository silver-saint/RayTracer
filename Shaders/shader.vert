#version 450

layout(location = 0) in vec2 inPos;
layout(location = 1) in vec3 inColor;

//layout(location = 0) out vec3 fragColor;
#ifdef VULKAN
layout(push_constant) uniform Push
{
    vec2 offset;
    vec3 color;
} push;
#endif
void main() {
    gl_Position = vec4(inPos + push.offset, 0.0, 1.0);
    //   fragColor = inColor;
}