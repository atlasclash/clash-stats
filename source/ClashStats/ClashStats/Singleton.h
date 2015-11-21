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

// PURPOSE : Use this template container class to make any class into a
//               singleton.  Normal usage:
//	
//				 <CODE>
//               class MyClass
//               {
//               public:
//                 // normal class stuff, but don't put ctor/dtor here.
//                 void InitClass();
//                 void ShutdownClass();
//                 int  GetData()         { return _someData; }
//               protected:
//                 // Make the ctor(s)/dtor protected, so this can only be
//                 // instantiated as a singleton.  Note: singleton will still
//                 // work on classes that do not follow this (public ctors)
//                 // but violation of the singleton is possible then, since non-
//                 // singleton versions of the class can be instantiated.
//                 MyClass() : _someData(5) {}
//                 MyClass(int arg) : _someData(arg) {} // etc...
//                 // don't implement the copy constructor, because singletons
//                 // shouldn't be copied!
//                 MyClass(const MyClass& mc) {}
//                 ~MyClass() {}
//               private:
//                 int _someData;
//               };
//
//               // now create a singleton of MyClass
//               typedef atSingleton<MyClass> MyClassSingleton;
//				</CODE>
//
//              Later, in your program code, you can instantiate the
//              singleton and access its members like so:
//
//				<CODE>
//              void somefunc()
//              {
//                // instantiate the MyClassSingleton
//                MyClassSingleton::Instantiate();
//                // could also call MyClassSingleton::Instantiate(10);
//                // since we have a constructor of that form in MyClass.
//
//                // access the methods in MyClass:
//                int data1 = MyClassSingleton::InstancePtr()->GetData();
//                // or...
//                int data2 = MyClassSingleton::InstanceRef().GetData();
//
//                // now destroy the singleton
//                MyClassSingleton::Destroy();
//             }
//				</CODE>
//
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
	
	//------------------------------------------------------------------------------
	// Function    : ResourceInstantiate
	// Description : 
	//------------------------------------------------------------------------------
	//
	// PURPOSE
	//	This is used during resource loading to set the pointer to
	//	an already created object. Can only be used once (unless
	//	the instance is destroyed by calling Destroy()).
	// PARAMS
	//	inst - the instance that is used as the singleton instance
	//
	static void ResourceInstantiate(_Type& inst)
	{
		assert(!sm_Instance);
		sm_Instance = &static_cast<atSingleton<_Type>&>(inst);
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

#endif //ATL_SINGLETON_H
