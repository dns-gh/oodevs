// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "OrderDispatcher.h"
#include "OrderTypes.h"
#include "OrderParameterSerializer.h"
#include "Workspace_ABC.h"
#include "Database_ABC.h"
#include "Table_ABC.h"
#include "Row_ABC.h"
#include "clients_kernel/OrderType.h"
#include "clients_kernel/OrderParameter.h"
#include "dispatcher/SimulationPublisher_ABC.h"
#include "dispatcher/Model_ABC.h"
#include "dispatcher/Agent_ABC.h"
#include "dispatcher/Automat_ABC.h"
#include "protocol/SimulationSenders.h"
#include "protocol/ClientSenders.h"
#include <boost/lexical_cast.hpp>
#include <boost/noncopyable.hpp>

using namespace plugins;
using namespace plugins::crossbow;

namespace 
{
    Database_ABC& GetDatabase( Workspace_ABC& workspace )
    {
        return workspace.GetDatabase( "flat" );
    }
}


// -----------------------------------------------------------------------------
// Name: OrderDispatcher constructor
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------
OrderDispatcher::OrderDispatcher( Workspace_ABC& workspace, const OrderTypes& types, const dispatcher::Model_ABC& model )
    : types_( types )
    , model_( model )
    , workspace_ ( workspace )
    , serializer_( new OrderParameterSerializer( workspace, model ) )
{
    // Clean();
}

// -----------------------------------------------------------------------------
// Name: OrderListener::Orders
// Created: JCR 2009-08-14
// -----------------------------------------------------------------------------
void OrderDispatcher::Clean()
{
//    try
//    {
//        std::string clause( "session_id=" + boost::lexical_cast< std::string >( session_.GetId() ) );
//        database_.Execute( DeleteQueryBuilder( database_.GetTableName( "OrderParameters_Area" ), clause ) );
//        database_.Execute( DeleteQueryBuilder( database_.GetTableName( "OrderParameters_Line" ), clause ) );
//        database_.Execute( DeleteQueryBuilder( database_.GetTableName( "OrderParameters_Point" ), clause ) );
//        database_.Execute( DeleteQueryBuilder( database_.GetTableName( "OrderParameters" ), clause ) );
//    }
//    catch ( std::exception& e )
//    {
//        MT_LOG_ERROR_MSG( "OrderListener is not correctly loaded : " + std::string( e.what() ) );
//    }
}

// -----------------------------------------------------------------------------
// Name: OrderDispatcher destructor
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------
OrderDispatcher::~OrderDispatcher()
{
    // NOTHING
}

namespace
{
    template< typename Type >
    Type GetField( const Row_ABC& row, const std::string& name )
    {
        return boost::get< Type >( row.GetField( name ) );
    }

    std::string BuildError( const std::string& mission, int id )
    {
        std::stringstream ssError;
        ssError << "Unable to serialize order " << mission
                << " for [" << id << "]" << std::endl;
        return ssError.str();
    }

    template< typename AsnType >
    class scoped_asn : public boost::noncopyable
    {
    public:
        explicit scoped_asn( OrderParameterSerializer& serializer ) : serializer_ ( serializer ) {}
        ~scoped_asn()
        {
            CleanParameters( asn_().parameters );
        }
        typename AsnType::T_AsnMsgType& operator()()
        {
            return asn_();
        }

        void Send( dispatcher::SimulationPublisher_ABC& publisher )
        {
            asn_.Send( publisher );
        }

    private:
        void CleanParameters( Common::MsgMissionParameters& parameters )
        {
            for( unsigned int i = 0; i < parameters.n; ++i )
                serializer_.Clean( parameters.elem[i] );
        }
    private:
        OrderParameterSerializer& serializer_;
        AsnType asn_;
    };
}

