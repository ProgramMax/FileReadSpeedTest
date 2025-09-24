#include "Win32Types.hpp"

#include <utility>

GenericHandleObject::GenericHandleObject(HANDLE handle) noexcept
	: handle_(std::move(handle))
{}

GenericHandleObject::GenericHandleObject(GenericHandleObject&& rhs) noexcept
	: handle_(std::move(rhs.handle_))
{
	rhs.handle_ = INVALID_HANDLE_VALUE;
}

GenericHandleObject::~GenericHandleObject() noexcept {
	if (handle_ != INVALID_HANDLE_VALUE) {
		BOOL result = CloseHandle(handle_);
		if (result == 0) {
			// GetLastError
		}
	}
}

GenericHandleObject& GenericHandleObject::operator =(GenericHandleObject&& rhs) noexcept {
	handle_ = std::move(rhs.handle_);
	rhs.handle_ = INVALID_HANDLE_VALUE;
	return *this;
}

SpecificHandleObject::SpecificHandleObject(HANDLE handle) noexcept
	: GenericHandleObject(std::move(handle))
{}