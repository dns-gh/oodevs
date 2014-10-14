// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef MockResolver_h
#define MockResolver_h

#include <tools/Resolver_ABC.h>

namespace tools
{
// =============================================================================
/** @class  MockResolver
    @brief  MockResolver
*/
// Created: VPR 2011-09-06
// =============================================================================
template< typename T, typename Identifier = unsigned long >
MOCK_BASE_CLASS( MockResolver, Resolver_ABC< T, Identifier > )
{
    MOCK_METHOD_TPL( Find, 1, T*( const Identifier& ) );
    MOCK_METHOD_TPL( Get, 1, T&( const Identifier& ) );
    MOCK_METHOD_TPL( CreateIterator, 0, Iterator< const T& >() );
};

}

#endif // MockResolver_h
