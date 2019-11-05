#version 150 core
uniform sampler2D diffuseTex;
uniform sampler2D diffuseTex2;

in Vertex {
	vec2 texCoord;
	vec4 colour;
} IN;

out vec4 fragColour ;

void main(void){
	
	fragColour  = texture(diffuseTex, IN.texCoord);
	fragColour  += texture(diffuseTex2, IN.texCoord);
	fragColour += IN.colour;
	fragColour = normalize(fragColour);
}