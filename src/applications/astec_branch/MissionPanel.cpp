// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-03-19 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/MissionPanel.cpp $
// $Author: Age $
// $Modtime: 6/04/05 17:33 $
// $Revision: 16 $
// $Workfile: MissionPanel.cpp $
//
// *****************************************************************************

#include "astec_pch.h"
#include "MissionPanel.h"
#include "moc_MissionPanel.cpp"

#include "ActionController.h"
#include "Decisions.h"
#include "Agent.h"
#include "Mission.h"
#include "AutomatDecisions.h"

//#include "UnitMissionInterface.h"
//#include "AutomateMissionInterface.h"
//#include "PopulationMissionInterface.h"

// -----------------------------------------------------------------------------
// Name: MissionPanel constructor
// Created: APE 2004-03-19
// -----------------------------------------------------------------------------
MissionPanel::MissionPanel( QWidget* pParent, ActionController& actions )
    : QDockWindow       ( pParent )
    , pMissionInterface_( 0 )
    , selected_         ( 0 )
{
    setResizeEnabled( true );
    setCaption( tr( "Mission" ) );
    setCloseMode( QDockWindow::Always );

    actions.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: MissionPanel destructor
// Created: APE 2004-03-19
// -----------------------------------------------------------------------------
MissionPanel::~MissionPanel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionPanel::NotifyContextMenu
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void MissionPanel::NotifyContextMenu( const Agent& agent, QPopupMenu& menu )
{
    selected_ = &agent;

    if( menu.count() > 0 )
        menu.insertSeparator();

    AddAgentMissions( agent.Get< Decisions >(), menu );
    if( agent.Retrieve< AutomatDecisions >() )
        AddAutomatMissions( agent.Get< AutomatDecisions >(), menu );
}

// -----------------------------------------------------------------------------
// Name: MissionPanel::AddMissions
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
int MissionPanel::AddMissions( Iterator< const Mission& > it, QPopupMenu& menu, const QString& name, const char* slot )
{
    QPopupMenu& missions = *new QPopupMenu( &menu );
    while( it.HasMoreElements() )
    {
        const Mission& mission = it.NextElement();
        int nId = missions.insertItem( mission.name_.c_str(), this, slot );
        missions.setItemParameter( nId, mission.id_ );
    }
    return menu.insertItem( name, &missions  );
}

// -----------------------------------------------------------------------------
// Name: MissionPanel::AddAgentMissions
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void MissionPanel::AddAgentMissions( const Decisions& decisions, QPopupMenu& menu )
{
    const int id = AddMissions( decisions.GetMissions(), menu, tr( "Missions Pion" ), SLOT( ActivateAgentMission( int ) ) );
    menu.setItemEnabled( id, ! decisions.IsEmbraye() );
}

// -----------------------------------------------------------------------------
// Name: MissionPanel::AddAutomatMissions
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void MissionPanel::AddAutomatMissions( const AutomatDecisions& decisions, QPopupMenu& menu )
{
    const int id = AddMissions( decisions.GetMissions(), menu, tr( "Missions Automate" ), SLOT( ActivateAutomatMission( int ) ) );
    menu.setItemEnabled( id, decisions.IsEmbraye() );
}

// -----------------------------------------------------------------------------
// Name: MissionPanel::ActivateAgentMission
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void MissionPanel::ActivateAgentMission( int id )
{
//    hide();
//    delete pMissionInterface_;
//    pMissionInterface_ = new UnitMissionInterface( *selected_, (uint)id , *this );
//    setWidget( pMissionInterface_ );
//
//    // For some magic reason, the following line resizes the widget
//    // to a nice size (but not the minimal one).
//    resize( 10, 10 );
//    show();
}

// -----------------------------------------------------------------------------
// Name: MissionPanel::ActivateAutomatMission
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void MissionPanel::ActivateAutomatMission( int )
{

}

// -----------------------------------------------------------------------------
// Name: MissionPanel::NotifyContextMenu
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void MissionPanel::NotifyContextMenu( const Population& agent, QPopupMenu& menu )
{
//    if( popupMenu.count() > 0 )
//        popupMenu.insertSeparator();
//
//    // Create and fill the unit menu.
//    QPopupMenu* pPopulationMenu = new QPopupMenu( &popupMenu );
//    const AgentModel::T_MissionVector& missions = population.GetModel().GetAvailableMissions();
//    for( AgentModel::CIT_MissionVector it = missions.begin(); it != missions.end(); ++it )
//    {
//        int nId = pPopulationMenu->insertItem( ENT_Tr::ConvertFromPopulationMission( E_PopulationMission( *it ), ENT_Tr::eToApp ).c_str(), this, SLOT( ActivatePopulationMission( int ) ) );
//        pPopulationMenu->setItemParameter( nId, (int)*it );
//    }
//
//    // Add the unit mission menu.
//    popupMenu.insertItem( tr( "Missions Population" ), pPopulationMenu );
}

// -----------------------------------------------------------------------------
// Name: MissionPanel::hideEvent
// Created: APE 2004-04-27
// -----------------------------------------------------------------------------
void MissionPanel::hideEvent( QHideEvent* pEvent )
{
    if( ! pEvent->spontaneous() )
    {
//        delete pMissionInterface_;
//        pMissionInterface_ = 0;
    }
}
