#include "Parser.h"
#include "Filestate.h"
#include "Actions.h"
#include <Windows.h>

using namespace glpipe;


void test01()
{
	Filestate fs("test.txt");

	while (true)
	{
		Sleep(1000);
		if (fs.FilestateChanged())
		{
			MessageBoxA(0,"File changed","Notification",MB_OK);
			fs.FilestateUpdate();
		}
	}

}

void test02()
{
	FragmentShader s; 
	s.setFilestateSource("shader.frag"); 
	s.sync();
}

void test03()
{
	Render r;
	r.setFilestateSource("first.render");
	r.sync();
}

int main()
{
	test03();
	//Action *a = parse(stdin);
	//delete a;
	//return 0;
};

