#pragma once

template<class KeyType, class ValueType>
class TTuple
{
public:
	KeyType Key;
	ValueType Value;
};

template <typename KeyType, typename ValueType>
using TPair = TTuple<KeyType, ValueType>;
