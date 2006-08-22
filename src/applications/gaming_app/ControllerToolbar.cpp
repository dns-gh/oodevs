// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ControllerToolbar.h"
#include "moc_ControllerToolbar.cpp"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Options.h"
#include "clients_gui/resources.h"

#include <qprocess.h>

using namespace kernel;

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
    const Team_ABC* current = teams_.at( nValue );
    controllers_.options_.Change( "CurrentTeam", current );
}

// -----------------------------------------------------------------------------
// Name: ControllerToolbar::LaunchScipio
// Created: APE 2004-06-18
// -----------------------------------------------------------------------------
void ControllerToolbar::LaunchScipio()
{
    QProcess sim(0);
	sim.addArgument( "sim.exe" );
	sim.addArgument( "-forceodbcomposition" );
	sim.start();
}

// -----------------------------------------------------------------------------
// Name: ControllerToolbar::LaunchScipioDbg
// Created: APE 2004-06-18
// -----------------------------------------------------------------------------
void ControllerToolbar::LaunchScipioDbg()
{
    QProcess sim(0);
	sim.addArgument( "sim_d.exe" );
	sim.addArgument( "-forceodbcomposition" );
	sim.start();
}

// -----------------------------------------------------------------------------
// Name: ControllerToolbar::NotifyCreated
// Created: AGE 2006-03-27
// -----------------------------------------------------------------------------
void ControllerToolbar::NotifyCreated( const Team_ABC& team )
{
    teams_.push_back( &team );
    pTeamCombo_->insertItem( team.GetName().c_str(), teams_.size() - 1 );
}

// -----------------------------------------------------------------------------
// Name: ControllerToolbar::NotifyDeleted
// Created: AGE 2006-03-27
// -----------------------------------------------------------------------------
void ControllerToolbar::NotifyDeleted( const Team_ABC& team )
{
    for( unsigned i = 0; i < teams_.size(); ++i )
        if( teams_.at( i ) == &team )
        {
            teams_.erase( teams_.begin() + i );
            pTeamCombo_->removeItem( i );
        }
}
