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
#include "MissionInterfaceBuilder.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_gui/ParametersLayer.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: MissionPanel constructor
// Created: APE 2004-03-19
// -----------------------------------------------------------------------------
MissionPanel::MissionPanel( QWidget* pParent, Controllers& controllers, const StaticModel& model, Publisher_ABC& publisher, ParametersLayer& layer, const GlTools_ABC& tools, const kernel::Profile_ABC& profile, ActionsModel& actionsModel
                          , AgentKnowledgeConverter_ABC& knowledgeConverter, ObjectKnowledgeConverter_ABC& objectKnowledgeConverter )
    : QDockWindow              ( pParent )
    , controllers_             ( controllers )
    , static_                  ( model )
    , actionsModel_            ( actionsModel )
    , publisher_               ( publisher )
    , layer_                   ( layer )
    , converter_               ( static_.coordinateConverter_ )
    , tools_                   ( tools )
    , profile_                 ( profile )
    , pMissionInterface_       ( 0 )
    , interfaceBuilder_        ( new MissionInterfaceBuilder( controllers_.actions_, layer_, knowledgeConverter, objectKnowledgeConverter, static_ ) ) 
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
    QPopupMenu& orders = *new QPopupMenu( menu );
    Iterator< const FragOrder& > it = decisions.GetFragOrders();
    AddFragOrders( it, orders, slot );
    
    if( const Mission* mission = decisions.GetCurrentMission() )
    {
        it = static_cast< const Resolver< FragOrder >& >( *mission ).CreateIterator();
        AddFragOrders( it, orders, slot );
    }
    return menu.InsertItem( "Order", name, &orders );
}

namespace
{
    bool ItemExists( QPopupMenu& menu, int value )
    {
        for( unsigned int i = 0; i < menu.count(); ++i )
            if( menu.itemParameter( menu.idAt( i ) ) == value )
                return true;
        return false;
    }
}

// -----------------------------------------------------------------------------
// Name: MissionPanel::AddFragOrders
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void MissionPanel::AddFragOrders( Iterator< const FragOrder& > it, QPopupMenu& menu, const char* slot )
{
    while( it.HasMoreElements() )
    {
        const FragOrder& fragOrder = it.NextElement();
        if( ! ItemExists( menu, fragOrder.GetId() ) )
        {
            const int id = menu.insertItem( fragOrder.GetName(), this, slot );
            menu.setItemParameter( id, fragOrder.GetId() );
        }
    }
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
    SetInterface( 0 );
    const MissionType& mission = static_cast< Resolver_ABC< MissionType >& >( static_.types_).Get( id );
    SetInterface( new UnitMissionInterface( this, *selectedEntity_.ConstCast(), mission, controllers_.actions_, publisher_, *interfaceBuilder_, actionsModel_ ) );
}

// -----------------------------------------------------------------------------
// Name: MissionPanel::ActivateAutomatMission
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void MissionPanel::ActivateAutomatMission( int id )
{
    SetInterface( 0 );
    const MissionType& mission = static_cast< Resolver_ABC< MissionType >& >( static_.types_).Get( id );
    Entity_ABC* entity = selectedEntity_.ConstCast();
    if( !entity->Retrieve< AutomatDecisions >() )
        entity = const_cast< kernel::Entity_ABC* >( entity->Get< kernel::TacticalHierarchies >().GetSuperior() );
    SetInterface( new AutomateMissionInterface( this, *entity, mission, controllers_.actions_, publisher_, *interfaceBuilder_, actionsModel_ ) );
}

// -----------------------------------------------------------------------------
// Name: MissionPanel::ActivatePopulationMission
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
void MissionPanel::ActivatePopulationMission( int id )
{
    SetInterface( 0 );
    const MissionType& mission = static_cast< Resolver_ABC< MissionType >& >( static_.types_).Get( id );
    SetInterface( new PopulationMissionInterface( this, *selectedEntity_.ConstCast(), mission, controllers_.actions_, publisher_, *interfaceBuilder_, actionsModel_ ) );
}

// -----------------------------------------------------------------------------
// Name: MissionPanel::ActivateFragOrder
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
void MissionPanel::ActivateFragOrder( int id )
{
    SetInterface( 0 );
    const FragOrderType& order = static_cast< Resolver_ABC< FragOrderType >& >( static_.types_).Get( id );
    SetInterface( new FragmentaryOrderInterface( this, *selectedEntity_.ConstCast(), order, controllers_.actions_, publisher_, *interfaceBuilder_, actionsModel_ ) );
}

// -----------------------------------------------------------------------------
// Name: MissionPanel::hideEvent
// Created: APE 2004-04-27
// -----------------------------------------------------------------------------
void MissionPanel::hideEvent( QHideEvent* pEvent )
{
    if( ! pEvent->spontaneous() )
    {
        layer_.Reset();
        SetInterface( 0 );
    }
}

// -----------------------------------------------------------------------------
// Name: MissionPanel::Draw
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void MissionPanel::Draw( Viewport_ABC& viewport )
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

// -----------------------------------------------------------------------------
// Name: MissionPanel::SetInterface
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void MissionPanel::SetInterface( MissionInterface_ABC* missionInterface )
{
    if( !missionInterface )
    {
        hide();
        delete pMissionInterface_;
    }
    else
    {
        pMissionInterface_ = missionInterface;
        if( pMissionInterface_->IsEmpty() )
            pMissionInterface_->OnOk();
        else
        {
            setWidget( pMissionInterface_ );
            resize( 10, 10 );
            show();
        }
    }
}
