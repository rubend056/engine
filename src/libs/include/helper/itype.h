#ifndef itype_h
#define itype_h

#include "macros.h"

#define ITYPE const char* type()

struct IType {
	virtual ITYPE = 0;
};

#endif	// itype_h