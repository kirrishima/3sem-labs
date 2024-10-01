#include "stdafx.h"
#include "SVV.h"
#include "FST.h"

namespace SVV
{
	FST::FST* CreateIntegerFST(const char* str)
	{
		return new FST::FST(str,
			8,
			FST::NODE(1, FST::RELATION('i', 1)),
			FST::NODE(1, FST::RELATION('n', 2)),
			FST::NODE(1, FST::RELATION('t', 3)),
			FST::NODE(1, FST::RELATION('e', 4)),
			FST::NODE(1, FST::RELATION('g', 5)),
			FST::NODE(1, FST::RELATION('e', 6)),
			FST::NODE(1, FST::RELATION('r', 7)),
			FST::NODE()
		);
	}

	FST::FST* CreateStringFST(const char* str)
	{
		return new FST::FST(str,
			7,
			FST::NODE(1, FST::RELATION('s', 1)),
			FST::NODE(1, FST::RELATION('t', 2)),
			FST::NODE(1, FST::RELATION('r', 3)),
			FST::NODE(1, FST::RELATION('i', 4)),
			FST::NODE(1, FST::RELATION('n', 5)),
			FST::NODE(1, FST::RELATION('g', 6)),
			FST::NODE()
		);
	}

	FST::FST* CreateFunctionFST(const char* str)
	{
		return new FST::FST(str,
			9,
			FST::NODE(1, FST::RELATION('f', 1)),
			FST::NODE(1, FST::RELATION('u', 2)),
			FST::NODE(1, FST::RELATION('n', 3)),
			FST::NODE(1, FST::RELATION('c', 4)),
			FST::NODE(1, FST::RELATION('t', 5)),
			FST::NODE(1, FST::RELATION('i', 6)),
			FST::NODE(1, FST::RELATION('o', 7)),
			FST::NODE(1, FST::RELATION('n', 8)),
			FST::NODE()
		);
	}

	FST::FST* CreateDeclareFST(const char* str)
	{
		return new FST::FST(str,
			8,
			FST::NODE(1, FST::RELATION('d', 1)),
			FST::NODE(1, FST::RELATION('e', 2)),
			FST::NODE(1, FST::RELATION('c', 3)),
			FST::NODE(1, FST::RELATION('l', 4)),
			FST::NODE(1, FST::RELATION('a', 5)),
			FST::NODE(1, FST::RELATION('r', 6)),
			FST::NODE(1, FST::RELATION('e', 7)),
			FST::NODE()
		);
	}

	FST::FST* CreateReturnFST(const char* str)
	{
		return new FST::FST(str,
			7,
			FST::NODE(1, FST::RELATION('r', 1)),
			FST::NODE(1, FST::RELATION('e', 2)),
			FST::NODE(1, FST::RELATION('t', 3)),
			FST::NODE(1, FST::RELATION('u', 4)),
			FST::NODE(1, FST::RELATION('r', 5)),
			FST::NODE(1, FST::RELATION('n', 6)),
			FST::NODE()
		);
	}

	FST::FST* CreateMainFST(const char* str)
	{
		return new FST::FST(str,
			5,
			FST::NODE(1, FST::RELATION('m', 1)),
			FST::NODE(1, FST::RELATION('a', 2)),
			FST::NODE(1, FST::RELATION('i', 3)),
			FST::NODE(1, FST::RELATION('n', 4)),
			FST::NODE()
		);
	}

	FST::FST* CreatePrintFST(const char* str)
	{
		return new FST::FST(str,
			6,
			FST::NODE(1, FST::RELATION('p', 1)),
			FST::NODE(1, FST::RELATION('r', 2)),
			FST::NODE(1, FST::RELATION('i', 3)),
			FST::NODE(1, FST::RELATION('n', 4)),
			FST::NODE(1, FST::RELATION('t', 5)),
			FST::NODE()
		);
	}

	FST::FST* CreateINTLiteralFST(const char* str)
	{
		return new FST::FST(str,
			2,
			FST::NODE(20,
				FST::RELATION('0', 0), FST::RELATION('1', 0), FST::RELATION('2', 0),
				FST::RELATION('3', 0), FST::RELATION('4', 0), FST::RELATION('5', 0),
				FST::RELATION('6', 0), FST::RELATION('7', 0), FST::RELATION('8', 0),
				FST::RELATION('9', 0), FST::RELATION('0', 1), FST::RELATION('1', 1),
				FST::RELATION('2', 1), FST::RELATION('3', 1), FST::RELATION('4', 1),
				FST::RELATION('5', 1), FST::RELATION('6', 1), FST::RELATION('7', 1),
				FST::RELATION('8', 1), FST::RELATION('9', 1)),
			FST::NODE()
		);
	}

	constexpr int NUM_LETTERS = 26;

#define relationsForState(state) FST::RELATION('a', state), FST::RELATION('b', state), FST::RELATION('c', state),\
	FST::RELATION('d', state), FST::RELATION('e', state), FST::RELATION('f', state),\
	FST::RELATION('g', state), FST::RELATION('h', state), FST::RELATION('i', state),\
	FST::RELATION('j', state), FST::RELATION('k', state), FST::RELATION('l', state),\
	FST::RELATION('m', state), FST::RELATION('n', state), FST::RELATION('o', state),\
	FST::RELATION('p', state), FST::RELATION('q', state), FST::RELATION('r', state),\
	FST::RELATION('s', state), FST::RELATION('t', state), FST::RELATION('u', state),\
	FST::RELATION('v', state), FST::RELATION('w', state), FST::RELATION('x', state),\
	FST::RELATION('y', state), FST::RELATION('z', state)

	FST::FST* CreateIdentifierFST(const char* str)
	{
		return new FST::FST(str,
			6,
			FST::NODE(NUM_LETTERS, relationsForState(1)),
			FST::NODE(NUM_LETTERS + 1, relationsForState(2), FST::RELATION('\0', 2)),
			FST::NODE(NUM_LETTERS + 1, relationsForState(3), FST::RELATION('\0', 3)),
			FST::NODE(NUM_LETTERS + 1, relationsForState(4), FST::RELATION('\0', 4)),
			FST::NODE(NUM_LETTERS + 1, relationsForState(5), FST::RELATION('\0', 5)),
			FST::NODE()
		);
	}
#undef relationsForState

}