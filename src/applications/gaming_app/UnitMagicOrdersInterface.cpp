// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "UnitMagicOrdersInterface.h"
#include "moc_UnitMagicOrdersInterface.cpp"
#include "actions/ActionTasker.h"
#include "actions/ActionTiming.h"
#include "actions/Army.h"
#include "actions/Point.h"
#include "actions/UnitMagicAction.h"
#include "clients_gui/LocationCreator.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Location_ABC.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/AgentExtensions.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/Point.h"

#include "gaming/StaticModel.h"
#include "gaming/MagicOrders.h"
#include "gaming/AutomatDecisions.h"
#include "gaming/Attributes.h"
#include "protocol/simulationsenders.h"
#include <google/protobuf/Message.h>
#include <google/protobuf/Descriptor.h>
#include <boost/foreach.hpp>

using namespace Common;
using namespace kernel;
using namespace gui;
using namespace actions;

// -----------------------------------------------------------------------------
// Name: UnitMagicOrdersInterface constructor
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
UnitMagicOrdersInterface::UnitMagicOrdersInterface( QWidget* parent, kernel::Controllers& controllers, actions::ActionsModel& actionsModel, const ::StaticModel& staticModel, const kernel::Time_ABC& simulation, gui::ParametersLayer& layer, const kernel::Profile_ABC& profile )
    : QObject( parent )
    , controllers_( controllers )
    , actionsModel_( actionsModel )
    , static_( staticModel )
    , simulation_( simulation )
    , profile_( profile )
    , selectedEntity_( controllers )
    , magicMove_( false )
{
    magicMoveLocation_ = new LocationCreator( 0, layer, *this );
    magicMoveLocation_->Allow( false, false, false, false, false );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: UnitMagicOrdersInterface destructor
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
UnitMagicOrdersInterface::~UnitMagicOrdersInterface()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: UnitMagicOrdersInterface::NotifyContextMenu
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void UnitMagicOrdersInterface::NotifyContextMenu( const kernel::Agent_ABC& agent, kernel::ContextMenu& menu )
{
    if( !profile_.CanDoMagic( agent ) )
        return;

    selectedEntity_ = &agent;
    if( const MagicOrders* orders = agent.Retrieve< MagicOrders >() )
    {
        QPopupMenu* magicMenu = menu.SubMenu( "Order", tr( "Magic orders" ) );
        int moveId = AddMagic( tr( "Teleport" ), SLOT( Move() ), magicMenu );
        magicMenu->setItemEnabled( moveId, orders->CanMagicMove() );
        AddSurrenderMenu( magicMenu, agent );
        if( orders->CanRetrieveTransporters() )
            AddMagic( tr( "Recover - Transporters" ), SLOT( RecoverHumanTransporters() ), magicMenu );
        AddMagic( tr( "Destroy - Component" ),  SLOT( DestroyComponent() ),  magicMenu );
        FillCommonOrders( magicMenu );
    }
}

// -----------------------------------------------------------------------------
// Name: UnitMagicOrdersInterface::NotifyContextMenu
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void UnitMagicOrdersInterface::NotifyContextMenu( const kernel::Automat_ABC& agent, kernel::ContextMenu& menu )
{
    if( !profile_.CanDoMagic( agent ) )
        return;

    selectedEntity_ = &agent;
    QPopupMenu* magicMenu = menu.SubMenu( "Order", tr( "Magic orders" ) );
    int moveId = AddMagic( tr( "Teleport" ), SLOT( Move() ), magicMenu );
    bool bMoveAllowed = false;
    if( const AutomatDecisions* decisions = agent.Retrieve< AutomatDecisions >() )
        bMoveAllowed = decisions->CanBeOrdered();
    magicMenu->setItemEnabled( moveId, bMoveAllowed );
    AddSurrenderMenu( magicMenu, agent );
    FillCommonOrders( magicMenu );
}

// -----------------------------------------------------------------------------
// Name: UnitMagicOrdersInterface::NotifyContextMenu
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void UnitMagicOrdersInterface::NotifyContextMenu( const kernel::Formation_ABC& entity, kernel::ContextMenu& menu )
{
    if( !profile_.CanDoMagic( entity ) )
        return;
    selectedEntity_ = &entity;
    QPopupMenu* magicMenu = menu.SubMenu( "Order", tr( "Magic orders" ) );
    FillCommonOrders( magicMenu );
}

// -----------------------------------------------------------------------------
// Name: UnitMagicOrdersInterface::NotifyContextMenu
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void UnitMagicOrdersInterface::NotifyContextMenu( const kernel::Team_ABC& entity, kernel::ContextMenu& menu )
{
    if( !profile_.CanDoMagic( entity ) )
        return;
    selectedEntity_ = &entity;
    QPopupMenu* magicMenu = menu.SubMenu( "Order", tr( "Magic orders" ) );
    FillCommonOrders( magicMenu );
}

// -----------------------------------------------------------------------------
// Name: UnitMagicOrdersInterface::Handle
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void UnitMagicOrdersInterface::Handle( kernel::Location_ABC& location )
{
    if( magicMove_ && location.IsValid() )
    {
        if( selectedEntity_ )
        {
            // $$$$ _RC_ SBO 2010-05-17: use ActionFactory
            MagicActionType& actionType = static_cast< tools::Resolver< MagicActionType, std::string >& > ( static_.types_ ).Get( "teleport" );
            UnitMagicAction* action = new UnitMagicAction( *selectedEntity_, actionType, controllers_.controller_, true );
            tools::Iterator< const OrderParameter& > it = actionType.CreateIterator();
            action->AddParameter( *new parameters::Point( it.NextElement(), static_.coordinateConverter_, location ) );
            action->Attach( *new ActionTiming( controllers_.controller_, simulation_, *action ) );
            action->Attach( *new ActionTasker( *selectedEntity_, false ) );
            action->RegisterAndPublish( actionsModel_ );
        }
    }
    controllers_.Unregister( *magicMoveLocation_ );
    magicMove_ = false;
}

namespace
{
    struct MagicFunctor
    {
        MagicFunctor( const ::StaticModel& staticModel, kernel::Controllers& controllers, actions::ActionsModel& actionsModel, const kernel::Time_ABC& simulation, int id ) 
            : static_( staticModel )
            , controllers_( controllers )
            , actionsModel_( actionsModel)
            , simulation_( simulation )
            , id_( id )
        {}

        void operator()( const Agent_ABC& agent ) const
        {
            std::string strType;
            switch( id_ )
            {
            case MsgsClientToSim::MsgUnitMagicAction_Type_recover_all:
                strType = "recover_all"; 
                break;
            case MsgsClientToSim::MsgUnitMagicAction_Type_recover_troops:
                strType = "recover_troops"; 
                break;
            case MsgsClientToSim::MsgUnitMagicAction_Type_recover_equipments:
                strType = "recover_equipments"; 
                break;
            case MsgsClientToSim::MsgUnitMagicAction_Type_recover_resources:
                strType = "recover_resources"; 
                break;
            case MsgsClientToSim::MsgUnitMagicAction_Type_destroy_all:
                strType = "destroy_all"; 
                break;
            case MsgsClientToSim::MsgUnitMagicAction_Type_cancel_surrender:
                strType = "cancel_surrender"; 
                break;
            default:
                return;
            }

            // $$$$ _RC_ SBO 2010-05-17: use ActionFactory
            MagicActionType& actionType = static_cast< tools::Resolver< MagicActionType, std::string >& > ( static_.types_ ).Get( strType );
            UnitMagicAction* action = new UnitMagicAction( agent, actionType, controllers_.controller_, true );
            action->Attach( *new ActionTiming( controllers_.controller_, simulation_, *action ) );
            action->Attach( *new ActionTasker( agent, false ) );
            action->RegisterAndPublish( actionsModel_ );
        }
    private:
        MagicFunctor& operator=( const MagicFunctor& );
        const ::StaticModel& static_;
        kernel::Controllers& controllers_;
        actions::ActionsModel& actionsModel_;
        const kernel::Time_ABC& simulation_;
        int id_;
    };

    struct RecursiveMagicFunctor : public MagicFunctor
    {
        RecursiveMagicFunctor( const ::StaticModel& staticModel, Controllers& controllers, actions::ActionsModel& actionsModel, const kernel::Time_ABC& simulation, int id )
            : MagicFunctor( staticModel, controllers, actionsModel, simulation, id )
        {}
        void operator()( const Entity_ABC& entity ) const
        {
            if( const Agent_ABC* agent = dynamic_cast< const Agent_ABC* >( &entity ) )
                MagicFunctor::operator()( *agent );
            const Hierarchies* h = entity.Retrieve< CommunicationHierarchies >();
            if( ! h )
                h = entity.Retrieve< TacticalHierarchies >();
            if( h )
            {
                tools::Iterator< const Entity_ABC& > it = h->CreateSubordinateIterator();
                while( it.HasMoreElements() )
                {
                    const Entity_ABC& entity = it.NextElement();
                    operator()( entity ); 
                }
            }
        }
    };
}

// -----------------------------------------------------------------------------
// Name: UnitMagicOrdersInterface::Magic
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void UnitMagicOrdersInterface::Magic( int type )
{
    if( selectedEntity_ )
        ApplyOnHierarchy( *selectedEntity_, type );
    selectedEntity_ = 0;
}

// -----------------------------------------------------------------------------
// Name: UnitMagicOrdersInterface::DestroyComponent
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void UnitMagicOrdersInterface::DestroyComponent()
{
    if( selectedEntity_ )
    {
        CreateAndPublish( "destroy_component" );
    }
}

// -----------------------------------------------------------------------------
// Name: UnitMagicOrdersInterface::Move
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void UnitMagicOrdersInterface::Move()
{
    if( magicMove_ )
        return;
    controllers_.Register( *magicMoveLocation_ );
    magicMove_ = true;
    magicMoveLocation_->StartPoint();
}

// -----------------------------------------------------------------------------
// Name: UnitMagicOrdersInterface::RecoverHumanTransporters
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void UnitMagicOrdersInterface::RecoverHumanTransporters()
{
    if( selectedEntity_ )
    {
        CreateAndPublish( "recover_transporters" );
    }
}

// -----------------------------------------------------------------------------
// Name: UnitMagicOrdersInterface::SurrenderTo
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void UnitMagicOrdersInterface::SurrenderTo( int teamPtr )
{
    if( selectedEntity_ )
    {
        // $$$$ _RC_ SBO 2010-05-17: use ActionFactory
        MagicActionType& actionType = static_cast< tools::Resolver< MagicActionType, std::string >& > ( static_.types_ ).Get( "surrender" );
        UnitMagicAction* action = new UnitMagicAction( *selectedEntity_, actionType, controllers_.controller_, true );
        tools::Iterator< const OrderParameter& > it = actionType.CreateIterator();
        action->AddParameter( *new parameters::Army( it.NextElement(), *( Team_ABC* ) teamPtr, controllers_.controller_ ) );
        action->Attach( *new ActionTiming( controllers_.controller_, simulation_, *action ) );
        action->Attach( *new ActionTasker( *selectedEntity_, false ) );
        action->RegisterAndPublish( actionsModel_ );
    }
}

// -----------------------------------------------------------------------------
// Name: UnitMagicOrdersInterface::AddMagic
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void UnitMagicOrdersInterface::AddMagic( const QString& label, int id, QPopupMenu* menu )
{
    const int nId = menu->insertItem( label, this, SLOT( Magic( int ) ) );
    menu->setItemParameter( nId, id );
}

// -----------------------------------------------------------------------------
// Name: UnitMagicOrdersInterface::AddMagic
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
int UnitMagicOrdersInterface::AddMagic( const QString& label, const char* slot, QPopupMenu* menu )
{
    return menu->insertItem( label, this, slot );
}

// -----------------------------------------------------------------------------
// Name: UnitMagicOrdersInterface::ApplyOnHierarchy
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void UnitMagicOrdersInterface::ApplyOnHierarchy( const kernel::Entity_ABC& entity, int id )
{
    RecursiveMagicFunctor functor( static_, controllers_, actionsModel_, simulation_, id);
    functor( entity );
}

// -----------------------------------------------------------------------------
// Name: UnitMagicOrdersInterface::FillCommonOrders
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void UnitMagicOrdersInterface::FillCommonOrders( QPopupMenu* magicMenu )
{
    AddMagic( tr( "Recover - All" ),        MsgsClientToSim::MsgUnitMagicAction_Type_recover_all,      magicMenu );
    AddMagic( tr( "Recover - Troops" ),     MsgsClientToSim::MsgUnitMagicAction_Type_recover_troops,  magicMenu );
    AddMagic( tr( "Recover - Equipments" ), MsgsClientToSim::MsgUnitMagicAction_Type_recover_equipments, magicMenu );
    AddMagic( tr( "Recover - Resources" ),  MsgsClientToSim::MsgUnitMagicAction_Type_recover_resources, magicMenu );
    AddMagic( tr( "Destroy - All" ),        MsgsClientToSim::MsgUnitMagicAction_Type_destroy_all,        magicMenu );
}

// -----------------------------------------------------------------------------
// Name: UnitMagicOrdersInterface::CreateAndPublish
// Created: JSR 2010-04-13
// -----------------------------------------------------------------------------
void UnitMagicOrdersInterface::CreateAndPublish( const std::string& actionStr )
{
    // $$$$ _RC_ SBO 2010-05-17: use ActionFactory
    MagicActionType& actionType = static_cast< tools::Resolver< MagicActionType, std::string >& > ( static_.types_ ).Get( actionStr );
    UnitMagicAction* action = new UnitMagicAction( *selectedEntity_, actionType, controllers_.controller_, true );
    action->Attach( *new ActionTiming( controllers_.controller_, simulation_, *action ) );
    action->Attach( *new ActionTasker( *selectedEntity_, false ) );
    action->RegisterAndPublish( actionsModel_ );
}

namespace
{
    // $$$$ SBO 2007-03-08: temporary, surrendered status should be also present on Automat attributes...
    bool IsSurrendered( const kernel::Entity_ABC& entity )
    {
        if( const Attributes* attr = static_cast< const Attributes* >( entity.Retrieve< kernel::Attributes_ABC >() ) )
            if( attr->surrenderedTo_ )
                return true;
        if( const kernel::TacticalHierarchies* hierarchies = entity.Retrieve< kernel::TacticalHierarchies >() )
        {
            tools::Iterator< const kernel::Entity_ABC& > it = hierarchies->CreateSubordinateIterator();
            while( it.HasMoreElements() )
                if( IsSurrendered( it.NextElement() ) )
                    return true;
        }
        return false;
    }
}

// -----------------------------------------------------------------------------
// Name: UnitMagicOrdersInterface::AddSurrenderMenu
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void UnitMagicOrdersInterface::AddSurrenderMenu( QPopupMenu* parent, const kernel::Entity_ABC& entity )
{
    if( IsSurrendered( entity ) )
        AddMagic( tr( "Cancel surrender" ), MsgsClientToSim::MsgUnitMagicAction_Type_cancel_surrender, parent );
    else
    {
        const kernel::Entity_ABC& team = entity.Get< kernel::TacticalHierarchies >().GetTop();
        QPopupMenu* menu = new QPopupMenu( parent );
        for( CIT_Teams it = teams_.begin(); it != teams_.end(); ++it )
            if( *it != &team )
                menu->insertItem( (*it)->GetName(), this, SLOT( SurrenderTo( int ) ), 0, ( int ) *it );
        if( menu->count() )
            parent->insertItem( tr( "Surrender to" ), menu );
    }
}

// -----------------------------------------------------------------------------
// Name: UnitMagicOrdersInterface::NotifyCreated
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void UnitMagicOrdersInterface::NotifyCreated( const kernel::Team_ABC& team )
{
    teams_.push_back( &team );
}

// -----------------------------------------------------------------------------
// Name: UnitMagicOrdersInterface::NotifyDeleted
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void UnitMagicOrdersInterface::NotifyDeleted( const kernel::Team_ABC& team )
{
    T_Teams::iterator it = std::find( teams_.begin(), teams_.end(), &team );
    if( it != teams_.end() )
    {
        std::swap( *it, teams_.back() );
        teams_.pop_back();
    }
}
