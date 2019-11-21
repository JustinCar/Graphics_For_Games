#version 150 core

uniform sampler2D stoneTex;
uniform sampler2D grassTex;
uniform sampler2D snowTex;
uniform sampler2D waterTex;
uniform sampler2D lavaTex;

uniform sampler2D grassMap;
uniform sampler2D stoneMap;
uniform sampler2D snowMap;

//uniform sampler2D bumpTex;
uniform sampler2D grassBump;
uniform sampler2D stoneBump;
uniform sampler2D snowBump;
uniform sampler2DShadow shadowTex;

uniform vec3 cameraPos;
uniform vec4 lightColour;  
uniform vec3 lightPos;
uniform float lightRadius;

uniform bool isFoggy;

const vec3 fogColour = vec3(0.63, 0.63, 0.63);

in Vertex {
    vec3 colour;
    vec2 texCoord;
    vec3 normal;
    vec3 tangent;
    vec3 binormal;
    vec3 worldPos;
    vec4 shadowProj;
    float visibility;
} IN;

out vec4 fragColour;

vec3 getNormal() 
{
    mat3 TBN = mat3 ( IN.tangent , IN.binormal , IN.normal );

    if (texture(snowMap, IN.texCoord / 32).r >= 0.4) 
    {
        return normalize ( TBN *
        ( texture2D ( snowBump , IN.texCoord ).rgb * 2.0 - 1.0));
    }

    if (texture(grassMap, IN.texCoord / 32).r >= 0.5) 
    {
        return normalize ( TBN *
        ( texture2D ( grassBump , IN.texCoord ).rgb * 2.0 - 1.0));
    } 
   
    if (texture(stoneMap, IN.texCoord / 32).r >= 0.000000000000000000000000001) 
    {
        return normalize ( TBN *
        ( texture2D ( stoneBump , IN.texCoord ).rgb * 2.0 - 1.0));
    } 

        return normalize ( TBN *
        ( texture2D ( stoneBump , IN.texCoord ).rgb * 2.0 - 1.0));
}

vec4 getTexture() 
{
    if (texture(snowMap, IN.texCoord / 32).r >= 0.4) 
    {
        return texture(snowTex, IN.texCoord);
    }

    if (texture(grassMap, IN.texCoord / 32).r >= 0.5) 
    {
        return texture(grassTex, IN.texCoord);
    } 
   
    if (texture(stoneMap, IN.texCoord / 32).r >= 0.000000000000000000000000001) 
    {
        return texture(stoneTex, IN.texCoord);
    } 

        return texture(stoneTex, IN.texCoord);
}

void main ( void ) {
    //vec4 texture = texture ( stoneTex, IN.texCoord );
    vec4 texture = getTexture();

    vec3 normal = getNormal();

    vec3 incident = normalize ( lightPos - IN.worldPos );
    float lambert = max (0.0 , dot ( incident , normal ));

    float dist = length ( lightPos - IN.worldPos );
    float atten = 1.0 - clamp ( dist / lightRadius , 0.0 , 1.0);

    vec3 viewDir = normalize ( cameraPos - IN.worldPos );
    vec3 halfDir = normalize ( incident + viewDir );

    float rFactor = max (0.0 , dot ( halfDir , normal ));
    float sFactor = pow ( rFactor , 50.0 );
    
    float shadow = 1.0;

    if  (IN.shadowProj.w > 0.0) {
        shadow = textureProj ( shadowTex, IN.shadowProj );
    }

    lambert *= shadow; 

    vec3 colour = ( texture.rgb * lightColour.rgb );
    colour += ( lightColour.rgb * sFactor ) * 0.33;
    fragColour = vec4 ( colour * atten * lambert , texture.a );
    fragColour.rgb += ( texture.rgb * lightColour.rgb ) * 0.3;

    if (isFoggy) 
    {
        fragColour = mix(vec4(fogColour, 1.0), fragColour, IN.visibility);
    }
    
}
