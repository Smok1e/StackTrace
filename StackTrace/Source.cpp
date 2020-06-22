#define TX_COMPILING
#define ALLOW_TRACE

#include <TXLib.h>
#include <stack>
#include "DynamicStack.h"

struct Test 

{

	int value_;

	int value1_;

	Test (int value, int value1) : value_ (value), value1_ (value1)

	{
	
		$sG;

		//printf ("Test #%p constructed\n", this);

	}

	~Test ()

	{
	
		$sR;

		//printf ("Test #%p  destructed\n", this);

	}

};

int main ()

{

	SetWindowText (GetConsoleWindow (), "Dynamic stack test");
	size_t size = 5;

	DynamicStack <int> stack (0, "Test stack 1");

	stack.print ();

	for (size_t i = 0; i < 5; i++) stack.push (i);

	stack.print ();

	for (size_t i = 0; i < 5; i++) printf ("[%zu] = %d\n", i, stack.pop ());

	stack.print ();

}
