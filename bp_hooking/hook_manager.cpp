#include "hook_manager.h"

hook_manager hook_manager::_manager;

hook_manager::hook_manager() {}

hook_manager& hook_manager::get() {
	return _manager;
}

hooks& hook_manager::all() {
	return _hooks;
}

std::shared_ptr<bp_hook>& hook_manager::operator[](std::string hook) {
	
	if (_hooks.find(hook) != _hooks.end())
		return _hooks[hook];

	_hooks[hook] = std::make_shared<bp_hook>();
	return _hooks[hook];
}

bool hook_manager::init(PVECTORED_EXCEPTION_HANDLER handler) {
	
	if (handler == nullptr) return false;

	_handle = AddVectoredExceptionHandler(1, handler);
	if (_handle != nullptr) {
		_initialized = true;
		return true;
	}
	return false;
}

bool hook_manager::deinit() {
	if (_handle != nullptr && _initialized) {
		_initialized = false;
		for (auto& [name, hook] : _hooks)
			hook->unhook();
		return RemoveVectoredExceptionHandler(_handle);
	}
	return false;
}

hook_manager::~hook_manager() {
	if (_initialized) deinit();
}

bp_hook* hook_manager::get_hook(PEXCEPTION_POINTERS info) {
	for (auto& [name, hook] : hook_manager::get().all()) {

		if (!hook->hooked()) continue;

		if (info->ContextRecord->Ip == (uintptr_t)hook->original<void*>())
			return hook.get();
	}
	return nullptr;
}
