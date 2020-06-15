#define TX_COMPILED

template <typename T>
DynamicStack <T>::DynamicStack (size_t size /* = 0 */) :

	bound1_ (CanaryValue),

	stack_ (nullptr),

	bound2_ (CanaryValue),

	size_ (0),

	length_ (0),

	hash_ (0),

	destructed_ (0)

{

	$do (stack_ = new T [2] );

	//$do (stack_ [0] = CanaryValue);
	//$do (stack_ [1] = CanaryValue);

	$do (UpdateHash ());

	$do (resize (size));

}

template <typename T>
DynamicStack <T>::~DynamicStack ()

{

	CHECK;

	$do (for (size_t n = 0; n < size_; n++)

	{

		//$do (stack_[n] = Defecated);
	
	})

	$do (delete[] (stack_));

	$do (stack_ = nullptr);

	$do (destructed_ = Defecated);

}

//    {                                                                        //---/\---/\-------Это ASCII KOT!--//
//$1  va_list arg; va_start (arg, msg);                                        //  {  '-'  }                      //
//$$  const char* what = _txProcessError (file, line, func, color, msg, arg);  //  {  0 0  }     Добавь его себе  //
//    va_end (arg);                                                            //  --> V <--  в исходник, и тебе  //
//                                                                             //   \ \|/ /      будет, наверно,  //
//    if (!(msg && msg[0] == '\a')) return what;                               //    \___/  приятно отлаживаться  //
//                                                                            //---------------долгими ночами:)--//

template <typename T>
void DynamicStack <T>::resize (size_t size)

{

	CHECK;

	printf ("Resizing from %zu to %zu.\n", size_, size);

	$do (T * new_stack = new T [size + 2]);

	//$do (new_stack [0       ] = CanaryValue );
	//$do (new_stack [size + 1] = CanaryValue );

	$do (for (size_t i = 1; i <= size; i++)

	{

		//$do (new_stack[i] = (i <= size_)? stack_ [i] : T (FreeValue));

	})

	$do (delete[] (stack_) );
	$do (stack_ = new_stack );

	$do (size_ = size );

	$do (if (length_ > size_) length_ = size_ );

	$do (UpdateHash ());

	CHECK;

}

template <typename T>
void DynamicStack <T>::push (T value)

{

	CHECK

	if (length_ >= size_) resize ((size_ + 1) * 2);

	length_ ++;

	stack_ [length_] = value;

	UpdateHash ();

	CHECK

}

template <typename T>
T DynamicStack <T>::pop ()

{

	CHECK

	T value = top ();

	stack_ [length_] = FreeValue;

	length_ --;

	UpdateHash ();

	CHECK

	return value;

}

template <typename T>
T DynamicStack <T>::top ()

{

	CHECK

	return stack_ [length_];

}

template <typename T>
int DynamicStack <T>::OK ()

{

	return -1;

	if (length_ > size_)                     return StackErrorSize;

	if (bound1_ != CanaryValue)              return StackErrorBound1Broken;
	if (bound2_ != CanaryValue)              return StackErrorBound2Broken;

	long long hash = hash_;

	UpdateHash ();

	if (hash != hash_)                       return StackErrorWrongHash;

	for (size_t n = length_ + 1; n < size_; n++) 

		if (get () [n] != FreeValue)         return StackErrorValuePositionViolation;

	if (destructed_ == Defecated)            return StackErrorDestructed;
	
	if (stack_[0] != CanaryValue)            return StackErrorDynamicBound1Broken;

	if (stack_[size_ + 1] != CanaryValue)    return StackErrorDynamicBound1Broken;

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

template <typename T>
void DynamicStack <T>::print ()

{

	CHECK;

	$sc

	printf ("-=Stack=-\n"
		    "size   = %zu,\n"
	        "length = %zu\n"
	        "contains:\n", size_, length_);

	for (size_t i = 0; i < size_ + 2; i++)

	{

		printf ("[%zu] = ", i);
	
		if      (stack_[i] == CanaryValue) printf ("[Canary]\n");
		else if (stack_[i] ==   FreeValue) printf ("[Free]\n");
		else if (stack_[i] ==   Defecated) printf ("[Defecated!]\n");

		else txPrintf ("%a\n", stack_[i]);

	}


	printf ("-=Stack=-\n");

}

const char * strDynamicStackError (int err)

{

	#define STR_ERROR(err, descr) case err: return #err ": " descr;

	switch (err)

	{
	
		STR_ERROR (StackErrorSize,                   "Size error");
		STR_ERROR (StackErrorBound1Broken,           "Left canarry broken");
		STR_ERROR (StackErrorBound2Broken,           "Right cannary broken");
		STR_ERROR (StackErrorWrongHash,              "Wrong hash");
		STR_ERROR (StackErrorValuePositionViolation, "Value position violation");
		STR_ERROR (StackErrorDestructed,             "Stack is already destructed");
		STR_ERROR (StackErrorDynamicBound1Broken,    "Left iternal stack cannary broken");
		STR_ERROR (StackErrorDynamicBound2Broken,    "Right iternal stack cannary broken");

	}

	return "Unknown error";

}

template <typename unit>
void push_pop_test (unit value, size_t n)

{

	bool ok = true;

	printf ("Dynamic stack test (%zu, %zu): ", value, n);

	DynamicStack <unit> stack (n);

	for (size_t i = 0; i < n; i++)

		stack.push (n);

	if (stack.stack_ [0]               != CanaryValue) ok = false;
	if (stack.stack_ [stack.size_ + 1] != CanaryValue) ok = false; 

	for (size_t i = 0; i < n; i++)

		if (stack.pop () != n) ok = false;

	for (size_t i = 0; i < n; i++)

		if (stack.stack_ [i + 1] != FreeValue) ok = false;

	if (ok) { $sG printf ("Ok\n"); }

	else   
		
	{

		$sR

		printf ("Failed: Expected\n");

		printf ("{"
				"\n[0] = [Canary]\n");

		for (size_t i = 0; i < n; i++) printf ("[%zu] = [Free]\n", i + 1);

		printf ("[%zu] = [Canary]\n"
			    "},\n"
			    "Got:\n" 
			    "{\n", n + 1);

		for (size_t i = 0; i < stack.size_ + 2; i++)

		{

			printf ("[%zu] = ", i);
	
			if      (stack.stack_[i] == CanaryValue) printf ("[Canary]\n");
			else if (stack.stack_[i] ==   FreeValue) printf ("[Free]\n");
			else if (stack.stack_[i] ==   Defecated) printf ("[Defecated!]\n");

			else txPrintf ("%a\n", stack.stack_[i]);

		}

		printf ("}.\n");

	}

}