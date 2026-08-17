#shader vertex
#version 330 core

layout(location = 0) in vec3 positions;
layout(location = 1) in vec2 textureCoords;

uniform mat4 u_Model;
uniform mat4 u_Perspective;
uniform mat4 u_View;

out vec2 v_TextureCoords;

void main()
{
	gl_Position = u_Perspective * u_View * u_Model * vec4(positions, 1.0);
	v_TextureCoords = textureCoords;
}


#shader fragment
#version 330 core

uniform vec3 u_Color;
uniform sampler2D u_Texture;

in vec2 v_TextureCoords;

out vec4 FragColor;

void main()
{
	float alpha = texture(u_Texture, v_TextureCoords).r;
	FragColor = vec4(u_Color, alpha);
}

