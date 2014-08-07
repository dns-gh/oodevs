// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "AgentController.h"
#include "AgentListener_ABC.h"
#include "AgentProxy.h"
#include "AgentAdapter.h"
#include "AutomatProxy.h"
#include "FormationProxy.h"
#include "LocalAgentResolver_ABC.h"
#include "SideResolver_ABC.h"
#include "dispatcher/Model_ABC.h"
#include "dispatcher/Automat_ABC.h"
#include "dispatcher/Team_ABC.h"
#include "dispatcher/Agent.h"
#include "dispatcher/Automat.h"
#include "dispatcher/Formation.h"
#include "dispatcher/Logger_ABC.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/ComponentType.h"
#include "clients_kernel/Karma.h"
#include "rpr/EntityTypeResolver_ABC.h"
#include "tic/PlatformDelegateFactory_ABC.h"
#include "tic/Platform_ABC.h"

#include <xeumeuleu/xml.hpp>

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4724 )
#endif

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/bind.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/string_generator.hpp>

#ifdef _MSC_VER
#pragma warning( pop )
#endif

#include <limits>
#include <algorithm>

using namespace plugins::hla;

namespace
{
    template< typename T >
    rpr::EntityType ComputeEntityType( T& entity, dispatcher::Logger_ABC& logger, const rpr::EntityTypeResolver_ABC& resolver, const std::string& typeName )
    {
        std::string entityTypeString;
        rpr::EntityType result;
        bool hasEntityType = entity.GetExtension( "DIS_EntityType", entityTypeString );
        if( hasEntityType )
            result = rpr::EntityType( entityTypeString );
        else
            if( !resolver.Find( typeName, result ) )
                logger.LogWarning( std::string( "Could not find EntityType for type: " ) + typeName );
        return result;
    }

    void GenerateUniqueId( std::vector< char >& uniqueId, int netnVersion )
    {
        static boost::uuids::random_generator gen;
        boost::uuids::uuid uid = gen();
        std::size_t sz = netnVersion == 1 ? 11 : uid.size();
        uniqueId.resize( sz);
        std::copy( uid.begin(), uid.begin() + sz, uniqueId.begin() );
    }

    template< typename T >
    void GetUniqueId( T& entity, std::vector< char >& uniqueId, dispatcher::Logger_ABC& logger, int netnVersion )
    {
        static boost::uuids::string_generator gen;
        std::string tmp;
        uniqueId.clear();
        if( entity.GetExtension( "UUID", tmp ) )
        {
            try
            {
                boost::uuids::uuid uid = gen( tmp );
                std::size_t sz = netnVersion == 1 ? 11 : uid.size();
                uniqueId.resize( sz );
                std::copy( uid.begin(), uid.begin() + sz, uniqueId.begin() );
                return;
            }
            catch( const std::exception& e )
            {
                logger.LogError( std::string( "Invalid UUID from orbat " + tmp + " " + tools::GetExceptionMsg( e ) ) );
            }
        }
        GenerateUniqueId( uniqueId, netnVersion );
    }
}


// -----------------------------------------------------------------------------
// Name: AgentController constructor
// Created: SBO 2008-02-18
// -----------------------------------------------------------------------------
AgentController::AgentController( dispatcher::Model_ABC& model, const rpr::EntityTypeResolver_ABC& aggregatesResolver,
                                  const rpr::EntityTypeResolver_ABC& componentTypeResolver, const ComponentTypes_ABC& componentTypes,
                                  tic::PlatformDelegateFactory_ABC& factory, const kernel::CoordinateConverter_ABC& converter,
                                  bool sendPlatforms, const SideResolver_ABC& sideResolver, LocalAgentResolver_ABC& localAgentResolver,
                                  bool fullOrbat, dispatcher::Logger_ABC& logger, const rpr::EntityTypeResolver_ABC& automatEntityTypeResolver,
                                  int netnVersion )
    : model_                 ( model )
    , aggregatesResolver_    ( aggregatesResolver )
    , componentTypeResolver_ ( componentTypeResolver )
    , componentTypes_        ( componentTypes )
    , factory_               ( factory )
    , converter_             ( converter )
    , doDisaggregation_      ( sendPlatforms )
    , fullOrbat_             ( fullOrbat )
    , sideResolver_          ( sideResolver )
    , localAgentResolver_    ( localAgentResolver )
    , logger_                ( logger )
    , automatEntityTypeResolver_( automatEntityTypeResolver )
    , netnVersion_( netnVersion )
{
    model_.RegisterFactory( *this );
}

