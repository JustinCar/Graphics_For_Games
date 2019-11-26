#version 150 core
uniform sampler2D diffuseTex;
uniform sampler2D diffuseTex2;
uniform int texCount;

in Vertex {
	vec2 texCoord;
	vec4 colour;
} IN;

out vec4 fragColour ;

void main(void){

    if (texCount == 0) 
    {
        fragColour  = texture(diffuseTex, IN.texCoord);
        if (fragColour.b < 0.88) 
        {
            discard;
        }
    } else 
    {
        fragColour  = texture(diffuseTex2, IN.texCoord);
        if (fragColour.g < 0.88) 
        {
            discard;
        } 
    }
}