#version 330 core

in vec3 fragColor;

out vec3 color;

void main()
{
    vec3 frag = fragColor;

    if (frag.x < 0.0f) frag.x *= -1.0f;
    if (frag.y < 0.0f) frag.y *= -1.0f;
    if (frag.z < 0.0f) frag.z *= -1.0f;

    color = frag;
}