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
#include "Connector.h"
#include "OrderTypes.h"
#include "OrderType.h"
#include "OrderParameterSerializer.h"
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
OrderDispatcher::OrderDispatcher( Connector& connector, const OrderTypes& types, const dispatcher::Model& model )
    : types_( types )
    , model_( model )
    , paramTable_( connector.GetTable( "OrdersParameters" ) )
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

namespace
{
    CComVariant GetFieldValue( const IRowPtr& row, const std::string& name )
    {
        IFieldsPtr fields;
        row->get_Fields( &fields );
        long index;
        fields->FindField( CComBSTR( name.c_str() ), &index );
        CComVariant value;
        row->get_Value( index, &value );
        return value;
    }

    unsigned long GetInteger( const CComVariant& value )
    {
        switch( value.vt )
        {
        case VT_I2:
            return value.iVal;
        case VT_I4:
            return value.lVal;
        default:
            return 0;
        }
    }

    std::string GetString( const CComVariant& value )
    {
        switch( value.vt )
        {
        case VT_BSTR:
            return std::string( _bstr_t( value.bstrVal ) ); // $$$$ SBO 2007-05-31: !!
        default:
            return "";
        }
    }
}

// -----------------------------------------------------------------------------
// Name: OrderDispatcher::Dispatch
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------
void OrderDispatcher::Dispatch( dispatcher::SimulationPublisher_ABC& publisher, const IRowPtr& row )
{
    const unsigned long id = GetTargetId( row );
    if( const dispatcher::Agent* agent = model_.GetAgents().Find( id ) )
        DispatchMission( publisher, *agent, row );
    else if( const dispatcher::Automat* automat = model_.GetAutomats().Find( id ) )
        DispatchMission( publisher, *automat, row );
    else
        MT_LOG_ERROR_MSG( "Unable to resolve order target unit : " << id );
}

// -----------------------------------------------------------------------------
// Name: OrderDispatcher::DispatchMission
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------
void OrderDispatcher::DispatchMission( dispatcher::SimulationPublisher_ABC& publisher, const dispatcher::Agent& agent, const IRowPtr& row )
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

    long orderId;
    row->get_OID( &orderId );

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
void OrderDispatcher::DispatchMission( dispatcher::SimulationPublisher_ABC& publisher, const dispatcher::Automat& automat, const IRowPtr& row )
{
    const OrderType* type = GetAutomatMission( row );
    if( !type )
    {
        DispatchFragOrder( publisher, automat.GetID(), row );
        return;
    }

    long orderId;
    row->get_OID( &orderId );

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
void OrderDispatcher::DispatchFragOrder( dispatcher::SimulationPublisher_ABC& publisher, unsigned long targetId, const IRowPtr& row )
{
    const OrderType* type = types_.FindFragOrder( GetString( GetFieldValue( row, "OrderName" ) ) );
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
unsigned long OrderDispatcher::GetTargetId( const IRowPtr& row ) const
{
    return GetInteger( GetFieldValue( row, "target_id" ) );
}

// -----------------------------------------------------------------------------
// Name: OrderDispatcher::GetAgentMission
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------
const OrderType* OrderDispatcher::GetAgentMission( const IRowPtr& row ) const
{
    std::string         name( GetString( GetFieldValue( row, "OrderName" ) ) );
    const OrderType*    order( types_.FindAgentMission( name ) );

    if( !order )
        MT_LOG_ERROR_MSG( "Unknown agent mission : " << name );
    return order;
}

// -----------------------------------------------------------------------------
// Name: OrderDispatcher::GetAutomatMission
// Created: SBO 2007-06-01
// -----------------------------------------------------------------------------
const OrderType* OrderDispatcher::GetAutomatMission( const IRowPtr& row ) const
{
    std::string         name( GetString( GetFieldValue( row, "OrderName" ) ) );
    const OrderType*    order( types_.FindAutomatMission( name ) );

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

    IQueryFilterPtr parametersFilter;
    ICursorPtr cursor;
    std::stringstream ss;
    ss << "order_id=" << orderId << " AND context=false";
    parametersFilter.CreateInstance( CLSID_QueryFilter );
    parametersFilter->put_WhereClause( CComBSTR( ss.str().c_str() ) );
    HRESULT res = paramTable_->Search( parametersFilter, false, &cursor );
    if( FAILED( res ) )
        throw std::runtime_error( "Search failed" ); // $$$$ SBO 2007-05-30:
    IRowPtr row;
    cursor->NextRow( &row );
    for( unsigned int i = 0; row != 0 && i < parameters.n; ++i )
    {
        SetParameter( parameters.elem[i], row, type );
        cursor->NextRow( &row );
    }
}

// -----------------------------------------------------------------------------
// Name: OrderDispatcher::SetParameter
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------
void OrderDispatcher::SetParameter( ASN1T_MissionParameter& parameter, const IRowPtr& row, const OrderType& type )
{
    const std::string name = GetString( GetFieldValue( row, "name" ) );
    const OrderParameter* param = type.FindParameter( name );
    parameter.null_value = param ? 0 : 1;
    if( param )
        serializer_->Serialize( parameter, *param, GetString( GetFieldValue( row, "ParamValue" ) ) );
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

    IQueryFilterPtr parametersFilter;
    ICursorPtr cursor;
    std::stringstream ss;
    ss << "order_id=" << orderId << " AND context=true";
    parametersFilter.CreateInstance( CLSID_QueryFilter );
    parametersFilter->put_WhereClause( CComBSTR( ss.str().c_str() ) );
    HRESULT res = paramTable_->Search( parametersFilter, false, &cursor );
    if( FAILED( res ) )
        throw std::runtime_error( "Search failed" ); // $$$$ SBO 2007-05-30:
    IRowPtr row;
    cursor->NextRow( &row );
    for( unsigned int i = 0; row != 0; ++i )
    {
        SetParameter( asn, row );
        cursor->NextRow( &row );
    }
}

// -----------------------------------------------------------------------------
// Name: OrderDispatcher::SetParameter
// Created: SBO 2007-06-01
// -----------------------------------------------------------------------------
void OrderDispatcher::SetParameter( ASN1T_OrderContext& asn, const IRowPtr& row )
{
    const std::string name = GetString( GetFieldValue( row, "name" ) );
    if( name == "limits" )
        serializer_->SerializeLimits( asn, GetString( GetFieldValue( row, "ParamValue" ) ) );
    else if( name == "lima" )
        serializer_->SerializeLima( asn, GetString( GetFieldValue( row, "ParamValue" ) ) );
    else if( name == "direction" )
        serializer_->SerializeDirection( asn, GetString( GetFieldValue( row, "ParamValue" ) ) );
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
    IQueryFilterPtr parametersFilter;
    ICursorPtr cursor;
    std::stringstream ss;
    ss << "order_id=" << orderId << " AND context=true AND name='lima'";
    parametersFilter.CreateInstance( CLSID_QueryFilter );
    parametersFilter->put_WhereClause( CComBSTR( ss.str().c_str() ) );
    long count = 0;
    HRESULT res = paramTable_->RowCount( parametersFilter, &count );
    if( FAILED( res ) )
        throw std::runtime_error( "Search failed" ); // $$$$ SBO 2007-05-30:
    return count;
}
