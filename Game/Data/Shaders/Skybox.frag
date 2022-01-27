uniform samplerCube u_Cubemap;

varying vec3 v_DirToSurface;

void main()
{
    gl_FragColor = textureCube( u_Cubemap, v_DirToSurface );
}
