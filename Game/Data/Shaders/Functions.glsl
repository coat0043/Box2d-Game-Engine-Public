vec3 CalculatePointLight(vec4 lightSpaceVertexPos,vec3 lightPos, vec3 lightColor, float lightRange,float lightIntensity, vec3 cameraPos, vec3 materialColor,float materialShiness, vec3 surfacePos, vec3 normal)
{
    vec3 dirToLight = lightPos - surfacePos;
    float distanceFromLight = length( dirToLight );
    vec3 normalizedDirToLight = dirToLight / distanceFromLight;

    // Diffuse.
    float diffusePerc = max( 0.0, dot( normalizedDirToLight, normal ) );

    // Specular.
    vec3 normalizedDirToCamera = normalize( cameraPos - surfacePos );
    vec3 halfVector = normalize( normalizedDirToLight + normalizedDirToCamera );
    float specularPerc = max( 0.0, dot( halfVector, normal ) );
    specularPerc = pow( specularPerc, materialShiness );

    // Attenuation.
    float rangePerc = distanceFromLight / lightRange;
    float attenuation = max( 0.0, 1.0 - rangePerc );

    // Light components.
    vec3 diffuse = materialColor * lightColor * diffusePerc * attenuation;
    vec3 specular = lightColor * specularPerc * attenuation;

    // Combined.
    return  lightIntensity*(diffuse + specular);
}

vec3 CalculateSpotLight(vec4 lightSpaceVertexPos,vec3 lightPos,vec3 lightDirection, vec3 lightColor, float spotAngle,float lightIntensity, vec3 cameraPos, vec3 materialColor,float materialShiness, vec3 surfacePos, vec3 normal)
{
    vec3 dirToLight = lightPos - surfacePos;
    float distanceFromLight = length( dirToLight );
    vec3 normalizedDirToLight = dirToLight / distanceFromLight;

    vec3 diffuse;
    vec3 specular;

    // Diffuse.
    float diffusePerc;
    float spotRad=cos(spotAngle*0.017444);
    float AngleBetweenLightAndSurface =  dot(  normalizedDirToLight,-normalize(lightDirection));
    if(AngleBetweenLightAndSurface>=spotRad)//If the point on surface is withing the spotAngle of the light
    {

    diffusePerc = pow(AngleBetweenLightAndSurface,2);
   
     // Specular.
    vec3 normalizedDirToCamera = normalize( cameraPos - surfacePos );
    vec3 halfVector = normalize( normalizedDirToLight + normalizedDirToCamera );
    float specularPerc = max( 0.0, dot( halfVector, normal ) );
    specularPerc = pow( specularPerc,materialShiness );

    // Attenuation.
    float rangePerc = distanceFromLight / spotAngle;
    float attenuation = max( 0.0, 1.0 - rangePerc );

    // Light components.
    diffuse = materialColor * lightColor * diffusePerc * attenuation;
    specular = lightColor * specularPerc * attenuation;
    }
    else
    {
        return vec3(0);
    }

    // Combined.
    return  lightIntensity*(diffuse + specular);
}
