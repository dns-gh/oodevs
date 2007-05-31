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
    , drawEclairage_ ( false )
    , drawEtatOps_   ( false )
    , ratio_         ( 1.f )
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
    static const std::string contact( "Contact" ); 
    static const std::string sauvegarde( "eEtatDec_Sauvegarde" );
    static const std::string echelon( "Echelon" ); 
    static const std::string first     ( "eEtatEchelon_Premier" );
    static const std::string eclairage ( "eEtatEchelon_Eclairage" );
    static const std::string etat   ( "EtatOps" ); 
    static const std::string none      ( "eEtatDestruction_None" );

    // $$$$ AGE 2007-05-31: 
    if( message.key == contact )
        drawSauvegarde_ = message.value == sauvegarde;
    else if( message.key == echelon )
    {
        draw1stEchelon_ = message.value == first;
        drawEclairage_  = message.value == eclairage;
    }
    else if( message.key == etat )
        drawEtatOps_    = message.value != none;
    else
        values_[ message.key ] = message.value;
}

// -----------------------------------------------------------------------------
// Name: DecisionalStates::Draw
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
void DecisionalStates::Draw( const geometry::Point2f& where, const Viewport_ABC& viewport, const GlTools_ABC& tools ) const
{
    if( viewport.IsHotpointVisible() && tools.ShouldDisplay( "DecisionalState" ) )
    {
        if( drawSauvegarde_ )
            tools.DrawSvg( "sauvegarde.svg", where, ratio_ );
        if( drawEclairage_ )
            tools.DrawSvg( "eclairage.svg", where, ratio_ );
        if( draw1stEchelon_ )
            tools.DrawSvg( "1stechelon.svg", where, ratio_ );
        if( drawEtatOps_ )
            tools.DrawSvg( "opstatehs.svg", where, ratio_ );
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

// -----------------------------------------------------------------------------
// Name: DecisionalStates::Aggregate
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
void DecisionalStates::Aggregate( const bool& agg )
{
    ratio_ = agg ? 2.0 : 1.0;
}
