#version 330 core
in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D rainbowTexture;
uniform vec3 lightColor;

void main()
{
    vec4 texColor = texture(rainbowTexture, TexCoords);
    if (texColor.a < 0.1)
        discard;
    FragColor = texColor * vec4(lightColor, 1.0f);
}