// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "actions_pch.h"
#include "ActionFactory.h"
#include "ActionTasker.h"
#include "ActionTiming.h"
#include "DotationType.h"
#include "EngageMagicAction.h"
#include "EntityMission.h"
#include "Enumeration.h"
#include "FragOrder.h"
#include "Identifier.h"
#include "KnowledgeGroupMagicAction.h"
#include "MagicAction.h"
#include "Numeric.h"
#include "ObjectMagicAction.h"
#include "Parameter_ABC.h"
#include "ParameterFactory_ABC.h"
#include "ParameterList.h"
#include "Point.h"
#include "Quantity.h"
#include "String.h"
#include "Bool.h"
#include "UnitMagicAction.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/EntityResolver_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/FragOrderType.h"
#include "clients_kernel/Inhabitant_ABC.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/MissionType.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/UrbanObject_ABC.h"
#include "clients_kernel/OrderParameter.h"
#include "clients_kernel/Point.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/PopulationType.h"
#include "clients_kernel/StaticModel.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Tools.h"
#include "protocol/Protocol.h"
#include <xeumeuleu/xml.hpp>

using namespace actions;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ActionFactory constructor
// Created: SBO 2010-05-07
// -----------------------------------------------------------------------------
ActionFactory::ActionFactory( kernel::Controller& controller,
                              const ParameterFactory_ABC& factory,
                              const kernel::EntityResolver_ABC& entities,
                              const kernel::StaticModel& staticModel,
                              const kernel::Time_ABC& simulation )
    : controller_         ( controller )
    , factory_            ( factory )
    , entities_           ( entities )
    , staticModel_        ( staticModel )
    , missions_           ( staticModel.types_ )
    , fragOrders_         ( staticModel.types_ )
    , magicActions_       ( staticModel.types_ )
    , coordinateConverter_( staticModel.coordinateConverter_ )
    , simulation_         ( simulation )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionFactory destructor
