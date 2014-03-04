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

#include "actions/ActionsModel.h"
#include "actions/ActionTasker.h"
#include "actions/ActionTiming.h"
#include "actions/Army.h"
#include "actions/Bool.h"
#include "actions/EngageMagicAction.h"
#include "actions/Point.h"
#include "actions/String.h"

#include "actions/UnitMagicAction.h"

#include "clients_gui/Decisions_ABC.h"
#include "clients_gui/EntityType.h"
#include "clients_gui/GlSelector.h"
#include "clients_gui/LocationCreator.h"
#include "clients_gui/Tools.h"

#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/AgentExtensions.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/DecisionalModel.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Location_ABC.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/Point.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Team_ABC.h"

#include "gaming/Attributes.h"
#include "gaming/LogisticConsigns.h"
#include "gaming/MagicOrders.h"
#include "gaming/StaticModel.h"

#include "protocol/SimulationSenders.h"

#pragma warning( push, 0 )
#include <google/protobuf/message.h>
#include <google/protobuf/descriptor.h>
#pragma warning( pop )
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>

using namespace kernel;
using namespace gui;
using namespace actions;

// -----------------------------------------------------------------------------
// Name: UnitMagicOrdersInterface constructor
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
UnitMagicOrdersInterface::UnitMagicOrdersInterface( QWidget* parent,
                                                    kernel::Controllers& controllers,
                                                    actions::ActionsModel& actionsModel,
                                                    const ::StaticModel& staticModel,
                                                    const kernel::Time_ABC& simulation,
                                                    ::gui::ParametersLayer& layer,
                                                    const kernel::Profile_ABC& profile,
                                                    gui::GlSelector& selector )
    : QObject( parent )
    , controllers_   ( controllers )
    , actionsModel_  ( actionsModel )
    , static_        ( staticModel )
    , simulation_    ( simulation )
    , profile_       ( profile )
    , selector_      ( selector )
    , selectedEntity_( controllers )
    , magicMove_     ( false )
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

    kernel::ContextMenu* magicMenu = menu.SubMenu( "Order", tools::translate( "Magic orders", "Magic orders" ), false, 1 );
    AddMagic( tr( "Teleport" ), SLOT( Move() ), magicMenu );
    AddSurrenderMenu( magicMenu, agent );
    const gui::Decisions_ABC* decisions = agent.Retrieve< gui::Decisions_ABC >();
    if( decisions->IsDebugActivated() )
        AddMagic( tr( "Deactivate brain debug" ), SLOT( DeactivateBrainDebug() ), magicMenu );
    else
        AddMagic( tr( "Activate brain debug" ), SLOT( ActivateBrainDebug() ), magicMenu );
    AddReloadBrainMenu( magicMenu, static_.types_.unitModels_,
        agent.Retrieve< gui::Decisions_ABC >() ? agent.Retrieve< gui::Decisions_ABC >()->GetModelName() : "unknown",
        agent.GetType().GetDecisionalModel().GetName() );
    if( agent.Get< MagicOrders >().CanRetrieveTransporters() )
        AddMagic( tr( "Recover - Transporters" ), SLOT( RecoverHumanTransporters() ), magicMenu );
    AddMagic( tr( "Destroy - Component" ), SLOT( DestroyComponent() ), magicMenu );
    AddMagic( tr( "Delete unit" ), SLOT( DeleteUnit() ), magicMenu );

    const LogMaintenanceConsigns* maintenanceConsigns = agent.Retrieve< LogMaintenanceConsigns >();
    const LogMedicalConsigns* medicalConsigns = agent.Retrieve< LogMedicalConsigns >();
    const LogSupplyConsigns* supplyConsigns = agent.Retrieve< LogSupplyConsigns >();
    const LogFuneralConsigns* funeralConsigns = agent.Retrieve< LogFuneralConsigns >();
    if( maintenanceConsigns && maintenanceConsigns->IsHandlingConsigns() ||
        medicalConsigns && medicalConsigns->IsHandlingConsigns() ||
        supplyConsigns && supplyConsigns->IsHandlingConsigns() ||
        funeralConsigns && funeralConsigns->IsHandlingConsigns() )
        AddMagic( tr( "Finish logistic handlings" ), SLOT( FinishLogisticHandlings() ), magicMenu );
    FillCommonOrders( magicMenu );
}

