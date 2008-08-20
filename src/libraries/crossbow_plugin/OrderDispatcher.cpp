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
#include "Database_ABC.h"
#include "Table_ABC.h"
#include "Row_ABC.h"
#include "clients_kernel/OrderType.h"
#include "clients_kernel/OrderParameter.h"
#include "dispatcher/SimulationPublisher_ABC.h"
#include "dispatcher/Network_Def.h"
#include "dispatcher/Model.h"
#include "dispatcher/Agent.h"
#include "dispatcher/Automat.h"

using namespace crossbow;

// -----------------------------------------------------------------------------
// Name: OrderDispatcher constructor
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------
OrderDispatcher::OrderDispatcher( Database_ABC& database, const OrderTypes& types, const dispatcher::Model& model )
    : types_( types )
    , model_( model )
    , database_ ( database )
//    , paramTable_( database.OpenTable( "OrderParameters" ) )
    , serializer_( new OrderParameterSerializer( database, model ) )
{
    database.ClearTable( "OrderParameters" );
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
    if( const dispatcher::Agent* agent = model_.GetAgents().Find( id ) )
        DispatchMission( publisher, *agent, row );
    else if( const dispatcher::Automat* automat = model_.GetAutomats().Find( id ) )
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
}

// -----------------------------------------------------------------------------
// Name: OrderDispatcher::DispatchMission
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------
void OrderDispatcher::DispatchMission( dispatcher::SimulationPublisher_ABC& publisher, const dispatcher::Agent& agent, const Row_ABC& row )
{
    if( agent.GetAutomat().IsEngaged() )
    {
        DispatchMission( publisher, agent.GetAutomat(), row );
        return;
    }

    const kernel::OrderType* type = GetAgentMission( row );
    if( !type )
    {
        // DispatchFragOrder( publisher, agent.GetID(), row );
        return;
    }

    const long orderId = GetField< long >( row, "OrderID" );

    dispatcher::AsnMsgClientToSimUnitOrder asn;
    asn().oid = agent.GetID();
    asn().mission = type->GetId();
    SetParameters( asn().parametres, orderId, *type );
    asn.Send( publisher );
    CleanParameters( asn().parametres );
}

// -----------------------------------------------------------------------------
// Name: OrderDispatcher::DispatchMission
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------
void OrderDispatcher::DispatchMission( dispatcher::SimulationPublisher_ABC& publisher, const dispatcher::Automat& automat, const Row_ABC& row )
{
    const kernel::OrderType* type = GetAutomatMission( row );
    if( !type )
    {
        // DispatchFragOrder( publisher, automat.GetID(), row );
        return;
    }

    const long orderId = GetField< long >( row, "OrderID" );
    
    dispatcher::AsnMsgClientToSimAutomatOrder asn;
    asn().oid = automat.GetID();
    asn().mission = type->GetId();
    asn().formation = EnumAutomatOrderFormation::deux_echelons; // $$$$ SBO 2007-06-01:
    try  
    {
        SetParameters( asn().parametres, orderId, *type );
        asn.Send( publisher );
    }
    catch ( std::exception& e )
    {
        MT_LOG_ERROR_MSG( "Unable to resolve order target unit : " << e.what() );
    }
    CleanParameters( asn().parametres );
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

    dispatcher::AsnMsgClientToSimFragOrder asn;
    asn().oid = targetId;
    asn().frag_order = type->GetId();
    asn().parametres.n = 0; // $$$$ SBO 2007-06-07: parameters not supported !
    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: OrderDispatcher::GetTargetId
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------
unsigned long OrderDispatcher::GetTargetId( const Row_ABC& row ) const
{
    return GetField< long >( row, "TargetId" );
}

// -----------------------------------------------------------------------------
// Name: OrderDispatcher::GetAgentMission
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------
const kernel::OrderType* OrderDispatcher::GetAgentMission( const Row_ABC& row ) const
{
    const std::string name = GetField< std::string >( row, "Name" );
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
    const std::string name = GetField< std::string >( row, "Name" );
    const kernel::OrderType* order = types_.FindAutomatMission( name );
    if( !order )
        MT_LOG_ERROR_MSG( "Unknown automat mission : " << name );
    return order;
}

// -----------------------------------------------------------------------------
// Name: OrderDispatcher::SetParameters
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------
void OrderDispatcher::SetParameters( ASN1T_MissionParameters& parameters, unsigned long orderId, const kernel::OrderType& type )
{
    std::auto_ptr< Table_ABC > params_( database_.OpenTable( "OrderParameters" ) );

    parameters.n = type.Count();
    parameters.elem = new ASN1T_MissionParameter[ parameters.n ];

    std::stringstream ss;
    ss << "OrderID=" << orderId;
    Row_ABC* result = params_->Find( ss.str() );
    unsigned int i ; 
	for( i = 0; result != 0 && i < parameters.n; ++i )
    {
        SetParameter( parameters.elem[i], *result, type );
        result = params_->GetNextRow();
    }
    // TODO : if ( i < parameters.n )
}

namespace
{
    const kernel::OrderParameter* GetParameterByName( const kernel::OrderType& type, const std::string& name )
    {
        kernel::Iterator< const kernel::OrderParameter& > it( type.CreateIterator() );
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
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------
void OrderDispatcher::SetParameter( ASN1T_MissionParameter& parameter, const Row_ABC& row, const kernel::OrderType& type )
{
    const std::string name = GetField< std::string >( row, "Name" );
    long parameterId = GetField< long >( row, "OBJECTID" );
    const kernel::OrderParameter* param = GetParameterByName( type, name );
    parameter.null_value = param ? 0 : 1;
    if( param )
        serializer_->Serialize( parameter, *param, parameterId, GetField< std::string >( row, "Value" ) );
}

// -----------------------------------------------------------------------------
// Name: OrderDispatcher::CleanParameters
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------
void OrderDispatcher::CleanParameters( ASN1T_MissionParameters& parameters )
{
    for( unsigned int i = 0; i < parameters.n; ++i )
        serializer_->Clean( parameters.elem[i] );
}
