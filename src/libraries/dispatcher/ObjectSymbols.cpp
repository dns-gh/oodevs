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
#include "Object_ABC.h"
#include "Side.h"
#include "clients_kernel/ObjectType.h"
#include "tools/App6Symbol.h"

using namespace dispatcher;

// $$$$ _RC_ JSR 2011-11-15: A déplacer dans crossbow (utilisé que là a priori)

// -----------------------------------------------------------------------------
// Name: ObjectSymbols constructor
// Created: SBO 2008-02-14
// -----------------------------------------------------------------------------
ObjectSymbols::ObjectSymbols( const dispatcher::Object_ABC& holder )
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
    std::string symbol( holder_.GetType().GetSymbol() );
    if( up )
        return tools::app6::MergeSymbol( holder_.GetTeam().Get< EntitySymbols_ABC >().BuildSymbol(), symbol );
    return symbol;
}
