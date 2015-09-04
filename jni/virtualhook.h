#pragma once

#include <dlfcn.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>

int GetVtableIndex(char* vtname, char* function) {
	void** vtable = (void**) ((int) dlsym(RTLD_DEFAULT, vtname) + 8);
	int index = -1;

	for(int i = 0; i < 80; i++) {
		void* currentfunc = vtable[i];
		Dl_info info;
		int status = dladdr(currentfunc, &info);
		const char* sym = info.dli_sname;
		if(!strcmp(sym, function)) {
			index = i;
			break;
		}
	}
	return index;
}

uintptr_t VirtualHook(char* vtname, char* mangled, void* myfunc) {
	void** vtable = (void**) ((int) dlsym(RTLD_DEFAULT, vtname) + 8);
	int index = GetVtableIndex(vtname, mangled);

	if(index == -1) return 0x00000000;
	uintptr_t real = (uintptr_t) vtable[index];
	vtable[index] = myfunc;
	return real;
}