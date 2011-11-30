// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "MineAttribute.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/Units.h"
#include "clients_kernel/Tools.h"
#include "protocol/SimulationSenders.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: MineAttribute constructor
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
MineAttribute::MineAttribute( kernel::Controller& controller, const tools::Resolver_ABC< DotationType >& resolver, bool minefield /* = false */ )
    : controller_( controller )
    , resolver_ ( resolver )
    , valorization_ ( 0 )
    , minefield_( minefield )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MineAttribute destructor
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
MineAttribute::~MineAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MineAttribute::Display
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void MineAttribute::Display( kernel::Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "Object", "Information" ) )
             .Display( tools::translate( "Object", "Mining:" ), rValorizationPercentage_ * Units::percentage );

    if( ! valorization_ )
        return;
    displayer.Group( tools::translate( "Object", "Information" ) )
             .Item( tools::translate( "Object", "Development resource:" ) )
                .Start( nDotationValorization_ )
                .Add( " " ).Add( valorization_ ).End();
    if( density_.IsSet() )
        displayer.Group( tools::translate( "Object", "Mine parameters" ) )
                 .Display( tools::translate( "Object", "Density:" ), density_ * Units::minesPerMeter );
}

// -----------------------------------------------------------------------------
// Name: MineAttribute::DisplayInSummary
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
void MineAttribute::DisplayInSummary( kernel::Displayer_ABC& displayer ) const
{
    Display( displayer );
}

// -----------------------------------------------------------------------------
// Name: MineAttribute::DisplayInTooltip
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void MineAttribute::DisplayInTooltip( kernel::Displayer_ABC& displayer ) const
{
    if( rValorizationPercentage_.IsSet() )
        displayer.Group( tools::translate( "Object", "Information" ) )
                 .Display( tools::translate( "Object", "Mining:" ), rValorizationPercentage_ * Units::percentage );
}

// -----------------------------------------------------------------------------
// Name: MineAttribute::DoUpdate
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void MineAttribute::DoUpdate( const sword::ObjectKnowledgeUpdate& message )
{
    UpdateData( message.attributes() );
}

// -----------------------------------------------------------------------------
// Name: MineAttribute::DoUpdate
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void MineAttribute::DoUpdate( const sword::ObjectUpdate& message )
{
    UpdateData( message.attributes() );
}

// -----------------------------------------------------------------------------
// Name: MineAttribute::UpdateData
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
template< typename T >
void MineAttribute::UpdateData( const T& message )
{
    if( message.has_mine()  )
    {
        if( ! valorization_ && message.mine().has_resource()  )
            valorization_ = resolver_.Find( message.mine().resource().id() );

        if( message.mine().has_dotation()  )
            nDotationValorization_ = message.mine().dotation();
        if( message.mine().has_percentage()  )
            rValorizationPercentage_ = float( message.mine().percentage() );
        if( message.mine().has_density()  )
            density_ = float( message.mine().density() );
        controller_.Update( *(MineAttribute_ABC*)this );
    }
}
