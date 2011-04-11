// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MockResolver_h_
#define __MockResolver_h_

#include "tools/Resolver_ABC.h"

// =============================================================================
/** @class  MockResolver
    @brief  MockResolver
*/
// Created: LDC 2009-11-30
// =============================================================================
template< typename T, typename Identifier >
struct MockResolver : tools::Resolver_ABC< T,Identifier >, mock::object, mock::detail::base< tools::Resolver_ABC< T,Identifier > >
{
    MOCK_METHOD_TPL_EXT( Find, 1, T*( const Identifier& id ), Find )
    MOCK_METHOD_TPL_EXT( Get, 1, T&( const Identifier& id ), Get )
    MOCK_METHOD_TPL_EXT( CreateIterator, 0, Iterator< const T& >(), CreateIterator )
};

#endif // __MockResolver_h_
