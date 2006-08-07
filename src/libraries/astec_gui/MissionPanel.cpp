// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_gui_pch.h"
#include "MissionPanel.h"
#include "moc_MissionPanel.cpp"

#include "astec_kernel/Controllers.h"
#include "astec_kernel/Agent_ABC.h"
#include "astec_kernel/Mission.h"
#include "astec_kernel/FragOrder.h"

#include "astec_gaming/Decisions.h"
#include "astec_gaming/AutomatDecisions.h"
#include "astec_gaming/StaticModel.h"
#include "astec_gaming/PopulationDecisions.h"
#include "astec_gaming/Population.h"

#include "UnitMissionInterface.h"
#include "AutomateMissionInterface.h"
#include "PopulationMissionInterface.h"
#include "FragmentaryOrderInterface.h"
#include "astec_gaming/CoordinateConverter.h"
#include "astec_gaming/AgentKnowledgeConverter.h"
#include "GlTools_ABC.h"

// -----------------------------------------------------------------------------
// Name: MissionPanel constructor
// Created: APE 2004-03-19
// -----------------------------------------------------------------------------
MissionPanel::MissionPanel( QWidget* pParent, Controllers& controllers, const StaticModel& model, Publisher_ABC& publisher, ParametersLayer& layer, const GlTools_ABC& tools )
    : QDockWindow       ( pParent )
    , controllers_      ( controllers )
    , static_           ( model )
    , publisher_        ( publisher )
    , layer_            ( layer )
    , converter_        ( static_.coordinateConverter_ )
    , tools_            ( tools )
    , pMissionInterface_( 0 )
    , selected_         ( 0 )
    , selectedPopulation_( 0 )
{
    knowledgeConverter_ = new AgentKnowledgeConverter( controllers );
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
    delete knowledgeConverter_;
}

// -----------------------------------------------------------------------------
// Name: MissionPanel::NotifyContextMenu
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void MissionPanel::NotifyContextMenu( const Agent_ABC& agent, ContextMenu& menu )
{
    selected_ = &agent;

    if( const Decisions* decisions = agent.Retrieve< Decisions >() )
        AddAgentMissions( *decisions, menu );
    if( const AutomatDecisions* decisions = agent.Retrieve< AutomatDecisions >() )
        AddAutomatMissions( *decisions, menu );
}

// -----------------------------------------------------------------------------
// Name: MissionPanel::AddMissions
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
int MissionPanel::AddMissions( Iterator< const Mission& > it, ContextMenu& menu, const QString& name, const char* slot )
{
    QPopupMenu& missions = *new QPopupMenu( menu );
    while( it.HasMoreElements() )
    {
        const Mission& mission = it.NextElement();
        int nId = missions.insertItem( mission.GetName().c_str(), this, slot );
        missions.setItemParameter( nId, mission.GetId() );
    }
    return menu.InsertItem( "Ordre", name, &missions  );
}

// -----------------------------------------------------------------------------
// Name: MissionPanel::AddFragOrders
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
template< typename D >
int MissionPanel::AddFragOrders( const D& decisions, ContextMenu& menu, const QString& name, const char* slot )
{
    std::set< unsigned long > fragOrders_;
    QPopupMenu& orders = *new QPopupMenu( menu );
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
    return menu.InsertItem( "Ordre", name, &orders  );
}

// -----------------------------------------------------------------------------
// Name: MissionPanel::AddAgentMissions
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void MissionPanel::AddAgentMissions( const Decisions& decisions, ContextMenu& menu )
{
    const int id = AddMissions( decisions.GetMissions(), menu, tr( "Missions Pion" ), SLOT( ActivateAgentMission( int ) ) );
    menu.SetItemEnabled( id, ! decisions.IsEmbraye() );
    if( ! decisions.IsEmbraye() )
        AddFragOrders( decisions, menu, tr( "Ordres de conduite" ), SLOT( ActivateFragOrder( int ) ) );
}

// -----------------------------------------------------------------------------
// Name: MissionPanel::AddAutomatMissions
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void MissionPanel::AddAutomatMissions( const AutomatDecisions& decisions, ContextMenu& menu )
{
    const int id = AddMissions( decisions.GetMissions(), menu, tr( "Missions Automate" ), SLOT( ActivateAutomatMission( int ) ) );
    menu.SetItemEnabled( id, decisions.IsEmbraye() );
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
    pMissionInterface_ = new UnitMissionInterface( this, const_cast< Agent_ABC& >( *selected_ ), (uint)id , controllers_.actions_, layer_, converter_, *knowledgeConverter_, static_.objectTypes_, publisher_ );
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
    pMissionInterface_ = new AutomateMissionInterface( this, const_cast< Agent_ABC& >( *selected_ ), (uint)id, controllers_.actions_, layer_, converter_, *knowledgeConverter_, static_.objectTypes_, publisher_ );
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
    pMissionInterface_ = new FragmentaryOrderInterface( this, const_cast< Agent_ABC& >( *selected_ ), (uint)id, controllers_.actions_, layer_, converter_, *knowledgeConverter_, static_.objectTypes_, publisher_ );
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
void MissionPanel::NotifyContextMenu( const Population& agent, ContextMenu& menu )
{
    selectedPopulation_ = &agent;
    if( const PopulationDecisions* decisions = agent.Retrieve< PopulationDecisions >() )
    {
        QPopupMenu& missions = *new QPopupMenu( menu );
        Iterator< const Mission& > it = decisions->GetMissions();
        while( it.HasMoreElements() )
        {
            const Mission& mission = it.NextElement();
            int nId = missions.insertItem( mission.GetName().c_str(), this, SLOT( ActivatePopulationMission( int ) ) );
            missions.setItemParameter( nId, mission.GetId() );
        }
        menu.InsertItem( "Ordre", tr( "Missions Population" ), &missions  );
    }
}

// -----------------------------------------------------------------------------
// Name: MissionPanel::ActivatePopulationMission
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
void MissionPanel::ActivatePopulationMission( int id )
{
    hide();
    delete pMissionInterface_;
    // $$$$ AGE 2006-03-31: 
    pMissionInterface_ = new PopulationMissionInterface( this, const_cast< Population& >( *selectedPopulation_ ), (uint)id, controllers_.actions_, layer_, converter_, *knowledgeConverter_, static_.objectTypes_, publisher_ );
    setWidget( pMissionInterface_ );
    resize( 10, 10 );
    show();
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
void MissionPanel::Draw( const geometry::Rectangle2f& viewport )
{
    if( pMissionInterface_  && isVisible() )
    {
        glPushAttrib( GL_CURRENT_BIT | GL_LINE_BIT );
        glLineWidth( 2.f );
        glColor4d( COLOR_PARAM );
        pMissionInterface_->Draw( tools_, viewport );
        glPopAttrib();
    }
}
