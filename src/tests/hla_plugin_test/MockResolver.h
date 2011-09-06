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

#include "tools/Resolver_ABC.h"

namespace tools
{
// =============================================================================
/** @class  MockResolver
    @brief  MockResolver
*/
// Created: VPR 2011-09-06
// =============================================================================
template< typename T >
MOCK_BASE_CLASS( MockResolver, Resolver_ABC< T > )
{
    MOCK_METHOD_EXT_TPL( Find, 1, T*( const unsigned long& ), Find );
    MOCK_METHOD_EXT_TPL( Get, 1, T&( const unsigned long& ), Get );
    MOCK_METHOD_EXT_TPL( CreateIterator, 0, Iterator< const T& >(), CreateIterator );
};

}

#endif // MockResolver_h
