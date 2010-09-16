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
#include "protocol/clientsenders.h"
#include <boost/lexical_cast.hpp>
#include <boost/noncopyable.hpp>

using namespace plugins;
using namespace plugins::crossbow;

// -----------------------------------------------------------------------------
// Name: OrderDispatcher constructor
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------
OrderDispatcher::OrderDispatcher( Workspace_ABC& workspace, const OrderTypes& types, const dispatcher::Model_ABC& model )
    : types_     ( types )
    , model_     ( model )
    , database_  ( workspace.GetDatabase( "flat" ) )
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

// -----------------------------------------------------------------------------
// Name: OrderDispatcher::Dispatch
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------
void OrderDispatcher::Dispatch( dispatcher::SimulationPublisher_ABC& publisher, const Row_ABC& row )
{
    const unsigned long id = GetTargetId( row );
    if( const dispatcher::Agent_ABC* agent = model_.Agents().Find( id ) )
        DispatchMission( publisher, *agent, row );
    else if( const dispatcher::Automat_ABC* automat = model_.Automats().Find( id ) )
        DispatchMission( publisher, *automat, row );
    else
        MT_LOG_ERROR_MSG( "Unable to resolve order target unit : " << id );
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
            CleanParameters( asn_().parametres );
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
// Name: OrderDispatcher::DispatchMission
// Updated: JCR
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------
void OrderDispatcher::DispatchMission( dispatcher::SimulationPublisher_ABC& publisher, const dispatcher::Agent_ABC& agent, const Row_ABC& row )
{
    if( agent.GetSuperior().IsEngaged() )
    {
        DispatchMission( publisher, agent.GetSuperior(), row );
        return;
    }
    const kernel::OrderType* type = GetAgentMission( row );
    if( !type )
    {
        // DispatchFragOrder( publisher, agent.GetId(), row );
        return;
    }
    simulation::UnitOrder message;
    try
    {
        const long orderId = GetField< long >( row, "id" );
        message().mutable_tasker()->set_id( agent.GetId() );
        message().mutable_type()->set_id( type->GetId() );
        SetParameters( *message().mutable_parameters(), orderId, *type );
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
void OrderDispatcher::DispatchMission( dispatcher::SimulationPublisher_ABC& publisher, const dispatcher::Automat_ABC& automat, const Row_ABC& row )
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
        const long orderId = GetField< long >( row, "id" );
        message().mutable_tasker()->set_id( automat.GetId() );
        message().mutable_type()->set_id( type->GetId() );
        SetParameters( *message().mutable_parameters(), orderId, *type );
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
    message().mutable_parametres(); // $$$$ SBO 2007-06-07: parameters not supported !
    message.Send( publisher, 0 );
}

// -----------------------------------------------------------------------------
// Name: OrderDispatcher::GetTargetId
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------
unsigned long OrderDispatcher::GetTargetId( const Row_ABC& row ) const
{
    return GetField< long >( row, "unit_id" );
}

// -----------------------------------------------------------------------------
// Name: OrderDispatcher::GetAgentMission
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------
const kernel::OrderType* OrderDispatcher::GetAgentMission( const Row_ABC& row ) const
{
    const std::string name = GetField< std::string >( row, "name" );
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
    const std::string name = GetField< std::string >( row, "name" );
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
    std::auto_ptr< Table_ABC > params_( database_.OpenTable( "OrderParameters" ) );
    Row_ABC* result = params_->Find( "order_id=" + boost::lexical_cast< std::string >( orderId ) );
    for( unsigned long i = 0; result != 0 && i < type.Count(); ++i )
    {
        SetParameter( *parameters.add_elem(), *result, type );
        result = params_->GetNextRow();
    }
    // TODO : if( i < parameters.elem_size() )
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
// Name: OrderDispatcher::SetParameter
// Updated: JCR
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------
void OrderDispatcher::SetParameter( Common::MsgMissionParameter& parameter, const Row_ABC& row, const kernel::OrderType& type )
{
    const std::string name( GetField< std::string >( row, "name" ) );
    const long parameterId = GetField< long >( row, "id" ); // OBJECTID
    try
    {
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
