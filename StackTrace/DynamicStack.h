#pragma once

#include <stdio.h>

const long CanaryValue = 0xFEE1DEAD;
const char FreeValue   = 0xCC;
const char Defecated   = 0xDE;

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
	StackErrorWrongLength

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

#ifdef ALLOW_TRACE

	#define TRACE(format_, ...) { $sy; printf ("[=^.^=] %s [line %d]: ", __TX_FUNCTION__, __LINE__); \
                                       printf (format_, __VA_ARGS__);                                \
                                       printf ("\n"); } 

#else

	#define TRACE(msg_, ...) ;

#endif


long long HashOf (const void * ptr, size_t size);

const char * strDynamicStackError (int err);

template <typename DataType>
struct DataShell

{

	char data_[sizeof (DataType)];

	DataShell () :

		data_ ()

	{}

	void set (DataType value)

	{
	
		assert (data_);

		* (DataType*) (data_) = value;

	}

	DataType get ()

	{

		assert (data_);
	
		return * (DataType*) (data_);

	}

};

template <typename DataType>

class DynamicStack

{

public :

	DynamicStack (size_t size = 0, const char * name = "unnamed");

	~DynamicStack ();

	void resize (size_t size);

	void push (DataType value);

	DataType pop ();

	inline DataType top ();

	inline size_t getSize   () { CHECK return size_; }

	inline size_t getLength () { CHECK return length_; }

	inline size_t getBytes  () {       return size_ * sizeof (DataType) + 2 * sizeof (CanaryValue); }

	void print ();

	void printBytes ();

	template <typename unit>
	friend void push_pop_test (unit value, size_t n); 

private :

	long bound1_;

	long long hash_;

	char * data_;

	size_t size_;

	size_t length_;

	const char * label_;

	long bound2_;

	long destructed_;

	inline void     set (size_t index, DataType value);
	inline DataType get (size_t index);

	inline DataType * getData (char * data = nullptr, size_t index = 0);

	inline void   Free (size_t index, char * data = nullptr);
	inline bool isFree (size_t index, char * data = nullptr);

	void UpdateHash () 
	
	{ 
		
		long long hash = 0; 
		
		std::swap (hash, hash_); 
		
		hash = HashOf (this, sizeof (DynamicStack)); 
	
		std::swap (hash, hash_);

	}

	int OK ();

};

#include "DynamicStack.cpp"