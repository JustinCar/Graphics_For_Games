#version 150 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 textureMatrix;

uniform vec3 cameraPos;

uniform vec3 centerPos;

uniform float time;
in vec3 position;
in vec2 texCoord;
in vec4 colour;

out Vertex {
	vec2 texCoord;
	vec4 colour;
} OUT;

void main(void) {
	mat4 mvp = projMatrix * viewMatrix * modelMatrix;

    //vec3 center = mvp * vec4(centerPos, 1.0);

	gl_Position = mvp * vec4(position, 1.0);
    //gl_Position /= gl_Position.w;
	OUT.texCoord = (textureMatrix * vec4(texCoord, 0.0, 1.0)).xy;
	OUT.colour = vec4(colour.rgb, 1.0);
}