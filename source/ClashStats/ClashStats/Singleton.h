/*
 *  Singleton.h
 *
 *  Created by Atlas on 10/3/10.
 *  Copyright 2010 JTJ. All rights reserved.
 *
 */

#ifndef _SINGLETON_H_
#define _SINGLETON_H_

#include "architecture.h"
#include <assert.h>

template <typename _Type>
class atSingleton : protected _Type
{
public:
	//
	// PURPOSE
	//	creates the singleton instance for class _Type.
	//  Assures (by assertion) that the instance will only be created
	//  once.  This works for default constructors
	//
	static void Instantiate()
	{
		assert(!sm_Instance);
		sm_Instance = new atSingleton();
	}
	
	// PURPOSE: Deletes the singleton instance
	static void Destroy()           { delete sm_Instance; sm_Instance = NULL;  }
	
	// PURPOSE: returns a pointer/reference to the singleton instance.
	// Access members of the instance as you would members of any
	// object.
	// RETURNS: a pointer to the singleton
	static _Type* InstancePtr()         { assert(sm_Instance); return  sm_Instance; }
	
	// PURPOSE: returns a pointer/reference to the singleton instance.
	// Access members of the instance as you would members of any
	// object.
	// RETURNS: a reference to the singleton
	static _Type& InstanceRef()         { assert(sm_Instance); return *sm_Instance; }
	
	// PURPOSE: returns true if an instance of this singleton exists
	// RETURNS: returns true if an instance of this singleton exists
	static bool IsInstantiated()	{ return (sm_Instance!=NULL); }
	
	// PURPOSE: returns a pointer/reference to the singleton instance.
	// Access members of the instance as you would members of any
	// object.
	// RETURNS: a reference to the singleton
	static _Type& GetInstance()         { return InstanceRef(); }
		
protected:
	// although the instance is of type atSingleton<_Type>, the Instance***() funcs 
	// above implicitly cast it to type _Type.
	static atSingleton* sm_Instance;
	
private:
	//------------------------------------------------------------------------------
	// Function    : atSingleton (default constructor)
	// Description : This is hidden, so that the singleton can only be instantiated
	//               via the public static Instantiate function.
	//------------------------------------------------------------------------------
	atSingleton() : _Type() {}
	
	//------------------------------------------------------------------------------
	// Function    : atSingleton (copy constructor)
	// Description : Hidden, because you can't copy a singleton.
	//------------------------------------------------------------------------------
	atSingleton(const atSingleton&) {}
	
	//------------------------------------------------------------------------------
	// Function    : ~atSingleton
	// Description : Hidden: destroy via the public static Destroy() method.
	//------------------------------------------------------------------------------
	~atSingleton()                  {}
};

// declare the static instance pointer
template<typename _Type> atSingleton<_Type>* atSingleton<_Type>::sm_Instance = NULL;

#endif //_SINGLETON_H_