// -----------------------------------------------------------------------------
// Name: OrderDispatcher::IsValidOrder
// Created: JCR 2010-07-19
// -----------------------------------------------------------------------------
bool OrderDispatcher::IsValidOrder( const Row_ABC& row ) const
{
    const int targetId = GetField< int >( row, "unit_id" );
    const kernel::OrderType* type = 0;
    if( const dispatcher::Agent_ABC* agent = model_.Agents().Find( targetId ) )
    {
        if( agent->GetSuperior().IsEngaged() )
            type = GetAutomatMission( row );
        else
            type = GetAgentMission( row );
    } 
    else if( const dispatcher::Automat_ABC* automat = model_.Automats().Find( targetId ) )
        type = GetAutomatMission( row );
    if ( type ) 
        return IsValidOrder( row.GetID(), *type );
    MT_LOG_ERROR_MSG( "Unable to resolve order for the target unit: " << targetId );
    return false;
}


// -----------------------------------------------------------------------------
// Name: OrderDispatcher::Dispatch
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------
void OrderDispatcher::Dispatch( dispatcher::SimulationPublisher_ABC& publisher, const Row_ABC& row )
{
    const int id = GetField< int >( row, "unit_id" );
    if( const dispatcher::Agent_ABC* agent = model_.Agents().Find( id ) )
    {
        if( agent->GetSuperior().IsEngaged() )
            DispatchAutomatMission( publisher, agent->GetSuperior(), row );
        else
            DispatchAgentMission( publisher, *agent, row );
    }
    else if( const dispatcher::Automat_ABC* automat = model_.Automats().Find( id ) )
        DispatchAutomatMission( publisher, *automat, row );
    else
        MT_LOG_ERROR_MSG( "Unable to resolve order for the target unit: " << id );
}


// -----------------------------------------------------------------------------
// Name: OrderDispatcher::DispatchMission
// Updated: JCR
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------
void OrderDispatcher::DispatchAgentMission( dispatcher::SimulationPublisher_ABC& publisher, const dispatcher::Agent_ABC& agent, const Row_ABC& row )
{
    const kernel::OrderType* type = GetAgentMission( row );
    if( !type )
    {
        // DispatchFragOrder( publisher, agent.GetId(), row );
        return;
    }
    simulation::UnitOrder message;
    try
    {
        message().mutable_tasker()->set_id( agent.GetId() );
        message().mutable_type()->set_id( type->GetId() );
        SetParameters( *message().mutable_parameters(), row.GetID(), *type );
        message.Send( publisher );
    }
    catch ( std::exception& e )
    {
        throw std::runtime_error( BuildError( type->GetName(), agent.GetId() ) + e.what() );
    }
}

// -----------------------------------------------------------------------------
// Name: OrderDispatcher::DispatchMission
// Updated: JCR
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------
void OrderDispatcher::DispatchAutomatMission( dispatcher::SimulationPublisher_ABC& publisher, const dispatcher::Automat_ABC& automat, const Row_ABC& row )
{
    const kernel::OrderType* type = GetAutomatMission( row );
    if( !type )
    {
        // DispatchFragOrder( publisher, automat.GetId(), row );
        return;
    }
    simulation::AutomatOrder message;
    try
    {
        message().mutable_tasker()->set_id( automat.GetId() );
        message().mutable_type()->set_id( type->GetId() );
        SetParameters( *message().mutable_parameters(), row.GetID(), *type );
        message.Send( publisher );
    }
    catch ( std::exception& e )
    {
        throw std::runtime_error( BuildError( type->GetName(), automat.GetId() ) + e.what() );
    }
}

// -----------------------------------------------------------------------------
// Name: OrderDispatcher::DispatchFragOrder
// Created: SBO 2007-06-07
// -----------------------------------------------------------------------------
void OrderDispatcher::DispatchFragOrder( dispatcher::SimulationPublisher_ABC& publisher, unsigned long targetId, const Row_ABC& row )
{
    const kernel::OrderType* type = types_.FindFragOrder( GetField< std::string >( row, "OrderName" ) );
    if( !type )
        return; // $$$$ SBO 2007-06-07:
    simulation::FragOrder message;
    model_.SetToTasker( *message().mutable_tasker(), targetId );
    //message().set_id( targetId );
    message().mutable_frag_order()->set_id( type->GetId() );
    message().mutable_parameters(); // $$$$ SBO 2007-06-07: parameters not supported !
    message.Send( publisher, 0 );
}

