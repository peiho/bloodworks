#include <cstdio>
#include <ctime>

#define GLEW_STATIC
#include "glew.h"
#include "glfw3.h"

#include "..\glpipe\Actions.h";

glpipe::Render r;

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

void focus(GLFWwindow *win, int a)
{
	printf("Focus %x %d\n",win,a);

	if (a==1)
	{
		r.sync();
	}
}

int main()
{
	glfwInit();
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", nullptr, nullptr); // Windowed
	//GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", glfwGetPrimaryMonitor(), nullptr); // Fullscreen

	r.setFilestateSource("test.render");
	r.sync();

	glfwSetWindowFocusCallback(window,focus); 

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	glewInit();

	GLuint vbo;
	glGenBuffers(1, &vbo); // Generate 1 buffer

	float vertices[] = {
		 0.0f,  0.5f, // Vertex 1 (X, Y)
		 0.5f, -0.5f, // Vertex 2 (X, Y)
		-0.5f, -0.5f  // Vertex 3 (X, Y)
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

	GLint u_t = glGetUniformLocation(shaderProgram, "time");

	while(!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUniform1f(u_t,clock()*0.001);

		glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
}