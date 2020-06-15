#define TX_COMPILING

#include <TXLib.h>
#include <stack>
#include "DynamicStack.h"

//#include "Stack.h"

struct Location

{

	const char * funcName_;

	const char * file_;

	int line_;

	const Location * prev_;
	const Location * next_;

	Location () : funcName_ (""), file_ (""), line_ (0), prev_ (nullptr), next_ (nullptr) {}

	Location (const char * funcName, const char * file, int line) : funcName_ (funcName), file_ (file), line_ (line), prev_ (nullptr), next_ (nullptr) {}

};

struct FuncEntry

{

	static Location * current_;
	static Location * top_;

	Location cur;

	Location prev;

	FuncEntry (const char * funcName, const char * file, int line)

	{

		cur = {funcName, file, line};

		current_ -> next_ = &cur;

		prev = *current_;

		cur.prev_ = &prev;

		current_ = &cur;

	}

	~FuncEntry ()
	
	{
	
		current_ = &prev;

	}

};

Location top_loc = {"[Top level]", __FILE__, __LINE__};

Location * FuncEntry::current_ = &top_loc;
Location * FuncEntry::top_     = &top_loc; 

#define ENTRY FuncEntry _entry = {__TX_FUNCTION__, __FILE__, __LINE__}

/*struct testStruct

{

	char arr1 [1];

	Stack <int> stack_;

	char arr2 [1];

	testStruct ():

		arr1 (),

		stack_ (),

		arr2 ()

	{
	
		//arr1 [sizeof (long) + 15] = 0;

		stack_.push (1);

	}

};*/

struct MyType

{

	static size_t Counter;

	long value_;

	MyType () :

		value_ ((unsigned long) this)

	{

		$sy

		Counter ++;
	
		printf ("constructing mytype #%zu, this = %p, value = %X\n", Counter, this, value_);

	}

	MyType (int value) :

		value_ (value)

	{
	
		$sg

		Counter ++;

		printf ("constructing mytype #%zu, this = %p, value = %X\n", Counter, this, value_);

	}

	~MyType ()

	{
	
		$sr

		Counter --;

		printf (" destructing mytype #%zu, this = %p, value = %X\n", Counter, this, value_);

	}

	operator long ()

	{
	
		return value_;

	}

};

size_t MyType::Counter = 0;

void printStack ();

void foo1 ();

void foo2 ();

void foo3 ();

bool stack_push_test ();

bool stack_pop_test ();

int main ()

{

	ENTRY;

	SetWindowText (GetConsoleWindow (), "Dynamic stack test");

	/*DynamicStack <int> stack (20);

	for (int n = 0; n < 15; n++) stack.push (n);

	stack.print ();

	printf ("\n");

	size_t length = stack.getLength ();

	for (int n = 0; n < length; n++) printf ("%d: %d\n", n, stack.pop ());

	printf ("\n");

	stack.print ();

	printf ("\n");*/

    
    /*push_pop_test <int>  (123, 10);
	push_pop_test <int>  (0,    0);

	push_pop_test <char> (123,  '1');
	push_pop_test <char> (0,   '\0');*/

	DynamicStack <MyType> stack (10);

}

void foo1 ()

{

	ENTRY;

	foo2 ();

}

void foo2 ()

{

	ENTRY;

	foo3 ();

}

void foo3 ()

{

	ENTRY;

	printStack ();

}

/*bool stack_push_test ()

{

	ENTRY;

	printf ("Stack push test:\n");

	Stack <int> test;

	for (int i = 0; i < 10; i++) test.push (i);

	for (int i = 0; i < 10; i++) 
		
		if (test.get (i) != i)

		{

			printf ("Failed: Expected %d, got %d. i = %d.", i, test.get (i), i);

			return false;

		}

	printf ("Ok\n");

	return true;

}
*/
void printLoc (const Location * loc, int n)

{

	if (!loc) return;

	printLoc (loc -> prev_, n - 1);

	printf ("%02d: %*s%s [line %d]\n", n, n * 2, "", loc -> funcName_, loc -> line_);

}

void printStack ()

{

	size_t i = 0;

	for (const Location * loc = FuncEntry::top_; loc; loc = loc -> next_, i++)
		printf ("%zu: %*s%s [line %d]:\n", i, i * 2, "", loc -> funcName_, loc -> line_);

	/*std::stack <const Location*> stack;

	for (const Location * loc = &FuncEntry::current_; loc; loc = loc -> prev_)

		stack.push (loc);

	for (size_t i = 0; !stack.empty (); i++)

	{

		printf ("%d: %*s%s [line %d]:\n", i, i * 2, "", stack.top () -> funcName_, stack.top () -> line_);

		stack.pop ();

	}*/

	//printLoc (&FuncEntry::current_, 0);

}