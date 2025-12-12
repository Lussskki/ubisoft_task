💻  Real-Time Bloom Post-Processing in OpenGL
UBISTOFT_TASK (Custom OpenGL Renderer)
This project is a sophisticated rendering solution built from the ground up in C++ and OpenGL 3.3+ Core Profile, demonstrating mastery over modern graphics techniques, specifically real-time High Dynamic Range (HDR) Bloom.

The primary goal was to implement a visually compelling and performant post-processing effect that simulates light scattering/glow, a critical feature in contemporary game engines.

Key Technical Achievements
Bloom Post-Processing Pipeline: Designed and implemented the full three-stage rendering pipeline required for the bloom effect:
HDR Scene Render & Brightness 
Extraction: The scene is rendered to a Multi-Target Framebuffer Object (FBO) . 
This FBO stores the full HDR scene data in one color attachment and a 'bright mask' (fragments with intensity >1.0) in a second attachment.

Two-Pass Separable Gaussian Blur: Utilized the performance advantage of a separable kernel to apply a Gaussian blur to the bright mask. 
A "ping-pong" FBO system was implemented to efficiently switch between horizontal and vertical blur passes, ensuring high visual quality with minimal rendering overhead.

Final Composition: A final shader combines the original HDR scene texture and the blurred glow texture. 
This shader handles exposure control and tone mapping to correctly compress the high-range lighting data for display on a standard monitor.

Core OpenGL Engine: Includes robust architecture for fundamental rendering operations:

Custom Shader compilation and linking utility.

Perspective Camera management with keyboard (W, A, S, D) and mouse look controls.

Efficient rendering of basic geometry (Cubes) with custom shaders.

Input and State Management: Implemented an InputHandler class allowing for dynamic, frame-rate independent camera movement and a runtime toggle (B key) to switch the Bloom effect ON/OFF for immediate visual comparison.