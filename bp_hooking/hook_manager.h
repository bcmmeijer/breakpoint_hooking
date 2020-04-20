#pragma once
#include <map>
#include <string>
#include <memory>

#include "bp_hook.h"

#ifdef _WIN64
#define Ip Rip
#else
#define Ip Eip
#endif

using hooks = std::map<std::string, std::shared_ptr<bp_hook>>;

class hook_manager {
public:

	static hook_manager& get();
	hooks& all();
	std::shared_ptr<bp_hook>& operator[](std::string hook);

	bool init(PVECTORED_EXCEPTION_HANDLER handler);
	bool deinit();

	static bp_hook* get_hook(PEXCEPTION_POINTERS info);

private:
	hook_manager();
	~hook_manager();
	hook_manager(const hook_manager&) = delete;
	hook_manager& operator=(const hook_manager&) = delete;

private:
	static hook_manager _manager;
	hooks _hooks = {};
	void* _handle = nullptr;
	bool _initialized = false;
};