#version 330 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

uniform samplerBuffer weightTex;
uniform samplerBuffer transformTex;

in  vec3 position;
in 	vec3 normal;
in 	vec3 tangent;
in  vec2 texCoord;

layout(location = 10) in vec2 weighting;

out Vertex	{
	vec2 	texCoord;
	float visibility;
} OUT;

const float density = 0.003;
const float gradient = 1.5;

void main(void)	{
	int weightElements 	= int(weighting.x);	//These work!
	int weightIndex 	= int(weighting.y);	//These work!
	
	vec4 vertPos 	= vec4(0,0,0,1);

	for(int i = 0; i < weightElements; ++i) {	
		vec3 firstHalf  = texelFetch(weightTex,((weightIndex+i)*2)).rgb;
		vec3 weightPos  = texelFetch(weightTex,((weightIndex+i)*2)+1).rgb;
		
		mat4 jointTransform;
		jointTransform[0] = texelFetch(transformTex,(int(firstHalf.y)*8)+0);	//firstHalf.y = jointIndex
		jointTransform[1] = texelFetch(transformTex,(int(firstHalf.y)*8)+1);	//firstHalf.y = jointIndex
		jointTransform[2] = texelFetch(transformTex,(int(firstHalf.y)*8)+2);	//firstHalf.y = jointIndex
		jointTransform[3] = texelFetch(transformTex,(int(firstHalf.y)*8)+3);	//firstHalf.y = jointIndex
			
		vertPos += (jointTransform * vec4(weightPos,1.0)) * firstHalf.z; 		// firstHalf.z = weightValue;	
	}
	mat3 normalMatrix = transpose(mat3(modelMatrix));
	
	vertPos.w = 1.0f;
	
	OUT.texCoord 	= texCoord;

	vec3 worldPos = (modelMatrix * vec4(position, 1)).xyz;

	//fog --------------------------------------------------
    vec4 toCam = viewMatrix * vec4(worldPos, 1.0);
    float dis = length(toCam.xyz);
    OUT.visibility = exp(-pow((dis * density), gradient));
    OUT.visibility = clamp(OUT.visibility, 0.0, 1.0);
    //--------------------------------------------------
	
	gl_Position		= (projMatrix * viewMatrix * modelMatrix) * vertPos;
}