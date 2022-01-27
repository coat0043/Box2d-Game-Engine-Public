attribute vec3 a_Position;
attribute vec3 a_Normal;

uniform mat4 u_WorldTransform;
uniform mat4 u_ViewTransform;
uniform mat4 u_ProjectionMatrix;
uniform mat4 u_RotationMatrix;

varying vec3 v_Normal;
varying vec3 v_SurfacePos;

void main()
{
    vec4 localPos = vec4( a_Position, 1 );
    vec4 worldPos = u_WorldTransform * localPos;
    vec4 viewPos = u_ViewTransform * worldPos;
    vec4 clipPos = u_ProjectionMatrix * viewPos;

    gl_Position = clipPos;

    v_Normal = u_RotationMatrix * vec4( a_Normal, 0 );

    v_SurfacePos = worldPos.xyz;
}
