#pragma once

#include <atomic>
#include <functional>

template <typename T>
struct FInstruction
{
	std::atomic<bool> Finished{false};

	std::function<void(T& Obj)> Function;
};
