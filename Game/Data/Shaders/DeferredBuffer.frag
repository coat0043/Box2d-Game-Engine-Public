#version 330

layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gColorSpec;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

in vec2 v_UVCoord;
in vec3 v_Normal;
in vec3 v_SurfacePos;

void main()
{
    gPosition = vec4( v_SurfacePos , 1 );
    gNormal = vec4( v_Normal , 1 );
    gColorSpec.rgb = texture(u_Texture, v_UVCoord).rgb * u_Color.rgb;
    gColorSpec.a = 1.0; // no specular data as of yet.
}