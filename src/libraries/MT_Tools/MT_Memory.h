//*****************************************************************************
//
// $Created: FBD 02-11-15 $
// $Archive: /MVW_v10/Build/SDK/MT_Tools/Src/MT_Tools_Types.h $
// $Author: Nld $
// $Modtime: 19/08/04 16:34 $
// $Revision: 17 $
// $Workfile: MT_Tools_Types.h $
//
//*****************************************************************************

#ifndef __MT_Tools_Memory_h__
#define __MT_Tools_Memory_h__

#include <boost/pool/pool.hpp>

#define MT_BOOSTPOOLING_MEMBERS( classname )                \
public:                                                     \
    void* operator new   ( size_t size );                   \
    void* operator new   ( size_t size, void* pPlace );     \
    void  operator delete( void* rawMemory );               \
    void  operator delete( void* rawMemory, void* pPlace ); \
public:                                                     \
    static boost::pool<> pool_;

#define MT_BOOSTPOOLING_FUNCTIONS( classname )                       \
void* classname::operator new( size_t size )                         \
{                                                                    \
    if( size != sizeof( classname ) ) return ::operator new( size ); \
    return pool_.malloc();                                           \
}                                                                    \
void classname::operator delete( void* pElement )                    \
{                                                                    \
    if( pElement ) pool_.free( pElement );                           \
}                                                                    \
void* classname::operator new( size_t, void* pPlace )                \
{                                                                    \
    return pPlace;                                                   \
}                                                                    \
void classname::operator delete( void*, void* )                      \
{                                                                    \
}                                                                    \
boost::pool<> classname::pool_( sizeof( classname ) );

#ifdef min
#   undef min
#endif

#ifdef max
#   undef max
#endif

#ifdef Yield
#   undef Yield
#endif

#endif // __MT_Tools_Memory_h__