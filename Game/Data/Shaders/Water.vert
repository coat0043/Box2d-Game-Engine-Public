attribute vec3 a_Position;
attribute vec2 a_UVCoord; // Input from VBO in C++.

uniform mat4 u_WorldTransform;
uniform mat4 u_ViewTransform;
uniform mat4 u_ProjectionMatrix;

uniform vec2 u_UVScale;
uniform vec2 u_UVOffset;

uniform vec2 u_CameraPosition;
uniform vec2 u_ProjectionScale;

uniform float u_Time;

varying vec2 v_UVCoord; // Output to frag shader.

void main()
{
    vec4 localPos = vec4( a_Position, 1 );
    vec4 worldPos = u_WorldTransform * localPos;

    worldPos.y += sin( (worldPos.x + u_Time)       ) * 0.15;
    worldPos.y += sin( (worldPos.z + u_Time) * 0.5 ) * 0.1;

    vec4 viewPos = u_ViewTransform * worldPos;
    vec4 clipPos = u_ProjectionMatrix * viewPos;

    gl_Position = clipPos;

    // Transform UV coordinates.
    v_UVCoord = a_UVCoord * u_UVScale + u_UVOffset;
}
