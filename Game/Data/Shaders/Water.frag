uniform vec4 u_Color;
uniform sampler2D u_Texture;

uniform float u_Time;

varying vec2 v_UVCoord;

void main()
{
    vec2 uv = v_UVCoord;
    uv.x += sin( u_Time + v_UVCoord.y * 10.0 ) * 0.08;
    uv.y -= sin( u_Time + v_UVCoord.x * 15.0 ) * 0.12;

    vec4 color1 = texture2D( u_Texture, uv + u_Time / 20.0 );
    vec4 color2 = texture2D( u_Texture, uv + vec2( u_Time / 21.0, 0.0 ) );

    color1 += vec4( 15.0/255.0, 103.0/255.0, 227.0/255.0, 0.0 );

    vec4 finalColor = color1 - color2 * 0.13;

    gl_FragColor = finalColor;
}
