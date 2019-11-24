#version 150 core

uniform sampler2D diffuseTex;
uniform samplerCube cubeTex;
uniform samplerCube cubeTexFog;
uniform float time;

uniform bool isFoggy;
uniform float fogTime;

const vec3 fogColour = vec3(0.63, 0.63, 0.63);

uniform vec4 lightColour;
uniform vec3 lightPos;
uniform vec3 cameraPos;
uniform float lightRadius;

in Vertex {
    vec4 colour;
    vec2 texCoord;
    vec3 normal;
    vec3 worldPos;
    float visibility;
} IN;

out vec4 fragColour;

void main ( void ) {
    vec4 diffuse = texture (diffuseTex, IN.texCoord ) * IN.colour;
    vec3 incident = normalize (IN.worldPos - cameraPos);
    float dist = length (lightPos - IN.worldPos);
    float atten = 1.0 - clamp (dist / lightRadius, 0.2, 1.0);

    vec4 reflection;
    if (!isFoggy) 
    {
        reflection = texture(cubeTex, reflect(incident, normalize(IN.normal)));
    } else 
    {
        reflection = mix(texture(cubeTex, reflect(incident, normalize(IN.normal))), texture(cubeTexFog, reflect(incident, normalize(IN.normal))), clamp(time / 56, 0.0, 1.0));
    }

    fragColour = (lightColour * diffuse * atten)*(diffuse + reflection);

    //fragColour = mix(vec4(fogColour, 1.0), fragColour, 0.1);
    //fragColour = vec4(fogColour, 1.0);


    if (isFoggy) 
    {
        float nTime = fogTime / 1000;
        float visibility = nTime / 10;
        visibility = 1 - visibility;

        if (visibility <= IN.visibility) 
        {
            visibility = IN.visibility;
        }

        fragColour = mix(vec4(fogColour, 1.0), fragColour, visibility);
    }

    
}