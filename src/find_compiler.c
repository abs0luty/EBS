/// MIT License
///
/// Copyright (c) 2022 Salimgereyev Adi
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"),
/// to deal in the Software without restriction, including without limitation
/// the rights to use, copy, modify, merge, publish, distribute, sublicense,
/// and/or sell copies of the Software, and to permit persons to whom the
/// Software is furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included
/// in all copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
/// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
/// DEALINGS IN THE SOFTWARE.

#include "find_compiler.h"

static const char* compiler_names[] = {
	"cc", "gcc", "cl", "bcc", "xlc", "clang"
};

static int program_exists(const char* name);

char* find_compiler(void) {
	register int i;
	for (i = 0; i < sizeof(compiler_names) / sizeof(const char*); i++) {
		if (program_exists(compiler_names[i]))
			return (char*) compiler_names[i];
	}
	return NULL;
}

static int program_exists(const char* name) {
   if(strchr(name, '/')) {
        return access(name, X_OK) == 0;
    }

    const char *path = getenv("PATH");
    
    if(!path) return 0;
    
    char *buf = malloc(strlen(path) + strlen(name) + 3);

    if(!buf) return 0;

    for(; *path; ++path) {
        char *p = buf;
        for(; *path && *path!=':'; ++path,++p) {
            *p = *path;
        }
    
    	if(p == buf) *p++='.';
        if(p[-1] != '/') *p++='/';
    
    	strcpy(p, name);
        
	    if(access(buf, X_OK) == 0) {
            free(buf);
            return 1;
        }
        
	    if(!*path) break;
    }
    
    free(buf);
    return 0;
}
