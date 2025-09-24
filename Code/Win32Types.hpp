#ifndef WIN32TYPES_HPP
#define WIN32TYPES_HPP

#define NOMINMAX
#include <Windows.h>

class GenericHandleObject {
public:

	explicit GenericHandleObject(HANDLE handle) noexcept;
	GenericHandleObject(const GenericHandleObject& rhs) = delete;
	GenericHandleObject(GenericHandleObject&& rhs) noexcept;
	~GenericHandleObject() noexcept;

	GenericHandleObject& operator =(const GenericHandleObject& rhs) = delete;
	GenericHandleObject& operator =(GenericHandleObject&& rhs) noexcept;

	HANDLE handle_ = INVALID_HANDLE_VALUE;

};

class SpecificHandleObject : public GenericHandleObject {
public:

	explicit SpecificHandleObject(HANDLE handle) noexcept;

};

#endif // #ifndef WIN32TYPES_HPP