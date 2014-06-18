#version 150
uniform float iGlobalTime;
uniform vec3 iResolution;

#define noise_use_smoothstep

float hash(float x)
{
	return fract(sin(x*.0127863)*17143.321); 
}

float hashmix(float x0, float x1, float interp)
{
	x0 = hash(x0);
	x1 = hash(x1);
	#ifdef noise_use_smoothstep
	interp = smoothstep(0.0,1.0,interp);
	#endif
	return mix(x0,x1,interp);
}

float hashmix(vec2 p0, vec2 p1, vec2 interp)
{
	float v0 = hashmix(p0[0]+p0[1]*128.0,p1[0]+p0[1]*128.0,interp[0]);
	float v1 = hashmix(p0[0]+p1[1]*128.0,p1[0]+p1[1]*128.0,interp[0]);
	#ifdef noise_use_smoothstep
	interp = smoothstep(vec2(0.0),vec2(1.0),interp);
	#endif
	return mix(v0,v1,interp[1]);
}

float hashmix(vec3 p0, vec3 p1, vec3 interp)
{
	float v0 = hashmix(p0.xy+vec2(p0.z*43.0,0.0),p1.xy+vec2(p0.z*43.0,0.0),interp.xy);
	float v1 = hashmix(p0.xy+vec2(p1.z*43.0,0.0),p1.xy+vec2(p1.z*43.0,0.0),interp.xy);
	#ifdef noise_use_smoothstep
	interp = smoothstep(vec3(0.0),vec3(1.0),interp);
	#endif
	return mix(v0,v1,interp[2]);
}

float hashmix(vec4 p0, vec4 p1, vec4 interp)
{
	float v0 = hashmix(p0.xyz+vec3(p0.w*17.0,0.0,0.0),p1.xyz+vec3(p0.w*17.0,0.0,0.0),interp.xyz);
	float v1 = hashmix(p0.xyz+vec3(p1.w*17.0,0.0,0.0),p1.xyz+vec3(p1.w*17.0,0.0,0.0),interp.xyz);
	#ifdef noise_use_smoothstep
	interp = smoothstep(vec4(0.0),vec4(1.0),interp);
	#endif
	return mix(v0,v1,interp[3]);
}



float noisei(float p)
{
	float pm = mod(p,1.0);
	float pd = p-pm;
	return hashmix(pd,pd+1.0,pm);
}

float noisei(vec2 p)
{
	vec2 pm = mod(p,1.0);
	vec2 pd = p-pm;
	return hashmix(pd,(pd+vec2(1.0,1.0)), pm);
}

float noisei(vec3 p)
{
	vec3 pm = mod(p,1.0);
	vec3 pd = p-pm;
	return hashmix(pd,(pd+vec3(1.0,1.0,1.0)), pm);
}

float noisei(vec4 p)
{
	vec4 pm = mod(p,1.0);
	vec4 pd = p-pm;
	return hashmix(pd,(pd+vec4(1.0,1.0,1.0,1.0)), pm);
}

float noisxe(vec2 p)
{
	vec2 pm = mod(p,1.0);
	vec2 pd = p-pm;
	float v0=hash(pd.x+pd.y*141.0);
	float v1=hash(pd.x+1.0+pd.y*141.0);
	float v2=hash(pd.x+pd.y*141.0+141.0);
	float v3=hash(pd.x+pd.y*141.0+142.0);
	v0 = mix(v0,v1,smoothstep(0.0,1.0,pm.x));
	v2 = mix(v2,v3,smoothstep(0.0,1.0,pm.x));
	return mix(v0,v2,smoothstep(0.0,1.0,pm.y));
}
//functions that build rotation matrixes
mat2 rotate_2D(float a){float sa = sin(a); float ca = cos(a); return mat2(ca,sa,-sa,ca);}
mat3 rotate_x(float a){float sa = sin(a); float ca = cos(a); return mat3(1.,.0,.0,    .0,ca,sa,   .0,-sa,ca);}
mat3 rotate_y(float a){float sa = sin(a); float ca = cos(a); return mat3(ca,.0,sa,    .0,1.,.0,   -sa,.0,ca);}
mat3 rotate_z(float a){float sa = sin(a); float ca = cos(a); return mat3(ca,sa,.0,    -sa,ca,.0,  .0,.0,1.);}


float t = iGlobalTime+noisei(gl_FragCoord.xy)*(1.0/24.0);


