// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "MaintenanceResolver.h"
#include "ConsignWriter.h"
#include "NameResolver_ABC.h"
#include "clients_kernel/Tools.h"
#include "tools/FileWrapper.h"

using namespace plugins::logistic;

// -----------------------------------------------------------------------------
// Name: MaintenanceConsignData::WriteConsign
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
void MaintenanceConsignData::WriteConsign( ConsignWriter& output ) const
{
    output  << requestId_
            << tick_
            << simTime_
            << unit_
            << provider_
            << equipment_
            << breakdown_
            << state_
            << stateEndTick_;
    output.End();
}

// -----------------------------------------------------------------------------
// Name: MaintenanceConsignData::ManageMessage
// Created: MMC 2012-08-21
// -----------------------------------------------------------------------------
const ConsignData_ABC& MaintenanceConsignData::ManageMessage( const ::sword::LogMaintenanceHandlingCreation& msg, ConsignResolver_ABC& resolver )
{
    const NameResolver_ABC& nameResolver = resolver.GetNameResolver();
    if( msg.has_tick() )
        creationTick_ = boost::lexical_cast< std::string >( msg.tick() );
    if( msg.has_unit() )
    {
        int unitId = msg.unit().id();
        unitId_ = boost::lexical_cast< std::string >( unitId );
        nameResolver.GetAgentName( unitId, unit_ );
    }
    if( msg.has_equipement() )
    {
        equipmentId_ = boost::lexical_cast< std::string >( msg.equipement().id() );
        nameResolver.GetEquipmentName( msg.equipement(), equipment_ );
    }
    if( msg.has_breakdown() )
    {
        int breakdownId = msg.breakdown().id();
        breakdownId_ = boost::lexical_cast< std::string >( breakdownId );
        nameResolver.GetBreakdownName( msg.breakdown(), breakdown_ );
    }
    resolver.AddToLineIndex( 1 );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: MaintenanceConsignData::ManageMessage
// Created: MMC 2012-08-21
// -----------------------------------------------------------------------------
const ConsignData_ABC& MaintenanceConsignData::ManageMessage( const ::sword::LogMaintenanceHandlingUpdate& msg, ConsignResolver_ABC& resolver )
{
    const NameResolver_ABC& nameResolver = resolver.GetNameResolver();
    if( msg.has_current_state_end_tick() )
    {
        int entTick = msg.current_state_end_tick();
        if( entTick > 0 )
            stateEndTick_ = boost::lexical_cast< std::string >( entTick );
        if( entTick <= resolver.GetCurrentTick() )
            stateEndTick_.clear();
    }
    if( msg.has_unit() )
    {
        int unitId = msg.unit().id();
        unitId_ = boost::lexical_cast< std::string >( unitId );
        nameResolver.GetAgentName( unitId, unit_ );
    }
    if( msg.has_provider() )
    {
        int providerId = msg.provider().id();
        providerId_ = boost::lexical_cast< std::string >( providerId );
        nameResolver.GetAgentName( providerId, provider_ );
    }
    if( msg.has_state() )
    {
        sword::LogMaintenanceHandlingUpdate::EnumLogMaintenanceHandlingStatus eState = msg.state();
        nameResolver.GetMaintenanceName( eState, state_ );
        stateId_ = boost::lexical_cast< std::string >( static_cast< int >( eState ) );
    }
    resolver.AddToLineIndex( 1 );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: MaintenanceResolver constructor
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
MaintenanceResolver::MaintenanceResolver( const tools::Path& name, const NameResolver_ABC& nameResolver )
    : ConsignResolver_ABC( name, nameResolver )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MaintenanceResolver destructor
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
MaintenanceResolver::~MaintenanceResolver()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MaintenanceResolver::IsManageable
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
bool MaintenanceResolver::IsManageable( const sword::SimToClient& message )
{
    return      message.message().has_log_maintenance_handling_creation()
            ||  message.message().has_log_maintenance_handling_update()
            ||  message.message().has_log_maintenance_handling_destruction();
}

// -----------------------------------------------------------------------------
// Name: MaintenanceResolver::IsEmptyLineMessage
// Created: MMC 2012-09-11
// -----------------------------------------------------------------------------
bool MaintenanceResolver::IsEmptyLineMessage( const sword::SimToClient& message )
{
    return message.message().has_log_maintenance_handling_destruction();
}

// -----------------------------------------------------------------------------
// Name: MaintenanceResolver::ManageMessage
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
void MaintenanceResolver::ManageMessage( const sword::SimToClient& message )
{
    if( message.message().has_log_maintenance_handling_creation() )
        TraceConsign< ::sword::LogMaintenanceHandlingCreation, MaintenanceConsignData >( message.message().log_maintenance_handling_creation(), output_ );
    if( message.message().has_log_maintenance_handling_update() )
        TraceConsign< ::sword::LogMaintenanceHandlingUpdate, MaintenanceConsignData >( message.message().log_maintenance_handling_update(), output_ );
    if( message.message().has_log_maintenance_handling_destruction() && message.message().log_maintenance_handling_destruction().has_request() )
        DestroyConsignData( message.message().log_maintenance_handling_destruction().request().id() );
}

// -----------------------------------------------------------------------------
// Name: MaintenanceResolver::InitHeader
// Created: MMC 2012-08-24
// -----------------------------------------------------------------------------
void MaintenanceResolver::InitHeader()
{
    MaintenanceConsignData consign( tools::translate( "logistic", "request id" ).toStdString() );
    consign.tick_           = tools::translate( "logistic", "tick" ).toStdString();
    consign.creationTick_   = tools::translate( "logistic", "creation tick" ).toStdString();
    consign.stateEndTick_   = tools::translate( "logistic", "state end tick" ).toStdString();
    consign.unitId_         = tools::translate( "logistic", "unit id" ).toStdString();
    consign.providerId_     = tools::translate( "logistic", "provider id" ).toStdString();
    consign.stateId_        = tools::translate( "logistic", "state id" ).toStdString();
    consign.equipmentId_    = tools::translate( "logistic", "equipment id" ).toStdString();
    consign.breakdownId_    = tools::translate( "logistic", "breakdown id" ).toStdString();
    consign.simTime_        = tools::translate( "logistic", "GDH" ).toStdString();
    consign.unit_           = tools::translate( "logistic", "unit" ).toStdString();
    consign.provider_       = tools::translate( "logistic", "provider" ).toStdString();
    consign.equipment_      = tools::translate( "logistic", "equipment" ).toStdString();
    consign.breakdown_      = tools::translate( "logistic", "breakdown" ).toStdString();
    consign.state_          = tools::translate( "logistic", "state" ).toStdString();
    SetHeader( consign );
}

// -----------------------------------------------------------------------------
// Name: MaintenanceResolver::MaintenanceResolver
// Created: MMC 2012-09-03
// -----------------------------------------------------------------------------
ConsignData_ABC* MaintenanceResolver::CreateConsignData( int requestId )
{
    return static_cast< ConsignData_ABC* >( new MaintenanceConsignData( boost::lexical_cast< std::string >( requestId ) ) ); 
}
