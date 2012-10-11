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
// Created: AHC 2012-10-11
// =============================================================================
template< typename T, typename I >
MOCK_BASE_CLASS( MockResolver, tools::Resolver_ABC<T BOOST_PP_COMMA() I> )
{
    MOCK_METHOD_EXT_TPL( Find, 1, T*( const I& ), Find );
    MOCK_METHOD_EXT_TPL( Get, 1, T&( const I& ), Get );
    MOCK_METHOD_EXT_TPL( CreateIterator, 0, Iterator< const T& >(), CreateIterator );
};

}

#endif // MockResolver_h
