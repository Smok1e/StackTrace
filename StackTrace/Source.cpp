#define TX_COMPILING

#include <TXLib.h>
#include <stack>
#include "DynamicStack.h"

struct Test 

{
	int value_;

};

int main ()

{

	SetWindowText (GetConsoleWindow (), "Dynamic stack test");
	size_t size = 10;

	DynamicStack <Test> stack;

	for (size_t i = 0; i < size; i++)
		stack.push ({(int) i});

	for (size_t i = 0; i < size; i++) 
		printf ("[%zu] = %d\n", i, stack.pop ().value_); 

	stack.print ();

}
