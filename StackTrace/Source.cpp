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

	DynamicStack <Test> stack (size);

	for (size_t i = 0; i < size; i++)
		stack.push ({ (int) random (0, 1000) });

	for (size_t i = 0; i < size; i++)
		printf ("[%zu] = %d\n", i, stack.pop ().value_); 

	push_pop_test <int> (123, 10);
	push_pop_test <int> (123,  0);

}
