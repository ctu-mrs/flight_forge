#pragma once

#include <string>

#include "CoreMinimal.h"
#include "MessageSerialization/Public/FlightForgeWire.h"

struct FWireReply
{
	flatbuffers::FlatBufferBuilder Builder{4096};

	flightforge::wire::ResponseBody Type = flightforge::wire::ResponseBody::NONE;
	flatbuffers::Offset<void>       Body;

	bool        bOk = true;
	std::string Error;

	template <typename T>
	void Set(flatbuffers::Offset<T> Offset)
	{
		Type = flightforge::wire::ResponseBodyTraits<T>::enum_value;
		Body = Offset.Union();
	}

	void Fail(const FString& Message)
	{
		bOk   = false;
		Error = TCHAR_TO_UTF8(*Message);
		Type  = flightforge::wire::ResponseBody::NONE;
		Body  = 0;
	}

	// bulk payloads go in with one copy
	template <typename T>
	flatbuffers::Offset<flatbuffers::Vector<T>> CopyVector(const T* Data, const size_t Count)
	{
		T*         Dst    = nullptr;
		const auto Offset = Builder.CreateUninitializedVector(Count, &Dst);
		if (Count > 0)
		{
			FMemory::Memcpy(Dst, Data, Count * sizeof(T));
		}
		return Offset;
	}

	template <typename T>
	flatbuffers::Offset<flatbuffers::Vector<const T*>> CopyStructs(const T* Data, const size_t Count)
	{
		T*         Dst    = nullptr;
		const auto Offset = Builder.CreateUninitializedVectorOfStructs(Count, &Dst);
		if (Count > 0)
		{
			FMemory::Memcpy(Dst, Data, Count * sizeof(T));
		}
		return Offset;
	}

	flatbuffers::Offset<flatbuffers::String> String(const FString& Value)
	{
		return Builder.CreateString(TCHAR_TO_UTF8(*Value));
	}
};

inline FString WireString(const flatbuffers::String* Value)
{
	return Value ? FString(UTF8_TO_TCHAR(Value->c_str())) : FString();
}