vec3 flare(vec2 uv, vec2 pos, float seed, float size)
{
	vec4 gn = vec4(.0,noisei(seed-1.0),noisei(seed*5.0-1.0),noisei(seed*13.0-1.0));
	gn.x = size;
	vec3 c = vec3(.0);
	vec2 p = pos;
	vec2 d = uv-p;
	
	
	c += (0.01+gn.x*.2)/(length(d));
	
	c += vec3(noisei(atan(d.x,d.y)*256.9+pos.x*2.0)*.25)*c;
	
	float filter = length(uv);
	filter = (filter*filter)*.5+.5;
	filter = min(filter,1.0);
	
	for (float i=.0; i<3.; i++)
	{
		vec4 n = vec4(noisei(seed+i),noisei(seed*5.0+i),noisei(seed*17.0+i),noisei(seed*13.0+i));
		vec4 n2 = vec4(noisei(seed+i*2.1),noisei(seed*5.0+i*2.1),noisei(seed*17.0+i*2.1),noisei(seed*13.0+i*2.1));
		vec4 nc = vec4(noisei(seed+i*3.3),noisei(seed*5.0+i*3.3),noisei(seed*17.0+i*3.3),noisei(seed*13.0+i*3.3));
		nc+=vec4(length(nc));
		nc*=.5;
		
		for (int i=0; i<3; i++)
		{
			float ip = n.x*2.0+float(i)*.1*n2.y*n2.y*n2.y;
			float is = n.y*n.y*2.5*gn.x+.1;
			float ia = (n.z*4.0-2.0)*n2.x*n.y;
			vec2 iuv = (uv*(mix(1.0,length(uv),n.w*n.w)))*mat2(cos(ia),sin(ia),-sin(ia),cos(ia));
			vec2 id = mix(iuv-p,iuv+p,ip);
			c[i] += pow(max(.0,is-(length(id))),.45)/is*.1*gn.x*nc[i]*filter*4.0;
		}
		
	}
	
	
	return c;
}

float df(vec3 p)
{
	vec2 m = mod(p.xz,vec2(1.0));
	vec2 i = p.xz-m;
	m=m-vec2(.5); m*=2.0;
	m = m*m;
	vec3 q = p;
	q.y+=t*3.0;
	q.x+=t*.1;
	q.z+=t*.1;
	float amount = sin(t)*.1+.1;
	float pillars = noisei(p.xz)-amount;
	float waves = sin(pillars*8.0-t*2.0+p.x*.5+p.z*.7)*.1/(pillars+.7);
	return min(p.y+3.0+waves,noisei(p.xz)-.1)+noisei(q.xyz*vec3(4.0,1.0,4.0)*2.0)*.1;
}

vec3 nf(vec3 p)
{
	vec2 e = vec2(.05,.0);
	float c = df(p);
	return normalize(vec3(c-df(p+e.xyy),c-df(p+e.yxy),c-df(p+e.yyx)));
}

vec3 background(vec3 d)
{
	return d.y*vec3(.2,.4,.8)*.5+.5;	
}

vec3 reflection(vec3 d)
{
	return noisei(d*7.0)*background(d);
}

vec4 noise(float x)
{
	return vec4(noisei(x+612.0),noisei(x+421.0),noisei(x+112.0),noisei(x+16.0));
}

float mmm(float a, float b, float c, float d)
{
	return min(min(a,b),min(c,d));
}

void main(void)
{
	vec2 uv = gl_FragCoord.xy / iResolution.yy - vec2(.9,.5);
	
	float t=iGlobalTime;
	vec3 p = vec3(.1,.0,.0);
	p.z+=t;
	vec3 turb = (noise(t+10.0).xyz*noise(t+22.0).xyz)
		*((sin(t*.1)+sin(t*3.10)+sin(t*7.10)-3.0)/6.0)*.2;
	turb*=1.0-cos(t*.2);
	turb*=sin(t*.793)*.5+.5;
	mat3 rot = rotate_x(.1+turb.x*.15) * rotate_y(turb.y*.15);
	vec3 d = normalize(vec3(uv,.9-pow(length(uv),4.0)*.2));
	d*=rot;
	float td = .0;
	for (int i=0; i<100; i++)
	{
		float dd = df(p);
		p += d*dd;
		td+=dd;
		if (dd>100.0||abs(dd)<.01) break;
		
	}
	
	vec3 l = -normalize(vec3(.4,.2,.8));
	vec3 n = nf(p);
	vec3 r = reflect(d,n);
	vec3 color = (dot(l,n)*.5+.5)*vec3(1.0,.1,.2);
	//color*=mix(texture2D(iChannel2,vec2(p.xz)).xyz,vec3(1.0),.2);
	float specular = -dot(reflect(l,n),n);
	specular = max(.0,specular);
	specular = pow(specular,400.0);
	color += vec3(specular);
	float occ = mmm(df(p+r*.5)*2.0, df(p+r), df(p+r*1.5)/1.5, df(p+r*2.0)*.5);
	//color = vec3(occ);
	occ = min(1.0,occ*2.0);
	
	color += reflection(reflect(l,n))*occ;
	color *= (.6+occ*.4);

	color = mix(color,background(d)*.5,min(max(df(p),.0),1.0));
	color-=pow(length(uv),2.0)*.5;
	color+=vec3(noisei(gl_FragCoord.xy+vec2(.0,t*16.0)))*.03;
	color+=background(d)*.1;
	color = mix(color,vec3(length(color)),length(color)*.5);
	color /= (1.0+td/(4.0-cos(t*.2)*3.0));

	color += flare(d.xy,vec2(.0,.69),t*.01,0.04)*1.5;
	gl_FragColor = vec4(color*1.5,1.0);
}