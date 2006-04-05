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

#include "Controllers.h"
#include "Decisions.h"
#include "Agent.h"
#include "Mission.h"
#include "AutomatDecisions.h"
#include "Model.h"
#include "GlTools_ABC.h"
#include "FragOrder.h"

#include "UnitMissionInterface.h"
#include "AutomateMissionInterface.h"
#include "FragmentaryOrderInterface.h"

//#include "PopulationMissionInterface.h"

// -----------------------------------------------------------------------------
// Name: MissionPanel constructor
// Created: APE 2004-03-19
// -----------------------------------------------------------------------------
MissionPanel::MissionPanel( QWidget* pParent, Controllers& controllers, Model& model, ParametersLayer& layer, const GlTools_ABC& tools )
    : QDockWindow       ( pParent )
    , controllers_      ( controllers )
    , layer_            ( layer )
    , converter_        ( model.coordinateConverter_ )
    , tools_            ( tools )
    , pMissionInterface_( 0 )
    , selected_         ( 0 )
{
    setResizeEnabled( true );
    setCaption( tr( "Mission" ) );
    setCloseMode( QDockWindow::Always );

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: MissionPanel destructor
// Created: APE 2004-03-19
// -----------------------------------------------------------------------------
MissionPanel::~MissionPanel()
{
    controllers_.Remove( *this );
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
        int nId = missions.insertItem( mission.GetName().c_str(), this, slot );
        missions.setItemParameter( nId, mission.GetId() );
    }
    return menu.insertItem( name, &missions  );
}

// -----------------------------------------------------------------------------
// Name: MissionPanel::AddFragOrders
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
template< typename D >
int MissionPanel::AddFragOrders( const D& decisions, QPopupMenu& menu, const QString& name, const char* slot )
{
    std::set< unsigned long > fragOrders_;
    QPopupMenu& orders = *new QPopupMenu( &menu );
    Iterator< const FragOrder& > fragIt = decisions.GetFragOrders();
    while( fragIt.HasMoreElements() )
    {
        const FragOrder& fragOrder = fragIt.NextElement();
        if( fragOrders_.insert( fragOrder.GetId() ).second )
        {
            int nId = orders.insertItem( fragOrder.GetName().c_str(), this, slot );
            orders.setItemParameter( nId, fragOrder.GetId() );
        }
    }
    if( decisions.GetCurrentMission() )
    {
        const Mission& mission = *decisions.GetCurrentMission();
        Iterator< const FragOrder& > fragIt = mission.CreateIterator();
        while( fragIt.HasMoreElements() )
        {
            const FragOrder& fragOrder = fragIt.NextElement();
            if( fragOrders_.insert( fragOrder.GetId() ).second )
            {
                int nId = orders.insertItem( fragOrder.GetName().c_str(), this, slot );
                orders.setItemParameter( nId, fragOrder.GetId() );
            }
        }
    }
    return menu.insertItem( name, &orders  );
}

// -----------------------------------------------------------------------------
// Name: MissionPanel::AddAgentMissions
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void MissionPanel::AddAgentMissions( const Decisions& decisions, QPopupMenu& menu )
{
    const int id = AddMissions( decisions.GetMissions(), menu, tr( "Missions Pion" ), SLOT( ActivateAgentMission( int ) ) );
    menu.setItemEnabled( id, ! decisions.IsEmbraye() );
    if( ! decisions.IsEmbraye() )
        AddFragOrders( decisions, menu, tr( "Ordres de conduite" ), SLOT( ActivateFragOrder( int ) ) );
}

// -----------------------------------------------------------------------------
// Name: MissionPanel::AddAutomatMissions
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void MissionPanel::AddAutomatMissions( const AutomatDecisions& decisions, QPopupMenu& menu )
{
    const int id = AddMissions( decisions.GetMissions(), menu, tr( "Missions Automate" ), SLOT( ActivateAutomatMission( int ) ) );
    menu.setItemEnabled( id, decisions.IsEmbraye() );
    if( decisions.IsEmbraye() )
        AddFragOrders( decisions, menu, tr( "Ordres de conduite" ), SLOT( ActivateFragOrder( int ) ) );
}

// -----------------------------------------------------------------------------
// Name: MissionPanel::ActivateAgentMission
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void MissionPanel::ActivateAgentMission( int id )
{
    hide();
    delete pMissionInterface_;
    // $$$$ AGE 2006-03-31: 
    pMissionInterface_ = new UnitMissionInterface( this, const_cast< Agent& >( *selected_ ), (uint)id , controllers_.actions_, layer_, converter_ );
    setWidget( pMissionInterface_ );

    // For some magic reason, the following line resizes the widget
    // to a nice size (but not the minimal one).
    resize( 10, 10 );
    show();
}

// -----------------------------------------------------------------------------
// Name: MissionPanel::ActivateAutomatMission
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void MissionPanel::ActivateAutomatMission( int id )
{
    hide();
    delete pMissionInterface_;
    // $$$$ AGE 2006-03-31: 
    pMissionInterface_ = new AutomateMissionInterface( this, const_cast< Agent& >( *selected_ ), (uint)id, controllers_.actions_, layer_, converter_ );
    setWidget( pMissionInterface_ );
    resize( 10, 10 );
    show();
}

// -----------------------------------------------------------------------------
// Name: MissionPanel::ActivateFragOrder
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
void MissionPanel::ActivateFragOrder( int id )
{
    hide();
    delete pMissionInterface_;
    // $$$$ AGE 2006-03-31: 
    pMissionInterface_ = new FragmentaryOrderInterface( this, const_cast< Agent& >( *selected_ ), (uint)id, controllers_.actions_, layer_, converter_ );
    if( pMissionInterface_->IsEmpty() )
        pMissionInterface_->OnOk();
    else
    {
        setWidget( pMissionInterface_ );
        resize( 10, 10 );
        show();
    }
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
        delete pMissionInterface_;
        pMissionInterface_ = 0;
    }
}

// -----------------------------------------------------------------------------
// Name: MissionPanel::Draw
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void MissionPanel::Draw( const geometry::Rectangle2f& )
{
    if( isVisible() && pMissionInterface_ )
    {
        glPushAttrib( GL_CURRENT_BIT | GL_LINE_BIT );
        glLineWidth( 2.f );
        glColor4d( COLOR_PARAM );
        pMissionInterface_->Draw( tools_ );
        glPopAttrib();
    }
}
