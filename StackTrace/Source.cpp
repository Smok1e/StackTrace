#define ALLOW_TRACE

#include <TXLib.h>

#ifdef ALLOW_TRACE

	#define TRACE(format_, ...) { $sC; printf ("[~^.^~] %s [line %d]: ", __TX_FUNCTION__, __LINE__); \
                                       printf (format_, __VA_ARGS__);                                \
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

};

Controlled operator+ (const Controlled & a, const Controlled & b);
Controlled operator- (const Controlled & a, const Controlled & b);
Controlled operator* (const Controlled & a, const Controlled & b);
Controlled operator/ (const Controlled & a, const Controlled & b);

int main ()

{

	CREATE_CONTROLLED (test1, __LINE__);
	CREATE_CONTROLLED (test2, __LINE__);
	CREATE_CONTROLLED (test3, __LINE__);

	Controlled result = test1 * (test2 - test1) * test3 - test2;

	printf ("Result: %d, expected %d\n", result.value_, test1.value_ * (test2.value_ - test1.value_) * test3.value_ - test2.value_);

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

	return {a.value_ - b.value_, "result"};

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