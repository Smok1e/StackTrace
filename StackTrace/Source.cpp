#define ALLOW_TRACE

#include <TXLib.h>
#include "Graph.h"
#include <vector>
#include "Iterator.h"

//----------------------------------------------------------------

#define __CLEAR_FUNCNAME__ StrFilterFuncName (__TX_FUNCTION__).c_str ()

#ifdef ALLOW_TRACE

	#define TRACE(format_, ...) { $sC; printf ("[~^.^~] %s [line %d]: ", __CLEAR_FUNCNAME__, __LINE__); \
                                       printf (format_, __VA_ARGS__);                                   \
                                       printf ("\n"); }

#else

	#define TRACE(msg_, ...) ;

#endif

#define CREATE_CONTROLLABLE(name__, value__) Controllable name__ (value__, #name__);

#define OPERATION_HISTORY(c) "(tmp result #" + std::to_string (operation_counter) + ") = "    \
		                            + a.label_ + " (" + std::to_string (a.value_) + ") "#c" " \
		                            + b.label_ + " (" + std::to_string (b.value_) + ") "

#define INDEX(c__) StrIndex (c__).c_str ()

#define THIS_INDEX StrIndex (*this).c_str ()

#define DUMP_OPERATION(c, color, result)                                                                                  \
                                                                                                                          \
	_Graph -> Add ("\"" #c "#%zu\" [shape = circle, color = " #color ", label = \""#c" %s\"];\n", operation_counter, ""); \
                                                                                                                          \
	_Graph -> Add ("\"%s\" -> \"" #c "#%zu\"[color = " #color "];\n", INDEX (a), operation_counter);                      \
	_Graph -> Add ("\"%s\" -> \"" #c "#%zu\"[color = " #color "];\n", INDEX (b), operation_counter);                      \
                                                                                                                          \
	_Graph -> Add ("\"" #c "#%zu\" -> \"%s\"[color = " #color "];\n", operation_counter, INDEX (result));

//----------------------------------------------------------------

Graph * _Graph = nullptr;

//----------------------------------------------------------------

struct Controllable

{

	static size_t Counter;

	size_t index_counter_;

	size_t index_;

	int value_;

	std::string label_;

	Controllable (int value = 0, std::string label = "Unnamed");

	Controllable (const Controllable &  that);
	Controllable (const Controllable && that);

	~Controllable ();

	Controllable & operator= (const Controllable &  that);
	Controllable & operator= (const Controllable && that);

	void updateLabel (const std::string & label);

	void update_index ()

	{
	
		char index[20] = "";

		strncpy_s (index, THIS_INDEX, 20);

		index_counter_ ++;

		_Graph -> Add ("\"%s\" [shape = record, label = \" { %s | { %d | index: %s} } \"]\n",
		               THIS_INDEX, label_.c_str (), value_, THIS_INDEX);

		_Graph -> Add ("\"%s\" -> \"%s\"\n [label = \"%s\", color = lightslategray, style = dashed, arrowhead = none];", index, THIS_INDEX, __TX_FUNCTION__);

	}

	friend std::string StrIndex (const Controllable & that);

	void print ();

};

size_t Controllable::Counter = 0;

//----------------------------------------------------------------

Controllable   operator +  (const Controllable & a, const Controllable & b);
Controllable   operator -  (const Controllable & a, const Controllable & b);
Controllable   operator *  (const Controllable & a, const Controllable & b);
Controllable   operator /  (const Controllable & a, const Controllable & b);
Controllable & operator += (      Controllable & a, const Controllable & b);
Controllable & operator -= (      Controllable & a, const Controllable & b);

std::string StrReplaceInstances (std::string str, std::string from, std::string to);

std::string StrFilterFuncName (std::string name);

std::string StrIndex (const Controllable & c);

void ControllableTest (int n1, int n2, int n3);

template <typename Data>
Data && GetRvalue (Data & lvalue);

//----------------------------------------------------------------

#include "DynamicStack.h"

//----------------------------------------------------------------

int main ()

{

	CREATE_GRAPH (graph);

	_Graph = &graph;

	size_t size = 20;

	DynamicStack <Controllable> stack;

	for (size_t i = 0; i < size; i++)

	{
	
		stack.emplace_back (i, "test" + std::to_string (i));

	}

	_Graph -> Render ();

}

//----------------------------------------------------------------

Controllable::Controllable (int value, std::string label) :

	index_ (Counter),

	index_counter_ (0),

	value_ (value),

	label_ (label)

{
	
	_Graph -> Add ("\"%s\" [shape = record, label = \" { %s | { %d | index: %s | <ptr> %p } } \"]\n",
		           THIS_INDEX, label_.c_str (), value_, THIS_INDEX, this);

	TRACE ("Created Controllable '%s' with value %d, index: %s", label_.c_str (), value, THIS_INDEX);

	Counter ++;

}

//----------------------------------------------------------------

Controllable::Controllable (const Controllable & that) :

	index_ (Counter),

	index_counter_ (0),

	value_ (that.value_),

	label_ ("Copied from '" + that.label_ + "'")

{

	_Graph -> Add ("\"%s\" [shape = record, label = \" { %s | { %d | index: %s | <ptr> %p } } \"]\n",
		           THIS_INDEX, label_.c_str (), value_, THIS_INDEX, this);

	_Graph -> Add ("\"%s\" -> \"%s\" [label = \"%s\", color = darkviolet];\n", INDEX (that), THIS_INDEX, __TX_FUNCTION__);

	TRACE ("Created Controllable '%s' with value %d, index: %s", label_.c_str (), value_, THIS_INDEX);

	Counter ++;

}

//----------------------------------------------------------------

Controllable::Controllable (const Controllable && that) :

	index_ (Counter),

	index_counter_ (0),

	value_ (that.value_),

	label_ ("MOVED from '" + that.label_ + "'")

{

	_Graph -> Add ("\"%s\" [shape = record, label = \" { %s | { %d | index: %s | <ptr> %p } } \"]\n",
		           THIS_INDEX, label_.c_str (), value_, THIS_INDEX, this);

	_Graph -> Add ("\"%s\" -> \"%s\" [label = \"%s\", color = darkviolet];\n", INDEX (that), THIS_INDEX, __TX_FUNCTION__);

	TRACE ("Created Controllable '%s' with value %d, index: %s", label_.c_str (), value_, THIS_INDEX);

	Counter ++;

}

//----------------------------------------------------------------

Controllable::~Controllable ()

{

	_Graph -> Add ("\"%s\" [shape = record, label = \" { %s | { %d | index: %s | <ptr> %p } } \"]\n",
		           THIS_INDEX, label_.c_str (), value_, THIS_INDEX, this);

}

//----------------------------------------------------------------

Controllable & Controllable::operator= (const Controllable & that)

{

	TRACE ("Assignment %d from '%s' to '%s'", that.value_, that.label_.c_str (), label_.c_str ());

	if (this == &that) return *this;

	value_ = that.value_;

	label_ += " (= " + that.label_ + ") (Copied from " + that.label_ + ")";

	update_index ();

	_Graph -> Add ("\"%s\" -> \"%s\" [label = \"%s\", color = mediumslateblue];\n", INDEX (that), THIS_INDEX, __TX_FUNCTION__);

	return *this;

}

//----------------------------------------------------------------

Controllable & Controllable::operator= (const Controllable && that)

{

	TRACE ("MOVING %d from '%s' to '%s'", that.value_, that.label_.c_str (), label_.c_str ());

	if (this == &that) return *this;

	value_ = that.value_;

	label_ += " (= " + that.label_ + ") (Copied from " + that.label_ + ")";

	update_index ();

	_Graph -> Add ("\"%s\" -> \"%s\" [label = \"%s\", color = mediumslateblue];\n", INDEX (that), THIS_INDEX, __TX_FUNCTION__);

	return *this;

}

//----------------------------------------------------------------

Controllable operator+ (const Controllable & a, const Controllable & b)

{

	static size_t operation_counter = 0;
	operation_counter ++;

	TRACE ("Adding %d '%s' to %d '%s'", a.value_, a.label_.c_str (), b.value_, b.label_.c_str ());

	Controllable result (a.value_ + b.value_, OPERATION_HISTORY (+));

	DUMP_OPERATION (+, gold1, result);

	return result;

}

//----------------------------------------------------------------

Controllable operator- (const Controllable & a, const Controllable & b)

{

	static size_t operation_counter = 0;
	operation_counter ++;

	TRACE ("Substracting %d '%s' from %d '%s'", b.value_, b.label_.c_str (), a.value_, a.label_.c_str ());

	Controllable result (a.value_ - b.value_, OPERATION_HISTORY (-));

	DUMP_OPERATION (-, deepskyblue, result);

	return result;

}

//----------------------------------------------------------------

Controllable operator* (const Controllable & a, const Controllable & b)

{

	static size_t operation_counter = 0;
	operation_counter ++;

	TRACE ("Multiplicating %d '%s' to %d '%s'", a.value_, a.label_.c_str (), b.value_, b.label_.c_str ());

	Controllable result (a.value_ * b.value_, OPERATION_HISTORY (*));

	DUMP_OPERATION (*, darkorange2, result);

	return result;

}

//----------------------------------------------------------------

Controllable operator/ (const Controllable & a, const Controllable & b)

{
	static size_t operation_counter = 0;
	operation_counter ++;

	TRACE ("Division %d '%s' by %d '%s'", a.value_, a.label_.c_str (), b.value_, b.label_.c_str ());

	Controllable result (a.value_ / b.value_, OPERATION_HISTORY (/));

	DUMP_OPERATION (/, mediumblue, result);

	return result;

}

//----------------------------------------------------------------

Controllable & operator+= (Controllable & a, const Controllable & b)

{

	static size_t operation_counter = 0;
	operation_counter ++;

	TRACE ("operator += %d '%s' by %d '%s'", a.value_, a.label_.c_str (), b.value_, b.label_.c_str ());

	a.value_ = a.value_ + b.value_;

	a.update_index ();

	DUMP_OPERATION (+=, crimson, a);

	return a;

}

//----------------------------------------------------------------

Controllable & operator-= (Controllable & a, const Controllable & b)

{

	static size_t operation_counter = 0;
	operation_counter ++;

	TRACE ("operator -= %d '%s' by %d '%s'", a.value_, a.label_.c_str (), b.value_, b.label_.c_str ());

	a.value_ = a.value_ - b.value_;

	a.update_index ();

	DUMP_OPERATION (-=, powderblue, a);

	return a;

}

//----------------------------------------------------------------

void Controllable::updateLabel (const std::string & label)

{

	label_ = label + ", last label: '" + label_ + "'";

	TRACE ("Label updated: new label '%s'", label_.c_str ());

}

//----------------------------------------------------------------

void Controllable::print ()

{

	$sc;

	printf ("Controllable '%s' = %d\n", label_.c_str (), value_);

}

//----------------------------------------------------------------

std::string StrIndex (const Controllable & c)

{

	return std::to_string (c.index_) + "." + std::to_string (c.index_counter_);

}

//----------------------------------------------------------------

std::string StrReplaceInstances (std::string str, std::string from, std::string to)

{

	size_t found  = 0;

	while ((found = str.find (from)) != std::string::npos)

		str.replace (found, from.length (), to);

	return str;

}

//----------------------------------------------------------------

std::string StrFilterFuncName (std::string name)

{

	return StrReplaceInstances (name,
		                        "class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> >",
		                        "std::srting");

}

//----------------------------------------------------------------

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

//----------------------------------------------------------------

template <typename Data>
Data && GetRvalue (Data & lvalue)

{

	return (Data &&) lvalue;

}