// -----------------------------------------------------------------------------
// Name: AgentController destructor
// Created: SBO 2008-02-18
// -----------------------------------------------------------------------------
AgentController::~AgentController()
{
    model_.UnregisterFactory( *this );
    std::for_each( parents_.begin(), parents_.end(), [&](const T_Parents::value_type& v )
        {
            unsigned long childId = v.first;
            unsigned long parentId = v.second;
            auto itP( agents_.find( parentId ) );
            if( agents_.end() != itP )
                itP->second->RemoveSubordinate( childId );
        });
}

// -----------------------------------------------------------------------------
// Name: AgentController::Visit
// Created: SLI 2011-09-28
// -----------------------------------------------------------------------------
void AgentController::Visit( dispatcher::Model_ABC& model )
{
    for( tools::Iterator< const dispatcher::Formation_ABC& > it = model.Formations().CreateIterator(); it.HasMoreElements(); )
    {
        dispatcher::Formation_ABC& agent = const_cast< dispatcher::Formation_ABC& >( it.NextElement() ); // $$$$ _RC_ SLI 2011-09-28: erk...
        CreateFormation( agent );
    }
    for( tools::Iterator< const dispatcher::Automat_ABC& > it = model.Automats().CreateIterator(); it.HasMoreElements(); )
    {
        dispatcher::Automat_ABC& agent = const_cast< dispatcher::Automat_ABC& >( it.NextElement() ); // $$$$ _RC_ SLI 2011-09-28: erk...
        CreateAutomat( agent );
    }

    for( tools::Iterator< const dispatcher::Agent_ABC& > it = model.Agents().CreateIterator(); it.HasMoreElements(); )
    {
        dispatcher::Agent_ABC& agent = const_cast< dispatcher::Agent_ABC& >( it.NextElement() ); // $$$$ _RC_ SLI 2011-09-28: erk...
        CreateAgent( agent );
    }
}

// -----------------------------------------------------------------------------
// Name: AgentController::Create
// Created: SLI 2011-10-04
// -----------------------------------------------------------------------------
void AgentController::Create( dispatcher::Agent& entity )
{
    CreateAgent( entity );
}

