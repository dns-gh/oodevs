// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "UnitMagicOrdersInterface.h"
#include "moc_UnitMagicOrdersInterface.cpp"

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

// -----------------------------------------------------------------------------
// Name: UnitMagicOrdersInterface constructor
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
UnitMagicOrdersInterface::UnitMagicOrdersInterface( QWidget* parent, kernel::Controllers& controllers, Publisher_ABC& publisher, const StaticModel& staticModel, gui::ParametersLayer& layer, const kernel::Profile_ABC& profile )
    : QObject( parent )
    , controllers_( controllers )
    , publisher_( publisher )
    , static_( staticModel )
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
        location.Accept( *this );
    controllers_.Unregister( *magicMoveLocation_ );
    magicMove_ = false;
}

namespace
{
    struct MagicFunctor
    {
        MagicFunctor( Publisher_ABC& publisher, int id ) : publisher_( publisher ), id_( id ) {};
        void operator()( const Agent_ABC& agent ) const
        {
            simulation::UnitMagicAction magicAction;
            magicAction().set_oid( agent.GetId() );
            const google::protobuf::Reflection* reflect = magicAction().mutable_action()->GetReflection();
            const google::protobuf::Descriptor* descriptor = magicAction().mutable_action()->GetDescriptor();
            const google::protobuf::FieldDescriptor* field = descriptor->FindFieldByNumber( id_ );
            reflect->SetInt32(magicAction().mutable_action(), field, 1);
            magicAction.Send( publisher_ );
        }
    private:
        MagicFunctor& operator=( const MagicFunctor& );
        Publisher_ABC& publisher_;
        int id_;
    };

    struct RecursiveMagicFunctor : public MagicFunctor
    {
        RecursiveMagicFunctor( Publisher_ABC& publisher, int id ) : MagicFunctor( publisher, id ) {};
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
        simulation::UnitMagicAction magicAction;
        magicAction().set_oid( selectedEntity_->GetId() );
        magicAction().mutable_action()->set_destruction_composante( 1 );
        magicAction.Send( publisher_ );
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
        simulation::UnitMagicAction magicAction;
        magicAction().set_oid( selectedEntity_ ->GetId() );
        magicAction().mutable_action()->set_recuperer_transporteurs( 1 );
        magicAction.Send( publisher_ );
    }
}

// -----------------------------------------------------------------------------
// Name: UnitMagicOrdersInterface::SurrenderTo
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void UnitMagicOrdersInterface::SurrenderTo( int id )
{
    if( selectedEntity_ )
    {
        simulation::UnitMagicAction magicAction;
        magicAction().set_oid( selectedEntity_->GetId() );

        magicAction().mutable_action()->set_se_rendre( id );
        magicAction.Send( publisher_ );
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
    RecursiveMagicFunctor functor( publisher_, id );
    functor( entity );
}

// -----------------------------------------------------------------------------
// Name: UnitMagicOrdersInterface::FillCommonOrders
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void UnitMagicOrdersInterface::FillCommonOrders( QPopupMenu* magicMenu )
{
    AddMagic( tr( "Recover - All" ),        MsgsClientToSim::MsgUnitMagicAction_action::kRecompletementTotalFieldNumber,      magicMenu );
    AddMagic( tr( "Recover - Troops" ),     MsgsClientToSim::MsgUnitMagicAction_action::kRecompletementPersonnelFieldNumber,  magicMenu );
    AddMagic( tr( "Recover - Equipments" ), MsgsClientToSim::MsgUnitMagicAction_action::kRecompletementEquipementFieldNumber, magicMenu );
    AddMagic( tr( "Recover - Resources" ),  MsgsClientToSim::MsgUnitMagicAction_action::kRecompletementRessourcesFieldNumber, magicMenu );
    AddMagic( tr( "Destroy - All" ),        MsgsClientToSim::MsgUnitMagicAction_action::kDestructionTotaleFieldNumber,        magicMenu );
}

// -----------------------------------------------------------------------------
// Name: UnitMagicOrdersInterface::VisitPoint
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void UnitMagicOrdersInterface::VisitPoint( const geometry::Point2f& point )
{
    if( selectedEntity_ )
    {
        MsgCoordLatLong utm;
        static_.coordinateConverter_.ConvertToGeo( point, utm );
        simulation::MagicActionMoveTo message;
        message().set_oid( selectedEntity_->GetId() );
        Common::MsgLocation* location = message().mutable_parametres()->add_elem()->mutable_value()->mutable_point()->mutable_location();
        location->set_type( MsgLocation_Geometry_point );
        *location->mutable_coordinates()->add_elem() = utm;
        message.Send( publisher_ );
        const_cast< kernel::Entity_ABC& >( *selectedEntity_ ).Update( message() );
        message().clear_parametres();
    }
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
        AddMagic( tr( "Cancel surrender" ), MsgsClientToSim::MsgUnitMagicAction_action::kAnnulerRedditionFieldNumber, parent );
    else
    {
        const kernel::Entity_ABC& team = entity.Get< kernel::TacticalHierarchies >().GetTop();
        QPopupMenu* menu = new QPopupMenu( parent );
        for( CIT_Teams it = teams_.begin(); it != teams_.end(); ++it )
            if( *it != &team )
                menu->insertItem( (*it)->GetName(), this, SLOT( SurrenderTo( int ) ), 0, (*it)->GetId() );
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
