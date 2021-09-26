#pragma once

#include "Core.h"
#include "UObject.h"
#include "Field.h"
#include "CoreNative.h"
#include "UnrealTypes.h"
#include "GC.h"
#include "Containers/Pair.h"
#include "FieldPath.h"

#include "Windows/WindowsCriticalSection.h"

namespace UE4
{
	class UField : public UObject
	{
	public:
		//DECLARE_CASTED_CLASS_INTRINSIC(UField, UObject, CLASS_Abstract, TEXT("/Script/CoreUObject"), CASTCLASS_UField)

		typedef UField BaseFieldClass;
		typedef UClass FieldTypeClass;

		/** Next Field in the linked list */
		UField *Next;
	};

	/*-----------------------------------------------------------------------------
	UStruct.
-----------------------------------------------------------------------------*/
class FStructBaseChain
{
public:
	FStructBaseChain** StructBaseChainArray;
	int32 NumStructBasesInChainMinusOne;

	friend class UStruct;
};
	/**
 * Base class for all UObject types that contain fields.
 */
	class UStruct : public UField, private FStructBaseChain
	{
	public:
		//	DECLARE_CASTED_CLASS_INTRINSIC(UStruct, UField, CLASS_MatchedSerializers, TEXT("/Script/CoreUObject"), CASTCLASS_UStruct)

		/** Struct this inherits from, may be null */
		UStruct *SuperStruct;
		/** Pointer to start of linked list of child fields */
		UField *Children;
		/** Pointer to start of linked list of child fields */
		FField *ChildProperties;

		/** Total size of all UProperties, the allocated structure may be larger due to alignment */
		int32 PropertiesSize;
		/** Alignment of structure in memory, structure will be at least this large */
		int32 MinAlignment;

		/** Script bytecode associated with this object */
		TArray<uint8> Script;

		/** In memory only: Linked list of properties from most-derived to base */
		FProperty *PropertyLink;
		/** In memory only: Linked list of object reference properties from most-derived to base */
		FProperty *RefLink;
		/** In memory only: Linked list of properties requiring destruction. Note this does not include things that will be destroyed byt he native destructor */
		FProperty *DestructorLink;
		/** In memory only: Linked list of properties requiring post constructor initialization */
		FProperty *PostConstructLink;

		/** Array of object references embedded in script code and referenced by FProperties. Mirrored for easy access by realtime garbage collection code */
		TArray<UObject *> ScriptAndPropertyObjectReferences;

		typedef TArray<TPair<TFieldPath<FField>, int32>> FUnresolvedScriptPropertiesArray;
		/** Contains a list of script properties that couldn't be resolved at load time */
		class FUnresolvedScriptPropertiesArray *UnresolvedScriptProperties;

		/** Cached schema for optimized unversioned property serialization, owned by this. */
		mutable const struct FUnversionedStructSchema *UnversionedSchema = nullptr;
	};

	struct FRepRecord
	{
		class FProperty *Property;
		int32 Index;
	};

	struct FNativeFunctionLookup
	{
		FName Name;
		FNativeFuncPtr Pointer;
	};

	struct FImplementedInterface
	{
		/** the interface class */
		class UClass *Class;
		/** the pointer offset of the interface's vtable */
		int32 PointerOffset;
		/** whether or not this interface has been implemented via K2 */
		bool bImplementedByK2;
	};

	class UClass : public UStruct
	{
		//DECLARE_CASTED_CLASS_INTRINSIC_NO_CTOR(UClass, UStruct, 0, TEXT("/Script/CoreUObject"), CASTCLASS_UClass, NO_API)
		//DECLARE_WITHIN_UPACKAGE()

	public:
		friend class FRestoreClassInfo;
		friend class FBlueprintEditorUtils;

		typedef void (*ClassConstructorType)(const class FObjectInitializer &);
		typedef UObject *(*ClassVTableHelperCtorCallerType)(class FVTableHelper &Helper);
		typedef void (*ClassAddReferencedObjectsType)(UObject *, class FReferenceCollector &);
		typedef UClass *(*StaticClassFunctionType)();

		ClassConstructorType ClassConstructor;
		ClassVTableHelperCtorCallerType ClassVTableHelperCtorCaller;
		/** Pointer to a static AddReferencedObjects method. */
		ClassAddReferencedObjectsType ClassAddReferencedObjects;

		/** Class pseudo-unique counter; used to accelerate unique instance name generation */
		mutable uint32 ClassUnique : 31;

		/** Used to check if the class was cooked or not */
		uint32 bCooked : 1;

		/** Class flags; See EClassFlags for more information */
		EClassFlags ClassFlags;

		/** Cast flags used to accelerate dynamic_cast<T*> on objects of this type for common T */
		EClassCastFlags ClassCastFlags;

		/** The required type for the outer of instances of this class */
		UClass *ClassWithin;

		/** This is the blueprint that caused the generation of this class, or null if it is a native compiled-in class */
		UObject *ClassGeneratedBy;

		/** Which Name.ini file to load Config variables out of */
		FName ClassConfigName;

		/** List of replication records */
		TArray<FRepRecord> ClassReps;

		/** List of network relevant fields (functions) */
		TArray<UField *> NetFields;

		/** Index of the first ClassRep that belongs to this class. Anything before that was defined by / belongs to parent classes. */
		int32 FirstOwnedClassRep = 0;

		/** The class default object; used for delta serialization and object initialization */
		UObject *ClassDefaultObject;

		void *SparseClassData;

		class UScriptStruct *SparseClassDataStruct;

		/** Map of all functions by name contained in this class */
		TMap<FName, class UFunction *> FuncMap;

		/** A cache of all functions by name that exist in a parent (superclass or interface) context */
		mutable TMap<FName, class UFunction *> SuperFuncMap;

		/** Scope lock to avoid the SuperFuncMap being read and written to simultaneously on multiple threads. */
		mutable FRWLock SuperFuncMapLock;

		/**
	 * The list of interfaces which this class implements, along with the pointer property that is located at the offset of the interface's vtable.
	 * If the interface class isn't native, the property will be null.
	 */
		TArray<FImplementedInterface> Interfaces;

		/** Reference token stream used by realtime garbage collector, finalized in AssembleReferenceTokenStream */
		FGCReferenceTokenStream ReferenceTokenStream;
		/** CS for the token stream. Token stream can assemble code can sometimes be called from two threads throuh a web of async loading calls. */
		FCriticalSection ReferenceTokenStreamCritical;

		/** This class's native functions. */
		TArray<FNativeFunctionLookup> NativeFunctionLookupTable;
	};

}
