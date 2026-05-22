#shader vertex
#version 330 core
layout(location = 0) in vec2 positions; 
layout(location = 1) in vec2 translation;
layout(location = 2) in vec2 rotation; // (cosTheta, sinTheta)
layout(location = 3) in vec2 scale;

uniform mat4 u_OrthoProjection;


void main() {
    // Scale, Rotate and translate in that order
    vec2 scaled = positions * scale;


    vec2 rotated = vec2( scaled.x * rotation.x - scaled.y * rotation.y
                        , scaled.x * rotation.y + scaled.y * rotation.x);


    vec2 pos = rotated + translation;

    gl_Position = u_OrthoProjection * vec4(pos, 0.0, 1.0);
}




#shader fragment
#version 330 core

uniform vec4 u_Color;

out vec4 fragColor;

void main()
{
	fragColor = u_Color;
}



