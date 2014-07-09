// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __MIL_h_
#define __MIL_h_

#include "CheckPoints/MIL_CheckPointInArchive.h"
#include "CheckPoints/MIL_CheckPointOutArchive.h"
#include "MT_Tools/MT_Vector3D.h"
#include "MT_Tools/MT_Vector2D.h"

template< class T >
std::ostream& operator<<( std::ostream& out, const std::vector< T >& vect )
{
    out << "{ ";
    if( !vect.empty() )
    {
        std::copy( vect.begin(), vect.end() - 1, std::ostream_iterator< T >( out, ", " ) );
        out << vect.back() << ' ';
    }
    return out << '}';
}

template< class T, class U >
std::pair< T, U >& operator+=( std::pair< T, U >& lhs, const std::pair< T, U >& rhs )
{
    lhs.first += rhs.first;
    lhs.second += rhs.second;
    return lhs;
}

template< class T, class U >
std::pair< T, U > operator+( const std::pair< T, U >& lhs, const std::pair< T, U >& rhs )
{
    return std::make_pair( lhs.first + rhs.first, lhs.second + rhs.second );
}

#define INTERNAL_BOOST_SAVE_LOAD_CONSTRUCT_DATA_HEADER( BASE_NAME )                                                                                     \
template< typename Archive > friend void save_construct_data( Archive& archive, const BASE_NAME* role, const unsigned int /*version*/ );     \
template< typename Archive > friend void load_construct_data( Archive& archive, BASE_NAME* role, const unsigned int /*version*/ );

#define INTERNAL_BOOST_SAVE_LOAD_CONSTRUCT_DATA( BASE_NAME, CONSTRUCTOR_PARAMETER )                     \
template< typename Archive >                                                                            \
void save_construct_data( Archive& archive, const BASE_NAME* role, const unsigned int /*version*/ )     \
{                                                                                                       \
    const CONSTRUCTOR_PARAMETER* const pion = &role->owner_;                                            \
    archive << pion;                                                                                    \
}                                                                                                       \
template< typename Archive >                                                                            \
void load_construct_data( Archive& archive, BASE_NAME* role, const unsigned int /*version*/ )           \
{                                                                                                       \
    CONSTRUCTOR_PARAMETER* pion;                                                                                \
    archive >> pion;                                                                                    \
    ::new( role )BASE_NAME( *pion );                                                                    \
}

#endif // __MIL_h_
