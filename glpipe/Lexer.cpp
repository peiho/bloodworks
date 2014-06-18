
#include "Lexer.h"
#include <stdio.h>

Lexer::Lexer(std::FILE* stream) :
line(0),
position(0),
commentLine(false),
comment(false),
stream(stream),
c_prev(0){
};

inline bool issep(char c)
{
	switch (c)
	{
	case '_': case'.': 
		return false;
	default:
		return !isalnum(c);
	}
}

const char* Lexer::nextBlock(){
  buffer.clear();
  int para = 0;
  while(!stream.end)
  {
    char c = stream.pop();
    if (c == '{') para ++;
    if (c == '}') 
    {
      para --;
      if (para < 0)
      {
        stream.push(c);
        break;
      }
    }
    buffer.push_back(c);
  }
  
	if (buffer.size())
		return buffer.c_str();
	else
		return NULL;
}

const char* Lexer::next(){

restart: 
	char nc = 0;
	char c = ' ';
	char lc = c_prev;
	buffer.clear();
	
	bool first = true;

	while (!stream.end)
	{
		char c = stream.pop();
		position++;
		if (c == '\n')
		{
			position = 0;
			line++;
		}

		//consume whitespace
		if (first)
		{
			while (!stream.end&&isspace(c)){
				c = stream.pop();
				position++;
				if (c == '\n')
				{
					position = 0;
					line++;
				}
			}
			first = false;
		}

		//handle comments
		if (c == '/'){
			char nc = stream.pop(); //lookahead
			if (nc == '/')
			{
				while (!stream.end&&stream.pop() != '\n'){}
				line++;
				position = 0;
				if (buffer.size())
					break;
				else
					goto restart;
			}
			else if (nc == '*')
			{
				while (!stream.end)
				{
					lc = c;
					c = stream.pop();
					if (c == '\n')
					{
						line++;
						position = 0;
					}
					if (lc == '*' && c == '/')
					{
						break;
					}
				}
				if (buffer.size())
					break;
				else
					goto restart;
			}
			else
			{
				stream.push(nc);
			}
		}
		
		//handle separators
		if (issep(c))
		{
			if (buffer.size())
				stream.push(c);
			else
				buffer.push_back(c);
			break;
		}

		//build token
		if (!isspace(c))
			buffer.push_back(c);
		
	}

	if (buffer.size())
		return buffer.c_str();
	else
		return NULL;
}

int Lexer::getLine(){ 
	return line; 
}

int Lexer::getPosition(){ 
	return position; 
}