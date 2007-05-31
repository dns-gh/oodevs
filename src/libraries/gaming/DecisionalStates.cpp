// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "DecisionalStates.h"
#include "clients_kernel/Displayer_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: DecisionalStates constructor
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
DecisionalStates::DecisionalStates()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DecisionalStates destructor
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
DecisionalStates::~DecisionalStates()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DecisionalStates::DoUpdate
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
void DecisionalStates::DoUpdate( const ASN1T_MsgDecisionalState& message )
{
    values_[ message.key ] = message.value;
}

// -----------------------------------------------------------------------------
// Name: DecisionalStates::Draw
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
void DecisionalStates::Draw( const geometry::Point2f& where, const Viewport_ABC& viewport, const GlTools_ABC& tools ) const
{
    // $$$$ AGE 2007-05-31: 
}

// -----------------------------------------------------------------------------
// Name: DecisionalStates::DisplayInTooltip
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
void DecisionalStates::DisplayInTooltip( Displayer_ABC& displayer ) const
{
    for( CIT_Values it = values_.begin(); it != values_.end(); ++it )
        displayer.Display( it->first, it->second );
}
