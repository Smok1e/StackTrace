#pragma once

template <typename Data>
class Iterator 

{

public :

	Iterator (Data * ptr);

	//Data & operator* ();

	operator Data * ();

protected :

	Data * pointer;

};

template <typename Data>
Iterator <Data>::Iterator (Data * ptr) :

	pointer (ptr)

{}

/*template <typename Data>
Data & Iterator <Data>::operator* () 

{

	return *pointer;

}*/

template <typename Data>
Iterator <Data>::operator Data * ()

{

	return pointer;

}