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
#include "clients_kernel/FragOrderType.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/TacticalHierarchies.h"

#include "gaming/AutomatDecisions.h"
#include "gaming/CommandPublisher.h"
#include "gaming/Decisions.h"
#include "gaming/PopulationDecisions.h"
#include "gaming/StaticModel.h"

#include "protocol/ServerPublisher_ABC.h"

#include "UnitMissionInterface.h"
#include "AutomateMissionInterface.h"
#include "PopulationMissionInterface.h"
#include "FragmentaryOrderInterface.h"
#include "MissionInterfaceBuilder.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_gui/ParametersLayer.h"
#include "icons.h"

#include "actions/ActionTasker.h"
#include "actions/EngageMagicAction.h"
#include "actions/ActionTiming.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/Entity_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: MissionPanel constructor
// Created: APE 2004-03-19
// -----------------------------------------------------------------------------
MissionPanel::MissionPanel( QWidget* pParent, Controllers& controllers, const ::StaticModel& model, Publisher_ABC& publisher, gui::ParametersLayer& layer, const GlTools_ABC& tools, const kernel::Profile_ABC& profile, actions::ActionsModel& actionsModel
                          , AgentKnowledgeConverter_ABC& knowledgeConverter, ObjectKnowledgeConverter_ABC& objectKnowledgeConverter, const kernel::Time_ABC& simulation )
    : QDockWidget              ( "mission", pParent )
    , controllers_             ( controllers )
    , static_                  ( model )
    , actionsModel_            ( actionsModel )
    , layer_                   ( layer )
    , converter_               ( static_.coordinateConverter_ )
    , tools_                   ( tools )
    , profile_                 ( profile )
    , commandPublisher_        ( new CommandPublisher( controllers_, publisher, profile_ ) )
    , pMissionInterface_       ( 0 )
    , interfaceBuilder_        ( new MissionInterfaceBuilder( controllers_, layer_, knowledgeConverter, objectKnowledgeConverter, static_, simulation ) )
    , selectedEntity_          ( controllers )
    , isPlanifMode_            ( false )
    , simulation_              (simulation)
{
    setObjectName( "missionPanel" );
    setFloating( true );
    setWindowTitle( tr( "Mission" ) );

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: MissionPanel destructor
// Created: APE 2004-03-19
// -----------------------------------------------------------------------------
MissionPanel::~MissionPanel()
{
    controllers_.Unregister( *this );
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
            AddMissions( *decisions, menu, tr( "Agent missions" ), SLOT( ActivateAgentMission( int ) ) );
        if( const Automat_ABC* automat = static_cast< const Automat_ABC* >( agent.Get< kernel::TacticalHierarchies >().GetSuperior() ) )
            AddMissions( automat->Get< AutomatDecisions >(), menu, tr( "Automat missions" ), SLOT( ActivateAutomatMission( int ) ), MAKE_PIXMAP( lock ) );
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
        AddMissions( decisions, menu, tr( "Automat missions" ), SLOT( ActivateAutomatMission( int ) ), MAKE_PIXMAP( lock ) );
        if( ! decisions.IsEmbraye() )
            menu.InsertItem( "Command", tr( "Engage" ), this, SLOT( Engage() ) );
        else if( decisions.CanBeOrdered() )
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
        AddMissions( decisions, menu, tr( "Crowd missions" ), SLOT( ActivatePopulationMission( int ) ) );
    }
}

// -----------------------------------------------------------------------------
// Name: MissionPanel::NotifyDeleted
// Created: SBO 2008-08-12
// -----------------------------------------------------------------------------
void MissionPanel::NotifyDeleted( const kernel::Entity_ABC& entity )
{
    if( &entity == selectedEntity_ || selectedEntity_ == 0 )
        hide();
}

