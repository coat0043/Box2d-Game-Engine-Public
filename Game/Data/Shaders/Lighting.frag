//Change this to change memory allocated to arrays of light struct
#define MAX_LIGHTS 16
vec3 globalAmbientLight=vec3(0.1f,0.15f,0.2);
struct Light
{
 mat4 ViewMatrix;
 vec3 position;  
 vec3 direction;   
 vec3 color; 
 float range;
 float intensity;
 int type;//0:Point,1:SpotLight
};

uniform vec4 u_Color;
uniform sampler2D u_Texture;
uniform sampler2D u_NormalMap;  
uniform sampler2D u_TextureCookie;  
uniform vec3 u_CameraPosition;

varying vec2 v_UVCoord;
varying vec3 v_Normal;
varying vec4 v_SurfacePos;

uniform mat4 u_LightViewMatrix;

uniform float u_MaterialShine;  
uniform int u_LightsCount;
uniform Light u_Lights[MAX_LIGHTS];  

void main()
{
    vec3 NormalMap_UV = normalize(texture( u_NormalMap, v_UVCoord ).rgb);
    
    vec3 litColor=globalAmbientLight;
    
    
    for(int i=0;i<u_LightsCount;i++)
    {
    vec4 lightSpaceVertexPos = u_Lights[i].ViewMatrix * v_SurfacePos;
    lightSpaceVertexPos.xyz /= lightSpaceVertexPos.w;

    if(u_Lights[i].type==0)
    litColor += CalculatePointLight(lightSpaceVertexPos, u_Lights[i].position, u_Lights[i].color, u_Lights[i].range,u_Lights[i].intensity, u_CameraPosition, vec3(1),u_MaterialShine, v_SurfacePos, NormalMap_UV );   
    else
    litColor += CalculateSpotLight(lightSpaceVertexPos, u_Lights[i].position,u_Lights[i].direction, u_Lights[i].color, u_Lights[i].range,u_Lights[i].intensity, u_CameraPosition, vec3(1),u_MaterialShine, v_SurfacePos, NormalMap_UV ); 
    
   }

    gl_FragColor = texture2D( u_Texture, v_UVCoord )*vec4(litColor,1);

}


