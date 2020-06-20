#pragma once

const long CanaryValue = 0xFEE1DEAD;
const long FreeValue   = 0xFEEDFACE;
const long Defecated   = 0xDEFECE8D;

enum StackError

{

	StackErrorUnknown,
	StackErrorSize,
	StackErrorBound1Broken,
	StackErrorBound2Broken,
	StackErrorWrongHash,
	StackErrorValuePositionViolation,
	StackErrorDestructed,
	StackErrorDynamicBound1Broken,
	StackErrorDynamicBound2Broken,

};

#ifndef NDEBUG

	#define CHECK 	          	                               	                                                    \
	{                                                                                                               \
                                                                                                                    \
		/*printf ("%s [%d]:\n", __TX_FUNCTION__, __LINE__);*/                                                       \
																													\
		int res_ = OK ();	                               	                                                        \
	                              																			        \
		if (res_ >= 0)	                               	                               	                            \
		{	                               	                               	                                        \
			char message_ [512] = "";	                               	                                            \
			sprintf_s (message_, "Check failed:\n"                                                                  \
								 "File '%s'\n"                                                                      \
								 "Function '%s'\n"                                                                  \
								 "Line %d:\n"                                                                       \
			                     "Error 0x%06X: %s\n", __FILE__, __TX_FUNCTION__, __LINE__, res_, strDynamicStackError (res_)); \
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

const char * strDynamicStackError (int err);

template <typename DataType>

class DynamicStack

{

public :

	DynamicStack (size_t size = 0);

	~DynamicStack ();

	void resize (size_t size);

	void push (DataType value);

	DataType pop ();

	DataType top ();

	size_t getSize () { CHECK return size_; }

	size_t getLength () { CHECK return length_; }

	void UpdateHash () 
	
	{ 
		
		long long hash = 0; 
		
		std::swap (hash, hash_); 
		
		hash = HashOf (this, sizeof (DynamicStack)); 
	
		std::swap (hash, hash_);

	}

	int OK ();

	void print ();

	template <typename unit>
	friend void push_pop_test (unit value, size_t n); 

private :

	long bound1_;

	long long hash_;

	char * data_;

	size_t size_;

	size_t length_;

	long bound2_;

	long destructed_;

	void     set (size_t index, DataType value);

	DataType get (size_t index);

};

#include "DynamicStack.cpp"