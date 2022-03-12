#version 440 core

out vec4 FragColor;

in vec3 WorldPos_FS_in;
in float MapPos_FS_in;
in vec3 Blend_Normal_FS_in;
in vec3 Normal_FS_in;
in vec3 Color_FS_in;
in vec2 uvCoords;

uniform vec3 lDirection;
uniform mat4 Model;
uniform vec3 camPos;

uniform vec2 chunkCoords;
uniform int selected;

uniform float layerRanges[7];
uniform float layerColors[24];
uniform int layerType[6];
uniform int numLayers;

float rand(vec2 n) 
{ 
	return fract(sin(dot(n, vec2(12.9898, 4.1414))) * 43758.5453);
}

vec2 Hash(vec2 c)
{
    //Non Correlated coords
	float x = fract(sin(dot(vec2(float(c.x),float(c.y)) ,vec2(12.9898,78.233))) * 43758.5453);
    float y = fract(sin(dot(c+0.5 ,vec2(12.9898,78.233))) * 43758.5453);
    
    //returning the value converted from range [0,1.0] to [-1.0,1.0]
	return vec2(2.0*x-1.0,2.0*y-1.0);
}

float noise(vec2 uv, float size)
{
    vec2 tile = floor(uv*size);    
    vec2 tile_right = vec2(tile.x+1.0,tile.y);
    vec2 tile_up = vec2(tile.x,tile.y+1.0);
    vec2 tile_diag = vec2(tile.x+1.0,tile.y+1.0);

    float tileColor_1 = rand(tile);
    float tileColor_2 = rand(tile_right);
    float tileColor_3 = rand(tile_up);
    float tileColor_4 = rand(tile_diag);
    
    float mixFactor_x=smoothstep(0.0,1.0,fract(uv.x*size));
    
    float mix_1=mix(tileColor_1,tileColor_2,mixFactor_x);
	float mix_2=mix(tileColor_3,tileColor_4,mixFactor_x);
    
    float mixFactor_y = smoothstep(0.0,1.0,fract(uv.y*size));

    float finalMix=mix(mix_1,mix_2,mixFactor_y);
    
    return finalMix;
  
    return tileColor_1;
}

float Noise(vec2 test, float size)
{
    //Converting decimals to integers [0,0,0,.....2,2,2,....,9,9,9]
	vec2 uv =test * size;
    vec2 tile = floor(uv);
    
    //Bottom Left Corner
    vec2 botLeft = tile;						//Tile Coords
    vec2 botLeftG = Hash(botLeft);				//Setting Random Gradient 
    vec2 offsetBL = uv - botLeft;				//Distance vector from coords to Corner (Now yes :D)
    float dotBL=dot(botLeftG,offsetBL);			//Dot between distance and Corner
    
    //Bottom Right Corner
    vec2 botRight = tile+vec2(1.0,0.0);
    vec2 botRightG = Hash(botRight);
    vec2 offsetBR = uv - botRight;
    float dotBR=dot(botRightG,offsetBR);
    
    //Top Left Corner
    vec2 topLeft = tile+vec2(0.0,1.0);
    vec2 topLeftG = Hash(topLeft);
    vec2 offsetTL = uv - topLeft;
    float dotTL=dot(topLeftG,offsetTL);
    
    //Top Left Corner
    vec2 topRight = tile+vec2(1.0,1.0);
    vec2 topRightG = Hash(topRight);
    vec2 offsetTR = uv - topRight;
    float dotTR=dot(topRightG,offsetTR);
    
    
    float mixFactor_x=smoothstep(0.0,1.0,fract(uv.x)); //mix factor depending on the x coords 
    float mixFactor_y=smoothstep(0.0,1.0,fract(uv.y)); //mix factor depending on the y coords 

    
    float RLBR = mix(dotBL,dotBR,mixFactor_x);
    float TLTR = mix(dotTL,dotTR,mixFactor_x);
    
    float total = mix(RLBR,TLTR,mixFactor_y);

    return (total+1.0)*0.5;
}

float mixColor(vec2 uv,float size)
{
    float col =0.0;
    float a =0.5;
    float f=1.0;
	
	vec2 coords = uv+chunkCoords;
    
   	for(int i=0; i<5;i++)
   	{
    	col += a*Noise(f*(coords), size);
        a*=0.5;
        f*=2.0;
    }
    
	return col;
}

