
#pragma once

#include <cstdio>
#include "Action.h"

namespace glpipe
{
	class Parser
	{
	public:
		virtual void parse(FILE* f);
	};

};