#include "stdafx.h"
#include "SVV.h"
#include "FST.h"

namespace SVV
{
	FST::FST* CreateIntegerFST(const char* str)
	{
		return new FST::FST(str,
			4,
			FST::NODE(1, FST::RELATION('i', 1)),
			FST::NODE(1, FST::RELATION('n', 2)),
			FST::NODE(1, FST::RELATION('t', 3)),
			FST::NODE()
		);
	}

	FST::FST* CreateStringFST(const char* str)
	{
		return new FST::FST(str,
			4,
			FST::NODE(1, FST::RELATION('s', 1)),
			FST::NODE(1, FST::RELATION('t', 2)),
			FST::NODE(1, FST::RELATION('r', 3)),
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

#define nums(state) 	FST::RELATION('0', state), FST::RELATION('1', state), FST::RELATION('2', state),\
						FST::RELATION('3', state), FST::RELATION('4', state), FST::RELATION('5', state),\
						FST::RELATION('6', state), FST::RELATION('7', state), FST::RELATION('8', state),\
						FST::RELATION('9', state)

#define octal(state)	FST::RELATION('0', state), FST::RELATION('1', state), FST::RELATION('2', state),\
						FST::RELATION('3', state), FST::RELATION('4', state), FST::RELATION('5', state),\
						FST::RELATION('6', state), FST::RELATION('7', state)

	FST::FST* CreateIntDECIMALLiteralFST(const char* str)
	{
		return new FST::FST(str,
			4,
			FST::NODE(20,
				FST::RELATION('0', 1), FST::RELATION('1', 0), FST::RELATION('2', 0),
				FST::RELATION('3', 0), FST::RELATION('4', 0), FST::RELATION('5', 0),
				FST::RELATION('6', 0), FST::RELATION('7', 0), FST::RELATION('8', 0),
				FST::RELATION('9', 0), nums(2)),

			FST::NODE(15, FST::RELATION('x', 2), FST::RELATION('X', 2), FST::RELATION('b', 2), FST::RELATION('B', 2),
				FST::RELATION('0', 3), nums(2)),
			FST::NODE(20, nums(2), nums(3)),
			FST::NODE()
		);
	}

#define R(c, x) FST::RELATION(c, x)
#define HEX(x) R('a',x), R('b',x), R('c',x), R('d',x), R('e',x), R('f',x),\
			   R('A', x), R('B', x), R('C', x), R('D', x), R('E', x), R('F', x)

	FST::FST* CreateIntHEXLiteralFST(const char* str)
	{
		return new FST::FST(str,
			4,
			FST::NODE(1, R('0', 1)),
			FST::NODE(2, R('x', 2), R('X', 2)),
			FST::NODE(44, HEX(2), nums(2), nums(3), HEX(3)),
			FST::NODE()
		);
	}

	FST::FST* CreateIntOCTALLiteralFST(const char* str)
	{
		return new FST::FST(str,
			3,
			FST::NODE(1, R('0', 1)),
			FST::NODE(16, octal(1), octal(2)),
			FST::NODE()
		);
	}

	FST::FST* CreateIntBINARYLiteralFST(const char* str)
	{
		return new FST::FST(str,
			4,
			FST::NODE(1, R('0', 1)),
			FST::NODE(2, R('b', 2), R('B', 2)),
			FST::NODE(4, R('0', 2), R('1', 2), R('0', 3), R('1', 3)),
			FST::NODE()
		);
	}

	FST::FST* CreateIfFST(const char* str)
	{
		return new FST::FST(str,
			3,
			FST::NODE(1, FST::RELATION('i', 1)),
			FST::NODE(1, FST::RELATION('f', 2)),
			FST::NODE()
		);
	}

	FST::FST* CreateCompareFST(const char* str)
	{
		return new FST::FST(str,
			3,
			FST::NODE(6, FST::RELATION('>', 2), FST::RELATION('<', 2), FST::RELATION('>', 1), FST::RELATION('<', 1),
				FST::RELATION('=', 1), FST::RELATION('!', 1)),
			FST::NODE(1, FST::RELATION('=', 2)),
			FST::NODE()
		);
	}

	FST::FST* CreateElseFST(const char* str)
	{
		return new FST::FST(str,
			5,
			FST::NODE(1, FST::RELATION('e', 1)),
			FST::NODE(1, FST::RELATION('l', 2)),
			FST::NODE(1, FST::RELATION('s', 3)),
			FST::NODE(1, FST::RELATION('e', 4)),
			FST::NODE()
		);
	}

	constexpr int NUM_LETTERS = 26 * 2;

#define relationsForState(state) \
    FST::RELATION('a', state), FST::RELATION('b', state), FST::RELATION('c', state), \
    FST::RELATION('d', state), FST::RELATION('e', state), FST::RELATION('f', state), \
    FST::RELATION('g', state), FST::RELATION('h', state), FST::RELATION('i', state), \
    FST::RELATION('j', state), FST::RELATION('k', state), FST::RELATION('l', state), \
    FST::RELATION('m', state), FST::RELATION('n', state), FST::RELATION('o', state), \
    FST::RELATION('p', state), FST::RELATION('q', state), FST::RELATION('r', state), \
    FST::RELATION('s', state), FST::RELATION('t', state), FST::RELATION('u', state), \
    FST::RELATION('v', state), FST::RELATION('w', state), FST::RELATION('x', state), \
    FST::RELATION('y', state), FST::RELATION('z', state), \
    FST::RELATION('A', state), FST::RELATION('B', state), FST::RELATION('C', state), \
    FST::RELATION('D', state), FST::RELATION('E', state), FST::RELATION('F', state), \
    FST::RELATION('G', state), FST::RELATION('H', state), FST::RELATION('I', state), \
    FST::RELATION('J', state), FST::RELATION('K', state), FST::RELATION('L', state), \
    FST::RELATION('M', state), FST::RELATION('N', state), FST::RELATION('O', state), \
    FST::RELATION('P', state), FST::RELATION('Q', state), FST::RELATION('R', state), \
    FST::RELATION('S', state), FST::RELATION('T', state), FST::RELATION('U', state), \
    FST::RELATION('V', state), FST::RELATION('W', state), FST::RELATION('X', state), \
    FST::RELATION('Y', state), FST::RELATION('Z', state)

	//#define FRN(c, s) FST::RELATION(c, s)

	FST::FST* CreateIdentifierFST(const char* str)
	{
		return new FST::FST(str,
			2,
			FST::NODE(NUM_LETTERS * 2 + 20,
				relationsForState(0), nums(0),
				relationsForState(1), nums(1)),
			FST::NODE()
		);
	}

#undef nums
#undef relationsForState
#undef octal
#undef R
#undef HEX
}