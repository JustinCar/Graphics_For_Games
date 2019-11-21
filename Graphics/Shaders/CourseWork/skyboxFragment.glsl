#version 150 core

uniform samplerCube cubeTex;
uniform samplerCube cubeTexFog;
uniform vec3 cameraPos;
uniform float time;
uniform bool blendBoxes;

in Vertex {
    vec3 normal;
} IN;

out vec4 fragColour;

void main ( void ) {

    vec4 tex;
    if (!blendBoxes) 
    {
       tex = texture(cubeTex, normalize(IN.normal));
    }
    else 
    {
       tex = mix(texture(cubeTex, normalize(IN.normal)), texture(cubeTexFog, normalize(IN.normal)), clamp(time / 5, 0.0, 1.0));
    }
    fragColour = tex;
}