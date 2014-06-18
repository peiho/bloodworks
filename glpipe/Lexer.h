
#pragma once
#include <string>
#include <cstdlib>

class CharStream
{
	char read_buffer[96];
	char* read_head;
	int count;
	std::FILE* stream;

public:
	bool last, end;

	inline char pop()
	{
		if (count)
		{
			count--;
			return *read_head++;
		}
		else
		{
			if (last)
			{
				end = true;
				return 0;
			}
			count = fread(read_buffer+32, 1, 64, stream);
			read_head = read_buffer + 32;
			if (count < 64)
			{
				last = true;
			}
			count--;
			return *read_head++;
		}
	}

	inline void push(char c)
	{
		count++;
		read_head--;
		*read_head = c;
	}

	inline CharStream(std::FILE* stream)
	{
		this->stream = stream;
		count = 0;
		end = last = false;
	}
};

class Lexer
{
public:
	Lexer(std::FILE* stream);
	const char* next();
  const char* nextBlock();
	int getLine();
	int getPosition();

private:
	CharStream stream;
	std::string buffer;
	int line, position;
	bool commentLine, comment;
	char c_prev;
};
