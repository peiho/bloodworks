

#include "Action.h"


namespace glpipe
{
	Action::Action()
	{
	};

	void Action::exec()
	{
		throw "This should never be called.";
	};

	void Action::sync()
	{
		if (this->FilestateChanged())
		{
			if (this->isFilestate())
			{
        Lexer lexer(openStream());
				parse(&lexer);
				closeStream();
				FilestateUpdate();
			}
			else
			{
				throw "trying to sync without a source";
			}
		}
	};

	void Action::parse(Lexer* f)
	{
		throw "shouln't be called";
	}

	void Action::prepare()
	{
		 
	}

	void CompositeAction::bind(Action *a)
	{
		alist.push_back(a);
	};

	void CompositeAction::exec()
	{
		for (auto i = alist.begin(); i != alist.end(); i++)
		{
			(*i)->exec();
		}
	};

	CompositeAction::~CompositeAction()
	{
		for (auto i = alist.begin(); i != alist.end(); i++)
		{
			delete (*i);
		}
	}

	void CompositeAction::sync()
	{
		for (auto i = alist.begin(); i != alist.end(); i++)
		{
			(*i)->sync();
		}
	}

	void CompositeAction::prepare()
	{
		for (auto i = alist.begin(); i != alist.end(); i++)
		{
			(*i)->prepare();
		}
	}

};