// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "MissionPanel.h"
#include "moc_MissionPanel.cpp"

#include "clients_kernel/Controllers.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Mission.h"
#include "clients_kernel/FragOrder.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/TacticalHierarchies.h"

#include "gaming/Decisions.h"
#include "gaming/AutomatDecisions.h"
#include "gaming/StaticModel.h"
#include "gaming/PopulationDecisions.h"

#include "UnitMissionInterface.h"
#include "AutomateMissionInterface.h"
#include "PopulationMissionInterface.h"
#include "FragmentaryOrderInterface.h"
#include "MissionInterfaceFactory.h"
#include "MissionInterfaceBuilder.h"
#include "gaming/AgentKnowledgeConverter.h"
#include "gaming/ObjectKnowledgeConverter.h"
#include "clients_kernel/GlTools_ABC.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: MissionPanel constructor
// Created: APE 2004-03-19
// -----------------------------------------------------------------------------
MissionPanel::MissionPanel( QWidget* pParent, Controllers& controllers, const StaticModel& model, Publisher_ABC& publisher, ParametersLayer& layer, const GlTools_ABC& tools, const kernel::Profile_ABC& profile )
    : QDockWindow              ( pParent )
    , controllers_             ( controllers )
    , static_                  ( model )
    , publisher_               ( publisher )
    , layer_                   ( layer )
    , converter_               ( static_.coordinateConverter_ )
    , tools_                   ( tools )
    , profile_                 ( profile )
    , knowledgeConverter_      ( new AgentKnowledgeConverter( controllers ) )
    , objectKnowledgeConverter_( new ObjectKnowledgeConverter( controllers ) )
    , pMissionInterface_       ( 0 )
    , interfaceFactory_        ( new MissionInterfaceFactory( controllers ) )
    , interfaceBuilder_        ( new MissionInterfaceBuilder( controllers_.actions_, layer_, converter_, *knowledgeConverter_, *objectKnowledgeConverter_, static_.objectTypes_ ) ) 
    , selectedEntity_          ( controllers )
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
    delete interfaceBuilder_;
    delete interfaceFactory_;
    delete knowledgeConverter_;
    delete objectKnowledgeConverter_;
}

// -----------------------------------------------------------------------------
// Name: MissionPanel::NotifyContextMenu
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void MissionPanel::NotifyContextMenu( const Agent_ABC& agent, ContextMenu& menu )
{
    if( profile_.CanBeOrdered( agent ) )
    {
        selectedEntity_ = &agent;
        if( const Decisions* decisions = agent.Retrieve< Decisions >() )
            AddAgentMissions( *decisions, menu );
        if( const Automat_ABC* automat = static_cast< const Automat_ABC* >( agent.Get< kernel::TacticalHierarchies >().GetSuperior() ) )
            AddAutomatMissions( automat->Get< AutomatDecisions >(), menu );
    }
}

// -----------------------------------------------------------------------------
// Name: MissionPanel::NotifyContextMenu
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
void MissionPanel::NotifyContextMenu( const kernel::Automat_ABC& agent, kernel::ContextMenu& menu )
{
    if( profile_.CanBeOrdered( agent ) )
    {
        selectedEntity_ = &agent;
        const AutomatDecisions& decisions = agent.Get< AutomatDecisions >();
        AddAutomatMissions( decisions, menu );
        if( ! decisions.IsEmbraye() )
            menu.InsertItem( "Command", tr( "Engage" ), this, SLOT( Engage() ) );
        else
            menu.InsertItem( "Command", tr( "Disengage" ), this, SLOT( Disengage() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: MissionPanel::NotifyContextMenu
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void MissionPanel::NotifyContextMenu( const Population_ABC& entity, ContextMenu& menu )
{
    if( profile_.CanBeOrdered( entity ) )
    {
        selectedEntity_ = &entity;
        const PopulationDecisions& decisions = entity.Get< PopulationDecisions >();
        AddPopulationMissions( decisions, menu );
    }
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
        int nId = missions.insertItem( mission.GetName(), this, slot );
        missions.setItemParameter( nId, mission.GetId() );
    }
    return menu.InsertItem( "Order", name, &missions  );
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
            int nId = orders.insertItem( fragOrder.GetName(), this, slot );
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
                int nId = orders.insertItem( fragOrder.GetName(), this, slot );
                orders.setItemParameter( nId, fragOrder.GetId() );
            }
        }
    }
    return menu.InsertItem( "Order", name, &orders  );
}

