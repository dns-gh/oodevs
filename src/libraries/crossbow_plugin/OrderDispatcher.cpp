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
#include "OrderType.h"
#include "OrderParameterSerializer.h"
#include "Database_ABC.h"
#include "Table_ABC.h"
#include "Row_ABC.h"
#include "dispatcher/SimulationPublisher_ABC.h"
#include "dispatcher/Network_Def.h"
#include "dispatcher/Model.h"
#include "dispatcher/Agent.h"
#include "dispatcher/Automat.h"

using namespace kernel;
using namespace crossbow;

// -----------------------------------------------------------------------------
// Name: OrderDispatcher constructor
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------
OrderDispatcher::OrderDispatcher( Database_ABC& database, const OrderTypes& types, const dispatcher::Model& model )
    : types_( types )
    , model_( model )
    , paramTable_( database.OpenTable( "OrdersParameters" ) )
    , serializer_( new OrderParameterSerializer( model ) )
{
    // NOTHING
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

    const OrderType* type = GetAgentMission( row );
    if( !type )
    {
        DispatchFragOrder( publisher, agent.GetID(), row );
        return;
    }

    const long orderId = GetField< long >( row, "id" );

    dispatcher::AsnMsgClientToSimUnitOrder asn;
    asn().oid = agent.GetID();
    asn().mission = type->GetId();
    SetParameters( asn().parametres, orderId, *type );
    SetOrderContext( asn().order_context, orderId );
    asn.Send( publisher );
    CleanParameters( asn().parametres );
    CleanOrderContext( asn().order_context );
}

// -----------------------------------------------------------------------------
// Name: OrderDispatcher::DispatchMission
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------
void OrderDispatcher::DispatchMission( dispatcher::SimulationPublisher_ABC& publisher, const dispatcher::Automat& automat, const Row_ABC& row )
{
    const OrderType* type = GetAutomatMission( row );
    if( !type )
    {
        DispatchFragOrder( publisher, automat.GetID(), row );
        return;
    }

    const long orderId = GetField< long >( row, "id" );

    dispatcher::AsnMsgClientToSimAutomatOrder asn;
    asn().oid = automat.GetID();
    asn().mission = type->GetId();
    SetParameters( asn().parametres, orderId, *type );
    SetOrderContext( asn().order_context, orderId );
    asn().formation = EnumAutomatOrderFormation::deux_echelons; // $$$$ SBO 2007-06-01:
    asn.Send( publisher );
    CleanParameters( asn().parametres );
    CleanOrderContext( asn().order_context );
}

// -----------------------------------------------------------------------------
// Name: OrderDispatcher::DispatchFragOrder
// Created: SBO 2007-06-07
// -----------------------------------------------------------------------------
void OrderDispatcher::DispatchFragOrder( dispatcher::SimulationPublisher_ABC& publisher, unsigned long targetId, const Row_ABC& row )
{
    const OrderType* type = types_.FindFragOrder( GetField< std::string >( row, "OrderName" ) );
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
    return GetField< int >( row, "target_id" );
}

// -----------------------------------------------------------------------------
// Name: OrderDispatcher::GetAgentMission
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------
const OrderType* OrderDispatcher::GetAgentMission( const Row_ABC& row ) const
{
    std::string      name( GetField< std::string >( row, "OrderName" ) );
    const OrderType* order( types_.FindAgentMission( name ) );
    if( !order )
        MT_LOG_ERROR_MSG( "Unknown agent mission : " << name );
    return order;
}

// -----------------------------------------------------------------------------
// Name: OrderDispatcher::GetAutomatMission
// Created: SBO 2007-06-01
// -----------------------------------------------------------------------------
const OrderType* OrderDispatcher::GetAutomatMission( const Row_ABC& row ) const
{
    std::string      name( GetField< std::string >( row, "OrderName" ) );
    const OrderType* order( types_.FindAutomatMission( name ) );
    if( !order )
        MT_LOG_ERROR_MSG( "Unknown automat mission : " << name );
    return order;
}

// -----------------------------------------------------------------------------
// Name: OrderDispatcher::SetParameters
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------
void OrderDispatcher::SetParameters( ASN1T_MissionParameters& parameters, unsigned long orderId, const OrderType& type )
{
    parameters.n = type.GetParameterCount();
    parameters.elem = new ASN1T_MissionParameter[ parameters.n ];

    std::stringstream ss;
    ss << "order_id=" << orderId << " AND context=0";
    Row_ABC* result = paramTable_.Find( ss.str() );
    for( unsigned int i = 0; result != 0 && i < parameters.n; ++i )
    {
        SetParameter( parameters.elem[i], *result, type );
        result = paramTable_.GetNextRow();
    }
}

// -----------------------------------------------------------------------------
// Name: OrderDispatcher::SetParameter
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------
void OrderDispatcher::SetParameter( ASN1T_MissionParameter& parameter, const Row_ABC& row, const OrderType& type )
{
    const std::string name = GetField< std::string >( row, "name" );
    const OrderParameter* param = type.FindParameter( name );
    parameter.null_value = param ? 0 : 1;
    if( param )
        serializer_->Serialize( parameter, *param, GetField< std::string >( row, "ParamValue" ) );
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

// -----------------------------------------------------------------------------
// Name: OrderDispatcher::SetOrderContext
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------
void OrderDispatcher::SetOrderContext( ASN1T_OrderContext& asn, unsigned long orderId )
{
    asn.direction_dangereuse = 0;
    asn.limas.n = GetLimaCount( orderId );
    asn.limas.elem = 0;
    asn.m.limite_droitePresent = 0;
    asn.m.limite_gauchePresent = 0;
    asn.intelligences.n = 0;
    asn.intelligences.elem = 0;

    std::stringstream ss;
    ss << "order_id=" << orderId << " AND context=-1";
    Row_ABC* result = paramTable_.Find( ss.str() );
    while( result )
    {
        SetParameter( asn, *result );
        result = paramTable_.GetNextRow();
    }
}

// -----------------------------------------------------------------------------
// Name: OrderDispatcher::SetParameter
// Created: SBO 2007-06-01
// -----------------------------------------------------------------------------
void OrderDispatcher::SetParameter( ASN1T_OrderContext& asn, const Row_ABC& row )
{
    const std::string name = GetField< std::string >( row, "name" );
    if( name == "limits" )
        serializer_->SerializeLimits( asn, GetField< std::string >( row, "ParamValue" ) );
    else if( name == "lima" )
        serializer_->SerializeLima( asn, GetField< std::string >( row, "ParamValue" ) );
    else if( name == "direction" )
        serializer_->SerializeDirection( asn, GetField< std::string >( row, "ParamValue" ) );
}

// -----------------------------------------------------------------------------
// Name: OrderDispatcher::CleanOrderContext
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------
void OrderDispatcher::CleanOrderContext( ASN1T_OrderContext& asn )
{
    serializer_->Clean( asn );
}

// -----------------------------------------------------------------------------
// Name: OrderDispatcher::GetLimaCount
// Created: SBO 2007-06-06
// -----------------------------------------------------------------------------
unsigned int OrderDispatcher::GetLimaCount( unsigned long orderId )
{
    std::stringstream ss;
    ss << "order_id=" << orderId << " AND context=-1 AND name='lima'";
    Row_ABC* result = paramTable_.Find( ss.str() );
    unsigned int count = 0;
    while( result != 0 )
    {
        ++count;
        result = paramTable_.GetNextRow();
    }
    return count;
}
