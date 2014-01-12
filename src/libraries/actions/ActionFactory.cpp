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
#include "AutomatCreationMagicAction.h"
#include "CreationListener_ABC.h"
#include "EngageMagicAction.h"
#include "EntityMission.h"
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
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/LogisticLevel.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/MissionType.h"
#include "clients_kernel/Object_ABC.h"
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
ActionFactory::ActionFactory( kernel::Controller& controller, const ParameterFactory_ABC& factory, const kernel::EntityResolver_ABC& entities
                            , const kernel::StaticModel& staticModel, const kernel::Time_ABC& simulation )
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

namespace
{
    tools::Exception MissingParameter( const Action_ABC& action, const kernel::OrderParameter& param )
    {
        return MASA_EXCEPTION( tools::translate( "ActionFactory", "Parameter mismatch in action '%1' (id: %2): missing parameter '%3'." )
                                .arg( action.GetName() ).arg( action.GetId() ).arg( param.GetName().c_str() ).toStdString() );
    }
    tools::Exception TargetNotFound( unsigned long id = 0 )
    {
        if( id )
            return MASA_EXCEPTION( tools::translate( "ActionFactory", "Unable to find executing entity '%1'." ).arg( id ).toStdString() );
        return MASA_EXCEPTION( tools::translate( "ActionFactory", "Executing target not set" ).toStdString() );
    }
    tools::Exception MagicIdNotFound( const std::string& id )
    {
        return MASA_EXCEPTION( tools::translate( "ActionFactory", "Magic action type '%1' unknown." ).arg( id.c_str() ).toStdString() );
    }
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateAction
// Created: SBO 2010-05-07
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateAction( const kernel::Entity_ABC* target, const kernel::MissionType& mission ) const
{
    std::unique_ptr< Action_ABC > action;
    if( mission.GetType() == eMissionType_Pawn )
        action.reset( new AgentMission( target, mission, controller_, true ) );
    if( mission.GetType() == eMissionType_Automat )
        action.reset( new AutomatMission( target, mission, controller_, true ) );
    else if( mission.GetType() == eMissionType_Population )
        action.reset( new PopulationMission( target, mission, controller_, true ) );

    action->Attach( *new ActionTiming( controller_, simulation_ ) );
    action->Attach( *new ActionTasker( target, false ) );
    action->Polish();
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateAction
// Created: SBO 2010-05-07
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateAction( const kernel::Entity_ABC* target, const kernel::FragOrderType& fragOrder ) const
{
    std::unique_ptr< Action_ABC > action( new FragOrder( target, fragOrder, controller_, true ) );
    action->Attach( *new ActionTiming( controller_, simulation_ ) );
    action->Attach( *new ActionTasker( target, false ) );
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
    action->Attach( *new ActionTasker( &target, false ) );
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
// Name: ActionFactory::CreateStubAction
// Created: LDC 2010-07-28
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateStubAction( xml::xistream& xis ) const
{
    const std::string type = xis.attribute< std::string >( "type" );
    if( type == "mission" )
        return CreateStubMission( xis );
    if( type == "fragorder" )
        return CreateStubFragOrder( xis );
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
    return 0;
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateAction
// Created: SBO 2010-05-07
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateAction( const sword::UnitOrder& message, bool needRegistration ) const
{
    const kernel::MissionType& mission = missions_.Get( message.type().id() );
    const kernel::Entity_ABC* tasker = entities_.FindAgent( message.tasker().id() );
    std::unique_ptr< Action_ABC > action( new AgentMission( tasker, mission, controller_, needRegistration ) );
    AddTiming( *action, message );
    action->Attach( *new ActionTasker( tasker ) );
    action->Polish();
    AddParameters( *action, mission, message.parameters() );
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateAction
// Created: SBO 2010-05-07
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateAction( const sword::AutomatOrder& message, bool needRegistration ) const
{
    const kernel::MissionType& mission = missions_.Get( message.type().id() );
    const kernel::Entity_ABC* tasker = entities_.FindAutomat( message.tasker().id() );
    std::unique_ptr< Action_ABC > action( new AutomatMission( tasker, mission, controller_, needRegistration ) );
    AddTiming( *action, message );
    action->Attach( *new ActionTasker( tasker ) );
    action->Polish();
    AddParameters( *action, mission, message.parameters() );
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateAction
// Created: SBO 2010-05-07
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateAction( const sword::CrowdOrder& message, bool needRegistration ) const
{
    const kernel::MissionType& mission = missions_.Get( message.type().id() );
    const kernel::Entity_ABC* tasker = entities_.FindPopulation( message.tasker().id() );
    std::unique_ptr< Action_ABC > action( new PopulationMission( tasker, mission, controller_, needRegistration ) );
    AddTiming( *action, message );
    action->Attach( *new ActionTasker( tasker ) );
    action->Polish();
    AddParameters( *action, mission, message.parameters() );
    return action.release();
}

namespace
{
    const kernel::Entity_ABC* FindTasker( sword::FragOrder message, const kernel::EntityResolver_ABC& entities )
    {
        if( message.tasker().has_automat() )
            return entities.FindAutomat( message.tasker().automat().id() );
        if( message.tasker().has_crowd() )
            return entities.FindPopulation( message.tasker().crowd().id() );
        if( message.tasker().has_unit() )
            return entities.FindAgent( message.tasker().unit().id() );
        return 0;
    }
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateAction
// Created: SBO 2010-05-07
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateAction( const sword::FragOrder& message, bool needRegistration ) const
{
    const kernel::Entity_ABC* tasker = FindTasker( message, entities_ );
    const kernel::FragOrderType& order = fragOrders_.Get( message.type().id() );
    std::unique_ptr< Action_ABC > action( new FragOrder( tasker, order, controller_, needRegistration ) );
    AddTiming( *action, message );
    action->Attach( *new ActionTasker( tasker ) );
    action->Polish();
    AddParameters( *action, order, message.parameters() );
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateAction
// Created: ABR 2014-01-08
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateAction( const sword::SetAutomatMode& message, bool needRegistration ) const
{
    int targetid = message.automate().id();
    const kernel::Entity_ABC* target = entities_.FindAutomat( targetid );
    if( !target )
        return 0;
    bool engaged = message.mode() == sword::engaged;
    std::unique_ptr< EngageMagicAction > action( new EngageMagicAction( *target,
                                                                        magicActions_.Get( "change_mode" ),
                                                                        controller_,
                                                                        engaged,
                                                                        needRegistration ) );
    action->Attach( *new ActionTiming( controller_, simulation_ ) );
    action->Attach( *new ActionTasker( target ) );
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
    action->Attach( *new ActionTasker( 0 ) );
    action->Polish();
    AddParameters( *action, orderType, message.parameters() );
    return action.release();
}

namespace
{
    unsigned long GetTaskerId( const sword::Tasker& tasker )
    {
        if( tasker.has_automat() )
            return tasker.automat().id();
        else if( tasker.has_unit() )
            return tasker.unit().id();
        else if( tasker.has_crowd() )
            return tasker.crowd().id();
        else if( tasker.has_formation() )
            return tasker.formation().id();
        else if( tasker.has_party() )
            return tasker.party().id();
        else if( tasker.has_population() )
            return tasker.population().id();
        return 0;
    }
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateAction
// Created: ABR 2014-01-08
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateAction( const sword::UnitMagicAction& message, bool needRegistration ) const
{
    unsigned long taskerId = GetTaskerId( message.tasker() );
    const kernel::Entity_ABC* target = entities_.FindEntity( taskerId );
    if( !target )
        return 0;
    auto& type = magicActions_.Get( ENT_Tr::ConvertFromUnitMagicActionType( message.type(), ENT_Tr::eToSim ) );
    std::unique_ptr< UnitMagicAction > action( new UnitMagicAction( *target,
                                                                    type,
                                                                    controller_,
                                                                    needRegistration ) );
    action->Attach( *new ActionTiming( controller_, simulation_ ) );
    action->Attach( *new ActionTasker( target ) );
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
    const kernel::Entity_ABC* target = entities_.FindKnowledgeGroup( message.knowledge_group().id() );
    if( !target )
        return 0;
    auto& type = magicActions_.Get( ENT_Tr::ConvertFromKnowledgeMagicActionType( message.type(), ENT_Tr::eToSim ) );
    std::unique_ptr< Action_ABC > action( new KnowledgeGroupMagicAction( *target,
                                                                         type,
                                                                         controller_,
                                                                         needRegistration ) );
    action->Attach( *new ActionTiming( controller_, simulation_ ) );
    action->Attach( *new ActionTasker( target ) );
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
    const kernel::Entity_ABC* target = entities_.FindObject( message.object().id() );
    auto& type = magicActions_.Get( ENT_Tr::ConvertFromObjectMagicActionType( message.type(), ENT_Tr::eToSim ) );
    std::unique_ptr< Action_ABC > action( new ObjectMagicAction( target,
                                                                 type,
                                                                 controller_,
                                                                 needRegistration ) );
    action->Attach( *new ActionTiming( controller_, simulation_ ) );
    action->Attach( *new ActionTasker( target ) );
    action->Polish();
    AddParameters( *action, type, message.parameters() );
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateMission
// Created: LDC 2010-07-28
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateMission( xml::xistream& xis, bool readonly, bool stub ) const
{
    const unsigned long id = xis.attribute< unsigned long >( "target" );
    std::unique_ptr< Mission > action;
    const kernel::Entity_ABC* target = entities_.FindAgent( id );
    if( target )
        action.reset( new AgentMission( xis, controller_, missions_, *target, stub ) );
    else
    {
        target = entities_.FindAutomat( id );
        if( target )
            action.reset( new AutomatMission( xis, controller_, missions_, *target, stub ) );
        else
        {
            target = entities_.FindPopulation( id );
            if( target )
                action.reset( new PopulationMission( xis, controller_, missions_, *target, stub ) );
            else
                throw TargetNotFound( id );
        }
    }
    action->Attach( *new ActionTiming( xis, controller_, simulation_ ) );
    action->Attach( *new ActionTasker( target, readonly ) );
    action->Polish();

    tools::Iterator< const kernel::OrderParameter& > it = action->GetType().CreateIterator();
    if( stub )
    {
        xis >> xml::list( "parameter", *this, &ActionFactory::ReadStubParameter, *action, it, *target );
        action->Invalidate();
    }
    else
    {
        xis >> xml::list( "parameter", *this, &ActionFactory::ReadParameter, *action, it, *target );
        if( it.HasMoreElements() )
            throw MissingParameter( *action, it.NextElement() );
    }
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateMission
// Created: SBO 2010-05-07
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateMission( xml::xistream& xis, bool readonly ) const
{
    return CreateMission( xis, readonly, false );
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateStubMission
// Created: LDC 2010-07-28
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateStubMission( xml::xistream& xis ) const
{
    return CreateMission( xis, false, true );
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateFragOrder
// Created: LDC 2010-07-28
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateFragOrder( xml::xistream& xis, bool readonly, bool stub ) const
{
    const unsigned long id = xis.attribute< unsigned long >( "target" );

    const kernel::Entity_ABC* target = entities_.FindAgent( id );
    if( !target )
        target = entities_.FindAutomat( id );
    if( !target )
        target = entities_.FindPopulation( id );
    if( !target )
        throw TargetNotFound( id );
    std::unique_ptr< FragOrder > action( new FragOrder( xis, controller_, fragOrders_, *target ) );
    action->Attach( *new ActionTiming( xis, controller_, simulation_ ) );
    action->Attach( *new ActionTasker( target, readonly ) );
    action->Polish();

    tools::Iterator< const kernel::OrderParameter& > it = action->GetType().CreateIterator();
    if( stub )
    {
        xis >> xml::list( "parameter", *this, &ActionFactory::ReadStubParameter, *action, it, *target );
        action->Invalidate();
    }
    else
    {
        xis >> xml::list( "parameter", *this, &ActionFactory::ReadParameter, *action, it, *target );
        if( it.HasMoreElements() )
            throw MissingParameter( *action, it.NextElement() );
    }
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateFragOrder
// Created: SBO 2010-05-07
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateFragOrder( xml::xistream& xis, bool readonly ) const
{
    return CreateFragOrder( xis, readonly, false );
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateStubFragOrder
// Created: LDC 2010-07-28
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateStubFragOrder( xml::xistream& xis ) const
{
    return CreateFragOrder( xis, false, true );
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateMagicAction
// Created: SBO 2010-05-07
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateMagicAction( xml::xistream& xis, bool readonly ) const
{
    const std::string id = xis.attribute< std::string >( "id" );
    std::unique_ptr< MagicAction > action( new MagicAction( xis, controller_, magicActions_.Get( id ) ) );
    action->Attach( *new ActionTiming( xis, controller_, simulation_ ) );
    action->Attach( *new ActionTasker( 0, readonly ) );
    action->Polish();

    tools::Iterator< const kernel::OrderParameter& > it = action->GetType().CreateIterator();
    xis >> xml::list( "parameter", *this, &ActionFactory::ReadParameter, *action, it );
    if( it.HasMoreElements() )
        throw MissingParameter( *action, it.NextElement() );
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateUnitMagicAction
// Created: SBO 2010-05-07
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateUnitMagicAction( xml::xistream& xis, bool readonly ) const
{
    const unsigned long targetid = xis.attribute< unsigned long >( "target" );
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
    if( !target )
        throw TargetNotFound( targetid );

    std::unique_ptr< UnitMagicAction > action( new UnitMagicAction( xis, controller_, magicActions_.Get( id ), *target ) );
    action->Attach( *new ActionTiming( xis, controller_, simulation_ ) );
    action->Attach( *new ActionTasker( target, readonly ) );
    action->Polish();

    tools::Iterator< const kernel::OrderParameter& > it = action->GetType().CreateIterator();
    xis >> xml::list( "parameter", *this, &ActionFactory::ReadParameter, *action, it, *target );
    if( it.HasMoreElements() )
        throw MissingParameter( *action, it.NextElement() );
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::AutomateChangeModeMagicAction
// Created: FDS 2010-11-23
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::AutomateChangeModeMagicAction( xml::xistream& xis, bool readonly ) const
{
    const unsigned long targetid = xis.attribute< unsigned long >( "target" );
    const std::string type = xis.attribute< std::string >( "type" );
    std::string name;
    xis >> xml::optional >> xml::attribute( "name", name );
    bool engaged;
    xis >> xml::attribute( "engaged", engaged );

    const kernel::Entity_ABC* target = entities_.FindAutomat( targetid );
    if( !target )
        throw TargetNotFound( targetid );

    std::unique_ptr< EngageMagicAction > action( new EngageMagicAction( xis, controller_, magicActions_.Get( type ), *target, engaged ) );
    action->Attach( *new ActionTiming( xis, controller_, simulation_ ) );
    action->Attach( *new ActionTasker( target, readonly ) );
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
        const unsigned long targetid = xis.attribute< unsigned long >( "target" );
        target = entities_.FindObject( targetid );
        if( !target )
            target = entities_.FindUrbanObject( targetid );
        if( !target )
            throw TargetNotFound( targetid );
    }


    std::unique_ptr< Action_ABC > action( new ObjectMagicAction( xis, controller_, magicActions_.Get( id ), target ) );
    action->Attach( *new ActionTiming( xis, controller_, simulation_ ) );
    action->Attach( *new ActionTasker( 0, readonly ) );
    action->Polish();

    tools::Iterator< const kernel::OrderParameter& > it = action->GetType().CreateIterator();
    if( target )
        xis >> xml::list( "parameter", *this, &ActionFactory::ReadParameter, *action, it, *target );
    else
        xis >> xml::list( "parameter", *this, &ActionFactory::ReadParameter, *action, it );
    if( it.HasMoreElements() )
        throw MissingParameter( *action, it.NextElement() );
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateObjectMagicAction
// Created: JCR 2011-01-04
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateObjectMagicAction( const std::string& magicAction, unsigned long targetId  ) const
{
    kernel::Entity_ABC* target = 0;
    if( magicAction != "create_object" )
    {
        target = entities_.FindObject( targetId );
        if( !target )
            throw TargetNotFound( targetId );
    }
    std::unique_ptr< Action_ABC > action( new ObjectMagicAction( target, magicActions_.Get( magicAction ), controller_, true ) );
    action->Attach( *new ActionTiming( controller_, simulation_ ) );
    action->Attach( *new ActionTasker( 0, false ) );
    action->Polish();
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateObjectUpdateMagicAction
// Created: JSR 2011-03-01
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateObjectUpdateMagicAction( const kernel::Entity_ABC& object, parameters::ParameterList& attribute ) const
{
    std::unique_ptr< Action_ABC > action( new ObjectMagicAction( &object, magicActions_.Get( "update_object" ), controller_, true ) );
    tools::Iterator< const OrderParameter& > it = action->GetType().CreateIterator();
    parameters::ParameterList* attributesList = new parameters::ParameterList( it.NextElement() );
    action->AddParameter( *attributesList );
    attributesList->AddParameter( attribute );
    action->Attach( *new ActionTiming( controller_, simulation_ ) );
    action->Attach( *new ActionTasker( 0, false ) );
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
    action.reset( new ObjectMagicAction( &object, magicActions_.Get( "destroy_object" ), controller_, true ) );
    action->Attach( *new ActionTiming( controller_, simulation_ ) );
    action->Attach( *new ActionTasker( 0, false ) );
    action->Polish();
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateKnowledgeGroupMagicAction
// Created: SBO 2010-05-07
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateKnowledgeGroupMagicAction( xml::xistream& xis, bool readonly ) const
{
    const unsigned long targetid = xis.attribute< unsigned long >( "target" );
    const std::string id = xis.attribute< std::string >( "id" );

    const kernel::KnowledgeGroup_ABC* target = entities_.FindKnowledgeGroup( targetid );
    if( !target )
        throw TargetNotFound( targetid );

    std::unique_ptr< Action_ABC > action( new KnowledgeGroupMagicAction( xis, controller_, magicActions_.Get( id ), *target ) );
    action->Attach( *new ActionTiming( xis, controller_, simulation_ ) );
    action->Attach( *new ActionTasker( target, readonly ) );
    action->Polish();

    tools::Iterator< const kernel::OrderParameter& > it = action->GetType().CreateIterator();
    xis >> xml::list( "parameter", *this, &ActionFactory::ReadParameter, *action, it, *target );
    if( it.HasMoreElements() )
        throw MissingParameter( *action, it.NextElement() );
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::AddParameters
// Created: SBO 2010-05-07
// -----------------------------------------------------------------------------
void ActionFactory::AddParameters( Action_ABC& action, const kernel::OrderType& order, const sword::MissionParameters& message ) const
{
    tools::Iterator< const kernel::OrderParameter& > it = order.CreateIterator();
    int i = 0;
    while( it.HasMoreElements() )
    {
        const kernel::OrderParameter& param = it.NextElement();
        if( i >= message.elem_size() )
            throw MASA_EXCEPTION( "Mission parameter count does not match mission definition" );
        if( const ActionTasker* tasker = action.Retrieve< ActionTasker >() )
        {
            const kernel::Entity_ABC* entity = entities_.FindEntity( tasker->GetTaskerId() );
            if( Parameter_ABC* newParam = factory_.CreateParameter( param, message.elem( i++ ), entity ) )
                action.AddParameter( *newParam );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::ReadParameter
// Created: SBO 2010-05-07
// -----------------------------------------------------------------------------
void ActionFactory::ReadParameter( xml::xistream& xis, Action_ABC& action, tools::Iterator< const kernel::OrderParameter& >& it, const kernel::Entity_ABC& entity ) const
{
    try
    {
        if( !it.HasMoreElements() )
            throw MASA_EXCEPTION( tools::translate( "ActionFactory", "too many parameters provided" ).toStdString() );
        // $$$$ LDC AddParameter can throw after having taken ownership of the parameter thus do not use auto_ptr here
        const kernel::OrderParameter& parameter = it.NextElement();
        Parameter_ABC* param = factory_.CreateParameter( parameter, xis, entity );
        if( !param )
        {
            std::string errorMessage( "Impossible to create parameter " );
            errorMessage = errorMessage + parameter.GetName();
            throw MASA_EXCEPTION( errorMessage.c_str() );
        }
        action.AddParameter( *param );
    }
    catch( const std::exception& e )
    {
        throw MASA_EXCEPTION( tools::translate( "ActionFactory", "Parameter mismatch in action '%1' (id: %2): %3." )
                                .arg( action.GetName() ).arg( action.GetId() ).arg( tools::GetExceptionMsg( e ).c_str() ).toStdString() );
    }
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::ReadStubParameter
// Created: LDC 2010-07-28
// -----------------------------------------------------------------------------
void ActionFactory::ReadStubParameter( xml::xistream& xis, Action_ABC& action, tools::Iterator< const kernel::OrderParameter& >& it, const kernel::Entity_ABC& entity ) const
{
    try
    {
        ReadParameter( xis, action, it, entity );
    }
    catch( const std::exception& /*e*/ ) {}
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::ReadParameter
// Created: SBO 2010-05-07
// -----------------------------------------------------------------------------
void ActionFactory::ReadParameter( xml::xistream& xis, Action_ABC& action, tools::Iterator< const kernel::OrderParameter& >& it ) const
{
    try
    {
        if( !it.HasMoreElements() )
            throw MASA_EXCEPTION( tools::translate( "ActionFactory", "too many parameters provided" ).toStdString() );
        std::unique_ptr< Parameter_ABC > param( factory_.CreateParameter( it.NextElement(), xis ) );
        action.AddParameter( *param );
        param.release();
    }
    catch( const std::exception& e )
    {
        throw MASA_EXCEPTION( tools::translate( "ActionFactory", "Parameter mismatch in action '%1' (id: %2): %3." )
                                .arg( action.GetName() ).arg( action.GetId() ).arg( tools::GetExceptionMsg( e ).c_str() ).toStdString() );
    }
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
Action_ABC* ActionFactory::CreateAutomatCreationAction( const AutomatType& type, const Entity_ABC& selected, const geometry::Point2f& point,
                                                                tools::Resolver_ABC< Automat_ABC >& agentsModel, CreationListener_ABC& agentMessenger,
                                                                ActionsModel& actionsModel, const Time_ABC& simulation ) const
{
    kernel::MagicActionType& actionType = magicActions_.Get( "automat_creation" );

    std::unique_ptr< AutomatCreationMagicAction > action( new AutomatCreationMagicAction( selected, actionType, controller_,
          staticModel_, type, point, agentsModel, agentMessenger, actionsModel, simulation, false ) );

    tools::Iterator< const kernel::OrderParameter& > it = actionType.CreateIterator();
    action->AddParameter( *new parameters::Identifier( it.NextElement(), type.GetId() ) );
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
    action->Attach( *new ActionTasker( &selected, false ) );
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
    std::unique_ptr< UnitMagicAction > action( new UnitMagicAction( selected, actionType, controller_, false ) );
    tools::Iterator< const kernel::OrderParameter& > it = actionType.CreateIterator();
    action->AddParameter( *new parameters::Identifier( it.NextElement(), type.GetId() ) );
    action->AddParameter( *new parameters::Point( it.NextElement(), coordinateConverter_, location ) );
    action->Attach( *new ActionTiming( controller_, simulation_ ) );
    action->Attach( *new ActionTasker( &selected, false ) );
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateFormationCreationAction
// Created: LDC 2010-10-20
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateFormationCreationAction( int level, const kernel::Entity_ABC& selected, bool isLogisticBase ) const
{
    kernel::MagicActionType& actionType = magicActions_.Get( "formation_creation" );
    std::unique_ptr< UnitMagicAction > action( new UnitMagicAction( selected, actionType, controller_, false ) );
    tools::Iterator< const kernel::OrderParameter& > it = actionType.CreateIterator();
    action->AddParameter( *new parameters::Numeric( it.NextElement(), static_cast<float>( level ) ) );
    action->AddParameter( *new parameters::String( it.NextElement(), std::string() ) );
    action->AddParameter( *new parameters::String( it.NextElement(), isLogisticBase ? kernel::LogisticLevel::logistic_base_.GetName() : std::string() ) );
    action->Attach( *new ActionTiming( controller_, simulation_ ) );
    action->Attach( *new ActionTasker( &selected, false ) );
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
    std::unique_ptr< UnitMagicAction > action( new UnitMagicAction( selected, actionType, controller_, false ) );
    tools::Iterator< const kernel::OrderParameter& > it = actionType.CreateIterator();
    action->AddParameter( *new parameters::String( it.NextElement(), type.GetName() ) );
    action->AddParameter( *new parameters::Point( it.NextElement(), coordinateConverter_, location ) );
    action->AddParameter( *new parameters::Quantity( it.NextElement(), numberHealthy ) );
    action->AddParameter( *new parameters::Quantity( it.NextElement(), numberWounded ) );
    action->AddParameter( *new parameters::Quantity( it.NextElement(), numberDead ) );
    action->AddParameter( *new parameters::String( it.NextElement(), std::string() ) );
    action->Attach( *new ActionTiming( controller_, simulation_ ) );
    action->Attach( *new ActionTasker( &selected, false ) );
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateCrowdChangeHealthStateAction
// Created: JSR 2011-03-15
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateCrowdChangeHealthStateAction( int healthy, int wounded, int contaminated, int dead, const kernel::Entity_ABC& selected ) const
{
    kernel::MagicActionType& actionType = magicActions_.Get( "crowd_change_health_state" );
    std::unique_ptr< UnitMagicAction > action( new UnitMagicAction( selected, actionType, controller_, false ) );
    tools::Iterator< const kernel::OrderParameter& > it = actionType.CreateIterator();
    action->AddParameter( *new parameters::Quantity( it.NextElement(), healthy ) );
    action->AddParameter( *new parameters::Quantity( it.NextElement(), wounded ) );
    action->AddParameter( *new parameters::Quantity( it.NextElement(), contaminated ) );
    action->AddParameter( *new parameters::Quantity( it.NextElement(), dead ) );
    action->Attach( *new ActionTiming( controller_, simulation_ ) );
    action->Attach( *new ActionTasker( &selected, false ) );
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateInhabitantChangeHealthStateAction
// Created: ABR 2011-01-26
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateInhabitantChangeHealthStateAction( int healthy, int wounded, int dead, const kernel::Entity_ABC& selected ) const
{
    kernel::MagicActionType& actionType = magicActions_.Get( "inhabitant_change_health_state" );
    std::unique_ptr< UnitMagicAction > action( new UnitMagicAction( selected, actionType, controller_, false ) );
    tools::Iterator< const kernel::OrderParameter& > it = actionType.CreateIterator();
    action->AddParameter( *new parameters::Quantity( it.NextElement(), healthy ) );
    action->AddParameter( *new parameters::Quantity( it.NextElement(), wounded ) );
    action->AddParameter( *new parameters::Quantity( it.NextElement(), dead ) );
    action->Attach( *new ActionTiming( controller_, simulation_ ) );
    action->Attach( *new ActionTasker( &selected, false ) );
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateInhabitantChangeAlertedStateAction
// Created: BCI 2011-02-03
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateInhabitantChangeAlertedStateAction( bool alerted, const kernel::Entity_ABC& selected ) const
{
    kernel::MagicActionType& actionType = magicActions_.Get( "inhabitant_change_alerted_state" );
    std::unique_ptr< UnitMagicAction > action( new UnitMagicAction( selected, actionType, controller_, false ) );
    tools::Iterator< const kernel::OrderParameter& > it = actionType.CreateIterator();
    action->AddParameter( *new parameters::Bool( it.NextElement(), alerted ) );
    action->Attach( *new ActionTiming( controller_, simulation_ ) );
    action->Attach( *new ActionTasker( &selected, false ) );
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateInhabitantChangeConfinedStateAction
// Created: BCI 2011-02-03
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateInhabitantChangeConfinedStateAction( bool confined, const kernel::Entity_ABC& selected ) const
{
    kernel::MagicActionType& actionType = magicActions_.Get( "inhabitant_change_confined_state" );
    std::unique_ptr< UnitMagicAction > action( new UnitMagicAction( selected, actionType, controller_, false ) );
    tools::Iterator< const kernel::OrderParameter& > it = actionType.CreateIterator();
    action->AddParameter( *new parameters::Bool( it.NextElement(), confined ) );
    action->Attach( *new ActionTiming( controller_, simulation_ ) );
    action->Attach( *new ActionTasker( &selected, false ) );
    return action.release();
}

namespace
{
    class InvalidAction : public Action_ABC
    {
    public:
        InvalidAction( kernel::Controller& controller, const kernel::OrderType& type )
            : Action_ABC( controller, type )
        {
            Invalidate();
        }
        virtual ~InvalidAction() {};
    public:
        virtual void Publish( Publisher_ABC&, int ) const {};
        virtual void Polish() {};
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