// -----------------------------------------------------------------------------
// Name: MissionPanel::AddAgentMissions
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void MissionPanel::AddAgentMissions( const Decisions& decisions, ContextMenu& menu )
{
    const int id = AddMissions( decisions.GetMissions(), menu, tr( "Unit missions" ), SLOT( ActivateAgentMission( int ) ) );
    const bool isEmbraye = decisions.IsEmbraye();
    menu.SetItemEnabled( id, ! isEmbraye );
    if( ! isEmbraye )
        AddFragOrders( decisions, menu, tr( "Fragmentary orders" ), SLOT( ActivateFragOrder( int ) ) );
}

// -----------------------------------------------------------------------------
// Name: MissionPanel::AddAutomatMissions
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void MissionPanel::AddAutomatMissions( const AutomatDecisions& decisions, ContextMenu& menu )
{
    const int id = AddMissions( decisions.GetMissions(), menu, tr( "Automat missions" ), SLOT( ActivateAutomatMission( int ) ) );
    menu.SetItemEnabled( id, decisions.IsEmbraye() );
    if( decisions.IsEmbraye() )
        AddFragOrders( decisions, menu, tr( "Fragmentary orders" ), SLOT( ActivateFragOrder( int ) ) );
}

// -----------------------------------------------------------------------------
// Name: MissionPanel::AddPopulationMissions
// Created: SBO 2006-12-18
// -----------------------------------------------------------------------------
void MissionPanel::AddPopulationMissions( const PopulationDecisions& decisions, kernel::ContextMenu& menu )
{
    AddMissions( decisions.GetMissions(), menu, tr( "Population missions" ), SLOT( ActivatePopulationMission( int ) ) );
    AddFragOrders( decisions, menu, tr( "Fragmentary orders" ), SLOT( ActivateFragOrder( int ) ) );
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
    Mission& mission = ((Resolver_ABC< Mission >&)static_.types_).Get( id );
    pMissionInterface_ = new UnitMissionInterface( this, *selectedEntity_.ConstCast(), mission, controllers_.actions_, publisher_, *interfaceFactory_, *interfaceBuilder_ );
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
    Mission& mission = ((Resolver_ABC< Mission >&)static_.types_).Get( id );
    Entity_ABC* entity = selectedEntity_.ConstCast();
    if( !entity->Retrieve< AutomatDecisions >() )
        entity = const_cast< kernel::Entity_ABC* >( entity->Get< kernel::TacticalHierarchies >().GetSuperior() );
    pMissionInterface_ = new AutomateMissionInterface( this, *entity, mission, controllers_.actions_, publisher_, *interfaceFactory_, *interfaceBuilder_ );
    setWidget( pMissionInterface_ );
    resize( 10, 10 );
    show();
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
    Mission& mission = ((Resolver_ABC< Mission >&)static_.types_).Get( id );
    pMissionInterface_ = new PopulationMissionInterface( this, const_cast< Entity_ABC& >( *selectedEntity_ ), mission, controllers_.actions_, publisher_, *interfaceFactory_, *interfaceBuilder_ );
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
    FragOrder& order = ((Resolver_ABC< FragOrder >&)static_.types_).Get( id );
    pMissionInterface_ = new FragmentaryOrderInterface( this, *selectedEntity_.ConstCast(), order, controllers_.actions_, publisher_, *interfaceFactory_, *interfaceBuilder_ );
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
        glColor4f( COLOR_PARAM );
        pMissionInterface_->Draw( tools_, viewport );
        glPopAttrib();
    }
}

// -----------------------------------------------------------------------------
// Name: MissionPanel::Engage
// Created: SBO 2006-06-19
// -----------------------------------------------------------------------------
void MissionPanel::Engage()
{
    AutomatDecisions* decisions = selectedEntity_ ? selectedEntity_.ConstCast()->Retrieve< AutomatDecisions >() : 0;
    decisions->Engage();
}

// -----------------------------------------------------------------------------
// Name: MissionPanel::Disengage
// Created: SBO 2006-06-19
// -----------------------------------------------------------------------------
void MissionPanel::Disengage()
{
    AutomatDecisions* decisions = selectedEntity_ ? selectedEntity_.ConstCast()->Retrieve< AutomatDecisions >() : 0;
    decisions->Disengage();
}
