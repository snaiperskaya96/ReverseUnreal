#pragma once

#include "Core.h"
#include "ObjectMacros.h"
#include "HAL/ThreadSafeCounter.h"

namespace UE4
{
	class FFieldVariant;

	class FFieldClass
	{
	public:
		/** Name of this field class */
		FName Name;
		/** Unique Id of this field class (for casting) */
		uint64 Id;
		/** Cast flags used for casting to other classes */
		uint64 CastFlags;
		/** Class flags */
		EClassFlags ClassFlags;
		/** Super of this class */
		FFieldClass *SuperClass;
		/** Default instance of this class */
		class FField *DefaultObject;
		/** Pointer to a function that can construct an instance of this class */
		class FField *(*ConstructFn)(const class FFieldVariant &, const FName &, EObjectFlags);
		/** Counter for generating runtime unique names */
		FThreadSafeCounter UnqiueNameIndexCounter;
	};

	class FFieldVariant
	{
		union FFieldObjectUnion
		{
			class FField *Field;
			class UObject *Object;
		} Container;

		bool bIsUObject;
	};

	class FField
	{
	public:
		void *Vfptr = nullptr;
		/** Pointer to the class object representing the type of this FField */
		FFieldClass *ClassPrivate;

		typedef FField Super;
		typedef FField ThisClass;
		typedef FField BaseFieldClass;
		typedef FFieldClass FieldTypeClass;

		/** Owner of this field */
		FFieldVariant Owner;

		/** Next Field in the linked list */
		FField *Next;

		/** Name of this field */
		FName NamePrivate;

		/** Object flags */
		EObjectFlags FlagsPrivate;
	};

}
