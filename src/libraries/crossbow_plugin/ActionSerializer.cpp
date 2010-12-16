// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "ActionSerializer.h"
#include "ActionParameterSerializer.h"
#include "Workspace_ABC.h"
#include "Database_ABC.h"
#include "Row_ABC.h"
#include "Table_ABC.h"
#include "dispatcher/Config.h"
#include "dispatcher/AgentKnowledgeConverter.h"
#include "dispatcher/ObjectKnowledgeConverter.h"
#include "dispatcher/ModelAdapter.h"
#include "dispatcher/Agent_ABC.h"
#include "dispatcher/Automat_ABC.h"
#include "actions/ActionFactory.h"
#include "actions/ParameterFactory_ABC.h"
#include "actions/Action_ABC.h"
#include "actions/Parameter_ABC.h"
#include "clients_kernel/Mission.h"
#include "clients_kernel/OrderType.h"
#include "clients_kernel/OrderParameter.h"
#include "clients_kernel/StaticModel.h"
#include "clients_kernel/Time_ABC.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/DecisionalModel.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/CoordinateConverter.h"
#include "clients_kernel/Controller.h"
#include "tools/Iterator.h"
#include "tools/Observer_ABC.h"
#include "tools/ElementObserver_ABC.h"
#include <boost/lexical_cast.hpp>
#pragma warning( push, 0 )
#include <boost/algorithm/string.hpp>
#pragma warning( pop )

using namespace plugins::crossbow;

namespace
{
    Database_ABC& GetDatabase( Workspace_ABC& workspace )
    {
        return workspace.GetDatabase( "flat" );
    }

    template< typename Type >
    Type GetField( const Row_ABC& row, const std::string& name )
    {
        return boost::get< Type >( row.GetField( name ) );
    }

    struct SimulationTime : public kernel::Time_ABC
    {
        virtual QDateTime GetDateTime() const { return QDateTime(); }
        virtual QDateTime GetInitialDateTime() const { return QDateTime(); }
        virtual unsigned int GetTickDuration() const { return 0; }
    };

    struct NullParameterFactory : public actions::ParameterFactory_ABC
    {
        virtual actions::Parameter_ABC* CreateParameter( const kernel::OrderParameter&, xml::xistream&, const kernel::Entity_ABC& ) const { return 0; }
        virtual actions::Parameter_ABC* CreateParameter( const kernel::OrderParameter&, xml::xistream& ) const { return 0; }
        virtual actions::Parameter_ABC* CreateParameter( const kernel::OrderParameter&, const sword::MissionParameter&, const kernel::Entity_ABC& ) const { return 0; }
        virtual actions::Parameter_ABC* CreateParameter( const kernel::OrderParameter&, const sword::MissionParameter_Value&, const kernel::Entity_ABC& ) const { return 0; }
    };
}

