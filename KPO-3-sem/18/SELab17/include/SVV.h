#pragma once
#include "FST.h"

namespace SVV
{
	FST::FST* CreateIntegerFST(const char* str);
	FST::FST* CreateStringFST(const char* str);
	FST::FST* CreateFunctionFST(const char* str);
	FST::FST* CreateDeclareFST(const char* str);
	FST::FST* CreateReturnFST(const char* str);
	FST::FST* CreateMainFST(const char* str);
	FST::FST* CreatePrintFST(const char* str);
	FST::FST* CreateINTLiteralFST(const char* str);
	FST::FST* CreateIdentifierFST(const char* str);
}