// -----------------------------------------------------------------------------
// Name: UnitMagicOrdersInterface::NotifyContextMenu
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void UnitMagicOrdersInterface::NotifyContextMenu( const kernel::Automat_ABC& agent, kernel::ContextMenu& menu )
{
    selectedEntity_ = &agent;
    // Display if the profile controls the automat
    if( profile_.CanBeOrdered( agent ) )
        if( tools::IsEngaged( agent ) )
            menu.InsertItem( "Command", tr( "Disengage" ), this, SLOT( Disengage() ) );
        else
            menu.InsertItem( "Command", tr( "Engage" ), this, SLOT( Engage() ) );

    // Display if the profile is supervisor
    if( !profile_.CanDoMagic( agent ) )
        return;

    kernel::ContextMenu* magicMenu = menu.SubMenu( "Order", tools::translate( "Magic orders", "Magic orders" ), false, 1 );
    int moveId = AddMagic( tr( "Teleport" ), SLOT( Move() ), magicMenu );
    bool bMoveAllowed = false;
    const gui::Decisions_ABC* decisions = agent.Retrieve< gui::Decisions_ABC >();
    if( decisions )
    {
        bMoveAllowed = decisions->CanBeOrdered();
        if( decisions->IsDebugActivated() )
            AddMagic( tr( "Deactivate brain debug" ), SLOT( DeactivateBrainDebug() ), magicMenu );
        else
            AddMagic( tr( "Activate brain debug" ), SLOT( ActivateBrainDebug() ), magicMenu );
    }
    magicMenu->setItemEnabled( moveId, bMoveAllowed );
    AddReloadBrainMenu( magicMenu, static_.types_.automatModels_, decisions ? decisions->GetModelName() : "unknown",
        agent.Get< gui::EntityType< kernel::AutomatType > >().GetType().GetDecisionalModel().GetName() );
    AddSurrenderMenu( magicMenu, agent );

    const LogMaintenanceConsigns* maintenanceConsigns = agent.Retrieve< LogMaintenanceConsigns >();
    const LogMedicalConsigns* medicalConsigns = agent.Retrieve< LogMedicalConsigns >();
    const LogSupplyConsigns* supplyConsigns = agent.Retrieve< LogSupplyConsigns >();
    const LogFuneralConsigns* funeralConsigns = agent.Retrieve< LogFuneralConsigns >();
    if( maintenanceConsigns && maintenanceConsigns->IsHandlingConsigns() ||
        medicalConsigns && medicalConsigns->IsHandlingConsigns() ||
        supplyConsigns && supplyConsigns->IsHandlingConsigns() ||
        funeralConsigns && funeralConsigns->IsHandlingConsigns() )
        AddMagic( tr( "Finish logistic handlings" ), SLOT( FinishLogisticHandlings() ), magicMenu );
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
    kernel::ContextMenu* magicMenu = menu.SubMenu( "Order", tools::translate( "Magic orders", "Magic orders" ), false, 1 );
    FillCommonOrders( magicMenu );
}

// -----------------------------------------------------------------------------
// Name: UnitMagicOrdersInterface::NotifyContextMenu
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void UnitMagicOrdersInterface::NotifyContextMenu( const kernel::Team_ABC& entity, kernel::ContextMenu& menu )
{
    if( !profile_.CanDoMagic( entity ) || entity.GetId() == 0 ) // no side team
        return;
    selectedEntity_ = &entity;
    kernel::ContextMenu* magicMenu = menu.SubMenu( "Order", tools::translate( "Magic orders", "Magic orders" ), false, 1 );
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
            try
            {
                // $$$$ _RC_ SBO 2010-05-17: use ActionFactory
                MagicActionType& actionType = static_cast< tools::Resolver< MagicActionType, std::string >& > ( static_.types_ ).Get( "teleport" );
                std::unique_ptr< Action_ABC > action( new UnitMagicAction( actionType, controllers_.controller_, false ) );
                tools::Iterator< const OrderParameter& > it = actionType.CreateIterator();
                action->AddParameter( *new parameters::Point( it.NextElement(), static_.coordinateConverter_, location ) );
                action->Attach( *new ActionTiming( controllers_.controller_, simulation_ ) );
                action->Attach( *new ActionTasker( controllers_.controller_, selectedEntity_, false ) );
                actionsModel_.Publish( *action );
            }
            catch( ... )
            {
            }
        }
    }
    controllers_.Unregister( *magicMoveLocation_ );
    magicMove_ = false;
}

