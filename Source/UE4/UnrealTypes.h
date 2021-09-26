#pragma once

#include "Field.h"
#include "Primitives.h"
#include "NetTypes.h"
#include "EnumAsByte.h"

namespace UE4
{
	class FProperty : public FField
	{
	public:
		// Persistent variables.
		int32 ArrayDim;
		int32 ElementSize;
		EPropertyFlags PropertyFlags;
		uint16 RepIndex;

		TEnumAsByte<ELifetimeCondition> BlueprintReplicationCondition;

		// In memory variables (generated during Link()).
		int32 Offset_Internal;

		FName RepNotifyFunc;

		/** In memory only: Linked list of properties from most-derived to base **/
		FProperty *PropertyLinkNext;
		/** In memory only: Linked list of object reference properties from most-derived to base **/
		FProperty *NextRef;
		/** In memory only: Linked list of properties requiring destruction. Note this does not include things that will be destroyed byt he native destructor **/
		FProperty *DestructorLinkNext;
		/** In memory only: Linked list of properties requiring post constructor initialization.**/
		FProperty *PostConstructLinkNext;
	};

	class FBoolProperty : public FProperty
	{
	public:
		/** Size of the bitfield/bool property. Equal to ElementSize but used to check if the property has been properly initialized (0-8, where 0 means uninitialized). */
		uint8 FieldSize;
		/** Offset from the memeber variable to the byte of the property (0-7). */
		uint8 ByteOffset;
		/** Mask of the byte with the property value. */
		uint8 ByteMask;
		/** Mask of the field with the property value. Either equal to ByteMask or 255 in case of 'bool' type. */
		uint8 FieldMask;
	};
}