// -----------------------------------------------------------------------------
// Name: ActionSerializer constructor
// Created: JCR 2010-12-07
// -----------------------------------------------------------------------------
ActionSerializer::ActionSerializer( const dispatcher::Config& config, dispatcher::Model_ABC& model, const kernel::StaticModel& staticModel, Workspace_ABC& workspace )
    : workspace_        ( workspace )
    , controller_       ( new kernel::Controller() )
    , entities_         ( new dispatcher::ModelAdapter( model ) )
    , converter_        ( new kernel::CoordinateConverter( config ) )
    , time_             ( new SimulationTime() )
    , agentsKnowledges_ ( new dispatcher::AgentKnowledgeConverter( model ) )
    , objectsKnowledges_( new dispatcher::ObjectKnowledgeConverter( model ) )
    , nullParameters_   ( new NullParameterFactory() )
    , factory_          ( new actions::ActionFactory( *controller_, *nullParameters_, *entities_, staticModel, *time_ ) )
    , serializer_       ( new ActionParameterSerializer( workspace, *converter_, *agentsKnowledges_, *objectsKnowledges_, *entities_, *controller_ ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionSerializer destructor
// Created: JCR 2010-12-07
// -----------------------------------------------------------------------------
ActionSerializer::~ActionSerializer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionSerializer::SerializeMission
// Created: JCR 2010-12-07
// -----------------------------------------------------------------------------
void ActionSerializer::SerializeMission( const dispatcher::Agent_ABC& agent, const Row_ABC& orderRow, std::auto_ptr< actions::Action_ABC >& action )
{
    unsigned long missionId = static_cast< unsigned long >( GetField< int >( orderRow, "type_id" ) );
    const tools::Resolver_ABC< kernel::Mission >& resolver = agent.GetType().GetDecisionalModel();
    const kernel::Mission* mission = resolver.Find( missionId );
    
    if( mission )
    {
        try
        {
            action.reset( factory_->CreateAction( agent, mission->GetType() ) );
            SetParameters( orderRow.GetID(), agent, *action );
        }
        catch ( std::exception& e )
        {
            throw std::runtime_error( "Agent '" + agent.GetType().GetName() + "' with mission '" + mission->GetName() + "': \n" + e.what() );
        }
    }
    else
        throw std::runtime_error( "Unknown mission id '" + boost::lexical_cast< std::string >( missionId ) + "' for agent '" + agent.GetType().GetName() + "'" ); 
}
    
// -----------------------------------------------------------------------------
// Name: ActionSerializer::SerializeMission
// Created: JCR 2010-12-07
// -----------------------------------------------------------------------------
void ActionSerializer::SerializeMission( const dispatcher::Automat_ABC& automat, const Row_ABC& orderRow, std::auto_ptr< actions::Action_ABC >& action )
{
    unsigned long missionId = static_cast< unsigned long >( GetField< int >( orderRow, "type_id" ) );
    const tools::Resolver_ABC< kernel::Mission >& resolver = automat.GetType().GetDecisionalModel();
    const kernel::Mission* mission = resolver.Find( missionId );
    
    if( mission )
    {
        try 
        {
            action.reset( factory_->CreateAction( automat, mission->GetType() ) );
            SetParameters( orderRow.GetID(), automat, *action );
        }
        catch ( std::exception& e )
        {
            throw std::runtime_error( "Automat '" + automat.GetType().GetName() + "' with mission '" + mission->GetName() + "': \n" + e.what() );
        }
    }
    else
        throw std::runtime_error( "Unknown mission id '" + boost::lexical_cast< std::string >( missionId ) + "' for automat '" + automat.GetType().GetName() + "'" ); 
}
    
// -----------------------------------------------------------------------------
// Name: ActionSerializer::SerializeFragOrder
// Created: JCR 2010-12-07
// -----------------------------------------------------------------------------
void ActionSerializer::SerializeFragOrder( const kernel::Entity_ABC& /*target*/, const Row_ABC& /*orderRow*/, std::auto_ptr< actions::Action_ABC >& /*action*/ )
{
    // Created: JCR 2010-12-07
    /*
    const kernel::FragOrderType* fragOrder;
    action.reset( factory_->CreateAction( target, *fragOrder );
    SetParameters( row.GetID(), *action );
    */
    // $$$$ JCR 2010-12-07: TODO
}

namespace
{
    class ErrorLogger
    {
    public:
        ErrorLogger() : empty_ ( true ) {}

        template< typename T >
        ErrorLogger& operator<< ( const T& error ) 
        {
            ss_ << error << std::endl;
            empty_ = false;
            return *this;
        }

        bool empty() const 
        {
            return empty_;
        }

        std::string str() const
        {
            return ss_.str();
        }

    private:
        bool empty_;
        std::stringstream ss_;
    };
}

// -----------------------------------------------------------------------------
// Name: ActionSerializer::SetParameters
// Created: JCR 2010-12-07
// -----------------------------------------------------------------------------
void ActionSerializer::SetParameters( unsigned long orderId, const kernel::Entity_ABC& entity, actions::Action_ABC& action )
{
    ErrorLogger error;
    std::auto_ptr< Table_ABC > params_( GetDatabase( workspace_ ).OpenTable( "OrderParameters" ) );

    tools::Iterator< const kernel::OrderParameter& > it = action.GetType().CreateIterator();
    Row_ABC* result = params_->Find( "order_id=" + boost::lexical_cast< std::string >( orderId ) );
    while( it.HasMoreElements() && result )
    {
        const std::string name( GetField< std::string >( *result, "name" ) );
        const kernel::OrderParameter& elem = it.NextElement();

        if ( name == elem.GetName() )
        {
            const std::string value( GetField< std::string >( *result, "value" ) );
            const std::string type( boost::algorithm::to_lower_copy( GetField< std::string >( *result, "type" ) ) );
            try
            {
                actions::Parameter_ABC* parameter = CreateParameter( elem, result->GetID(), type, value, entity );
                if ( parameter )
                    action.AddParameter( *parameter );
                else
                    throw std::runtime_error( "value " + value );
            }
            catch ( std::exception& e )
            {
                error << "Can not create parameter: " + name + " [" + type + "]:" + e.what();
            }
        }
        else
            error << "Parameter name mismatched, expected: " + elem.GetName() + " got " + name;
        result = params_->GetNextRow();
    }
    if ( !(!it.HasMoreElements() && !result) )
        error << "Mismatch parameter definition.";
    if ( it.HasMoreElements() )
        error << "Waiting for the element " + it.NextElement().GetName();
    if ( !error.empty() )
        throw std::runtime_error( error.str() );
}

// -----------------------------------------------------------------------------
// Name: ActionSerializer::SetParameters
// Created: JCR 2010-12-07
// -----------------------------------------------------------------------------
actions::Parameter_ABC* ActionSerializer::CreateParameter( const kernel::OrderParameter& parameter, unsigned long parameterId, const std::string& type, const std::string& value, const kernel::Entity_ABC& entity ) const
{
    const std::string lowType( parameter.CompatibleType( type ) );
    
    if( lowType == "" )
        throw std::runtime_error( "Unknown parameter type '" + type + "'" );
    std::auto_ptr< actions::Parameter_ABC > param;
    bool found = serializer_->Serialize( parameter, value, entity, param );
    if( found == false )
        found = serializer_->Serialize( parameter, parameterId, value, param );
    if( found == false )
        throw std::runtime_error( "Unable to serialize parameter type '" + type + "'." );
    param->Set( true ); // $$$$ SBO 2007-10-11: ...
    return param.release();
}
