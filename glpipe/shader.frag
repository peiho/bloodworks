

uniform float time;


void main()
{
	gl_FragColor = vec4(gl_FragCoord.x+.4f*sin(t));
}