namespace
{
    struct MagicFunctor
    {
        MagicFunctor( const ::StaticModel& staticModel,
                      kernel::Controllers& controllers,
                      actions::ActionsModel& actionsModel,
                      const kernel::Time_ABC& simulation,
                      sword::UnitMagicAction::Type id )
            : static_( staticModel )
            , controllers_( controllers )
            , actionsModel_( actionsModel)
            , simulation_( simulation )
            , type_( ENT_Tr::ConvertFromUnitMagicActionType( id, ENT_Tr::eToSim ) )
        {}

        void operator()( const Agent_ABC& agent ) const
        {
            // $$$$ _RC_ SBO 2010-05-17: use ActionFactory
            MagicActionType& actionType = static_cast< tools::Resolver< MagicActionType, std::string >& > ( static_.types_ ).Get( type_ );
            std::unique_ptr< Action_ABC > action( new UnitMagicAction( actionType, controllers_.controller_, false ) );
            action->Attach( *new ActionTiming( controllers_.controller_, simulation_ ) );
            action->Attach( *new ActionTasker( controllers_.controller_, &agent, false ) );
            actionsModel_.Publish( *action );
        }
    private:
        MagicFunctor& operator=( const MagicFunctor& );
        const ::StaticModel& static_;
        kernel::Controllers& controllers_;
        actions::ActionsModel& actionsModel_;
        const kernel::Time_ABC& simulation_;
        const std::string type_;
    };

