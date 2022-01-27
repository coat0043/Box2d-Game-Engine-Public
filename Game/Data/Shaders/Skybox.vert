attribute vec3 a_Position;
attribute vec2 a_UVCoord;

uniform mat4 u_WorldTransform;
uniform mat4 u_ViewTransform;
uniform mat4 u_ProjectionMatrix;

uniform vec2 u_UVScale;
uniform vec2 u_UVOffset;

varying vec3 v_DirToSurface;

void main()
{
    vec4 localPos = vec4( a_Position, 0 );
    vec4 viewPos = u_ViewTransform * localPos;
    viewPos.w = 1;
    vec4 clipPos = u_ProjectionMatrix * viewPos;

    gl_Position = clipPos;

    v_DirToSurface = a_Position;
}
