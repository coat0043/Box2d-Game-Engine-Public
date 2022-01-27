uniform vec4 u_Color;
uniform sampler2D u_Texture;

varying vec2 v_UVCoord;

void main()
{
	gl_FragColor = u_Color;
}
