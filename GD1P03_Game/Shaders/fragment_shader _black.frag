varying in vec4 vert_pos;

uniform sampler2D texture;
uniform bool hasTexture;
uniform vec2 lightPos;

void main()
{
	//Ambient light
	vec4 ambient = vec4(0,0,0,0);
	
	//Convert light to view coords
	lightPos = (gl_ModelViewProjectionMatrix * vec4(lightPos, 0, 1)).xy;
	
	//Calculate the vector from light to pixel (Make circular)
	vec2 lightToFrag = lightPos - vert_pos.xy;
	lightToFrag.y = lightToFrag.y / 1.9;



    gl_FragColor = vec4(0,0,0,0);
}