// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "OrderTypes.h"
#include "OrderType.h"
#include "dispatcher/Config.h"
#include "xeumeuleu/xml.h"

using namespace xml;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: OrderTypes constructor
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------
OrderTypes::OrderTypes( const dispatcher::Config& config )
{
    xml::xifstream xis( config.GetPhysicalFile() );
    xis >> start( "physical" );
    std::string missions;
    xis >> start( "missions" ) >> attribute( "file", missions );

    Load( config.BuildPhysicalChildFile( missions ) );
}

// -----------------------------------------------------------------------------
// Name: OrderTypes destructor
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------
OrderTypes::~OrderTypes()
{
    for( CIT_OrderTypes it = fragOrders_.begin(); it != fragOrders_.end(); ++it )
        delete it->second;
    for( CIT_OrderTypes it = unitMissions_.begin(); it != unitMissions_.end(); ++it )
        delete it->second;
    for( CIT_OrderTypes it = automatMissions_.begin(); it != automatMissions_.end(); ++it )
        delete it->second;
}

// -----------------------------------------------------------------------------
// Name: OrderTypes::Load
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------
void OrderTypes::Load( const std::string& filename )
{
    xml::xifstream xis( filename );
    xis >> start( "missions" )
            >> start( "units" )
                >> list( "mission", *this, &OrderTypes::ReadOrderType, unitMissions_ )
            >> end()
            >> start( "automats" )
                >> list( "mission", *this, &OrderTypes::ReadOrderType, automatMissions_ )
            >> end()
            >> start( "fragorders" )
                >> list( "fragorder", *this, &OrderTypes::ReadOrderType, fragOrders_ )
            >> end()
        >> end();
}

// -----------------------------------------------------------------------------
// Name: OrderTypes::ReadOrderType
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------
void OrderTypes::ReadOrderType( xml::xistream& xis, T_OrderTypes& resolver )
{
    const OrderType* type = new OrderType( xis );
    resolver[type->GetName()] = type;
}

// -----------------------------------------------------------------------------
// Name: OrderTypes::FindAgentMission
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------
const OrderType* OrderTypes::FindAgentMission( const std::string& name ) const
{
    CIT_OrderTypes it = unitMissions_.find( name );
    if( it != unitMissions_.end() )
        return it->second;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: OrderTypes::FindAutomatMission
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------
const OrderType* OrderTypes::FindAutomatMission( const std::string& name ) const
{
    CIT_OrderTypes it = automatMissions_.find( name );
    if( it != automatMissions_.end() )
        return it->second;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: OrderTypes::FindFragOrder
// Created: SBO 2007-06-07
// -----------------------------------------------------------------------------
const OrderType* OrderTypes::FindFragOrder( const std::string& name ) const
{
    CIT_OrderTypes it = fragOrders_.find( name );
    if( it != fragOrders_.end() )
        return it->second;
    return 0;
}