// Created: SBO 2010-05-07
// -----------------------------------------------------------------------------
ActionFactory::~ActionFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::AddTasker
// Created: ABR 2014-01-14
// -----------------------------------------------------------------------------
void ActionFactory::AddTasker( Action_ABC& action, const sword::Tasker& tasker, bool isSimulation /*= true*/ ) const
{
    unsigned int id = 0;
    std::string type;
    const kernel::Entity_ABC* entity = 0;
    if( tasker.has_automat() )
    {
        id = tasker.automat().id();
        entity = entities_.FindAutomat( id );
        type = kernel::Automat_ABC::typeName_;
    }
    else if( tasker.has_unit() )
    {
        id = tasker.unit().id();
        entity = entities_.FindAgent( id );
        type = kernel::Agent_ABC::typeName_;
    }
    else if( tasker.has_crowd() )
    {
        id = tasker.crowd().id();
        entity = entities_.FindPopulation( id );
        type = kernel::Population_ABC::typeName_;
    }
    else if( tasker.has_formation() )
    {
        id = tasker.formation().id();
        entity = entities_.FindFormation( id );
        type = kernel::Formation_ABC::typeName_;
    }
    else if( tasker.has_party() )
    {
        id = tasker.party().id();
        entity = entities_.FindTeam( id );
        type = kernel::Team_ABC::typeName_;
    }
    else if( tasker.has_population() )
    {
        id = tasker.population().id();
        entity = entities_.FindInhabitant( id );
        type = kernel::Inhabitant_ABC::typeName_;
    }
    if( !entity && id != 0 && !type.empty() )
        AddTasker( action, id, type, isSimulation );
    else
        AddTasker( action, entity, isSimulation );
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::AddTasker
// Created: ABR 2014-01-13
// -----------------------------------------------------------------------------
void ActionFactory::AddTasker( Action_ABC& action, unsigned int id, const std::string& type, bool isSimulation /*= true*/ ) const
{
    if( const kernel::Entity_ABC* entity = entities_.FindEntity( id ) )
        AddTasker( action, entity, isSimulation );
    else
    {
        action.Attach( *new ActionTasker( controller_, id, type, isSimulation ) );
        action.Invalidate();
    }
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::AddTasker
// Created: ABR 2014-01-13
// -----------------------------------------------------------------------------
void ActionFactory::AddTasker( Action_ABC& action, const kernel::Entity_ABC* entity, bool isSimulation /*= true*/ ) const
{
    action.Attach( *new ActionTasker( controller_, entity, isSimulation ) );
    if( !entity )
        action.Invalidate();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateAction
// Created: SBO 2010-05-07
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateAction( const kernel::Entity_ABC* target, const kernel::MissionType& mission ) const
{
    std::unique_ptr< Action_ABC > action;
    if( mission.GetType() == eMissionType_Pawn )
        action.reset( new AgentMission( &mission, controller_, true ) );
    if( mission.GetType() == eMissionType_Automat )
        action.reset( new AutomatMission( &mission, controller_, true ) );
    else if( mission.GetType() == eMissionType_Population )
        action.reset( new PopulationMission( &mission, controller_, true ) );

    action->Attach( *new ActionTiming( controller_, simulation_ ) );
    AddTasker( *action, target, false );
    action->Polish();
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateAction
// Created: SBO 2010-05-07
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateAction( const kernel::Entity_ABC* target, const kernel::FragOrderType& fragOrder ) const
{
    std::unique_ptr< Action_ABC > action( new FragOrder( &fragOrder, controller_, true ) );
    action->Attach( *new ActionTiming( controller_, simulation_ ) );
    AddTasker( *action, target, false );
    action->Polish();
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateAction
// Created: RCD 2011-07-06
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateAction( const kernel::Entity_ABC& target, const kernel::MagicActionType& magicAction ) const
{
    std::unique_ptr< Action_ABC > action( new MagicAction( magicAction, controller_, true ) );
    action->Attach( *new ActionTiming( controller_, simulation_ ) );
    AddTasker( *action, &target, false );
    action->Polish();
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateAction
// Created: ABR 2014-01-21
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateAction( const kernel::Entity_ABC* target, E_MissionType type ) const
{
    std::unique_ptr< Action_ABC > action;
    if( type == eMissionType_Pawn )
        action.reset( new AgentMission( 0, controller_, true ) );
    else if( type == eMissionType_Automat )
        action.reset( new AutomatMission( 0, controller_, true ) );
    else if( type == eMissionType_Population )
        action.reset( new PopulationMission( 0, controller_, true ) );
    else if( type == eMissionType_FragOrder )
        action.reset( new FragOrder( 0, controller_, true ) );
    else
        throw MASA_EXCEPTION( "Invalid mission type" );
    AddTasker( *action, target, false );
    action->Rename( target ? target->GetName() : tools::translate( "ActionFactory", "Incomplete order" ) );
    action->Invalidate();
    action->Polish();
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateAction
// Created: SBO 2010-05-07
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateAction( xml::xistream& xis, bool readonly /* = false*/ ) const
{
    const std::string type = xis.attribute< std::string >( "type" );
    if( type == "mission" )
        return CreateMission( xis, readonly );
    if( type == "fragorder" )
        return CreateFragOrder( xis, readonly );
    if( type == "magic" )
        return CreateMagicAction( xis, readonly );
    if( type == "magicunit" )
        return CreateUnitMagicAction( xis, readonly );
    if( type == "magicobject" )
        return CreateObjectMagicAction( xis, readonly );
    if( type == "magicknowledge" )
        return CreateKnowledgeGroupMagicAction( xis, readonly );
    if( type == "change_mode" )
        return AutomateChangeModeMagicAction( xis, readonly );
    return 0;
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::AddTiming
// Created: SBO 2010-09-21
// -----------------------------------------------------------------------------
template< typename Message >
void ActionFactory::AddTiming( Action_ABC& action, const Message& message ) const
{
    if( message.has_start_time() )
        action.Attach( *new ActionTiming( controller_, simulation_, message.start_time().data() ) );
    else
        action.Attach( *new ActionTiming( controller_, simulation_ ) );
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateAction
// Created: ABR 2014-01-08
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateAction( const sword::ClientToSim& message, bool needRegistration ) const
{
    if( message.has_message() )
    {
        const sword::ClientToSim_Content& content = message.message();
        if( content.has_unit_order() )
            return CreateAction( content.unit_order(), needRegistration );
        else if( content.has_automat_order() )
            return CreateAction( content.automat_order(), needRegistration );
        else if( content.has_crowd_order() )
            return CreateAction( content.crowd_order(), needRegistration );
        else if( content.has_frag_order() )
            return CreateAction( content.frag_order(), needRegistration );
        else if( content.has_set_automat_mode() )
            return CreateAction( content.set_automat_mode(), needRegistration );
        else if( content.has_magic_action() )
            return CreateAction( content.magic_action(), needRegistration );
        else if( content.has_unit_magic_action() )
            return CreateAction( content.unit_magic_action(), needRegistration );
        else if( content.has_knowledge_magic_action() )
            return CreateAction( content.knowledge_magic_action(), needRegistration );
        else if( content.has_object_magic_action() )
            return CreateAction( content.object_magic_action(), needRegistration );
    }
    throw MASA_EXCEPTION( "ActionFactory: Failed to create an action from this ClientToSim message" );
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateAction
// Created: SBO 2010-05-07
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateAction( const sword::UnitOrder& message, bool needRegistration ) const
{
    const kernel::MissionType* mission = missions_.Find( message.type().id() );
    std::unique_ptr< Action_ABC > action( new AgentMission( mission, controller_, needRegistration ) );
    AddTiming( *action, message );
    AddTasker( *action, message.tasker().id(), kernel::Agent_ABC::typeName_ );
    action->Polish();
    if( mission )
        AddParameters( *action, *mission, message.parameters() );
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateAction
// Created: SBO 2010-05-07
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateAction( const sword::AutomatOrder& message, bool needRegistration ) const
{
    const kernel::MissionType* mission = missions_.Find( message.type().id() );
    std::unique_ptr< Action_ABC > action( new AutomatMission( mission, controller_, needRegistration ) );
    AddTiming( *action, message );
    AddTasker( *action, message.tasker().id(), kernel::Automat_ABC::typeName_ );
    action->Polish();
    if( mission )
        AddParameters( *action, *mission, message.parameters() );
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateAction
// Created: SBO 2010-05-07
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateAction( const sword::CrowdOrder& message, bool needRegistration ) const
{
    const kernel::MissionType* mission = missions_.Find( message.type().id() );
    std::unique_ptr< Action_ABC > action( new PopulationMission( mission, controller_, needRegistration ) );
    AddTiming( *action, message );
    AddTasker( *action, message.tasker().id(), kernel::Population_ABC::typeName_ );
    action->Polish();
    if( mission )
        AddParameters( *action, *mission, message.parameters() );
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateAction
// Created: SBO 2010-05-07
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateAction( const sword::FragOrder& message, bool needRegistration ) const
{
    const kernel::FragOrderType* order = fragOrders_.Find( message.type().id() );
    std::unique_ptr< Action_ABC > action( new FragOrder( order, controller_, needRegistration ) );
    AddTiming( *action, message );
    AddTasker( *action, message.tasker() );
    action->Polish();
    if( order )
        AddParameters( *action, *order, message.parameters() );
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateAction
// Created: ABR 2014-01-08
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateAction( const sword::SetAutomatMode& message, bool needRegistration ) const
{
    std::unique_ptr< EngageMagicAction > action( new EngageMagicAction( magicActions_.Get( "change_mode" ),
                                                                        controller_,
                                                                        message.mode() == sword::engaged,
                                                                        needRegistration ) );
    action->Attach( *new ActionTiming( controller_, simulation_ ) );
    AddTasker( *action, message.automate().id(), kernel::Automat_ABC::typeName_ );
    action->Polish();
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateAction
// Created: ABR 2014-01-08
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateAction( const sword::MagicAction& message, bool needRegistration ) const
{
    auto type = message.type();
    auto& orderType = magicActions_.Get( ENT_Tr::ConvertFromMagicActionType( type, ENT_Tr::eToSim ) );
    std::unique_ptr< MagicAction > action( new MagicAction( orderType,
                                                            controller_,
                                                            needRegistration ) );
    action->Attach( *new ActionTiming( controller_, simulation_ ) );
    action->Polish();
    AddParameters( *action, orderType, message.parameters() );
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateAction
// Created: ABR 2014-01-08
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateAction( const sword::UnitMagicAction& message, bool needRegistration ) const
{
    auto& type = magicActions_.Get( ENT_Tr::ConvertFromUnitMagicActionType( message.type(), ENT_Tr::eToSim ) );
    std::unique_ptr< UnitMagicAction > action( new UnitMagicAction( type,
                                                                    controller_,
                                                                    needRegistration ) );
    action->Attach( *new ActionTiming( controller_, simulation_ ) );
    AddTasker( *action, message.tasker() );
    action->Polish();
    AddParameters( *action, type, message.parameters() );
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateAction
// Created: ABR 2014-01-08
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateAction( const sword::KnowledgeMagicAction& message, bool needRegistration ) const
{
    auto& type = magicActions_.Get( ENT_Tr::ConvertFromKnowledgeMagicActionType( message.type(), ENT_Tr::eToSim ) );
    std::unique_ptr< Action_ABC > action( new KnowledgeGroupMagicAction( type,
                                                                         controller_,
                                                                         needRegistration ) );
    action->Attach( *new ActionTiming( controller_, simulation_ ) );
    AddTasker( *action, message.knowledge_group().id(), kernel::KnowledgeGroup_ABC::typeName_ );
    action->Polish();
    AddParameters( *action, type, message.parameters() );
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateAction
// Created: ABR 2014-01-08
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateAction( const sword::ObjectMagicAction& message, bool needRegistration ) const
{
    auto& type = magicActions_.Get( ENT_Tr::ConvertFromObjectMagicActionType( message.type(), ENT_Tr::eToSim ) );
    std::unique_ptr< Action_ABC > action( new ObjectMagicAction( type,
                                                                 controller_,
                                                                 needRegistration ) );
    action->Attach( *new ActionTiming( controller_, simulation_ ) );
    AddTasker( *action, message.object().id(), kernel::Object_ABC::typeName_ );
    action->Polish();
    AddParameters( *action, type, message.parameters() );
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateMission
// Created: LDC 2010-07-28
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateMission( xml::xistream& xis, bool readonly ) const
{
    const unsigned long id = xis.attribute< unsigned long >( "target", 0 );
    std::unique_ptr< Mission > action;
    auto* type = missions_.Find( xis.attribute< unsigned int >( "id", 0 ) );
    const kernel::Entity_ABC* target = entities_.FindAgent( id );
    if( !target )
        target = entities_.FindAutomat( id );
    if( !target )
        target = entities_.FindPopulation( id );
    if( type )
    {
        if( type->GetType() == eMissionType_Automat )
            action.reset( new AutomatMission( type, controller_, xis ) );
        else if( type->GetType() == eMissionType_Population )
            action.reset( new PopulationMission( type, controller_, xis ) );
    }
    if( !action )
        action.reset( new AgentMission( type, controller_, xis ) );
    action->Attach( *new ActionTiming( xis, controller_, simulation_ ) );
    AddTasker( *action, target, readonly );
    action->Polish();
    AddParameters( *action, xis, target );
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateFragOrder
// Created: LDC 2010-07-28
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateFragOrder( xml::xistream& xis, bool readonly ) const
{
    const unsigned long id = xis.attribute< unsigned long >( "target", 0 );
    const kernel::Entity_ABC* target = entities_.FindAgent( id );
    if( !target )
        target = entities_.FindAutomat( id );
    if( !target )
        target = entities_.FindPopulation( id );
    std::unique_ptr< FragOrder > action( new FragOrder( xis, controller_, fragOrders_ ) );
    action->Attach( *new ActionTiming( xis, controller_, simulation_ ) );
    AddTasker( *action, target, readonly );
    action->Polish();
    AddParameters( *action, xis, target );
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateMagicAction
// Created: SBO 2010-05-07
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateMagicAction( xml::xistream& xis, bool ) const
{
    const std::string id = xis.attribute< std::string >( "id" );
    std::unique_ptr< MagicAction > action( new MagicAction( xis, controller_, magicActions_.Get( id ) ) );
    action->Attach( *new ActionTiming( xis, controller_, simulation_ ) );
    action->Polish();
    AddParameters( *action, xis, 0 );
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateUnitMagicAction
// Created: SBO 2010-05-07
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateUnitMagicAction( xml::xistream& xis, bool readonly ) const
{
    const unsigned long targetid = xis.attribute< unsigned long >( "target", 0 );
    const std::string id = xis.attribute< std::string >( "id" );
    const kernel::Entity_ABC* target = entities_.FindAgent( targetid );
    if( !target )
        target = entities_.FindAutomat( targetid );
    if( !target )
        target = entities_.FindPopulation( targetid );
    if( !target )
        target = entities_.FindFormation( targetid );
    if( !target )
        target = entities_.FindTeam( targetid );
    std::unique_ptr< UnitMagicAction > action( new UnitMagicAction( xis, controller_, magicActions_.Get( id ) ) );
    action->Attach( *new ActionTiming( xis, controller_, simulation_ ) );
    AddTasker( *action, target, readonly );
    action->Polish();
    AddParameters( *action, xis, target );
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::AutomateChangeModeMagicAction
// Created: FDS 2010-11-23
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::AutomateChangeModeMagicAction( xml::xistream& xis, bool readonly ) const
{
    const unsigned long targetid = xis.attribute< unsigned long >( "target", 0 );
    const std::string type = xis.attribute< std::string >( "type" );
    std::string name = xis.attribute< std::string >( "name", "" );
    bool engaged = xis.attribute< bool >( "engaged" );
    const kernel::Entity_ABC* target = entities_.FindAutomat( targetid );
    std::unique_ptr< EngageMagicAction > action( new EngageMagicAction( xis, controller_, magicActions_.Get( type ), engaged ) );
    action->Attach( *new ActionTiming( xis, controller_, simulation_ ) );
    AddTasker( *action, target, readonly );
    action->Polish();
    return action.release();
}
// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateObjectMagicAction
// Created: SBO 2010-05-07
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateObjectMagicAction( xml::xistream& xis, bool readonly ) const
{
    const std::string id = xis.attribute< std::string >( "id" );
    kernel::Entity_ABC* target = 0;
    if( id != "create_object" )
    {
        const unsigned long targetid = xis.attribute< unsigned long >( "target", 0 );
        target = entities_.FindObject( targetid );
        if( !target )
            target = entities_.FindUrbanObject( targetid );
    }

    std::unique_ptr< Action_ABC > action( new ObjectMagicAction( xis, controller_, magicActions_.Get( id ) ) );
    action->Attach( *new ActionTiming( xis, controller_, simulation_ ) );
    AddTasker( *action, target, readonly );
    action->Polish();
    AddParameters( *action, xis, target );
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateObjectMagicAction
// Created: JCR 2011-01-04
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateObjectMagicAction( const std::string& magicAction, unsigned long targetId ) const
{
    kernel::Entity_ABC* target = ( magicAction != "create_object" ) ? entities_.FindObject( targetId ) : 0;
    std::unique_ptr< Action_ABC > action( new ObjectMagicAction( magicActions_.Get( magicAction ), controller_, true ) );
    action->Attach( *new ActionTiming( controller_, simulation_ ) );
    AddTasker( *action, target, false );
    action->Polish();
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateObjectUpdateMagicAction
// Created: JSR 2011-03-01
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateObjectUpdateMagicAction( const kernel::Entity_ABC& object, parameters::ParameterList& attribute ) const
{
    std::unique_ptr< Action_ABC > action( new ObjectMagicAction( magicActions_.Get( "update_object" ), controller_, true ) );
    tools::Iterator< const OrderParameter& > it = action->GetType()->CreateIterator();
    parameters::ParameterList* attributesList = new parameters::ParameterList( it.NextElement() );
    action->AddParameter( *attributesList );
    attributesList->AddParameter( attribute );
    action->Attach( *new ActionTiming( controller_, simulation_ ) );
    AddTasker( *action, &object, false );
    action->Polish();
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateObjectDestroyMagicAction
// Created: JSR 2011-03-01
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateObjectDestroyMagicAction( const kernel::Entity_ABC& object ) const
{
    std::unique_ptr< Action_ABC > action;
    action.reset( new ObjectMagicAction( magicActions_.Get( "destroy_object" ), controller_, true ) );
    action->Attach( *new ActionTiming( controller_, simulation_ ) );
    AddTasker( *action, &object, false );
    action->Polish();
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateKnowledgeGroupMagicAction
// Created: SBO 2010-05-07
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateKnowledgeGroupMagicAction( xml::xistream& xis, bool readonly ) const
{
    const unsigned long targetid = xis.attribute< unsigned long >( "target", 0 );
    const std::string id = xis.attribute< std::string >( "id" );

    const kernel::Entity_ABC* target = entities_.FindKnowledgeGroup( targetid );
    std::unique_ptr< Action_ABC > action( new KnowledgeGroupMagicAction( xis, controller_, magicActions_.Get( id ) ) );
    action->Attach( *new ActionTiming( xis, controller_, simulation_ ) );
    AddTasker( *action, target, readonly );
    action->Polish();
    AddParameters( *action, xis, target );
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::AddParameters
// Created: ABR 2014-01-22
// -----------------------------------------------------------------------------
void ActionFactory::AddParameters( Action_ABC& action, xml::xistream& xis, const kernel::Entity_ABC* entity ) const
{
    if( action.GetType() )
    {
        tools::Iterator< const kernel::OrderParameter& > it = action.GetType()->CreateIterator();
        if( entity )
            xis >> xml::list( "parameter", *this, &ActionFactory::ReadParameter, action, it, *entity );
        else
            xis >> xml::list( "parameter", *this, &ActionFactory::ReadParameter, action, it, boost::none );
        if( it.HasMoreElements() )
            action.Invalidate();
    }
    else
        action.Invalidate();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::AddParameters
// Created: SBO 2010-05-07
// -----------------------------------------------------------------------------
void ActionFactory::AddParameters( Action_ABC& action, const kernel::OrderType& order, const sword::MissionParameters& message ) const
{
    tools::Iterator< const kernel::OrderParameter& > it = order.CreateIterator();
    if( static_cast< int >( order.Count() ) != message.elem_size() )
        action.Invalidate();
    for( int i = 0; it.HasMoreElements() && i < message.elem_size(); ++i )
    {
        const kernel::OrderParameter& orderParam = it.NextElement();
        const ActionTasker* tasker = action.Retrieve< ActionTasker >();
        const kernel::Entity_ABC* entityPtr = tasker ? entities_.FindEntity( tasker->GetId() ) : 0;
        boost::optional< const kernel::Entity_ABC& > entity = boost::none;
        if( entityPtr )
            entity = *entityPtr;
        std::unique_ptr< Parameter_ABC > param( factory_.CreateParameter( orderParam,
                                                                          message.elem( i ),
                                                                          entity ) );
        if( param )
            action.AddParameter( *param );
        else
            action.Invalidate();
        param.release();
    }
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::ReadParameter
// Created: SBO 2010-05-07
// -----------------------------------------------------------------------------
void ActionFactory::ReadParameter( xml::xistream& xis, Action_ABC& action, tools::Iterator< const kernel::OrderParameter& >& it, boost::optional< const kernel::Entity_ABC& > entity ) const
{
    std::unique_ptr< Parameter_ABC > param;
    if( it.HasMoreElements() )
        param.reset( factory_.CreateParameter( it.NextElement(), xis, entity ) );
    if( param )
        action.AddParameter( *param );
    else
        action.Invalidate();
    param.release();
}

namespace
{
    unsigned int FindKnowledgeGroupForArmy( const kernel::Entity_ABC& entity )
    {
        if( dynamic_cast< const kernel::KnowledgeGroup_ABC* >( &entity ) )
            return entity.GetId();
        const kernel::CommunicationHierarchies& hierarchy = entity.Get< kernel::CommunicationHierarchies >();
        tools::Iterator< const kernel::Entity_ABC& > it = hierarchy.CreateSubordinateIterator();
        while( it.HasMoreElements() )
        {
            unsigned int group = FindKnowledgeGroupForArmy( it.NextElement() );
            if( group )
                return group;
        }
        return 0;
    }
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateAutomatCreationAction
// Created: LDC 2010-10-06
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateAutomatCreationAction( const AutomatType& type, const Entity_ABC& selected, const geometry::Point2f& point ) const
{
    kernel::Point location;
    location.AddPoint( point );
    kernel::MagicActionType& actionType = magicActions_.Get( "automat_and_units_creation" );
    std::unique_ptr< UnitMagicAction > action( new UnitMagicAction( actionType, controller_, false ) );
    tools::Iterator< const kernel::OrderParameter& > it = actionType.CreateIterator();
    action->AddParameter( *new parameters::Identifier( it.NextElement(), type.GetId() ) );
    action->AddParameter( *new parameters::Point( it.NextElement(), coordinateConverter_, location ) );
    int knowledgeGroup = 0;
    const kernel::CommunicationHierarchies* hierarchy = selected.Retrieve< kernel::CommunicationHierarchies >();
    if( !hierarchy )
    {
        const kernel::TacticalHierarchies& tacticalHierarchy = selected.Get< kernel::TacticalHierarchies >();
        const kernel::Entity_ABC& top = tacticalHierarchy.GetTop();
        knowledgeGroup = FindKnowledgeGroupForArmy( top );
    }
    else
    {
        const kernel::Entity_ABC* superior = hierarchy->GetSuperior();
        while( superior )
        {
            if( dynamic_cast< const kernel::KnowledgeGroup_ABC* >( superior ) )
            {
                knowledgeGroup = superior->GetId();
                break;
            }
            superior = superior->Get< kernel::CommunicationHierarchies >().GetSuperior();
        }
    }
    action->AddParameter( *new parameters::Identifier( it.NextElement(), knowledgeGroup ) );
    action->Attach( *new ActionTiming( controller_, simulation_ ) );
    AddTasker( *action, &selected, false );
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateAgentCreationAction
// Created: LDC 2010-10-11
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateAgentCreationAction( const kernel::AgentType& type, const geometry::Point2f& point, const kernel::Entity_ABC& selected ) const
{
    kernel::Point location;
    location.AddPoint( point );
    kernel::MagicActionType& actionType = magicActions_.Get( "unit_creation" );
    std::unique_ptr< UnitMagicAction > action( new UnitMagicAction( actionType, controller_, false ) );
    tools::Iterator< const kernel::OrderParameter& > it = actionType.CreateIterator();
    action->AddParameter( *new parameters::Identifier( it.NextElement(), type.GetId() ) );
    action->AddParameter( *new parameters::Point( it.NextElement(), coordinateConverter_, location ) );
    action->Attach( *new ActionTiming( controller_, simulation_ ) );
    AddTasker( *action, &selected, false );
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateFormationCreationAction
// Created: LDC 2010-10-20
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateFormationCreationAction( int level, const kernel::Entity_ABC& selected, bool isLogisticBase ) const
{
    kernel::MagicActionType& actionType = magicActions_.Get( "formation_creation" );
    std::unique_ptr< UnitMagicAction > action( new UnitMagicAction( actionType, controller_, false ) );
    tools::Iterator< const kernel::OrderParameter& > it = actionType.CreateIterator();
    action->AddParameter( *new parameters::Numeric( it.NextElement(), static_cast<float>( level ) ) );
    action->AddParameter( *new parameters::String( it.NextElement(), std::string() ) );
    const std::string logisticLevel = isLogisticBase ? ENT_Tr::ConvertFromLogisticLevel( sword::logistic_base, ENT_Tr::eToSim ) : std::string();
    action->AddParameter( *new parameters::String( it.NextElement(), logisticLevel ) );
    action->Attach( *new ActionTiming( controller_, simulation_ ) );
    AddTasker( *action, &selected, false );
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateCrowdCreationAction
// Created: LDC 2010-10-22
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateCrowdCreationAction( const kernel::PopulationType& type, int numberHealthy, int numberWounded, int numberDead, const geometry::Point2f& point, const kernel::Entity_ABC& selected ) const
{
    kernel::Point location;
    location.AddPoint( point );
    kernel::MagicActionType& actionType = magicActions_.Get( "crowd_creation" );
    std::unique_ptr< UnitMagicAction > action( new UnitMagicAction( actionType, controller_, false ) );
    tools::Iterator< const kernel::OrderParameter& > it = actionType.CreateIterator();
    action->AddParameter( *new parameters::String( it.NextElement(), type.GetName() ) );
    action->AddParameter( *new parameters::Point( it.NextElement(), coordinateConverter_, location ) );
    action->AddParameter( *new parameters::Quantity( it.NextElement(), numberHealthy ) );
    action->AddParameter( *new parameters::Quantity( it.NextElement(), numberWounded ) );
    action->AddParameter( *new parameters::Quantity( it.NextElement(), numberDead ) );
    action->AddParameter( *new parameters::String( it.NextElement(), std::string() ) );
    action->Attach( *new ActionTiming( controller_, simulation_ ) );
    AddTasker( *action, &selected, false );
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateCrowdChangeHealthStateAction
// Created: JSR 2011-03-15
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateCrowdChangeHealthStateAction( int healthy, int wounded, int contaminated, int dead, const kernel::Entity_ABC& selected ) const
{
    kernel::MagicActionType& actionType = magicActions_.Get( "crowd_change_health_state" );
    std::unique_ptr< UnitMagicAction > action( new UnitMagicAction( actionType, controller_, false ) );
    tools::Iterator< const kernel::OrderParameter& > it = actionType.CreateIterator();
    action->AddParameter( *new parameters::Quantity( it.NextElement(), healthy ) );
    action->AddParameter( *new parameters::Quantity( it.NextElement(), wounded ) );
    action->AddParameter( *new parameters::Quantity( it.NextElement(), contaminated ) );
    action->AddParameter( *new parameters::Quantity( it.NextElement(), dead ) );
    action->Attach( *new ActionTiming( controller_, simulation_ ) );
    AddTasker( *action, &selected, false );
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateInhabitantChangeHealthStateAction
// Created: ABR 2011-01-26
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateInhabitantChangeHealthStateAction( int healthy, int wounded, int dead, const kernel::Entity_ABC& selected ) const
{
    kernel::MagicActionType& actionType = magicActions_.Get( "inhabitant_change_health_state" );
    std::unique_ptr< UnitMagicAction > action( new UnitMagicAction( actionType, controller_, false ) );
    tools::Iterator< const kernel::OrderParameter& > it = actionType.CreateIterator();
    action->AddParameter( *new parameters::Quantity( it.NextElement(), healthy ) );
    action->AddParameter( *new parameters::Quantity( it.NextElement(), wounded ) );
    action->AddParameter( *new parameters::Quantity( it.NextElement(), dead ) );
    action->Attach( *new ActionTiming( controller_, simulation_ ) );
    AddTasker( *action, &selected, false );
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateInhabitantChangeAlertedStateAction
// Created: BCI 2011-02-03
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateInhabitantChangeAlertedStateAction( bool alerted, const kernel::Entity_ABC& selected ) const
{
    kernel::MagicActionType& actionType = magicActions_.Get( "inhabitant_change_alerted_state" );
    std::unique_ptr< UnitMagicAction > action( new UnitMagicAction( actionType, controller_, false ) );
    tools::Iterator< const kernel::OrderParameter& > it = actionType.CreateIterator();
    action->AddParameter( *new parameters::Bool( it.NextElement(), alerted ) );
    action->Attach( *new ActionTiming( controller_, simulation_ ) );
    AddTasker( *action, &selected, false );
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateInhabitantChangeConfinedStateAction
// Created: BCI 2011-02-03
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateInhabitantChangeConfinedStateAction( bool confined, const kernel::Entity_ABC& selected ) const
{
    kernel::MagicActionType& actionType = magicActions_.Get( "inhabitant_change_confined_state" );
    std::unique_ptr< UnitMagicAction > action( new UnitMagicAction( actionType, controller_, false ) );
    tools::Iterator< const kernel::OrderParameter& > it = actionType.CreateIterator();
    action->AddParameter( *new parameters::Bool( it.NextElement(), confined ) );
    action->Attach( *new ActionTiming( controller_, simulation_ ) );
    AddTasker( *action, &selected, false );
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateLogMaintenanceSetManualAction
// Created: ABR 2014-01-21
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateLogMaintenanceSetManualAction( const kernel::Entity_ABC& tasker, bool manual ) const
{
    kernel::MagicActionType& actionType = magicActions_.Get( "log_maintenance_set_manual" );
    std::unique_ptr< UnitMagicAction > action( new UnitMagicAction( actionType, controller_, false ) );
    tools::Iterator< const kernel::OrderParameter& > it = actionType.CreateIterator();
    action->AddParameter( *new parameters::Bool( it.NextElement(), manual ) );
    action->Attach( *new ActionTiming( controller_, simulation_ ) );
    AddTasker( *action, &tasker, false );
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateSelectNewLogisticState
// Created: LGY 2014-01-24
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateSelectNewLogisticState( unsigned int consignId ) const
{
    kernel::MagicActionType& actionType = magicActions_.Get( "select_new_logistic_state" );
    std::unique_ptr< MagicAction > action( new MagicAction( actionType, controller_, false ) );
    tools::Iterator< const kernel::OrderParameter& > it = actionType.CreateIterator();
    action->AddParameter( *new parameters::Identifier( it.NextElement(), consignId ) );
    action->Attach( *new ActionTiming( controller_, simulation_ ) );
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateTransferToLogisticSuperior
// Created: ABR 2014-01-29
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateTransferToLogisticSuperior( unsigned int consignId ) const
{
    kernel::MagicActionType& actionType = magicActions_.Get( "transfer_to_logistic_superior" );
    std::unique_ptr< MagicAction > action( new MagicAction( actionType, controller_, false ) );
    tools::Iterator< const kernel::OrderParameter& > it = actionType.CreateIterator();
    action->AddParameter( *new parameters::Identifier( it.NextElement(), consignId ) );
    action->Attach( *new ActionTiming( controller_, simulation_ ) );
    return action.release();
}

namespace
{
    Action_ABC* CreateMaintenanceSelection( unsigned int consignId, unsigned int equipmentTypeId, kernel::MagicActionType& actionType,
                                            kernel::Controller& controller, const kernel::Time_ABC& simulation )
    {
        std::unique_ptr< MagicAction > action( new MagicAction( actionType, controller, false ) );
        tools::Iterator< const kernel::OrderParameter& > it = actionType.CreateIterator();
        action->AddParameter( *new parameters::Identifier( it.NextElement(), consignId ) );
        action->AddParameter( *new parameters::Identifier( it.NextElement(), equipmentTypeId ) );
        action->Attach( *new ActionTiming( controller, simulation ) );
        return action.release();
    }
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateSelectMaintenanceTransporter
// Created: ABR 2014-01-29
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateSelectMaintenanceTransporter( unsigned int consignId, unsigned int equipmentTypeId )
{
    return CreateMaintenanceSelection( consignId, equipmentTypeId, magicActions_.Get( "select_maintenance_transporter" ), controller_, simulation_ );
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateSelectMaintenanceDiagnosisTeam
// Created: SLI 2014-02-06
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateSelectMaintenanceDiagnosisTeam( unsigned int consignId, unsigned int equipmentTypeId )
{
    return CreateMaintenanceSelection( consignId, equipmentTypeId, magicActions_.Get( "select_diagnosis_team" ), controller_, simulation_ );
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateChangeDiplomacy
// Created: ABR 2014-02-07
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateChangeDiplomacy( unsigned int team1, unsigned int team2, sword::EnumDiplomacy diplomacy )
{
    MagicActionType& actionType = magicActions_.Get( "change_diplomacy" );
    std::unique_ptr< MagicAction > action( new MagicAction( actionType, controller_, false ) );
    tools::Iterator< const OrderParameter& > it = actionType.CreateIterator();
    action->AddParameter( *new parameters::Identifier( it.NextElement(), team1 ) );
    action->AddParameter( *new parameters::Identifier( it.NextElement(), team2 ) );
    action->AddParameter( *new parameters::Enumeration( it.NextElement(), diplomacy ) );
    action->Attach( *new ActionTiming( controller_, simulation_ ) );
    return action.release();
}


// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateKnowledgeGroup
// Created: ABR 2014-02-10
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateKnowledgeGroup( unsigned int id, const std::string& type ) const
{
    MagicActionType& actionType = magicActions_.Get( "create_knowledge_group" );
    std::unique_ptr< MagicAction > action( new MagicAction( actionType, controller_, false ) );
    tools::Iterator< const OrderParameter& > paramIt = actionType.CreateIterator();
    action->AddParameter( *new parameters::Identifier( paramIt.NextElement(), id ) );
    action->AddParameter( *new parameters::String( paramIt.NextElement(), type ) );
    action->Attach( *new ActionTiming( controller_, simulation_ ) );
    return action.release();
}


// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateFireOrderOnLocation
// Created: ABR 2014-02-10
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateFireOrderOnLocation( unsigned int resourceId, const kernel::Location_ABC& location, float interventionType ) const
{
    kernel::MagicActionType& actionType = magicActions_.Get( "fire_order_on_location" );
    std::unique_ptr< MagicAction > action( new MagicAction( actionType, controller_, false ) );
    tools::Iterator< const kernel::OrderParameter& > it = actionType.CreateIterator();
    action->AddParameter( *new parameters::Location( it.NextElement(), staticModel_.coordinateConverter_, location ) );
    action->AddParameter( *new parameters::DotationType( it.NextElement(), resourceId, staticModel_.objectTypes_ ) );
    action->AddParameter( *new parameters::Numeric( it.NextElement(), interventionType ) );
    action->Attach( *new actions::ActionTiming( controller_, simulation_ ) );
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateChangeResourceLinks
// Created: ABR 2014-02-10
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateChangeResourceLinks( unsigned int id, const gui::ResourceNetwork_ABC::T_ResourceNodes& resourceNodes ) const
{
    MagicActionType& actionType = magicActions_.Get( "change_resource_links" );
    std::unique_ptr< MagicAction > action( new MagicAction( actionType, controller_, false ) );
    tools::Iterator< const kernel::OrderParameter& > it = actionType.CreateIterator();
    action->AddParameter( *new parameters::Identifier( it.NextElement(), id ) );
    parameters::ParameterList* nodes = new parameters::ParameterList( it.NextElement() );
    action->AddParameter( *nodes );
    for( auto it = resourceNodes.begin(); it != resourceNodes.end(); ++it )
    {
        const auto& resource = it->second;
        parameters::ParameterList& node = nodes->AddList( "Node" );
        node.AddString( "Resource", resource.resource_ );
        node.AddQuantity( "Consumption", resource.consumption_ );
        node.AddBool( "Critical", resource.critical_ );
        node.AddBool( "Enabled", resource.isEnabled_ );
        node.AddQuantity( "Production", resource.production_ );
        node.AddQuantity( "MaxStock", resource.maxStock_ );
        parameters::ParameterList& links = node.AddList( "Links" );
        for( unsigned int i = 0; i < resource.links_.size(); ++i )
        {
            parameters::ParameterList& link = links.AddList( "Link" );
            link.AddIdentifier( "Link", resource.links_[ i ].id_ );
            link.AddQuantity( "Capacity", resource.links_[ i ].capacity_ );
        }
    }
    action->Attach( *new ActionTiming( controller_, simulation_ ) );
    return action.release();
}

namespace
{
    class InvalidAction : public Action_ABC
    {
    public:
        InvalidAction( kernel::Controller& controller, const kernel::OrderType& type )
            : Action_ABC( controller, &type )
        {
            Invalidate();
        }
        virtual ~InvalidAction() {}
    public:
        virtual void Publish( Publisher_ABC&, int ) const {}
        virtual void Polish() {}
    };
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateInvalidAction
// Created: LGY 2013-05-14
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateInvalidAction( const kernel::OrderType& mission ) const
{
    std::unique_ptr< Action_ABC > action( new InvalidAction( controller_, mission ) );
    action->Attach( *new ActionTiming( controller_, simulation_ ) );
    return action.release();
}
