#shader vertex 
#version 330 core


layout(location = 0) in vec3 positions;
layout(location = 1) in vec3 normals;

uniform mat4 u_Model;
uniform mat4 u_Perspective;
uniform mat4 u_View;


out vec3 fragNormals;
out vec3 fragPositions;


void main() 
{
    fragPositions = (u_Model * vec4(positions, 1.0)).xyz; //positions after moving them with u_Model //.xyz drops the fourth element

    //we need to transform normals to adapt to the u_model, so the normal is still pp after the transformation
	//The algebra of this is better explained in assets\shader
	fragNormals = normalize(mat3(transpose(inverse(u_Model))) * normals);

	//built-in output keyword, it outputs the final 2d location on the screen 
    gl_Position = u_Perspective * u_View * vec4(fragPositions, 1.0);
}




#shader fragment
#version 330 core



in vec3 fragNormals;
in vec3 fragPositions;

vec4 lightColor = vec4(1.0, 1.0, 1.0, 1.0);

uniform int u_fragmentMode; //1 for flat color, 0 for lit color
uniform vec3 u_CamPos;
uniform vec4 u_Color;
uniform vec3 u_lightPos;

//uniform vec2 u_CropCenter;
//uniform float u_CropRadius;
uniform float u_CropRadius;

out vec4 fragColor;

void main()
{
	//vec2 offset = fragPositions.xz - u_CropCenter;
	vec2 offset = fragPositions.xz;

	if (dot(offset, offset) > u_CropRadius * u_CropRadius)
	{
		discard;
	}

	if (u_fragmentMode == 0) 
	{
		//float ambient = 0.55f;
		float ambient = 0.75f;
		float specularLight = 0.7f;

		// diffuse lighting
		vec3 normal = fragNormals;
		vec3 lightDirection = normalize(u_lightPos - fragPositions);
		float diffuse = max(dot(normal, lightDirection), 0.0f);

		// specular lighting
		vec3 viewDirection = normalize(u_CamPos - fragPositions);
		vec3 reflectionDirection = reflect(-lightDirection, normal);
		float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 8);
		float specular = specAmount * specularLight*1.2;

		// outputs final color
		fragColor =  u_Color * lightColor * (diffuse + ambient + specular);
	}
	if (u_fragmentMode == 1) 
    {
        fragColor = u_Color;
    }

}


