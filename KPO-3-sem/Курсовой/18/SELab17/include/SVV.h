#pragma once
#include "FST.h"

namespace SVV
{
	FST::FST* CreateIntegerFST(const char* str);
	FST::FST* CreateStringFST(const char* str);
	FST::FST* CreateMainFST(const char* str);
	FST::FST* CreatePrintFST(const char* str);
	FST::FST* CreateINTLiteralFST(const char* str);
	FST::FST* CreateIdentifierFST(const char* str);
	FST::FST* CreateElseFST(const char* str);
	FST::FST* CreateIfFST(const char* str);
	FST::FST* CreateCompareFST(const char* str);
}