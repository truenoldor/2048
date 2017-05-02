#include "../definitions.h"
#include "tinybind.h"

// types conversion


void ConvertFromString(char const* s, float* f)
{
    *f = static_cast<float>(atof(s));
}

void ConvertFromString(char const* s, double* f)
{
    *f = atof(s);
}

void ConvertFromString(char const* strIn, void** dataOut)
{
   // OX_ASSERT(false);
}

void ConvertFromString( char const * strIn, int * dataOut )
{
    *dataOut = static_cast<int>(atoi(strIn));
}

void ConvertFromString( char const * strIn, unsigned int * dataOut )
{
    *dataOut = static_cast<unsigned int>(atoi(strIn));
}

void ConvertFromString( char const * strIn, short * dataOut )
{
    *dataOut = static_cast<short>(atoi(strIn));
}

void ConvertFromString( char const * strIn, unsigned short * dataOut )
{
    *dataOut = static_cast<unsigned short>(atoi(strIn));
}

void ConvertFromString( char const * strIn, std::string * dataOut )
{
    *dataOut = strIn;
}

void ConvertFromString( char const * strIn, const char * * dataOut )
{
    static std::string strHolder;
    strHolder = strIn;
    *dataOut = strHolder.c_str();
}

void ConvertFromString( char const * strIn, bool * dataOut )
{
    *dataOut = static_cast<int>(atoi(strIn)) != 0;
}


template<class T>
char const *
ConvertToString( T const & t )
{
  std::stringstream str;
  static std::string strOut;
  str << t;
  strOut = str.str();
  return strOut.c_str();
}

//template<class T>
//void
//ConvertFromString( char const * strIn, T * dataOut )
//{
//  std::stringstream str;
//  str << strIn;
//  str >> *dataOut;
//}
//
//void
//ConvertFromString( char const * strIn, string * dataOut )
//{
//	std::stringstream str;
//	*dataOut = strIn;
//// 	str << strIn;
//// 	str >> *dataOut;
//}


// abstract definition

template<class T>
TiXmlBinding<T> const *
GetTiXmlBinding( T const &, IdentityBase  )
{
  static GenericTiXmlBinding<T> binding;
  return &binding;
}


// generic types and clases

TiXmlBinding<float> const * GetTiXmlBinding( float const &, IdentityBase  )
{
    static GenericTiXmlBinding<float> binding;
    return &binding;
}
TiXmlBinding<double> const * GetTiXmlBinding( double const &, IdentityBase  )
{
    static GenericTiXmlBinding<double> binding;
    return &binding;
}
TiXmlBinding<int> const * GetTiXmlBinding( int const &, IdentityBase  )
{
    static GenericTiXmlBinding<int> binding;
    return &binding;
}
TiXmlBinding<unsigned int> const * GetTiXmlBinding( unsigned int const &, IdentityBase  )
{
    static GenericTiXmlBinding<unsigned int> binding;
    return &binding;
}
TiXmlBinding<short> const * GetTiXmlBinding( short const &, IdentityBase  )
{
    static GenericTiXmlBinding<short> binding;
    return &binding;
}
TiXmlBinding<unsigned short> const * GetTiXmlBinding( unsigned short const &, IdentityBase  )
{
    static GenericTiXmlBinding<unsigned short> binding;
    return &binding;
}
TiXmlBinding<bool> const * GetTiXmlBinding( bool const &, IdentityBase  )
{
    static GenericTiXmlBinding<bool> binding;
    return &binding;
}
TiXmlBinding<void *> const * GetTiXmlBinding( void * const &, IdentityBase  )
{
    static GenericTiXmlBinding<void *> binding;
    return &binding;
}
TiXmlBinding<char const *> const * GetTiXmlBinding( char const * const &, IdentityBase  )
{
    static GenericTiXmlBinding<char const *> binding;
    return &binding;
}
TiXmlBinding<std::string> const * GetTiXmlBinding( std::string const &, IdentityBase  )
{
    static GenericTiXmlBinding<std::string> binding;
    return &binding;
}

// stl abstract


char const* ConvertToString(unsigned long const&l)
{
    static char __buff[20];
    
    sprintf(__buff, "%lu", l);
    return __buff;
}