vec3 Grass(vec2 pos)
{
	vec3 color = vec3(0.0);
	
	float rand_1 = mixColor(pos,200.0);
	vec3 col1 = vec3(layerColors[6],layerColors[7],layerColors[8]);
    vec3 col2 = vec3(layerColors[9],layerColors[10],layerColors[11]);
	color = mix(col1,col2,(1.0-rand_1)*0.9);
	
	return color;
}

vec3 Rock(vec2 pos)
{
	vec3 color = vec3(0.0);
	
	float rand_1 = mixColor(pos,100.0);
	vec3 col3 = vec3(layerColors[12],layerColors[13],layerColors[14]);
    vec3 col4 = vec3(layerColors[15],layerColors[16],layerColors[17]);
	color = mix(col3,col4,(1.0-rand_1)*0.9);
	
	return color;
}

vec3 Beach(vec2 pos)
{
	vec3 color = vec3(0.0);
	
	float rand_1 = mixColor(pos,200.0);
	vec3 col3 = vec3(layerColors[0],layerColors[1],layerColors[2]);
    vec3 col4 = vec3(layerColors[3],layerColors[4],layerColors[5]);
	color = mix(col3,col4,(1.0-rand_1)*0.9);
	
	return color;
}

vec3 Snow(vec2 pos)
{
	vec3 color = vec3(0.0);
	
	float rand_1 = mixColor(pos,200.0);
	vec3 col3 = vec3(layerColors[18],layerColors[19],layerColors[20]);
    vec3 col4 = vec3(layerColors[21],layerColors[22],layerColors[23]);
	color = mix(col3,col4,(1.0-rand_1));
	
	return color;
}

vec3 ComputeLayerColor(vec3 normal, int type,vec2 uv)
{
	vec3 ret = vec3(0.0);
	
	vec3 blending = abs(normal);
	blending = normalize(max(blending, 0.00001));
	// Force weights to sum to 1.0
	float b = (blending.x + blending.y + blending.z);
	blending /= vec3(b, b, b);
	
	vec3 xaxis;
	vec3 yaxis;
	
	xaxis = Rock(uv);
	
	if(type == 0)
	{
		yaxis = Beach(uv);
	}
	else if(type == 1)
	{
		yaxis = Grass(uv);
	}
	else if(type == 2)
	{
		yaxis = Rock(uv);
	}
	else
	{
		yaxis = Snow(uv);
	}
	
	ret = xaxis * blending.x + yaxis * blending.y + xaxis * blending.z;
	
	return ret;
}
	


void main()
{
	vec3 normal = Normal_FS_in;
	vec3 fragPos = WorldPos_FS_in;
	
	vec3 vecColor = ComputeLayerColor(normal,layerType[0],uvCoords);
	
	for(int i=0; i<numLayers; ++i)
	{
		vec3 layerColor = ComputeLayerColor(normal,layerType[i],uvCoords);
		
		float margin = (layerRanges[i+1] - layerRanges[i])/4.0;
		float mixFactor = smoothstep(layerRanges[i]-margin,layerRanges[i]+margin,MapPos_FS_in);
		vecColor=mix(vecColor,layerColor,mixFactor);
	}
	
    vec3 lightColor = vec3(1.0);

    // ambient
    float ambientStrength = 0.5;
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(-lDirection);
    float diff = max(dot(lightDir,norm), 0.0);
    vec3 diffuse = diff * lightColor;
	
	// specular
	//float spec = 0.0;
	//vec3 viewDir = normalize(camPos-fragPos);
	//vec3 reflectDir = reflect(-lightDir,norm);
	//vec3 halfwayDir = normalize(lightDir + viewDir);
	//spec = pow(max(dot(normal,halfwayDir),0.0),256.0);
	//vec3 specular = 0.2*spec*lightColor;
	            
    vec3 result = (ambient + diffuse) * vecColor;
	
	if(selected==1)
	{
		result=mix(result,vec3(1.0,1.0,0.5),0.3);
	}
	
	//Gama Correction
	result=pow(result,vec3(1.0/(2.2)));
    FragColor = vec4(result, 1.0);
} 