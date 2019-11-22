#version 150 core
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 textureMatrix;

in vec3 position;
in vec4 colour;
in vec3 normal; // New Attribute !
in vec2 texCoord;

uniform float time;
uniform bool isFoggy;

out Vertex {
    vec4 colour;
    vec2 texCoord;
    vec3 normal;
    vec3 worldPos;
} OUT ;

void main ( void ) {
    OUT.colour = colour;
    OUT.texCoord = ( textureMatrix * vec4 ( texCoord , 0.0 , 1.0)).xy;

    mat3 normalMatrix = transpose ( inverse ( mat3 ( modelMatrix )));

    OUT.normal = normalize ( normalMatrix * normalize ( normal ));

    vec3 newPos = position;

    if (isFoggy) 
    {
        float limit = newPos.z - 7;

        newPos.z -= time / 10;

        if (newPos.z <= limit) 
        {
            newPos.z = limit;
        }
    }
    
    
    OUT.worldPos = ( modelMatrix * vec4 ( position ,1)). xyz;
    gl_Position = ( projMatrix * viewMatrix * modelMatrix ) *
        vec4 ( newPos , 1.0);
}