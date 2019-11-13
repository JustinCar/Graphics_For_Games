#version 150 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 textureMatrix;

uniform float time;
uniform sampler2D heightMap;

in vec3 position;
in vec4 colour;
in vec3 normal; // New Attribute !
in vec2 texCoord;

out Vertex {
    vec4 colour;
    vec2 texCoord;
    vec3 normal;
    vec3 worldPos;
} OUT;

float grow() 
{
    float height = texture(heightMap, texCoord / 32).r;

    float limit = (height * 8129);
    float pos = position.y;

    if (pos < limit) 
    {
        pos += time / 10;

        if (pos > limit) 
        {
            pos = limit;
        }
    }

    return pos;
}

void main ( void ) {
    OUT.colour = colour;
    OUT.texCoord = (textureMatrix * vec4(texCoord, 0.0, 1.0)).xy;

    mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));

    OUT.normal = normalize (normalMatrix * normalize(normal));

    vec3 newPosition = position;
    newPosition.y = grow();

    OUT.worldPos = (modelMatrix * vec4(position, 1)).xyz;
    gl_Position = (projMatrix * viewMatrix * modelMatrix) *
        vec4(newPosition, 1.0);
}