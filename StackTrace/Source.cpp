#define ALLOW_TRACE

#include <TXLib.h>
#include "Graph.h"

#define __CLEAR_FUNCNAME__ StrFilterFuncName (__TX_FUNCTION__).c_str ()

#ifdef ALLOW_TRACE

	#define TRACE(format_, ...) { $sC; printf ("[~^.^~] %s [line %d]: ", __CLEAR_FUNCNAME__, __LINE__); \
                                       printf (format_, __VA_ARGS__);                                   \
                                       printf ("\n"); } 

#else

	#define TRACE(msg_, ...) ;

#endif

#define CREATE_CONTROLLABLE(name__, value__) Controllable name__ (value__, #name__);

#define DUMP_OPERATION(c, color)                                                                                       \
                                                                                                                       \
	Controllable result (a.value_ c b.value_, "(tmp result #" + std::to_string (operation_counter) + ") =    "           \
		                                           + a.label_ + " (" + std::to_string (a.value_) + ") "#c" "           \
		                                           + b.label_ + " (" + std::to_string (b.value_) + ")      ");         \
                                                                                                                       \
	_Graph -> Add ("\""#c"#%d\" [shape = circle, color = " #color ", label = \""#c" %s\"];\n", operation_counter, ""); \
                                                                                                                       \
	_Graph -> Add ("\"%zu\" -> \""#c"#%zu\"[color = "#color"];\n", a.index_, operation_counter);                       \
	_Graph -> Add ("\"%zu\" -> \""#c"#%zu\"[color = "#color"];\n", b.index_, operation_counter);                       \
                                                                                                                       \
	_Graph -> Add ("\"" #c "#%zu\" -> \"%zu\"[color = "#color"];\n", operation_counter, result.index_);

Graph * _Graph = nullptr;

struct Controllable

{

	static size_t Counter;

	size_t index_;

	int value_;

	std::string label_;

	Controllable (int value = 0, std::string label = "Unnamed");

	Controllable (const Controllable & that);

	Controllable & operator= (const Controllable & that);

	void updateLabel (const std::string & label);

	void print ();

};

size_t Controllable::Counter = 0;

Controllable operator+ (const Controllable & a, const Controllable & b);
Controllable operator- (const Controllable & a, const Controllable & b);
Controllable operator* (const Controllable & a, const Controllable & b);
Controllable operator/ (const Controllable & a, const Controllable & b);

std::string StrReplaceInstances (std::string str, std::string from, std::string to);

std::string StrFilterFuncName (std::string name);

void ControllableTest (int n1, int n2, int n3);

int main ()

{

	CREATE_GRAPH (graph);

	_Graph = &graph;

	CREATE_CONTROLLABLE (c1, eu::rnd (0, 1000));
	CREATE_CONTROLLABLE (c2, eu::rnd (0, 1000));
	CREATE_CONTROLLABLE (c3, eu::rnd (0, 1000));

	c1 + c2 / c3;

	_Graph -> Render ();

}

Controllable::Controllable (int value, std::string label) :

	index_ (Counter),

	value_ (value),

	label_ (label)

{

	_Graph -> Add ("\"%zu\" [shape = record, label = \" { %s | { %d | index: %zu} } \"]\n", 
		            index_, label_.c_str (), value_, index_);

	TRACE ("Created Controllable '%s' with value %d, index: %zu", label_.c_str (), value, index_);

	Counter ++;

}

Controllable::Controllable (const Controllable & that) :

	index_ (Counter),

	value_ (that.value_)

{

	label_ = "Copied from '" + that.label_ + "'";

	_Graph -> Add ("\"%zu\" [shape = record, label = \" { %s | { %d | index: %zu} } \"]\n", 
		            index_, label_.c_str (), value_, index_);

	_Graph -> Add ("\"%zu\" -> \"%zu\" [label = \"%s\", color = darkviolet];\n", that.index_, index_, __TX_FUNCTION__);

	TRACE ("Created Controllable '%s' with value %d, index: %zu", label_.c_str (), value_, index_);

	Counter ++;

}

Controllable & Controllable::operator= (const Controllable & that)

{

	_Graph -> Add ("\"%zu\" -> \"%zu\" [label = \"%s\", color = mediumslateblue];\n", that.index_, index_, __TX_FUNCTION__);

	TRACE ("Assignment %d from '%s' to '%s'", that.value_, that.label_.c_str (), label_.c_str ());

	if (this == &that) return *this;

	value_ = that.value_;

	label_ += " (= " + that.label_ + ") (Copied from " + that.label_ + ")";

	return *this;

}

Controllable operator+ (const Controllable & a, const Controllable & b)

{

	static size_t operation_counter = 0;
	operation_counter ++;

	TRACE ("Adding %d '%s' to %d '%s'", a.value_, a.label_.c_str (), b.value_, b.label_.c_str ());

	DUMP_OPERATION (+, gold1);

	return result;

}

Controllable operator- (const Controllable & a, const Controllable & b)

{

	static size_t operation_counter = 0;
	operation_counter ++;

	TRACE ("Substracting %d '%s' from %d '%s'", b.value_, b.label_.c_str (), a.value_, a.label_.c_str ());

	DUMP_OPERATION (-, deepskyblue);

	return result;

}

Controllable operator* (const Controllable & a, const Controllable & b)

{

	static size_t operation_counter = 0;
	operation_counter ++;

	TRACE ("Multiplicating %d '%s' to %d '%s'", a.value_, a.label_.c_str (), b.value_, b.label_.c_str ());

	DUMP_OPERATION (*, darkorange2);

	return result;

}

Controllable operator/ (const Controllable & a, const Controllable & b)

{
	static size_t operation_counter = 0;
	operation_counter ++;

	TRACE ("Division %d '%s' by %d '%s'", a.value_, a.label_.c_str (), b.value_, b.label_.c_str ());

	DUMP_OPERATION (/, mediumblue);

	return result;

}

void Controllable::updateLabel (const std::string & label)

{

	label_ = label + ", last label: '" + label_ + "'";

	TRACE ("Label updated: new label '%s'", label_.c_str ());

}

void Controllable::print ()

{

	$sc;

	printf ("Controllable '%s' = %d\n", label_.c_str (), value_);

}

std::string StrReplaceInstances (std::string str, std::string from, std::string to)

{

	size_t found  = 0;

	while ((found = str.find (from)) != std::string::npos)

		str.replace (found, from.length (), to);

	return str;

}

std::string StrFilterFuncName (std::string name)

{

	return StrReplaceInstances (name, 
		                        "class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> >", 
		                        "std::srting");

}

void ControllableTest (int n1, int n2, int n3)

{

	printf ("Testing Controllable with %d, %d, %d...\n", n1, n2, n3);

	CREATE_CONTROLLABLE (c1,     n1);
	CREATE_CONTROLLABLE (c2,     n2);
	CREATE_CONTROLLABLE (c3,     n3);

	CREATE_CONTROLLABLE (result,  0);

	    result = c1 * (c2 + c1) - c3 / c2;

	int expect = n1 * (n2 + n1) - n3 / n2;

	printf ("Controllable test (%d * (%d + %d) - %d / %d): ", n1, n2, n1, n3, n2);

	if (result.value_ == expect) 
		
	{

		$sg; 
		
		printf ("OK\n");

	}

	else 
		
	{

		$sR; 
			
		printf ("Falied: Expected %d, got %d\n", expect, result.value_);

	}

}