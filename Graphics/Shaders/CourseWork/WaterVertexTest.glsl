#version 150 core
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 textureMatrix;
uniform mat4 shadowMatrix;

in vec3 position;
in vec4 colour;
in vec3 normal; // New Attribute !
in vec2 texCoord;
in vec3 tangent;

uniform float time;
uniform bool isFoggy;

const float density = 0.003;
const float gradient = 1.5;

out Vertex {
    vec4 colour;
    vec2 texCoord;
    vec3 normal;
    vec3 tangent;
    vec3 binormal;
    vec3 worldPos;
    vec4 shadowProj; 
    float visibility;
} OUT ;

void main ( void ) {
    OUT.colour = colour;
    OUT.texCoord = ( textureMatrix * vec4 ( texCoord , 0.0 , 1.0)).xy;

    mat3 normalMatrix = transpose ( inverse ( mat3 ( modelMatrix )));

    OUT.normal = normalize ( normalMatrix * normalize ( normal ));
    OUT.tangent = normalize ( normalMatrix * normalize ( tangent ));
    OUT.binormal = normalize ( normalMatrix *
       normalize ( cross ( normal , tangent )));

    vec3 newPos = position;

    newPos.y = (sin(newPos.x) + cos(newPos.z));

    if (isFoggy) 
    {
        float limit = newPos.y + 7;

        newPos.y += time / 10;

        if (newPos.y <= limit) 
        {
            newPos.y = limit;
        }
    }

    OUT.worldPos = ( modelMatrix * vec4 ( newPos, 1)).xyz;

    //fog --------------------------------------------------
    vec4 toCam = viewMatrix * vec4(OUT.worldPos, 1.0);
    float dis = length(toCam.xyz);

    OUT.visibility = exp(-pow((dis * density), gradient));

    OUT.visibility = clamp(OUT.visibility, 0.0, 1.0);
    //--------------------------------------------------

    OUT.shadowProj = ( shadowMatrix * vec4 ( newPos +( normal *1.5) ,1));

    gl_Position = ( projMatrix * viewMatrix * modelMatrix ) *
        vec4 ( newPos , 1.0);
}