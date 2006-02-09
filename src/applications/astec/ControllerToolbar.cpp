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

#ifdef __GNUG__
#   pragma implementation
#endif

#include "astec_pch.h"
#include "ControllerToolbar.h"
#include "moc_ControllerToolbar.cpp"

#include "App.h"
#include "AgentManager.h"
#include "Team.h"
#include "MainWindow.h"
#include "Options.h"


// -----------------------------------------------------------------------------
// Name: ControllerToolbar constructor
/** @param  pParent 
*/
// Created: APE 2004-09-21
// -----------------------------------------------------------------------------
ControllerToolbar::ControllerToolbar( QMainWindow* pParent )
    : QToolBar( pParent, "controller toolbar" )
{
    this->setLabel( tr( "Outils controlleur" ) );

    // Team selection combo box
    new QLabel( tr( "Camp:" ), this );
    pTeamCombo_ = new QComboBox( this );

    // Populate the combo box.
    pTeamCombo_->insertItem( tr( "Tous" ), 0 );
    const AgentManager::T_TeamMap& teamMap = App::GetApp().GetAgentManager().GetTeams();
    for( AgentManager::CIT_TeamMap it = teamMap.begin(); it != teamMap.end(); ++it )
        pTeamCombo_->insertItem( it->second->GetName().c_str(), it->second->GetIdx() + 1 );
    pTeamCombo_->setCurrentItem( 0 );

    // Sim launch buttons
    new QToolButton( MAKE_ICON( sim ),    tr( "Lancer Scipio" ),       "", this, SLOT( LaunchScipio() ),    this );
    new QToolButton( MAKE_ICON( simdbg ), tr( "Lancer Scipio debug" ), "", this, SLOT( LaunchScipioDbg() ), this);

    // Connexions
    connect( pTeamCombo_, SIGNAL( activated( int ) ), this,                             SLOT( OnTeamChanged( int ) ) );
    connect( this,       SIGNAL( TeamChanged() ),    &MainWindow::GetMainWindow(), SIGNAL( TeamChanged() ) );
    connect( &App::GetApp(), SIGNAL( TeamCreated( Team& ) ), this,   SLOT( OnTeamCreated( Team& ) ) );
    connect( &App::GetApp(), SIGNAL( TeamDeleted( Team& ) ), this,   SLOT( OnTeamDeleted( Team& ) ) );
}


// -----------------------------------------------------------------------------
// Name: ControllerToolbar destructor
// Created: APE 2004-09-21
// -----------------------------------------------------------------------------
ControllerToolbar::~ControllerToolbar()
{
}


// -----------------------------------------------------------------------------
// Name: ControllerToolbar::OnTeamChanged
/** @param  nValue 
*/
// Created: APE 2004-05-26
// -----------------------------------------------------------------------------
void ControllerToolbar::OnTeamChanged( int nValue )
{
    MainWindow::GetMainWindow().GetOptions().nPlayedTeam_ = nValue - 1;
    // The "- 1" above is to take into account that index 0 is
    // occupied by the 'all teams' (ie. controller view) option.

    emit TeamChanged();
}

// -----------------------------------------------------------------------------
// Name: ControllerToolbar::OnTeamCreated
// Created: AGE 2005-04-14
// -----------------------------------------------------------------------------
void ControllerToolbar::OnTeamCreated( Team& team )
{
    MainWindow::GetMainWindow().GetOptions().nPlayedTeam_ = Options::eController;
    pTeamCombo_->insertItem( team.GetName().c_str(), team.GetIdx() + 1 );
}

// -----------------------------------------------------------------------------
// Name: ControllerToolbar::OnTeamDeleted
// Created: AGE 2005-09-21
// -----------------------------------------------------------------------------
void ControllerToolbar::OnTeamDeleted( Team& team )
{
    MainWindow::GetMainWindow().GetOptions().nPlayedTeam_ = Options::eController;
    int nIndex = 0;
    while( nIndex < pTeamCombo_->count() && pTeamCombo_->text( nIndex ) != team.GetName().c_str() )
        ++nIndex;
    pTeamCombo_->removeItem( nIndex );
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
