#ifndef OSALLOCATOR_HPP
#define OSALLOCATOR_HPP

#include <optional>

class OSAllocation {
public:

	explicit OSAllocation(void* memory) noexcept;

	void* memory_;

};

class OSAllocator {
public:

	static std::optional<OSAllocation> allocate(size_t size) noexcept;
	static void deallocate(OSAllocation& allocation) noexcept;

};

#endif // #ifndef OSALLOCATOR_HPP