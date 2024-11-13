#pragma once
#include "FST.h"

namespace SVV
{
	FST::FST* CreateIntegerFST(const char* str);
	FST::FST* CreatePrintFST(const char* str);
	FST::FST* CreateINTLiteralFST(const char* str);
	FST::FST* CreateIdentifierFST(const char* str);
}