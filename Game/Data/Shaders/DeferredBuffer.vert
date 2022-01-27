#version 330 //DBv

in vec3 a_Position;
in vec2 a_UVCoord;
in vec3 a_Normal;

uniform mat4 u_WorldTransform;
uniform mat4 u_ViewTransform;
uniform mat4 u_ProjectionMatrix;
uniform mat4 u_RotationMatrix;

uniform vec2 u_UVScale;
uniform vec2 u_UVOffset;

out vec2 v_UVCoord;
out vec3 v_Normal;
out vec3 v_SurfacePos;

void main()
{
    vec4 localPos = vec4( a_Position, 1 );
    vec4 worldPos = u_WorldTransform * localPos;
    vec4 viewPos = u_ViewTransform * worldPos;
    vec4 clipPos = u_ProjectionMatrix * viewPos;

    gl_Position = clipPos;

    // Transform UV coordinates.
    v_UVCoord = a_UVCoord * u_UVScale + u_UVOffset;

    v_Normal = 1 - 1 + (u_RotationMatrix * vec4( a_Normal, 0 )).xyz;

    v_SurfacePos = worldPos.xyz;
}
