// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-09-21 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/ControllerToolbar.cpp $
// $Author: Age $
// $Modtime: 14/04/05 12:53 $
// $Revision: 2 $
// $Workfile: ControllerToolbar.cpp $
//
// *****************************************************************************

#include "astec_pch.h"
#include "ControllerToolbar.h"
#include "moc_ControllerToolbar.cpp"
#include "Team.h"
#include "Controllers.h"
#include "Options.h"

#include <shellapi.h>

// -----------------------------------------------------------------------------
// Name: ControllerToolbar constructor
/** @param  pParent 
*/
// Created: APE 2004-09-21
// -----------------------------------------------------------------------------
ControllerToolbar::ControllerToolbar( QMainWindow* pParent, Controllers& controllers )
    : QToolBar( pParent, "controller toolbar" )
    , controllers_( controllers )
{
    setLabel( tr( "Outils controlleur" ) );

    // Team selection combo box
    new QLabel( tr( "Camp:" ), this );
    pTeamCombo_ = new QComboBox( this );
    teams_.push_back( 0 ); // all
    pTeamCombo_->insertItem( tr( "tous" ), 0 );


    // Sim launch buttons
    new QToolButton( MAKE_ICON( sim ),    tr( "Lancer Scipio" ),       "", this, SLOT( LaunchScipio() ),    this );
    new QToolButton( MAKE_ICON( simdbg ), tr( "Lancer Scipio debug" ), "", this, SLOT( LaunchScipioDbg() ), this);

    // Connexions
    connect( pTeamCombo_, SIGNAL( activated( int ) ), this, SLOT( OnTeamChanged( int ) ) );

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ControllerToolbar destructor
// Created: APE 2004-09-21
// -----------------------------------------------------------------------------
ControllerToolbar::~ControllerToolbar()
{
    controllers_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: ControllerToolbar::OnTeamChanged
// Created: APE 2004-05-26
// -----------------------------------------------------------------------------
void ControllerToolbar::OnTeamChanged( int nValue )
{
    const Team* current = teams_.at( nValue );
    controllers_.options_.Change( "CurrentTeam", current );
}

// -----------------------------------------------------------------------------
// Name: ControllerToolbar::LaunchScipio
// Created: APE 2004-06-18
// -----------------------------------------------------------------------------
void ControllerToolbar::LaunchScipio()
{
    ShellExecute( 0, "open", "sim.exe", "-nodisplay -forceodbcomposition", NULL, SW_SHOW );
}

// -----------------------------------------------------------------------------
// Name: ControllerToolbar::LaunchScipioDbg
// Created: APE 2004-06-18
// -----------------------------------------------------------------------------
void ControllerToolbar::LaunchScipioDbg()
{
    ShellExecute( 0, "open", "sim_dbg.exe", "-nodisplay -forceodbcomposition", NULL, SW_SHOW );
}

// -----------------------------------------------------------------------------
// Name: ControllerToolbar::NotifyCreated
// Created: AGE 2006-03-27
// -----------------------------------------------------------------------------
void ControllerToolbar::NotifyCreated( const Team& team )
{
    teams_.push_back( &team );
    pTeamCombo_->insertItem( team.GetName().c_str(), teams_.size() - 1 );
}

// -----------------------------------------------------------------------------
// Name: ControllerToolbar::NotifyDeleted
// Created: AGE 2006-03-27
// -----------------------------------------------------------------------------
void ControllerToolbar::NotifyDeleted( const Team& team )
{
    for( unsigned i = 0; i < teams_.size(); ++i )
        if( teams_.at( i ) == &team )
        {
            teams_.erase( teams_.begin() + i );
            pTeamCombo_->removeItem( i );
        }
}
