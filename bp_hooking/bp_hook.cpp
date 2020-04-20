#include "bp_hook.h"

bp_hook::bp_hook() : _original_byte(0x00), _original(nullptr), _hook(nullptr), _hooked(false), _old(0) {
}

bp_hook::~bp_hook() {
	if (_hooked) unhook();
}

bool bp_hook::hook(void* target, void* hook) {
	if (_hooked) return true;

	_original = target;
	_hook = hook;

	if (!VirtualProtect(_original, 1, PAGE_EXECUTE_READWRITE, &_old))
		return false;

	_original_byte = *(unsigned char*)_original;
	*(unsigned char*)_original = 0xCC;

	_hooked = true;

	return true;
}

bool bp_hook::unhook() {

	if (!_hooked) return true;

	*(unsigned char*)_original = _original_byte;

	_hooked = false;
	return true;
}

bool bp_hook::rehook() {
	return hook(_original, _hook);
}

bool bp_hook::hooked() {
	return _hooked;
}
