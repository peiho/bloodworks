#include <cstdio>
#include <ctime>

#define GLEW_STATIC
#include "glew.h"
#include "glfw3.h"

char* loadString(char* src)
{
	auto f = fopen(src,"rb");
	fseek(f,0,SEEK_END);
	long s = ftell(f);
	char* p = new char[s+1];
	fseek(f,0,SEEK_SET);
	fread(p,s,1,f);
	fclose(f);
	p[s]=0;
	return p;
}

void checkReportShader(unsigned id)
{
	char buffer[4096];
	glGetShaderInfoLog(id, 512, NULL, buffer);
	if (buffer[0]) fprintf(stderr,"%s",buffer);
}

void checkReportProgram(unsigned id)
{
	char buffer[4096];
	glGetProgramInfoLog(id,512, NULL, buffer);
	if (buffer[0]) fprintf(stderr,"%s",buffer);
}

void reload()
{

}


void focus(GLFWwindow *win, int a)
{
	printf("Focus %x %d\n",win,a);
}

int main()
{
	glfwInit();
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(640, 360, "OpenGL", nullptr, nullptr); // Windowed
	//GLFWwindow* window = glfwCreateWindow(1280, 720, "OpenGL", glfwGetPrimaryMonitor(), nullptr); // Fullscreen

	glfwSetWindowFocusCallback(window,focus); 

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	glewInit();

reset:

	GLuint vbo;
	glGenBuffers(1, &vbo); // Generate 1 buffer

	float vertices[] = {
		 +0.5f, +0.5f, 
		 -0.5f, -0.5f, 
		 +0.5f, -0.5f,
		 +0.5f, +0.5f,
		 -0.5f, -0.5f, 
		 -0.5f, +0.5f,
	};

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	auto srcstr = loadString("vert1.vert");
	const char* shsrc[] = { srcstr };

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, shsrc, NULL);
	glCompileShader(vertexShader);
	
	checkReportShader(vertexShader);

	srcstr = loadString("frag1.frag");
	const char* shsrc2[] = { srcstr };

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, shsrc2, NULL);
	glCompileShader(fragmentShader);

	checkReportShader(fragmentShader);

	GLint status;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	glBindFragDataLocation(shaderProgram, 0, "outColor");

	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	checkReportProgram(shaderProgram);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(posAttrib);
	glClearColor(.1,.2,.3,.0);

	glDisable(GL_CULL_FACE);

	GLint u_t = glGetUniformLocation(shaderProgram, "iGlobalTime");
	GLint u_res = glGetUniformLocation(shaderProgram,"iResolution");

	while(!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUniform1f(u_t,clock()*0.001);
		glUniform3f(u_res,640,360,0);

		glDrawArrays(GL_TRIANGLES, 0, 6);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
}