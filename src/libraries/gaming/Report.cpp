// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Report.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/Styles.h"
#include "Simulation.h"
#include "Tools.h"
#include <qdatetime.h>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Report constructor
// Created: AGE 2006-03-09
// -----------------------------------------------------------------------------
Report::Report( const Entity_ABC& entity, const Simulation& simulation, Report::E_Type type, const QString& message )
    : entity_ ( entity )
    , type_   ( type )
    , message_( message )
    , time_   ( simulation.GetTime() )
    , isNew_  ( true )
    , point_  ( entity.Get< Positions >().GetPosition() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Report destructor
// Created: APE 2004-08-04
// -----------------------------------------------------------------------------
Report::~Report()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Report::Read
// Created: AGE 2006-09-21
// -----------------------------------------------------------------------------
void Report::Read()
{
    isNew_ = false;
}

// -----------------------------------------------------------------------------
// Name: Report::GetType
// Created: SBO 2005-09-08
// -----------------------------------------------------------------------------
Report::E_Type Report::GetType() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: Report::GetOwner
// Created: AGE 2006-03-09
// -----------------------------------------------------------------------------
const Entity_ABC& Report::GetOwner() const
{
    return entity_;
}

// -----------------------------------------------------------------------------
// Name: Report::DisplayInTooltip
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
void Report::DisplayInTooltip( Displayer_ABC& displayer ) const
{
    displayer.Item( "" )
             .Start( QColor( 220, 220, 220 ) )
                .Add( QTime().addSecs( time_ ) )
                .Add( " " )
                .Add( message_.section( " - ", 0 ) )
             .End();
}

// -----------------------------------------------------------------------------
// Name: Report::GetColor
// Created: AGE 2006-07-03
// -----------------------------------------------------------------------------
QColor Report::GetColor( E_Type type )
{
    static QColor colors[] = { QColor( 0, 0, 0 ),       // eRC      = 1000,
                               QColor( 150, 150, 150 ), // eTrace   = 1001,
                               QColor( 32, 200, 64 ),   // eEvent   = 1002,
                               QColor( 0, 0, 200 ),     // eMessage = 1003,
                               QColor( 255, 128, 64 )}; // eWarning = 1004
    return colors[ type - eRC ];
}

// -----------------------------------------------------------------------------
// Name: Report::Display
// Created: AGE 2006-07-03
// -----------------------------------------------------------------------------
void Report::Display( Displayer_ABC& displayer ) const
{
    displayer.Display( 0, GetColor( type_ ) );
    displayer.Display( 0, isNew_ ? Styles::bold : Styles::weak );
    displayer.Display( tools::translate( "Report", "Received" ), QTime().addSecs( time_ ) );
    displayer.Display( tools::translate( "Report", "Report" ), message_ );
}
