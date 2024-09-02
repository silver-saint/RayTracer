#version 450

//layout(location = 0) in vec3 fragColor;
layout(location = 0) out vec4 outColor;
layout(location = 1) out vec2 fragTexCoord;
layout(binding = 1) uniform sampler2D texSampler;

#ifdef VULKAN
layout(push_constant) uniform Push
{
    vec2 offset;
    vec3 color;
} push;
#endif

void main() {
    outColor = texture(texSampler, fragTexCoord); 
    //vec4(push.color, 1.0);
}