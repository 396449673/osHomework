#pragma once
#include<windows.h>
typedef struct {
    int code;
    char* result;
    void* object;
}Response;

typedef struct {
	int errorCode;
	const char* errorResult;
	HANDLE FileMap;
	char* Buff;
}ShareMemResponse;