    struct RecursiveMagicFunctor : public MagicFunctor
    {
        RecursiveMagicFunctor( const ::StaticModel& staticModel,
                               Controllers& controllers,
                               actions::ActionsModel& actionsModel,
                               const kernel::Time_ABC& simulation,
                               sword::UnitMagicAction::Type id )
            : MagicFunctor( staticModel, controllers, actionsModel, simulation, id )
        {}
        void operator()( const kernel::Entity_ABC& entity ) const
        {
            if( const Agent_ABC* agent = dynamic_cast< const Agent_ABC* >( &entity ) )
                MagicFunctor::operator()( *agent );
            if( const Hierarchies* h = entity.Retrieve< TacticalHierarchies >() )
            {
                tools::Iterator< const kernel::Entity_ABC& > it = h->CreateSubordinateIterator();
                while( it.HasMoreElements() )
                {
                    const kernel::Entity_ABC& entity = it.NextElement();
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
    if( type == sword::UnitMagicAction_Type_recover_all
     || type == sword::UnitMagicAction_Type_recover_equipments
     || type == sword::UnitMagicAction_Type_recover_resources
     || type == sword::UnitMagicAction_Type_recover_troops )
    {
        QMessageBox msgBox( QMessageBox::NoIcon
            , tr( "Recovery options" )
            , tr( "Also recover the elements already managed by the logistic system?" )
            , QMessageBox::Yes | QMessageBox::No );
        if( msgBox.exec() == QMessageBox::No )
        {
            if( type == sword::UnitMagicAction_Type_recover_all )
                type = sword::UnitMagicAction_Type_recover_all_except_log;
            else if( type == sword::UnitMagicAction_Type_recover_equipments )
                type = sword::UnitMagicAction_Type_recover_equipments_except_log;
            else if( type == sword::UnitMagicAction_Type_recover_resources )
                type = sword::UnitMagicAction_Type_recover_resources_except_log;
            else if( type == sword::UnitMagicAction_Type_recover_troops )
                type = sword::UnitMagicAction_Type_recover_troops_except_log;
        }
    }
    if( !sword::UnitMagicAction::Type_IsValid( type ) )
        throw MASA_EXCEPTION( "Invalid sword::UnitMagicActionType " + type );
    if( selectedEntity_ )
        ApplyOnHierarchy( *selectedEntity_, static_cast< sword::UnitMagicAction::Type >( type ) );
    selectedEntity_ = 0;
}

// -----------------------------------------------------------------------------
// Name: UnitMagicOrdersInterface::DestroyComponent
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void UnitMagicOrdersInterface::DestroyComponent()
{
    if( selectedEntity_ )
        CreateAndPublish( "destroy_component" );
}

// -----------------------------------------------------------------------------
// Name: UnitMagicOrdersInterface::DeleteUnit
// Created: JSR 2013-01-29
// -----------------------------------------------------------------------------
void UnitMagicOrdersInterface::DeleteUnit()
{
    if( selectedEntity_)
        if( QMessageBox::warning( QApplication::activeWindow(), tr( "Confirmation" ), tr( "Delete '%1'?" ).arg( selectedEntity_->GetName() ), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes ) == QMessageBox::Yes )
            CreateAndPublish( "delete_unit" );
}

// -----------------------------------------------------------------------------
// Name: UnitMagicOrdersInterface::Move
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void UnitMagicOrdersInterface::Move()
{
    if( !magicMove_ )
        controllers_.Register( *magicMoveLocation_ );
    magicMove_ = true;
    magicMoveLocation_->StartPoint();
    selector_.SetFocus();
}

// -----------------------------------------------------------------------------
// Name: UnitMagicOrdersInterface::RecoverHumanTransporters
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void UnitMagicOrdersInterface::RecoverHumanTransporters()
{
    if( selectedEntity_ )
        CreateAndPublish( "recover_transporters" );
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
        std::unique_ptr< Action_ABC > action( new UnitMagicAction( actionType, controllers_.controller_, false ) );
        tools::Iterator< const OrderParameter& > it = actionType.CreateIterator();
        action->AddParameter( *new parameters::Army( it.NextElement(), *( Team_ABC* ) teamPtr, controllers_.controller_ ) );
        action->Attach( *new ActionTiming( controllers_.controller_, simulation_ ) );
        action->Attach( *new ActionTasker( controllers_.controller_, selectedEntity_, false ) );
        actionsModel_.Publish( *action );
    }
}

// -----------------------------------------------------------------------------
// Name: UnitMagicOrdersInterface::ReloadBrain
// Created: LDC 2011-08-18
// -----------------------------------------------------------------------------
void UnitMagicOrdersInterface::ReloadBrain( QAction* action )
{
    if( selectedEntity_ )
    {
        std::string modelName = action->text();
        MagicActionType& actionType = static_cast< tools::Resolver< MagicActionType, std::string >& > ( static_.types_ ).Get( "reload_brain" );
        std::unique_ptr< Action_ABC > action( new UnitMagicAction( actionType, controllers_.controller_, false ) );
        tools::Iterator< const OrderParameter& > it = actionType.CreateIterator();
        action->AddParameter( *new parameters::String( it.NextElement(), modelName ) );
        action->Attach( *new ActionTiming( controllers_.controller_, simulation_ ) );
        action->Attach( *new ActionTasker( controllers_.controller_, selectedEntity_, false ) );
        actionsModel_.Publish( *action );
    }
}

// -----------------------------------------------------------------------------
// Name: UnitMagicOrdersInterface::ActivateBrainDebug
// Created: SLI 2013-06-18
// -----------------------------------------------------------------------------
void UnitMagicOrdersInterface::ActivateBrainDebug()
{
    if( selectedEntity_ )
    {
        MagicActionType& actionType = static_cast< tools::Resolver< MagicActionType, std::string >& > ( static_.types_ ).Get( "change_brain_debug" );
        std::unique_ptr< Action_ABC > action( new UnitMagicAction( actionType, controllers_.controller_, false ) );
        tools::Iterator< const OrderParameter& > it = actionType.CreateIterator();
        action->AddParameter( *new actions::parameters::Bool( it.NextElement(), true ) );
        action->Attach( *new ActionTiming( controllers_.controller_, simulation_ ) );
        action->Attach( *new ActionTasker( controllers_.controller_, selectedEntity_, false ) );
        actionsModel_.Publish( *action );
    }
}

// -----------------------------------------------------------------------------
// Name: UnitMagicOrdersInterface::DeactivateBrainDebug
// Created: SLI 2013-06-19
// -----------------------------------------------------------------------------
void UnitMagicOrdersInterface::DeactivateBrainDebug()
{
    if( selectedEntity_ )
    {
        MagicActionType& actionType = static_cast< tools::Resolver< MagicActionType, std::string >& > ( static_.types_ ).Get( "change_brain_debug" );
        std::unique_ptr< Action_ABC > action( new UnitMagicAction( actionType, controllers_.controller_, false ) );
        tools::Iterator< const OrderParameter& > it = actionType.CreateIterator();
        action->AddParameter( *new actions::parameters::Bool( it.NextElement(), false ) );
        action->Attach( *new ActionTiming( controllers_.controller_, simulation_ ) );
        action->Attach( *new ActionTasker( controllers_.controller_, selectedEntity_, false ) );
        actionsModel_.Publish( *action );
    }
}

// -----------------------------------------------------------------------------
// Name: UnitMagicOrdersInterface::FinishLogisticHandlings
// Created: NLD 2012-01-09
// -----------------------------------------------------------------------------
void UnitMagicOrdersInterface::FinishLogisticHandlings()
{
    if( selectedEntity_ )
    {
        MagicActionType& actionType = static_cast< tools::Resolver< MagicActionType, std::string >& > ( static_.types_ ).Get( "log_finish_handlings" );
        std::unique_ptr< Action_ABC > action( new UnitMagicAction( actionType, controllers_.controller_, false ) );
        action->Attach( *new ActionTiming( controllers_.controller_, simulation_ ) );
        action->Attach( *new ActionTasker( controllers_.controller_, selectedEntity_, false ) );
        actionsModel_.Publish( *action );
    }
}

// -----------------------------------------------------------------------------
// Name: UnitMagicOrdersInterface::AddMagic
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void UnitMagicOrdersInterface::AddMagic( const QString& label, sword::UnitMagicAction::Type id, kernel::ContextMenu* menu )
{
    const int nId = menu->insertItem( label, this, SLOT( Magic( int ) ) );
    menu->setItemParameter( nId, id );
}

// -----------------------------------------------------------------------------
// Name: UnitMagicOrdersInterface::AddMagic
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
int UnitMagicOrdersInterface::AddMagic( const QString& label, const char* slot, kernel::ContextMenu* menu )
{
    return menu->insertItem( label, this, slot );
}

// -----------------------------------------------------------------------------
// Name: UnitMagicOrdersInterface::ApplyOnHierarchy
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void UnitMagicOrdersInterface::ApplyOnHierarchy( const kernel::Entity_ABC& entity, sword::UnitMagicAction::Type id )
{
    RecursiveMagicFunctor functor( static_, controllers_, actionsModel_, simulation_, id );
    functor( entity );
}

// -----------------------------------------------------------------------------
// Name: UnitMagicOrdersInterface::FillCommonOrders
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
void UnitMagicOrdersInterface::FillCommonOrders( kernel::ContextMenu* magicMenu )
{
    AddMagic( tr( "Recover - All" ),        sword::UnitMagicAction_Type_recover_all,      magicMenu );
    AddMagic( tr( "Recover - Troops" ),     sword::UnitMagicAction_Type_recover_troops,  magicMenu );
    AddMagic( tr( "Recover - Equipments" ), sword::UnitMagicAction_Type_recover_equipments, magicMenu );
    AddMagic( tr( "Recover - Resources" ),  sword::UnitMagicAction_Type_recover_resources, magicMenu );
    AddMagic( tr( "Destroy - All" ),        sword::UnitMagicAction_Type_destroy_all,        magicMenu );
}

// -----------------------------------------------------------------------------
// Name: UnitMagicOrdersInterface::CreateAndPublish
// Created: JSR 2010-04-13
// -----------------------------------------------------------------------------
void UnitMagicOrdersInterface::CreateAndPublish( const std::string& actionStr )
{
    // $$$$ _RC_ SBO 2010-05-17: use ActionFactory
    MagicActionType& actionType = static_cast< tools::Resolver< MagicActionType, std::string >& > ( static_.types_ ).Get( actionStr );
    std::unique_ptr< Action_ABC > action( new UnitMagicAction( actionType, controllers_.controller_, false ) );
    action->Attach( *new ActionTiming( controllers_.controller_, simulation_ ) );
    action->Attach( *new ActionTasker( controllers_.controller_, selectedEntity_, false ) );
    actionsModel_.Publish( *action );
}

namespace
{
    // $$$$ SBO 2007-03-08: temporary, surrendered status should be also present on Automat attributes...
    bool IsSurrendered( const kernel::Entity_ABC& entity )
    {
        if( const Attributes* attr = entity.Retrieve< Attributes >() )
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
void UnitMagicOrdersInterface::AddSurrenderMenu( kernel::ContextMenu* parent, const kernel::Entity_ABC& entity )
{
    if( IsSurrendered( entity ) )
        AddMagic( tr( "Cancel surrender" ), sword::UnitMagicAction_Type_cancel_surrender, parent );
    else
    {
        const kernel::Entity_ABC& team = entity.Get< kernel::TacticalHierarchies >().GetTop();
        kernel::ContextMenu* menu = new kernel::ContextMenu( parent );
        for( auto it = teams_.begin(); it != teams_.end(); ++it )
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

// -----------------------------------------------------------------------------
// Name: UnitMagicOrdersInterface::AddReloadBrainMenu
// Created: AHC 2012-01-13
// -----------------------------------------------------------------------------
void UnitMagicOrdersInterface::AddReloadBrainMenu( QMenu* parent, const tools::StringResolver< DecisionalModel >& models,
    const std::string& currentModel, const std::string& defaultModel)
{
    QMenu* menu = new QMenu( tr( "Reload brain" ), parent );
    tools::Iterator< const kernel::DecisionalModel& > it( models.CreateIterator() );
    menu->addAction( currentModel.c_str() ) ;
    if( defaultModel != currentModel )
        menu->addAction( defaultModel.c_str() ) ;
    std::map< char, QMenu* > subMenus;
    while( it.HasMoreElements() )
    {
        const kernel::DecisionalModel& value = it.NextElement();
        const std::string& name(value.GetName());
        std::string nameCpy(name);
        boost::to_lower( nameCpy );
        char key = nameCpy[0];
        std::map<char, QMenu*>::iterator subIt = subMenus.find(key);
        QMenu* sub = 0;
        if( subMenus.end() == subIt )
        {
            sub = new QMenu( QChar(key), menu);
            menu->addMenu( sub );
            subMenus[key]=sub;
        }
        else
            sub = subIt->second;
        sub->addAction( name.c_str() );
    }
    connect(menu, SIGNAL( triggered(QAction*) ), this, SLOT( ReloadBrain(QAction*) ) );
    parent->addMenu( menu );
}

// -----------------------------------------------------------------------------
// Name: UnitMagicOrdersInterface::Engage
// Created: SBO 2006-06-19
// -----------------------------------------------------------------------------
void UnitMagicOrdersInterface::Engage()
{
    if( !selectedEntity_ )
        return;
    kernel::MagicActionType& actionType = static_cast< tools::Resolver< kernel::MagicActionType, std::string >& > ( static_.types_ ).Get( "change_mode" );
    std::unique_ptr< actions::EngageMagicAction > action( new actions::EngageMagicAction( actionType, controllers_.controller_, true ) );
    action->Attach( *new actions::ActionTiming( controllers_.controller_, simulation_ ) );
    action->Attach( *new actions::ActionTasker( controllers_.controller_, selectedEntity_, false ) );
    actionsModel_.Publish( *action );
}

// -----------------------------------------------------------------------------
// Name: UnitMagicOrdersInterface::Disengage
// Created: SBO 2006-06-19
// -----------------------------------------------------------------------------
void UnitMagicOrdersInterface::Disengage()
{
    if( !selectedEntity_ )
        return;
    kernel::MagicActionType& actionType = static_cast< tools::Resolver< kernel::MagicActionType, std::string >& > ( static_.types_ ).Get( "change_mode" );
    std::unique_ptr< actions::EngageMagicAction > action( new actions::EngageMagicAction( actionType, controllers_.controller_, false ) );
    action->Attach( *new actions::ActionTiming( controllers_.controller_, simulation_ ) );
    action->Attach( *new actions::ActionTasker( controllers_.controller_, selectedEntity_, false ) );
    actionsModel_.Publish( *action );
}
