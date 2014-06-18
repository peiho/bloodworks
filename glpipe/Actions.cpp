
#include "Actions.h"
#include <hash_set>
#include <stack>

namespace glpipe
{
	void Shader::parse(Lexer* f)
	{
    this->code = f->nextBlock();
	}

	void Shader::analizeShaderCode()
	{
		std::string token;
		bool ignore = false;
		tokens.clear();

		for (auto i = code.begin(); i!=code.end(); i++)
		{
			if (!isalnum(*i)&&*i!='_')
			{
				if (token.size() && !ignore)
				{
					tokens.push_back(token);
				}
				
				if (*i == '.')
				{
					ignore = true;
				}
				else
				{
					ignore = false;
				}

				token.clear();
			}
			else
			{
				token.push_back(*i);
			}
		}
	}

	enum _blocktype
	{
		NONE,
		BLOCK_PARAM,
		BLOCK_VERTEX,
		BLOCK_VERTEX_ARRAY,
		BLOCK_VERTEX_BUFFER,
    BLOCK_VERTEX_PROGRAM,
    BLOCK_FRAGMENT_PROGRAM,
	};

	void VertexBuffer::parse(Lexer* f)
	{
		
	}

  inline bool eq(const char* p, const char* q)
  {
    char *i = (char*) p;
    char *j = (char*) q;
    while (*i&&*j&&*i==*j){i++;j++;}
    if (*i==*j)
      return true;
    return false;
  }

	void Render::parse(Lexer* lexer)
	{
    int para = 0;
    _blocktype type = NONE;

    std::stack<_blocktype> blocktype;
    blocktype.push(NONE);

    while (const char* token = lexer->next())
    {
      if (eq(token,"param"))
        type = BLOCK_PARAM;
      else if (eq(token,"vertex"))
        type = BLOCK_VERTEX;
      else if (eq(token,"array") && blocktype.top() == BLOCK_VERTEX)
        type = BLOCK_VERTEX_ARRAY;
      else if (eq(token,"buffer") && blocktype.top()  == BLOCK_VERTEX)
        type = BLOCK_VERTEX_BUFFER;
      else if (eq(token,"program") && blocktype.top()  == BLOCK_VERTEX)
        type = BLOCK_VERTEX_PROGRAM;
      else if (eq(token,"fragment"))
        type = BLOCK_FRAGMENT_PROGRAM;
      else if (eq(token,"{"))
      {
        blocktype.push(type);
        switch(type)
        {
        case BLOCK_VERTEX_PROGRAM:
          vert.parse(lexer);
          break;
        case BLOCK_FRAGMENT_PROGRAM:
          frag.parse(lexer);
          break;
        }
      }
      else if (eq(token,"}"))
      {
        blocktype.pop();
      }
    }
	}

	/*virtual void exec();
		virtual void sync();
		virtual void parse(FILE *);*/

	void Render::exec()
	{
		// bunch of gl calls
	};

}