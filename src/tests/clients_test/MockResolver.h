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
    MOCK_METHOD_TPL( Find, 1 )
    MOCK_METHOD_TPL( Get, 1 )
    MOCK_METHOD_TPL( CreateIterator, 0 )
};

#endif // __MockResolver_h_
