#version 150 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 textureMatrix;

uniform float time;

in vec3 position;
in vec2 texCoord;
in vec4 colour;

out Vertex {
	vec2 texCoord;
	vec4 colour;
} OUT;

void main(void) {
	//mat4 mvp = projMatrix * viewMatrix * modelMatrix;

    vec3 pos = position;
	float start = pos.y;

    pos.y -= time / 5;

	while(pos.y < 0) 
	{
		pos.y += start;
	}

	gl_Position = vec4(pos, 1.0);
	OUT.texCoord = (textureMatrix * vec4(texCoord, 0.0, 1.0)).xy;
	OUT.colour = colour;
}