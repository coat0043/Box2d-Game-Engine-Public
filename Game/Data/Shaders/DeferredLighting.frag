#version 330 //DLf

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D u_ColorSpecTex;
uniform sampler2D u_PositionTex;
uniform sampler2D u_NormalTex;

uniform vec3 u_CameraPosition;

// Pretty inefficient. not every light needs every variable.
struct Light {

    int LightType; // 0 = pointlight, 1 = spotlight, 2 = directionallight

    vec3 Position;
    vec3 Direction;
    vec3 Color;

    float Range;
    float Intensity;
    float Ambient;
    float Attenuation;
    float SpecularIntensity;

    float SpotConeAngle;
    float SpotConeFalloff;
};

uniform int u_NR_LIGHTS;

const int MAX_NR_LIGHTS = 64;
uniform Light LightArray[MAX_NR_LIGHTS];

vec3 CalculateLight(int i, vec3 FragPosition, vec3 FragNormal, vec3 FragDiffuse, int type);
//vec3 CalculateDirectionalLight(int i, vec3 FragPosition, vec3 FragNormal, vec3 FragDiffuse);

void main()
{
    vec3 LightResult = vec3(0,0,0);

    //retrieve data from gbuffer
    vec3 FragPosTex = texture(u_PositionTex, TexCoords).rgb;
    vec3 NormalTex  = texture(u_NormalTex, TexCoords).rgb;
    vec3 DiffuseTex = texture(u_ColorSpecTex, TexCoords).rgb;

    for(int i = 0; i < u_NR_LIGHTS; i++)
    {
        LightResult += CalculateLight(i, FragPosTex, NormalTex, DiffuseTex, int(LightArray[i].LightType)); 
    }

	FragColor = vec4(LightResult, sign(DiffuseTex)); // this ensures we don't write anything to the sky/clearcolor background.
}


vec3 CalculateLight(int i, vec3 FragPosition, vec3 FragNormal, vec3 FragDiffuse, int type)
{
    // Diffuse
    vec3 SurfaceToLight = LightArray[i].Position - FragPosition;
    SurfaceToLight = SurfaceToLight - (SurfaceToLight - LightArray[i].Direction) * int(type == 2);
    float DistanceFromLight = length(SurfaceToLight);
    // DistanceFromLight = DistanceFromLight - (DistanceFromLight - LightArray[i].Attenuation) * int(type == 2);
    SurfaceToLight =  normalize(SurfaceToLight);

    vec3 Diffuse = max( dot(SurfaceToLight, FragNormal), 0.0 ) * FragDiffuse * LightArray[i].Intensity;
    vec3 ViewDir = normalize(u_CameraPosition - FragPosition);
    
    // Specular
    vec3 HalfwayDir = reflect( -SurfaceToLight, FragNormal ); 

    // a really cheap, sharp falloff function. 
    //
    //  y = 1 / ((1 - x + b) * a) - b;
    //
    //        -a + (sqrt (aa + 4a) )
    //  b = -------------------------- // this will result in an accurate 0 -> 1 mapping. we don't need very high accuracy, A ballpark value works.
    //                  2a
    //
    //  assigning 'a' from 1 to infinity results in a more and more steep falloff. in this case i'm using a = 10.
    //  i'm going to allow b to lerp a bit to allow for rescaling the reflection

    float SurfaceLightAngle = max( dot(ViewDir, HalfwayDir), 0.0 );

    float a = 20;
    float b = 0.02 - (0.02 - 0.66) * LightArray[i].Attenuation;
    //
    float FalloffResult = max( 1 / (( 1 - SurfaceLightAngle + b) * a) - b, 0.0 );
    //
    float SpecularFactor = pow( max(SurfaceLightAngle - (SurfaceLightAngle - FalloffResult) * int(type == 2) , 0.0f) , 40 ) * LightArray[i].SpecularIntensity;
    //float SpecularFactor = pow( SurfaceLightAngle , 20 ) * LightArray[i].SpecularIntensity;
    
    // Attenuation
    float Attenuation = max( (LightArray[i].Range - DistanceFromLight) / LightArray[i].Range, 0); // if this is a directional light, Attenuation = 1. otherwise it drops linearly. Another example below with better explanation.
    Attenuation = clamp( int(type == 2) + pow(Attenuation, max(LightArray[i].Attenuation, 0.005)), 0, 1);

    Diffuse *= Attenuation;
    SpecularFactor *= Attenuation - (Attenuation - 1) * int(type == 2);

    // The result:
    vec3 To_Return = Diffuse * LightArray[i].Color; // Diffuse effect
    To_Return += SpecularFactor * LightArray[i].Color; // Specular effect

    // Spotlight cone
    vec3 LightFacing = normalize(-LightArray[i].Direction); // Fix direction of light
    float theta = dot(SurfaceToLight, LightFacing);// Incorporate the angle between lightdir and surfacetolight

    float FadeoutAngle = 90.0 - (LightArray[i].SpotConeAngle) * 0.75;                         // rough rescaling of SpotConeAngle is required cause cos from the dot product isn't linear
    float SpotlightMask = ( theta - radians (FadeoutAngle) ) * LightArray[i].SpotConeFalloff; // theta - FadeoutEdge linearly fades the mask out the farther out you get from the spotlight's facing. 
                                                                                              // LightArray[i].SpotConeFalloff is just a multiplier to allow more control over the fade. it allows us to
                                                                                              // bring back some of the harsher edges.
                                                                                              // this mapping can go above 1 and below 0 so now we need to clamp it:
    
    To_Return *= clamp( int(type == 0 || type == 2) + SpotlightMask, 0, 1); // Clamp and apply the spotlight mask. if type == 0 or 2 (point light and directional light), this equals 1 and is basically ignored.

    To_Return += LightArray[i].Ambient * LightArray[i].Color; // Ambient effect. applied after any potential masking

    return To_Return;   
}
