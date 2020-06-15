#pragma once

const long CanaryValue = 0xFEE1DEAD;
const long FreeValue   = 0xFEEDFACE;

enum StackError

{

	StackErrorSize,
	StackErrorBound1Broken,
	StackErrorBound2Broken,
	StackErrorWrongHash,
	StackErrorValuePositionViolation

};

#ifndef NDEBUG

	#define CHECK 	          	                               	                                                    \
	{	                               	                                                                            \
		int res_ = OK ();	                               	                                                        \
	                              																			        \
		if (res_ >= 0)	                               	                               	                            \
		{	                               	                               	                                        \
			char message_ [512] = "";	                               	                                            \
			sprintf_s (message_, "Check failed:\n"                                                                  \
								 "File '%s'\n"                                                                      \
								 "Function '%s'\n"                                                                  \
								 "Line %d:\n"                                                                       \
			                     "Error 0x%06X: %s\n", __FILE__, __TX_FUNCTION__, __LINE__, res_, strStackError (res_)); \
			printf (message_);	                              	                                                    \
			MessageBox (NULL, message_, "Error", MB_OK | MB_ICONERROR);                               	            \
	                               																			        \
			abort (); 	                               													            \
		}	                               	                                                                        \
	} 	      

#else

	#define CHECK ;

#endif

long long HashOf (const void * ptr, size_t size);

const char * strStackError (int err);

template <typename T>

class Stack

{

public :

	static const size_t Max = 64;

	Stack ();

	void push (T value);

	T pop ();

	T top ();

	T get (size_t index) { CHECK assert (index < Max); return stack_[index]; }

	size_t getSize () { CHECK return size_; }

	size_t getMax  () { CHECK return Max;   }

	void UpdateHash () 
	
	{ 
		
		long long hash = 0; 
		
		std::swap (hash, hash_); 
		
		hash = HashOf (this, sizeof (Stack)); 
	
		std::swap (hash, hash_);

	}

	int OK ();

private :

	long bound1_;

	long long hash_;

	T stack_ [Max];

	size_t size_;

	long bound2_;

};

#include "Stack.cpp"