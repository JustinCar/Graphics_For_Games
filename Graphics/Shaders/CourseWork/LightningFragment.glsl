#version 150 core
uniform sampler2D diffuseTex;

in Vertex {
	vec2 texCoord;
	vec4 colour;
} IN;

out vec4 fragColour ;

void main(void){

    fragColour  = texture(diffuseTex, IN.texCoord);
	//fragColour += IN.colour;

    if (fragColour.r < 0.4 && fragColour.g < 0.4 && fragColour.b < 0.4) 
    {
        discard;
    }
}