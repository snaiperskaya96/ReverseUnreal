#pragma once

#include "Core.h"
#include "WeakObjPtr.h"

namespace UE4
{
	class UStruct;
	class UField;
	class FLinkerLoad;

	struct FFieldPath
	{
		// TWeakFieldPtr needs access to ClearCachedField
		template <class T>
		friend struct TWeakFieldPtr;

		// FFieldPathProperty needs access to ConvertFromFullPath
		friend class FFieldPathProperty;

	protected:
		/* Determines the behavior when resolving stored path */
		enum EPathResolveType
		{
			UseStructIfOuterNotFound = 0,
			UseStructAlways = 1
		};

		/** Untracked pointer to the resolved property */
		mutable FField *ResolvedField = nullptr;
		/** The cached owner of this field. Even though implemented as a weak pointer, GC will keep a strong reference to it if exposed through UPROPERTY macro */
		mutable TWeakObjectPtr<UStruct> ResolvedOwner;

		/** Path to the FField object from the innermost FField to the outermost UObject (UPackage) */
		TArray<FName> Path;
	};

	template <class PropertyType>
	struct TFieldPath : public FFieldPath
	{
	private:
		// These exists only to disambiguate the two constructors below
		enum EDummy1
		{
			Dummy1
		};

	public:
	};

	// Helper function which deduces the type of the initializer

	//template<class T> struct TIsPODType<TFieldPath<T> > { enum { Value = true }; };
	//template<class T> struct TIsZeroConstructType<TFieldPath<T> > { enum { Value = true }; };
	//template<class T> struct TIsWeakPointerType<TFieldPath<T> > { enum { Value = true }; };

}