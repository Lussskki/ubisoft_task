#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D scene;
uniform sampler2D bloomBlur;
uniform bool bloomEnabled;
uniform float exposure;

void main()
{
    vec3 sceneColor = texture(scene, TexCoords).rgb;
    vec3 bloomColor = texture(bloomBlur, TexCoords).rgb;

    vec3 finalColor = sceneColor;
    if (bloomEnabled)
    {
        // Add the blurred bright parts to the scene
        finalColor += bloomColor;
    }

    // Tone mapping and Exposure control for a better look
    finalColor = vec3(1.0) - exp(-finalColor * exposure);

    // No Gamma correction applied here for simplicity, but it should be done!
    FragColor = vec4(finalColor, 1.0);
}