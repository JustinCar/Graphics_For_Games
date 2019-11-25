#version 150 core
uniform sampler2D diffuseTex;
uniform sampler2D diffuseTex2;

uniform bool isFoggy;

uniform float fogTime;

const vec3 fogColour = vec3(0.63, 0.63, 0.63);

in Vertex {
	vec2 	texCoord;
	float visibility;
} IN;

out vec4 fragColour ;

void main(void){
	
	fragColour  = texture(diffuseTex, IN.texCoord);
	fragColour  += texture(diffuseTex2, IN.texCoord);
	//fragColour += IN.colour;
	//fragColour = normalize(fragColour);
	
    fragColour = mix(vec4(fogColour, 1.0), fragColour, IN.visibility);

}