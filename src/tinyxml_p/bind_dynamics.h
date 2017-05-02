#ifndef BIND_DYNAMICS_INCLUDED
#define BIND_DYNAMICS_INCLUDED

#include "../definitions.h"
#include "tinybind.h"

struct TTest 
{
	int val1;
	std::string val2;
};

typedef std::vector< TTest > TVecTest;

TiXmlBinding< TTest > const * GetTiXmlBinding( TTest const &, Identity<TTest> );
TiXmlBinding< TVecTest > const * GetTiXmlBinding( TVecTest const &, Identity<TVecTest> );

struct TTests
{
	TVecTest testing;
};

TiXmlBinding< TTests > const * GetTiXmlBinding( TTests const &, Identity<TTests> );


#endif // BIND_DYNAMICS_INCLUDED