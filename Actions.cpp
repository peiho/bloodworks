
#include "Actions.h"
#include <hash_set>

namespace glpipe
{
	void Shader::parse(FILE* f)
	{
		code.clear();
		{
			char c=0;
			int para = 0;
			while(fread(&c,1,1,f))
			{
				if (c == '{') para++;
				if (c == '}') para--;
				if (para<0) break;
				this->code.push_back(c);
			}
			if (para>0)
			{
				throw "stream ended before closing braces";
			}
		}
		analizeShaderCode();
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
		BLOCK_VERTEX,
		BLOCK_VERTEX_BUFFER,
		BLOCK_VERTEX_ARRAY,
		BLOCK_PROGRAM,
		BLOCK_PROGRAM_VERTEX,
		BLOCK_PROGRAM_FRAGMENT,
		BLOCK_PARAM
	};

	void VertexBuffer::parse(FILE* f)
	{
		int para(0);
		std::string line;
		std::string token;
		int line_counter(0);
		int char_counter(0);
		while (para >= 0 && !feof(f))
		{
			char c;
			line.clear();
			line_counter++;
			char_counter = 0;
			do
			{
				fread(&c, 1, 1, f);
				line.push_back(c);
			} while (c != '\n');
		}
	}

	void Render::parse(FILE* f)
	{
		int para(0);
		std::string line;
		std::string token;
		int line_counter(0);
		int char_counter(0);
		_blocktype blocktype;
	
		bool comment(false);

		char c_prev;
		char c;

		while (para>=0&&!feof(f))
		{
			line.clear();
			line_counter++;
			char_counter = 0;
			do
			{
				fread(&c,1,1,f);
				line.push_back(c);
			} while (c!='\n');

			c_prev = '\n';
			token.clear();
			for (auto i = line.begin(); i!= line.end(); i++)
			{
				char_counter++;
				if (isalnum(*i)||*i=='_')
				{
					token.push_back(*i);
				}
				else
				{
					if (!comment && token.size())
					{
						if (token.compare("param")==0) 
							blocktype = BLOCK_PARAM;

						else if (token.compare("vertex")==0)
							blocktype = BLOCK_VERTEX;

						else if (token.compare("buffer")==0 && 
								(blocktype == BLOCK_VERTEX))
							blocktype = BLOCK_VERTEX_BUFFER;

						else if (token.compare("array")==0 && 
								(blocktype == BLOCK_VERTEX))
							blocktype = BLOCK_VERTEX_ARRAY;

						else if (token.compare("program")==0 &&
								(blocktype == BLOCK_VERTEX))
							blocktype = BLOCK_PROGRAM_VERTEX;

						else if (token.compare("fragment"))
								blocktype = BLOCK_PROGRAM_FRAGMENT;

					}
					token.clear();

					if (c_prev=='/'&&*i=='*') comment = true;
					if (c_prev=='*'&&*i=='/') comment = false;

					if (!comment)
					{
						if (*i=='/'&&c_prev=='/') break; //line comment
						if (*i=='{') 
						{
							switch (blocktype)
							{
							case BLOCK_PROGRAM_VERTEX:
								this->vert.parse(f);
							case BLOCK_PROGRAM_FRAGMENT:
								this->frag.parse(f);
							default: case NONE:
								para++; break;
							}
						}
						if (*i=='}') 
						{
							switch (blocktype)
							{
							case BLOCK_VERTEX_BUFFER: case BLOCK_VERTEX_ARRAY:
								blocktype = BLOCK_VERTEX; break;
							case BLOCK_PROGRAM_FRAGMENT: case BLOCK_PROGRAM_VERTEX:
								blocktype = BLOCK_PROGRAM; break;
							default:
								blocktype = NONE; break;
							}
							para--;
						}
					}

				}
				c_prev = *i;
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