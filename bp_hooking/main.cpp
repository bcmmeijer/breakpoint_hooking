#include "hook_manager.h"
#include <iostream>
#include <memory>
#include <functional>

typedef void(__stdcall *hk_sleep_t)(DWORD duration);
typedef std::shared_ptr<void> _defer;

#define defer(x) _defer _(nullptr, std::bind([&] { x; }));

#define hook_entry(x)							\
		auto self = hook_manager::get()[x];		\
		self->unhook();							\
		defer(self->rehook());

long __stdcall handler(PEXCEPTION_POINTERS info);

void __stdcall hk_sleep(DWORD duration) {
	hook_entry("sleep");
	std::cout << "Sleeping for -> " << duration << "ms\n";
	self->original<hk_sleep_t>()(duration);
}

int main() {

	hook_manager::get().init(handler);
	defer(hook_manager::get().deinit());

	auto& mgr = hook_manager::get();

	mgr["sleep"]->hook(Sleep, hk_sleep);

	Sleep(250);
	Sleep(250);
	Sleep(250);

	return 0;
}

long __stdcall handler(PEXCEPTION_POINTERS info) {

	if (info->ExceptionRecord->ExceptionCode != EXCEPTION_BREAKPOINT)
		return EXCEPTION_CONTINUE_SEARCH;

	bp_hook* hk = hook_manager::get_hook(info);
	if(hk == nullptr)
		return EXCEPTION_CONTINUE_EXECUTION;

	info->ContextRecord->Ip = hk->hook_addr<uintptr_t>();

	return EXCEPTION_CONTINUE_EXECUTION;
}