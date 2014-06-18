#version 150

in vec2 position;
out vec3 pos;

uniform float time;

void main()
{
	vec2 newpos = position*mat2(cos(time),sin(time),-sin(time),cos(time))/(1.0+sin(time+position.x+position.y)*.5);
	pos = vec3(position.xy,(1.0+sin(time+position.x+position.y)*.5));
    gl_Position = vec4(newpos, 0.0, 1.0);
} 
