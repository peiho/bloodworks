#pragma once

#include <string>
#include "Action.h"


namespace glpipe
{
	/*
		This is the base class for both fragment and vertex shaders.
	*/
	class Shader : public Action
	{
		std::string code;
		std::vector<std::string> tokens;
		unsigned id;
	public:
		void getId();
		void analizeShaderCode();
		virtual void parse(FILE* f);
	};

	class FragmentShader : public Shader
	{
	public:
		//virtual void exec();
	};

	class VertexShader : public Shader
	{
	public:
		//virtual void exec();
	};

	class Program : public Action
	{
		
	};

	class VertexBuffer
	{

	};

	class VertexArray
	{

	};

	class Uniform
	{

	};

	class Element
	{

	};

	enum asmMode{
		GL_TRIANGLES,
		GL_QUADS,
		GL_TRIANGLE_STRIP
	};

	class Render : public Action
	{
		VertexBuffer vbo;
		VertexArray vao;
		Element elem;
		VertexShader vert;
		FragmentShader frag;
		asmMode mode;
	public:
		
		virtual void exec();
		virtual void parse(FILE *);
	};

	class Param
	{

	};

};


