// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Report_ABC.h"
#include "clients_kernel/Entity_ABC.h"
#include "Attributes.h"
#include "Simulation.h"
#include "clients_kernel/Displayer_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Report_ABC constructor
// Created: AGE 2006-03-09
// -----------------------------------------------------------------------------
Report_ABC::Report_ABC( const Entity_ABC& agent, const Simulation& simulation )
    : agent_  ( agent )
    , nTime_  ( simulation.GetTime() )
    , bNew_   ( true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Report_ABC destructor
// Created: APE 2004-08-04
// -----------------------------------------------------------------------------
Report_ABC::~Report_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Report_ABC::GetStrippedTitle
// Created: HME 2005-11-29
// -----------------------------------------------------------------------------
std::string Report_ABC::GetStrippedTitle() const
{
    std::string strTitle = strTitle_;
    uint pos = strTitle_.find( " - " );
    if( pos >= 0 && pos < strTitle.size() )
        strTitle.resize( pos );
    return strTitle;
}

// -----------------------------------------------------------------------------
// Name: Report_ABC::IsNew
// Created: APE 2004-08-04
// -----------------------------------------------------------------------------
bool Report_ABC::IsNew() const
{
    return bNew_;
}

// -----------------------------------------------------------------------------
// Name: Report_ABC::GetType
// Created: SBO 2005-09-08
// -----------------------------------------------------------------------------
Report_ABC::E_Type Report_ABC::GetType() const
{
    return eType_;
}

// -----------------------------------------------------------------------------
// Name: Report_ABC::IsRCType
// Created: SBO 2005-09-08
// -----------------------------------------------------------------------------
bool Report_ABC::IsRCType() const
{
    return eType_ == eRC || eType_ == eMessage || eType_ == eEvent || eType_ == eWarning;
}

// -----------------------------------------------------------------------------
// Name: Report_ABC::GetAgent
// Created: AGE 2006-03-09
// -----------------------------------------------------------------------------
const Entity_ABC& Report_ABC::GetAgent() const
{
    return agent_;
}

// -----------------------------------------------------------------------------
// Name: Report_ABC::DisplayInTooltip
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
void Report_ABC::DisplayInTooltip( Displayer_ABC& displayer ) const
{
    displayer.Item( "" )
             .Start( QColor( 220, 220, 220 ) )
                .Add( QTime().addSecs( nTime_ ) )
                .Add( " " )
                .Add( GetStrippedTitle() )
             .End();
}

// -----------------------------------------------------------------------------
// Name: Report_ABC::GetColor
// Created: AGE 2006-07-03
// -----------------------------------------------------------------------------
QColor Report_ABC::GetColor( E_Type type )
{
    static QColor colors[] = { QColor( 200, 0, 0 ),     // eRC      = 1000,
                               QColor( 150, 150, 150 ), // eTrace   = 1001,
                               QColor( 32, 200, 64 ),   // eEvent   = 1002,
                               QColor( 0, 0, 200 ),     // eMessage = 1003,
                               QColor( 255, 128, 64 )}; // eWarning = 1004
    return colors[ type - eRC ];
}

// -----------------------------------------------------------------------------
// Name: Report_ABC::Display
// Created: AGE 2006-07-03
// -----------------------------------------------------------------------------
void Report_ABC::Display( Displayer_ABC& displayer ) const
{
    displayer.Display( 0, GetColor( eType_ ) );
    displayer.Display( "Reçu", QTime().addSecs( nTime_ ) );
    displayer.Display( "Compte-rendu", strTitle_ );
    // $$$$ AGE 2006-07-03: bold si isNew !
}