// -----------------------------------------------------------------------------
// Name: AgentController::CreateAgent
// Created: SLI 2011-10-04
// -----------------------------------------------------------------------------
void AgentController::CreateAgent( dispatcher::Agent_ABC& agent )
{
    std::string remoteExt;
    bool isRemote = agent.GetExtension( "RemoteEntity", remoteExt ) && remoteExt == "true";

    if( !isRemote )
        localAgentResolver_.Add( agent.GetId(), agent );

    std::vector< char > uniqueId;
    GetUniqueId( agent, uniqueId, logger_, netnVersion_ );
    T_Agent proxy( new AgentProxy( agent, componentTypes_, componentTypeResolver_, localAgentResolver_, doDisaggregation_, logger_ ) );
    agents_.insert( T_Agents::value_type( agent.GetId(), proxy ) );
    const kernel::AgentType& agentType = agent.GetType();
    const std::string typeName = agentType.GetKeyName();
    rpr::EntityType entityType = ComputeEntityType( agent, logger_, aggregatesResolver_, typeName );
    const rpr::ForceIdentifier forceIdentifier = sideResolver_.ResolveForce( agent.GetSuperior().GetTeam().GetId() );
    for( auto it = listeners_.begin(); it != listeners_.end(); ++it )
        (*it)->AggregateCreated( *proxy, agent.GetId(), agent.GetName().toStdString(), forceIdentifier, entityType, agentType.GetSymbol(), !isRemote, uniqueId );
    if( !isRemote && doDisaggregation_ )
            adapters_.push_back( T_AgentAdapter( new AgentAdapter( factory_, converter_, agent,
                    AgentAdapter::T_NotificationCallback( boost::bind( &AgentController::NotifyPlatformCreation, boost::ref( *this ), _1, _2, _3, _4 ) ) ) ) );
    // Must be done after HLA object was created
    if( !isRemote )
    {
        unsigned long parentId = agent.GetSuperior().GetId();
        if( parentId != 0)
        {
            T_Agents::const_iterator parentIt( agents_.find( parentId ) );
            if( agents_.end() != parentIt )
            {
                parents_[ agent.GetId() ] = parentId;
                parentIt->second->AddSubordinate( agent.GetId(), *proxy );
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: AgentController::Register
// Created: SLI 2011-01-10
// -----------------------------------------------------------------------------
void AgentController::Register( AgentListener_ABC& listener )
{
    listeners_.push_back( &listener );
}

// -----------------------------------------------------------------------------
// Name: AgentController::Unregister
// Created: SLI 2011-01-10
// -----------------------------------------------------------------------------
void AgentController::Unregister( AgentListener_ABC& listener )
{
    listeners_.erase( std::remove( listeners_.begin(), listeners_.end(), &listener ), listeners_.end() );
}

// -----------------------------------------------------------------------------
// Name: AgentController::NotifyPlatformCreation
// Created: AHC 2012-07-26
// -----------------------------------------------------------------------------
void AgentController::NotifyPlatformCreation( Agent_ABC& agent, dispatcher::Agent_ABC& parent, const tic::Platform_ABC& platform, int childIndex )
{
    static unsigned long identifier_factory = std::numeric_limits< unsigned long >::max();
    const std::string symbol( parent.GetType().GetSymbol() ); // FIXME
    const std::string typeName = platform.GetType().GetName();
    rpr::EntityType entityType;
    if( !componentTypeResolver_.Find( typeName, entityType ) )
        logger_.LogWarning( std::string( "Could not find EntityType for equipment type: " ) + typeName );
    const rpr::ForceIdentifier forceIdentifier = sideResolver_.ResolveForce( parent.GetSuperior().GetTeam().GetId() );
    unsigned long identifier = --identifier_factory;  // FIXME AHC
    std::vector< char > uniqueId;
    GenerateUniqueId( uniqueId, netnVersion_ );
    const std::string name( std::string( parent.GetName().toStdString() ) + "_" + typeName + " " + boost::lexical_cast< std::string >( childIndex ) );

    for( auto it = listeners_.begin(); it != listeners_.end(); ++it )
        (*it)->PlatformCreated( agent, identifier, name, forceIdentifier, entityType, symbol, uniqueId );

    T_Agents::const_iterator itAgent( agents_.find( parent.GetId() ) );
    if( agents_.end() !=  itAgent )
    {
        Agent_ABC& proxy( *itAgent->second );
        proxy.AddSubordinate( identifier, agent );
    }
}

// -----------------------------------------------------------------------------
// Name: AgentController::Create
// Created: AHC 2012-10-02
// -----------------------------------------------------------------------------
void AgentController::Create( dispatcher::Automat& entity )
{
    CreateAutomat( entity );
}

// -----------------------------------------------------------------------------
// Name: AgentController::Create
// Created: AHC 2012-10-02
// -----------------------------------------------------------------------------
void AgentController::Create( dispatcher::Formation& entity )
{
    CreateFormation( entity );
}

// -----------------------------------------------------------------------------
// Name: AgentController::CreateAutomat
// Created: AHC 2012-10-02
// -----------------------------------------------------------------------------
void AgentController::CreateAutomat( dispatcher::Automat_ABC& entity )
{   
    if( !fullOrbat_ ) 
        return;

    std::string remoteExt;
    bool isRemote = entity.GetExtension( "RemoteEntity", remoteExt ) && remoteExt == "true";
    if( !isRemote ) 
    {
        std::vector< char > uniqueId;
        GetUniqueId( entity, uniqueId, logger_, netnVersion_ );
        T_Agent proxy( new AutomatProxy( entity, localAgentResolver_ ) );
        agents_.insert( T_Agents::value_type( entity.GetId(), proxy ) );
		const rpr::ForceIdentifier forceIdentifier = sideResolver_.ResolveForce( entity.GetTeam().GetId() );
        std::string typeName = entity.GetType().GetName();
        rpr::EntityType entityType = ComputeEntityType( entity, logger_, automatEntityTypeResolver_, typeName );
        for( auto it = listeners_.begin(); it != listeners_.end(); ++it )
            (*it)->AggregateCreated( *proxy, entity.GetId(), entity.GetName().toStdString(), forceIdentifier, entityType, entity.GetApp6Symbol(), true, uniqueId );
        // Must be done after HLA object was created
        unsigned long parentId = entity.GetFormation() != 0 ? 
            entity.GetFormation()->GetId() :
            ( entity.GetParentAutomat() != 0 ? entity.GetParentAutomat()->GetId() : 0 );
        if( parentId != 0)
        {
            T_Agents::const_iterator parentIt( agents_.find( parentId ) );
            if( agents_.end() != parentIt )
            {
                parents_[ entity.GetId() ] = parentId;
                parentIt->second->AddSubordinate( entity.GetId(), *proxy );
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: AgentController::CreateFormation
// Created: AHC 2012-10-02
// -----------------------------------------------------------------------------
void AgentController::CreateFormation( dispatcher::Formation_ABC& entity )
{
    if( !fullOrbat_ ) 
        return;
    std::string remoteExt;
    bool isRemote = entity.GetExtension( "RemoteEntity", remoteExt ) && remoteExt == "true";
    if( !isRemote ) 
    {
        std::vector< char > uniqueId;
        GetUniqueId( entity, uniqueId, logger_, netnVersion_ );
        T_Agent proxy( new FormationProxy( entity, localAgentResolver_ ) );
        agents_.insert( T_Agents::value_type( entity.GetId(), proxy ) );
		const rpr::ForceIdentifier forceIdentifier = sideResolver_.ResolveForce( entity.GetTeam().GetId() );
        
        std::string typeName = "formation"; // FIXME AHC
        rpr::EntityType entityType = ComputeEntityType( entity, logger_, automatEntityTypeResolver_, "formation" );

        for( auto it = listeners_.begin(); it != listeners_.end(); ++it )
            (*it)->AggregateCreated( *proxy, entity.GetId(), entity.GetName().toStdString(), forceIdentifier, entityType, entity.GetApp6Symbol(), true, uniqueId );
        // Must be done after HLA object was created
        unsigned long parentId = entity.GetParent() != 0 ? entity.GetParent()->GetId() : 0;
        if( parentId != 0)
        {
            T_Agents::const_iterator parentIt( agents_.find( parentId ) );
            if( agents_.end() != parentIt )
            {
                parents_[ entity.GetId() ] = parentId;
                parentIt->second->AddSubordinate( entity.GetId(), *proxy );
            }
        }
        // Formations creation order is not from top to bottom, so the code above do not create all links. Necessary to process children formations
        tools::Iterator< const dispatcher::Formation_ABC& > childIt( entity.GetFormations().CreateIterator() );
        while( childIt.HasMoreElements() )
        {
            const dispatcher::Formation_ABC& child( childIt.NextElement() );
            T_Agents::const_iterator childAgent( agents_.find( child.GetId() ) );
            if( agents_.end() !=  childAgent && !proxy->HasSubordinate( childAgent->first ) )
            {
                proxy->AddSubordinate( childAgent->first, *childAgent->second );
                if( dynamic_cast< FormationProxy* >( childAgent->second.get() ) )
                {
                    dynamic_cast< FormationProxy* >( childAgent->second.get() )->PublishParent();
                }
            }
        }
    }
}
