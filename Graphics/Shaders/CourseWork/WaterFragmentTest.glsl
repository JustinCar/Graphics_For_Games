#version 150 core

uniform sampler2D diffuseTex;
uniform samplerCube cubeTex;
uniform samplerCube cubeTexFog;

uniform float time;
uniform bool isFoggy;

uniform vec4 lightColour;
uniform vec3 lightPos;
uniform vec3 cameraPos;
uniform float lightRadius;

uniform sampler2DShadow shadowTex;
uniform sampler2D bumpTex;

uniform float fogTime;
const vec3 fogColour = vec3(0.63, 0.63, 0.63);

in Vertex {
    vec4 colour;
    vec2 texCoord;
    vec3 normal;
    vec3 tangent;
    vec3 binormal;
    vec3 worldPos;
    vec4 shadowProj; 
    float visibility;
} IN;

out vec4 fragColour;

void main ( void ) {
    vec4 diffuse = texture (diffuseTex, IN.texCoord ) * IN.colour;
    //diffuse += IN.colour;

    mat3 TBN = mat3 ( IN.tangent, IN.binormal, IN.normal );
    vec3 normal = normalize ( TBN *
        ( texture2D ( bumpTex, IN.texCoord ).rgb * 2.0 - 1.0));

    vec3 incident = normalize ( lightPos - IN.worldPos );
    float lambert = max (0.0 , dot ( incident , IN.normal ));

    float dist = length (lightPos - IN.worldPos);
    float atten = 1.0 - clamp (dist / lightRadius, 0.2, 1.0);

    vec3 viewDir = normalize ( cameraPos - IN.worldPos );
    vec3 halfDir = normalize ( incident + viewDir );

    float rFactor = max (0.0 , dot ( halfDir , IN.normal ));
    float sFactor = pow ( rFactor , 50.0 );
    
    float shadow = 1.0;

    if  (IN.shadowProj.w > 0.0) {
        shadow = textureProj ( shadowTex, IN.shadowProj );
    }

    lambert *= shadow; 

    vec4 reflection;
    if (!isFoggy) 
    {
        reflection = texture(cubeTex, reflect(incident, normalize(IN.normal)));
    } else 
    {
        reflection = mix(texture(cubeTex, reflect(incident, normalize(IN.normal))), texture(cubeTexFog, reflect(incident, normalize(IN.normal))), clamp(time / 56, 0.0, 1.0));
    }

    // vec3 colour = ( diffuse.rgb * lightColour.rgb );
    // colour += ( lightColour.rgb * sFactor ) * 0.33;
    // fragColour = vec4 ( colour * atten * lambert, diffuse.a );
    // fragColour.rgb += ( diffuse.rgb * lightColour.rgb ) * 0.3;

    // if (isFoggy) 
    // {
    //     float nTime = fogTime / 1000;
    //     float visibility = nTime / 10;
    //     visibility = 1 - visibility;

    //     if (visibility <= IN.visibility) 
    //     {
    //         visibility = IN.visibility;
    //     }

    //     fragColour = mix(vec4(fogColour, 1.0), fragColour, 1);
    // }

    

    fragColour = (lightColour * diffuse * atten)*(diffuse + reflection);

    //fragColour = mix(vec4(fogColour, 1.0), fragColour, 1.0);

}
