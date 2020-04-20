#pragma once
#include <Windows.h>

typedef unsigned char uint8_t;

class bp_hook {
public:
	bp_hook();
	~bp_hook();

	bool hook(void* target, void* hook);
	bool unhook();
	bool rehook();
	bool hooked();

	template <typename hook_t>
	hook_t original();
	template <typename hook_t>
	hook_t hook_addr();

private:
	bool			_hooked;
	void*			_original;
	void*			_hook;
	uint8_t			_original_byte;
	DWORD			_old;
	static void*	_handle;
};

template<typename hook_t>
inline hook_t bp_hook::original() {
	return (hook_t)_original;
}

template<typename hook_t>
inline hook_t bp_hook::hook_addr() {
	return (hook_t)_hook;
}
