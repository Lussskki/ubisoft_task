#version 330 core

in vec2 TexCoords;
// We output to two attachments: FragColor[0] (full scene), FragColor[1] (bright parts)
layout(location = 0) out vec4 FragColor[2];

uniform sampler2D scene;
uniform float bloomThreshold;

void main()
{
    vec4 color = texture(scene, TexCoords);

    // Attachment 0: Full color
    FragColor[0] = color;

    // Calculate brightness (luminance)
    float brightness = dot(color.rgb, vec3(0.2126, 0.7152, 0.0722));

    // Attachment 1: Only bright parts (above threshold)
    if (brightness > bloomThreshold)
        FragColor[1] = color;
    else
        FragColor[1] = vec4(0.0, 0.0, 0.0, 1.0);
}