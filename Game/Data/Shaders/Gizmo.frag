uniform vec4 u_Color;
uniform vec3 u_CameraPosition;

varying vec3 v_Normal;
varying vec3 v_SurfacePos;

void main()
{
    //gl_FragColor = texture2D( u_Texture, v_UVCoord ) * u_Color;

    vec3 lightColor = vec3(1,1,1);
    float lightRange = 1.0/0.0;
    vec3 materialColor = u_Color.xyz;

    vec3 litColor = vec3(0,0,0);
    litColor += materialColor;
    //litColor += CalculateLight( u_CameraPosition, lightColor, lightRange, u_CameraPosition, materialColor, v_SurfacePos, v_Normal );
    
    gl_FragColor.rgb = litColor;
    gl_FragColor.a = 1;
}
