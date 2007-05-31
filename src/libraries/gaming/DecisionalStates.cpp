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
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Viewport_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: DecisionalStates constructor
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
DecisionalStates::DecisionalStates()
    : drawSauvegarde_( false )
    , draw1stEchelon_( false )
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
    drawSauvegarde_ = ( message.key   == std::string( "Contact" ) )
                   && ( message.value == std::string( "eEtatDec_Sauvegarde" ) );
    draw1stEchelon_ = ( message.key   == std::string( "Echelon" ) )
                   && ( message.value == std::string( "eEtatEchelon_Premier" ) );
}

// -----------------------------------------------------------------------------
// Name: DecisionalStates::Draw
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
void DecisionalStates::Draw( const geometry::Point2f& where, const Viewport_ABC& viewport, const GlTools_ABC& tools ) const
{
    if( viewport.IsHotpointVisible() )
    {
        if( drawSauvegarde_ )
            tools.DrawSvg( "sauvegarde.svg", where );
        if( draw1stEchelon_ )
            tools.DrawSvg( "1stechelon.svg", where );
    }
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
