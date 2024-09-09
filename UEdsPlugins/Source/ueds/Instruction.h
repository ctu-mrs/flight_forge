#pragma once

template <typename T>
struct FInstruction
{
	FInstruction(): Finished(false) {};
	
	bool Finished;

	std::function<void(T& Obj)> Function;
};