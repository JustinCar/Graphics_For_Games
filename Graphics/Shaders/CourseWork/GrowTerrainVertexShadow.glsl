#version 150 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 textureMatrix;
uniform mat4 shadowMatrix;

uniform float time;
uniform sampler2D heightMap;

in vec3 position;
in vec4 colour;
in vec3 normal;
in vec3 tangent;
in vec2 texCoord;

uniform vec3 lightningPos;
uniform bool lightningPLaying;

out Vertex {
    vec4 colour;
    vec2 texCoord;
    vec3 normal;
    vec3 tangent;
    vec3 binormal;
    vec3 worldPos;
    vec4 shadowProj; 
    float visibility;
} OUT;

const float density = 0.003;
const float gradient = 1.5;

float grow() 
{
    float height = texture(heightMap, texCoord / 32).r;

    float limit = (height * 260);
    float pos = position.y;

    if (pos < limit) 
    {
        pos += time / 80;

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
    OUT.tangent = normalize ( normalMatrix * normalize ( tangent ));
    OUT.binormal = normalize ( normalMatrix *
       normalize ( cross ( normal , tangent )));
        
    vec3 newPosition = position;
    newPosition.y = grow();

    OUT.worldPos = (modelMatrix * vec4(newPosition, 1)).xyz;

    //fog --------------------------------------------------
    vec4 toCam = viewMatrix * vec4(OUT.worldPos, 1.0);
    float dis = length(toCam.xyz);

    OUT.visibility = exp(-pow((dis * density), gradient));

     if (lightningPLaying) 
    {
        vec3 toLightning = OUT.worldPos - lightningPos;
        float lightningDis = length(toLightning);
        OUT.visibility += exp(-pow((lightningDis * 0.0025), gradient));
    }
    OUT.visibility = clamp(OUT.visibility, 0.0, 1.0);
    //--------------------------------------------------

    OUT.shadowProj = ( shadowMatrix * vec4 ( newPosition +( normal *1.5) ,1));
    
    gl_Position = (projMatrix * viewMatrix * modelMatrix) *
        vec4(newPosition, 1.0);
}