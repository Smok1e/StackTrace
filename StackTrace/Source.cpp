#define ALLOW_TRACE

#include <TXLib.h>

#define __CLEAR_FUNCNAME__ StrFilterFuncName (__TX_FUNCTION__).c_str ()

#ifdef ALLOW_TRACE

	#define TRACE(format_, ...) { $sC; printf ("[~^.^~] %s [line %d]: ", __CLEAR_FUNCNAME__, __LINE__); \
                                       printf (format_, __VA_ARGS__);                                   \
                                       printf ("\n"); } 

#else

	#define TRACE(msg_, ...) ;

#endif

#define CREATE_CONTROLLED(name__, value__) Controlled name__ (value__, #name__);

struct Controlled

{

	int value_;

	std::string label_;

	Controlled (int value, const char * label);

	Controlled (const Controlled & that);

	Controlled & operator= (const Controlled & that);

	void updateLabel (const std::string & label);

};

Controlled operator+ (const Controlled & a, const Controlled & b);
Controlled operator- (const Controlled & a, const Controlled & b);
Controlled operator* (const Controlled & a, const Controlled & b);
Controlled operator/ (const Controlled & a, const Controlled & b);

std::string StrReplaceInstances (std::string str, std::string from, std::string to);

std::string StrFilterFuncName (std::string name);

void ControlledTest (int n1, int n2, int n3);

int main ()

{

	ControlledTest (14, 20, 98);

}

Controlled::Controlled (int value = 0, const char * label = "Unnamed") :

	value_ (value),

	label_ (label)

{

	TRACE ("Created controlled '%s' with value %d", label_.c_str (), value);

}

Controlled::Controlled (const Controlled & that) :

	value_ (that.value_)

{

	label_ = "Copied from '" + that.label_ + "'";

	TRACE ("Created controlled '%s' with value %d", label_.c_str (), value_);

}

Controlled & Controlled::operator= (const Controlled & that)

{

	TRACE ("Assignment %d from '%s' to '%s'", that.value_, that.label_.c_str (), label_.c_str ());

	if (this == &that) return *this;

	value_ = that.value_;

	return *this;

}

Controlled operator+ (const Controlled & a, const Controlled & b)

{

	TRACE ("Adding %d '%s' to %d '%s'", a.value_, a.label_.c_str (), b.value_, b.label_.c_str ());

	return {a.value_ + b.value_, "result"};

}

Controlled operator- (const Controlled & a, const Controlled & b)

{

	TRACE ("Substracting %d '%s' from %d '%s'", b.value_, b.label_.c_str (), a.value_, a.label_.c_str ());

	return {a.value_ + b.value_, "result"};

}

Controlled operator* (const Controlled & a, const Controlled & b)

{

	TRACE ("Multiplicating %d '%s' to %d '%s'", a.value_, a.label_.c_str (), b.value_, b.label_.c_str ());

	return {a.value_ * b.value_, "result"};

}

Controlled operator/ (const Controlled & a, const Controlled & b)

{

	TRACE ("Division %d '%s' by %d '%s'", a.value_, a.label_.c_str (), b.value_, b.label_.c_str ());

	return {a.value_ / b.value_, "result"};

}

void Controlled::updateLabel (const std::string & label)

{

	label_ = label + ", last label: '" + label_ + "'";

	TRACE ("Label updated: new label '%s'", label_.c_str ());

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

void ControlledTest (int n1, int n2, int n3)

{

	printf ("Testing Controlled with %d, %d, %d...\n", n1, n2, n3);

	CREATE_CONTROLLED (c1,     n1);
	CREATE_CONTROLLED (c2,     n2);
	CREATE_CONTROLLED (c3,     n3);

	CREATE_CONTROLLED (result,  0);

	    result = c1 * (c2 + c1) - c3 / c2;

	int expect = n1 * (n2 + n1) - n3 / n2;

	printf ("Controlled test (%d * (%d + %d) - %d / %d): ", n1, n2, n1, n3, n2);

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