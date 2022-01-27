uniform vec4 u_Color;

uniform sampler2D u_Texture;
uniform samplerCube u_Cubemap;

uniform vec3 u_CameraPosition;

varying vec2 v_UVCoord;
varying vec3 v_DirToSurface;
varying vec3 v_SurfacePosition;
varying vec3 v_Normal;

void main()
{
    vec3 cameraToSurfaceDir = v_SurfacePosition - u_CameraPosition;
    vec3 surfaceToCubemapDir = reflect( cameraToSurfaceDir, v_Normal );

    vec4 color2D = texture2D( u_Texture, v_UVCoord ) * u_Color;
    vec4 colorCube = textureCube( u_Cubemap, surfaceToCubemapDir );

    gl_FragColor = colorCube;
}