namespace
{
    QString GetPrefix( const QString& name )
    {
        {
            QRegExp regexp( "^([A-Z]{3,})\\s" );
            if( regexp.search( name ) > -1 )
                return regexp.cap( 1 );
        }
        {
            QRegExp regexp( "^(\\w{3,})\\s-\\s" );
            if( regexp.search( name ) > -1 )
                return regexp.cap( 1 );
        }
        return "";
    }

    QString FormatName( const Mission& mission, const QString& prefix )
    {
        QString result( QString( mission.GetName().c_str() ).mid( prefix.length() ).stripWhiteSpace() );
        if( result.startsWith( "-" ) )
            return result.mid( 1 ).stripWhiteSpace();
        return result;
    }

    QString FormatName( const FragOrder& order, const QString& /*prefix*/ )
    {
        return order.GetName().c_str();
    }

    struct MissionComparator
    {
        bool operator()( const Mission* lhs, const Mission* rhs )
        {
            return lhs->GetName() < rhs->GetName();
        }
        bool operator()( const FragOrder* lhs, const FragOrder* rhs )
        {
            return lhs->GetName() < rhs->GetName();
        }
        bool operator()( const QString& lhs, const QString& rhs ) const
        {
            return ( lhs.isEmpty() && ! rhs.isEmpty() ) ? false : ( ( rhs.isEmpty() && !lhs.isEmpty() ) ? true : lhs < rhs );
        }
    };
}

