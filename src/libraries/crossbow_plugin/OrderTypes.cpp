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
#include "clients_kernel/MissionType.h"
#include "clients_kernel/FragOrderType.h"
#include "dispatcher/Config.h"
#include <xeumeuleu/xml.hpp>

using namespace plugins;
using namespace plugins::crossbow;

// -----------------------------------------------------------------------------
// Name: OrderTypes constructor
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------
OrderTypes::OrderTypes( const dispatcher::Config& config )
{
    xml::xifstream xis( config.GetPhysicalFile() );
    xis >> xml::start( "physical" );
    std::string missions;
    xis >> xml::start( "missions" )
        >> xml::attribute( "file", missions );

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
// Created: SBO 2006-11-29
// -----------------------------------------------------------------------------
void OrderTypes::Load( const std::string& missions )
{
    xml::xifstream xis( missions );
    xis >> xml::start( "missions" );
    ReadMissions( xis, "units"      , unitMissions_ );
    ReadMissions( xis, "automats"   , automatMissions_ );
    xis     >> xml::start( "fragorders" )
                >> xml::list( "fragorder", *this, &OrderTypes::ReadFragOrderType )
            >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: OrderTypes::ReadMissions
// Created: SBO 2008-03-05
// -----------------------------------------------------------------------------
void OrderTypes::ReadMissions( xml::xistream& xis, const std::string& name, T_OrderTypes& missions )
{
    xis >> xml::start( name );
    xis     >> xml::list( "mission", *this, &OrderTypes::ReadMissionType, missions )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: OrderTypes::ReadMissionType
// Created: SBO 2006-11-29
// -----------------------------------------------------------------------------
void OrderTypes::ReadMissionType( xml::xistream& xis, T_OrderTypes& missions )
{
    kernel::MissionType* mission = new kernel::MissionType( xis );
    missions[mission->GetName()] = mission;
}

// -----------------------------------------------------------------------------
// Name: OrderTypes::ReadFragOrderType
// Created: SBO 2006-11-29
// -----------------------------------------------------------------------------
void OrderTypes::ReadFragOrderType( xml::xistream& xis )
{
    kernel::FragOrderType* order = new kernel::FragOrderType( xis );
    fragOrders_[order->GetName()] = order;
}

// -----------------------------------------------------------------------------
// Name: OrderTypes::FindAgentMission
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------
const kernel::OrderType* OrderTypes::FindAgentMission( const std::string& name ) const
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
const kernel::OrderType* OrderTypes::FindAutomatMission( const std::string& name ) const
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
const kernel::OrderType* OrderTypes::FindFragOrder( const std::string& name ) const
{
    CIT_OrderTypes it = fragOrders_.find( name );
    if( it != fragOrders_.end() )
        return it->second;
    return 0;
}
