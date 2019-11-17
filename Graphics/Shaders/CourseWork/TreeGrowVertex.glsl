#version 150 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 textureMatrix;

uniform float time;
uniform sampler2D grassMap;
uniform vec3 treePos;

in vec3 position;
in vec4 colour;
in vec3 normal;
in vec3 tangent;
in vec2 texCoord;

out Vertex {
	vec4 colour;
    vec2 texCoord;
    vec3 normal;
    vec3 tangent;
    vec3 binormal;
    vec3 worldPos;
    vec4 shadowProj; 
} OUT;

void main(void) {
	mat4 mvp = projMatrix * viewMatrix * modelMatrix;
    
    mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));

    //position.x = treePos;

    OUT.tangent = normalize ( normalMatrix * normalize ( tangent ));
    OUT.binormal = normalize ( normalMatrix *
        normalize ( cross ( normal , tangent )));

    OUT.normal = normalize (normalMatrix * normalize(normal));
	gl_Position = mvp * vec4(position, 1.0);
	OUT.texCoord = texCoord;
    OUT.texCoord.y = 1 - texCoord.y;
	OUT.colour = colour;
    OUT.worldPos = (modelMatrix * vec4(position, 1)).xyz;

    OUT.shadowProj = ( textureMatrix * vec4 ( position + ( normal * 1.5), 1));
}