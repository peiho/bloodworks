#version 150

in vec3 pos;
out vec4 outColor;

void main()
{
	vec3 p = pos*sin(pos*7.0).yzx*2.0;
    outColor = vec4(1.0*fract(p.x*4.01), 1.0*fract(p.y*4.01), 1.0*fract(p.z*20.01), 1.0);
}
