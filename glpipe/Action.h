#pragma once

#include <vector>
#include "Lexer.h"
#include "Filestate.h"

namespace glpipe
{	
	class Action : public Filestate //abstract
	{
	protected: 

		Action();

	public:

		virtual void sync();

		virtual void parse(Lexer* f);
		virtual void prepare();
		virtual void exec();
	};

	class CompositeAction : public Action
	{
	protected:
		
		std::vector<Action*> alist; //strong
	
	public:
		
		void bind(Action* a);
		virtual void sync();
		virtual void prepare();
		virtual void exec();
		CompositeAction::~CompositeAction();
	
	};

};