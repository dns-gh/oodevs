// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "DecisionalStates.h"
#include "clients_gui/Viewport_ABC.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "protocol/Protocol.h"

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
void DecisionalStates::DoUpdate( const sword::DecisionalState& message )
{
    static const std::string contact   ( "Contact" );
    static const std::string sauvegarde( "eEtatDec_Sauvegarde" );
    static const std::string echelon   ( "Echelon" );
    static const std::string first     ( "eEtatEchelon_Premier" );
    static const std::string eclairage ( "eEtatEchelon_Eclairage" );

    // $$$$ AGE 2007-05-31:
    if( message.key() == contact )
        drawSauvegarde_ = message.value() == sauvegarde;
    else if( message.key() == echelon )
    {
        draw1stEchelon_ = message.value() == first;
        drawEclairage_  = message.value() == eclairage;
    }
    else
        values_[ QString( message.key().c_str() ) ] = QString( message.value().c_str() );
}

// -----------------------------------------------------------------------------
// Name: DecisionalStates::DoUpdate
// Created: JSR 2012-12-07
// -----------------------------------------------------------------------------
void DecisionalStates::DoUpdate( const sword::UnitAttributes& message )
{
    if( message.has_operational_state() )
        drawEtatOps_ = message.operational_state() != sword::operational;
}

// -----------------------------------------------------------------------------
// Name: DecisionalStates::Draw
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
void DecisionalStates::Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    if( viewport.IsHotpointVisible() && tools.ShouldDisplay( "DecisionalState" ) )
    {
        if( drawSauvegarde_ )
            tools.DrawSvg( "sauvegarde.svg", where, ratio_ * tools.GetAdaptiveZoomFactor() );
        if( drawEclairage_ )
            tools.DrawSvg( "eclairage.svg", where, ratio_ * tools.GetAdaptiveZoomFactor() );
        if( draw1stEchelon_ )
            tools.DrawSvg( "1stechelon.svg", where, ratio_ * tools.GetAdaptiveZoomFactor() );
        if( drawEtatOps_ )
            tools.DrawSvg( "opstatehs.svg", where, ratio_ * tools.GetAdaptiveZoomFactor() );
    }
}

// -----------------------------------------------------------------------------
// Name: DecisionalStates::DisplayInTooltip
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
void DecisionalStates::DisplayInTooltip( kernel::Displayer_ABC& displayer ) const
{
    for( auto it = values_.begin(); it != values_.end(); ++it )
        displayer.Display( it->first + ": ", it->second );
}

// -----------------------------------------------------------------------------
// Name: DecisionalStates::Aggregate
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
void DecisionalStates::Aggregate( const bool& agg )
{
    ratio_ = agg ? 2.f : 1.f;
}