// -----------------------------------------------------------------------------
// Name: MissionPanel::AddMissionGroup
// Created: SBO 2008-10-20
// -----------------------------------------------------------------------------
template< typename E, typename T >
void MissionPanel::AddMissionGroup( Q3PopupMenu& menu, const QString& prefix, const T& list, const char* slot, int current )
{
    if( list.empty() )
        return;
     if( prefix.isEmpty() )
     {
        if( menu.idAt( 0 ) != -1 )
           menu.addSeparator()->setText( tools::translate( "MissionPanel", "" ) );
     }
    else
    {
        if ( !menu.count() )  // $$$$ FPT 2011-08-12 : Can't have a separator without an item before
           menu.insertItem( "", this, "" );
        menu.addSeparator()->setText( prefix );
     }
    for( T::const_iterator it = list.begin(); it != list.end(); ++it )
    {
        const E& order = **it;
        const int id = menu.insertItem( FormatName( order, prefix ), this, slot );
        menu.setItemParameter( id, order.GetId() );
        menu.setItemChecked( id, current == int( order.GetId() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: MissionPanel::AddMissions
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
QAction* MissionPanel::AddMissions( tools::Iterator< const Mission& > it, ContextMenu& menu, const QString& name, const char* slot, int current )
{
    Q3PopupMenu& missions = *new Q3PopupMenu( menu );
    QString lastPrefix;
    typedef std::map< QString, std::set< const Mission*, MissionComparator >, MissionComparator > T_Missions;
    T_Missions list;
    while( it.HasMoreElements() )
    {
        const Mission& mission = it.NextElement();
        const QString prefix = GetPrefix( mission.GetName().c_str() );
        list[ prefix ].insert( &mission );
    }
    for( T_Missions::const_iterator itM = list.begin(); itM != list.end(); ++itM )
        AddMissionGroup< Mission >( missions, itM->first, itM->second, slot, current );
    return menu.InsertItem( "Order", name, &missions );
}

// -----------------------------------------------------------------------------
// Name: MissionPanel::AddFragOrders
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
QAction* MissionPanel::AddFragOrders( const Decisions_ABC& decisions, ContextMenu& menu, const QString& name, const char* slot )
{
    Q3PopupMenu& orders = *new Q3PopupMenu( menu );
    typedef std::map< QString, std::set< const FragOrder*, MissionComparator >, MissionComparator > T_FragOrders;
    T_FragOrders list;
    if( const Mission* mission = decisions.GetCurrentMission() )
    {
        tools::Iterator< const FragOrder& > it = static_cast< const tools::Resolver< FragOrder >& >( *mission ).CreateIterator();
        while( it.HasMoreElements() )
        {
            const FragOrder& element = it.NextElement();
            list[ element.GetType().IsMissionRequired() ? mission->GetName().c_str() : "" ].insert( &element );
        }
        it = decisions.GetFragOrders();
        while( it.HasMoreElements() )
        {
            const FragOrder& element = it.NextElement();
            if( element.GetType().IsMissionRequired() && element.GetType().IsAvailableFor( *selectedEntity_ ) )
                list[ "" ].insert( &element );
        }
    }
    {
        tools::Iterator< const FragOrder& > it = decisions.GetFragOrders();
        while( it.HasMoreElements() )
        {
            const FragOrder& element = it.NextElement();
            if( !element.GetType().IsMissionRequired() && element.GetType().IsAvailableFor( *selectedEntity_ ) )
                list[ "" ].insert( &element );
        }
    }
    for( T_FragOrders::const_iterator itM = list.begin(); itM != list.end(); ++itM )
        AddMissionGroup< FragOrder >( orders, itM->first, itM->second, slot, -1 );
    return menu.InsertItem( "Order", name, &orders );
}

// -----------------------------------------------------------------------------
// Name: MissionPanel::AddMissions
// Created: AGE 2007-04-04
// -----------------------------------------------------------------------------
void MissionPanel::AddMissions( const Decisions_ABC& decisions, kernel::ContextMenu& menu, const QString& name, const char* slot, const QPixmap& pixmap /* = QPixmap()*/ )
{
    if( !decisions.CanBeOrdered() )
        return;

    const kernel::Mission* current = decisions.GetCurrentMission();
    QAction* action = AddMissions( decisions.GetMissions(), menu, name, slot, current ? current->GetId() : -1 );
    if( !pixmap.isNull() )
    {
        QIcon icon( pixmap );
        action->setIcon( icon );
    }

    AddFragOrders( decisions, menu, tr( "Fragmentary orders" ), SLOT( ActivateFragOrder( int ) ) );
}

// -----------------------------------------------------------------------------
// Name: MissionPanel::ActivateAgentMission
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void MissionPanel::ActivateAgentMission( int id )
{
    SetInterface( 0 );
    const kernel::MissionType& mission = static_cast< tools::Resolver_ABC< kernel::MissionType >& >( static_.types_).Get( id );
    SetInterface( new UnitMissionInterface( this, *selectedEntity_.ConstCast(), mission, controllers_.actions_, *interfaceBuilder_, actionsModel_ ) );
}

// -----------------------------------------------------------------------------
// Name: MissionPanel::ActivateAutomatMission
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void MissionPanel::ActivateAutomatMission( int id )
{
    SetInterface( 0 );
    const kernel::MissionType& mission = static_cast< tools::Resolver_ABC< kernel::MissionType >& >( static_.types_).Get( id );
    Entity_ABC* entity = selectedEntity_.ConstCast();
    if( !entity->Retrieve< AutomatDecisions >() )
        entity = const_cast< kernel::Entity_ABC* >( entity->Get< kernel::TacticalHierarchies >().GetSuperior() );
    SetInterface( new AutomateMissionInterface( this, *entity, mission, controllers_.actions_, *interfaceBuilder_, actionsModel_ ) );
}

// -----------------------------------------------------------------------------
// Name: MissionPanel::ActivatePopulationMission
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
void MissionPanel::ActivatePopulationMission( int id )
{
    SetInterface( 0 );
    const kernel::MissionType& mission = static_cast< tools::Resolver_ABC< kernel::MissionType >& >( static_.types_).Get( id );
    SetInterface( new PopulationMissionInterface( this, *selectedEntity_.ConstCast(), mission, controllers_.actions_, *interfaceBuilder_, actionsModel_ ) );
}

// -----------------------------------------------------------------------------
// Name: MissionPanel::ActivateFragOrder
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
void MissionPanel::ActivateFragOrder( int id )
{
    SetInterface( 0 );
    const kernel::FragOrderType& order = static_cast< tools::Resolver_ABC< kernel::FragOrderType >& >( static_.types_).Get( id );
    Entity_ABC* entity = selectedEntity_.ConstCast();
    if( !entity->Retrieve< AutomatDecisions >() )
    {
        Entity_ABC* superior = const_cast< kernel::Entity_ABC* >( entity->Get< kernel::TacticalHierarchies >().GetSuperior() );
        if( const AutomatDecisions* decisions = superior->Retrieve< AutomatDecisions >() )
            if( decisions->IsEmbraye() )
                entity = superior;
    }
    SetInterface( new FragmentaryOrderInterface( this, *entity, order, controllers_.actions_, *interfaceBuilder_, actionsModel_ ) );
}

// -----------------------------------------------------------------------------
// Name: MissionPanel::Draw
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void MissionPanel::Draw( Viewport_ABC& viewport )
{
    if( pMissionInterface_ && isVisible() )
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

    MagicActionType& actionType = static_cast< tools::Resolver< MagicActionType, std::string >& > ( static_.types_ ).Get( "change_mode" );
    actions::EngageMagicAction* action = new actions::EngageMagicAction( *selectedEntity_, actionType, controllers_.controller_, tr( "Engage" ), true, true );
    action->Attach( *new actions::ActionTiming( controllers_.controller_, simulation_ ) );
    action->Attach( *new actions::ActionTasker( selectedEntity_, false ) );
    action->RegisterAndPublish( actionsModel_ );

    decisions->Engage();

}

// -----------------------------------------------------------------------------
// Name: MissionPanel::Disengage
// Created: SBO 2006-06-19
// -----------------------------------------------------------------------------
void MissionPanel::Disengage()
{
    AutomatDecisions* decisions = selectedEntity_ ? selectedEntity_.ConstCast()->Retrieve< AutomatDecisions >() : 0;

    MagicActionType& actionType = static_cast< tools::Resolver< MagicActionType, std::string >& > ( static_.types_ ).Get( "change_mode" );
    actions::EngageMagicAction* action = new actions::EngageMagicAction( *selectedEntity_, actionType, controllers_.controller_, tr( "Disengage" ), false, true );
    action->Attach( *new actions::ActionTiming( controllers_.controller_, simulation_ ) );
    action->Attach( *new actions::ActionTasker( selectedEntity_, false ) );
    action->RegisterAndPublish( actionsModel_ );

    decisions->Disengage();
}

// -----------------------------------------------------------------------------
// Name: MissionPanel::SetInterface
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void MissionPanel::SetInterface( actions::gui::MissionInterface_ABC* missionInterface )
{
    if( !missionInterface )
    {
        hide();
        delete pMissionInterface_;
        pMissionInterface_ = 0;
    }
    else
    {
        pMissionInterface_ = missionInterface;
        pMissionInterface_->ChangeOkValueButton( isPlanifMode_ );
        NotifyMission();
        if( pMissionInterface_->IsEmpty() )
            pMissionInterface_->OnOk();
        else
        {
            setWidget( pMissionInterface_ );
            show();
        }
    }
}

// -----------------------------------------------------------------------------
// Name: MissionPanel::ActivatePlanification
// Created: HBD 2010-09-06
// -----------------------------------------------------------------------------
void MissionPanel::ActivatePlanification()
{
    isPlanifMode_ = !isPlanifMode_;
    if( pMissionInterface_ )
        pMissionInterface_->ChangeOkValueButton( isPlanifMode_ );
}

// -----------------------------------------------------------------------------
// Name: MissionPanel::NotifyMission
// Created: AGE 2008-07-15
// -----------------------------------------------------------------------------
void MissionPanel::NotifyMission()
{
    if( pMissionInterface_ && selectedEntity_ )
    {
        std::stringstream message;
        message << "/mission "
                << selectedEntity_->GetTypeName().ascii() << " " << selectedEntity_->GetId() << " "
                << "\"" << pMissionInterface_->Title().ascii() << "\"";
        commandPublisher_->Send( "", message.str() );
    }
}
