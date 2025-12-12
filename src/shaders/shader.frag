//
// Created by Luka on 09.12.2025.
//

// Created by Luka on 09.12.2025.
//

#version 330 core

in vec3 vColor;
out vec4 FragColor[2]; // Outputting to two attachments (scene color and bright part is done in main.cpp, but let's assume one output for now and ensure it's bright)

// The main pass now needs two output attachments defined in main.cpp:
// layout(location = 0) is the scene color, layout(location = 1) is the bright color
layout(location = 0) out vec4 FragColor0;
layout(location = 1) out vec4 FragColor1;

uniform float intensity; // Uniform to make the object extra bright (e.g., 5.0)

void main()
{
    // The cube's color, boosted by intensity
    vec4 finalColor = vec4(vColor * intensity, 1.0);

    // Attachment 0: Full color (potentially HDR)
    FragColor0 = finalColor;

    // Attachment 1: Bright parts only (simple threshold: everything above 1.0 is considered bright)
    float brightness = dot(finalColor.rgb, vec3(0.2126, 0.7152, 0.0722)); // Luminance
    if (brightness > 1.0)
        FragColor1 = finalColor; // Send the bright color
    else
        FragColor1 = vec4(0.0, 0.0, 0.0, 1.0); // Send black
}
