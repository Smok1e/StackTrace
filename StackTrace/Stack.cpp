#define TX_COMPILED

template <typename T>
Stack <T>::Stack () :

	bound1_ (CanaryValue),

	stack_ (),

	bound2_ (CanaryValue),

	size_ (0),

	hash_ (0)

{

	for (size_t n = 0; n < Max; n++) stack_[n] = FreeValue;

	UpdateHash (); 

	stack_[30] = 123;

}

template <typename T>
void Stack <T>::push (T value)

{

	CHECK

	stack_ [size_] = value;

	size_ ++;

	UpdateHash ();

	CHECK

}

template <typename T>
T Stack <T>::pop ()

{

	CHECK

	T value = stack_[size_];

	size_ --;

	UpdateHash ();

	CHECK

	return value;

}

template <typename T>
T Stack <T>::top ()

{

	CHECK

	return stack_[size_];

}

template <typename T>
int Stack <T>::OK ()

{

	if (size_ >= Max)	            return StackErrorSize;

	if (bound1_ != CanaryValue)     return StackErrorBound1Broken;
	if (bound2_ != CanaryValue)     return StackErrorBound2Broken;

	long long hash = hash_;

	UpdateHash ();

	if (hash != hash_)              return StackErrorWrongHash;

	for (size_t n = size_; n < Max; n++) 

		if (stack_[n] != FreeValue) return StackErrorValuePositionViolation;

	return -1;

}

long long HashOf (const void * ptr, size_t size)

{

	long long hash = 0;

	for (size_t i = 0; i < size; i++)

	{
	
		hash += *((char *)ptr + i);

	}

	return hash;

}

const char * strStackError (int err)

{

#define STR_ERROR(err, descr) case err: return #err ": " descr;

	switch (err)

	{
	
		STR_ERROR (StackErrorSize,                   "Size error");
		STR_ERROR (StackErrorBound1Broken,           "Left canarry broken");
		STR_ERROR (StackErrorBound2Broken,           "Right cannary broken");
		STR_ERROR (StackErrorWrongHash,              "Wrong hash");
		STR_ERROR (StackErrorValuePositionViolation, "Value position violation");

	}

	return "Unknown error";

}