// -----------------------------------------------------------------------------
// Name: OrderDispatcher::GetAgentMission
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------
const kernel::OrderType* OrderDispatcher::GetAgentMission( const Row_ABC& row ) const
{
    const std::string name( GetField< std::string >( row, "name" ) );
    const kernel::OrderType* order = types_.FindAgentMission( name );
    if( !order )
        MT_LOG_ERROR_MSG( "Unknown agent mission : " << name );
    return order;
}

// -----------------------------------------------------------------------------
// Name: OrderDispatcher::GetAutomatMission
// Created: SBO 2007-06-01
// -----------------------------------------------------------------------------
const kernel::OrderType* OrderDispatcher::GetAutomatMission( const Row_ABC& row ) const
{
    const std::string name( GetField< std::string >( row, "name" ) );
    const kernel::OrderType* order = types_.FindAutomatMission( name );
    if( !order )
        MT_LOG_ERROR_MSG( "Unknown automat mission : " << name );
    return order;
}

// -----------------------------------------------------------------------------
// Name: OrderDispatcher::SetParameters
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------
void OrderDispatcher::SetParameters( Common::MsgMissionParameters& parameters, unsigned long orderId, const kernel::OrderType& type )
{
    std::auto_ptr< Table_ABC > params_( GetDatabase( workspace_ ).OpenTable( "OrderParameters" ) );

    Row_ABC* result = params_->Find( "order_id=" + boost::lexical_cast< std::string >( orderId ) );
    for( unsigned long i = 0; result != 0 && i < type.Count(); ++i )
    {
        SetParameter( *parameters.add_elem(), *result, type );
        result = params_->GetNextRow();
    }
}

namespace
{
    const kernel::OrderParameter* GetParameterByName( const kernel::OrderType& type, const std::string& name )
    {
        tools::Iterator< const kernel::OrderParameter& > it( type.CreateIterator() );
        while( it.HasMoreElements() )
        {
            const kernel::OrderParameter& parameter = it.NextElement();
            if( parameter.GetName() == name )
                return &parameter;
        }
        return 0;
    }
}

// -----------------------------------------------------------------------------
// Name: OrderDispatcher::IsValidOrder
// Created: JCR 2010-07-19
// -----------------------------------------------------------------------------
bool OrderDispatcher::IsValidOrder( unsigned long orderId, const kernel::OrderType& type ) const
{
    std::auto_ptr< Table_ABC > params_( GetDatabase( workspace_ ).OpenTable( "OrderParameters" ) );
    Row_ABC* result = params_->Find( "order_id=" + boost::lexical_cast< std::string >( orderId ) );
    unsigned n = 0;

    try 
    {
        while ( result != NULL && ++n )
        {
            const std::string name( GetField< std::string >( *result, "name" ) );
            const kernel::OrderParameter* param = GetParameterByName( type, name );        
            if( ! ( param && serializer_->IsValidParameter( *param ) ) )
                throw std::runtime_error( "Unknown parameter: " + name );
            result = params_->GetNextRow();
        }
    }
    catch ( std::exception& e )
    {
        MT_LOG_ERROR_MSG( "Order [" << type.GetName() << "] validation error: " << e.what() );
        return false;
    }
    return n == type.Count();
}

// -----------------------------------------------------------------------------
// Name: OrderDispatcher::SetParameter
// Updated: JCR
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------
void OrderDispatcher::SetParameter( Common::MsgMissionParameter& parameter, const Row_ABC& row, const kernel::OrderType& type )
{
    const std::string name( GetField< std::string >( row, "name" ) );
    try
    {
        const long parameterId = row.GetID();
        const kernel::OrderParameter* param = GetParameterByName( type, name );
        parameter.set_null_value( ! param );
        if( param )
            serializer_->Serialize( parameter, *param, parameterId, GetField< std::string >( row, "value" ) );
    }
    catch( std::exception& e )
    {
        std::stringstream ssError;
        ssError << "Cannot serialize parameter : " << name << " [" << type.GetName() << "]" << std::endl << e.what();
        throw std::runtime_error( ssError.str() );
    }
}