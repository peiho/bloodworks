#version 150

in vec2 position;

uniform float iGlobalTime;

void main()
{
	//pos = vec3(position,.0);
    gl_Position = vec4(position*2.0, 0.0, 1.0);
} 
