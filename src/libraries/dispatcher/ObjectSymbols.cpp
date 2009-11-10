// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "ObjectSymbols.h"
#include "Object.h"
#include "Side.h"
#include "clients_kernel/ObjectType.h"
#include "tools/App6Symbol.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: ObjectSymbols constructor
// Created: SBO 2008-02-14
// -----------------------------------------------------------------------------
ObjectSymbols::ObjectSymbols( const Object& holder )
    : holder_( holder )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectSymbols destructor
// Created: SBO 2008-02-14
// -----------------------------------------------------------------------------
ObjectSymbols::~ObjectSymbols()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectSymbols::BuildSymbol
// Created: SBO 2008-02-14
// -----------------------------------------------------------------------------
std::string ObjectSymbols::BuildSymbol( bool up /*= true*/ ) const
{
    std::string symbol( holder_.type_.GetSymbol() );
    if( up )
        return tools::app6::MergeSymbol( holder_.side_.Get< EntitySymbols_ABC >().BuildSymbol(), symbol );
    return symbol;
}
