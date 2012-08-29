// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "MaintenanceResolver.h"
#include "clients_kernel/Tools.h"

using namespace plugins::logistic;

// -----------------------------------------------------------------------------
// Name: MaintenanceConsignData::Write
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
void MaintenanceConsignData::operator>>( std::stringstream& output ) const
{
    static const std::string separator = ConsignData_ABC::GetSeparator();
    output  << requestId_    << separator
            << tick_         << separator
            << simTime_      << separator   // << creationTick_     << separator    << unitId_  << separator
            << unit_         << separator   // << providerId_       << separator
            << provider_     << separator   // << equipmentId_      << separator
            << equipment_    << separator   // << breakdownId_      << separator
            << breakdown_    << separator   // << stateId_          << separator
            << state_        << separator
            << stateEndTick_ << std::endl;
}

// -----------------------------------------------------------------------------
// Name: MaintenanceConsignData::ManageMessage
// Created: MMC 2012-08-21
// -----------------------------------------------------------------------------
const ConsignData_ABC& MaintenanceConsignData::ManageMessage( const ::sword::LogMaintenanceHandlingCreation& msg, ConsignResolver_ABC& resolver )
{
    resolver.GetSimTime( simTime_, tick_ );
    if( msg.has_tick() )
        creationTick_ = boost::lexical_cast< std::string >( msg.tick() );
    if( msg.has_unit() )
    {
        int unitId = msg.unit().id();
        unitId_ = boost::lexical_cast< std::string >( unitId );
        resolver.GetAgentName( unitId, unit_ );
    }
    if( msg.has_equipement() )
    {
        equipmentId_ = boost::lexical_cast< std::string >( msg.equipement().id() );
        resolver.GetEquipmentName( msg.equipement(), equipment_ );
    }
    if( msg.has_breakdown() )
    {
        int breakdownId = msg.breakdown().id();
        breakdownId_ = boost::lexical_cast< std::string >( breakdownId );
        resolver.GetBreakdownName( msg.breakdown(), breakdown_ );
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
    resolver.GetSimTime( simTime_, tick_ );
    if( msg.has_current_state_end_tick() )
    {
        int entTick = msg.current_state_end_tick();
        if( entTick > 0 )
            stateEndTick_ = boost::lexical_cast< std::string >( entTick );
    }
    if( msg.has_unit() )
    {
        int unitId = msg.unit().id();
        unitId_ = boost::lexical_cast< std::string >( unitId );
        resolver.GetAgentName( unitId, unit_ );
    }
    if( msg.has_provider() )
    {
        int providerId = msg.provider().id();
        providerId_ = boost::lexical_cast< std::string >( providerId );
        resolver.GetAgentName( providerId, provider_ );
    }
    if( msg.has_state() )
    {
        sword::LogMaintenanceHandlingUpdate::EnumLogMaintenanceHandlingStatus eState = msg.state();
        resolver.GetMaintenanceName( eState, state_ );
        stateId_ = boost::lexical_cast< std::string >( static_cast< int >( eState ) );
    }
    resolver.AddToLineIndex( 1 );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: MaintenanceResolver constructor
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
MaintenanceResolver::MaintenanceResolver( const std::string& name, const dispatcher::Model_ABC& model, const kernel::StaticModel& staticModel )
    : ConsignResolver_ABC( name, model, staticModel )
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
            ||  message.message().has_log_maintenance_handling_update();
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
}

// -----------------------------------------------------------------------------
// Name: MaintenanceResolver::InitHeader
// Created: MMC 2012-08-24
// -----------------------------------------------------------------------------
void MaintenanceResolver::InitHeader()
{
    MaintenanceConsignData consign( tools::translate( "logistic", "request id" ).toAscii().constData() );
    consign.tick_           = tools::translate( "logistic", "tick" ).toAscii().constData();
    consign.creationTick_   = tools::translate( "logistic", "creation tick" ).toAscii().constData();
    consign.stateEndTick_   = tools::translate( "logistic", "state end tick" ).toAscii().constData();
    consign.unitId_         = tools::translate( "logistic", "unit id" ).toAscii().constData();
    consign.providerId_     = tools::translate( "logistic", "provider id" ).toAscii().constData();
    consign.stateId_        = tools::translate( "logistic", "state id" ).toAscii().constData();
    consign.equipmentId_    = tools::translate( "logistic", "equipment id" ).toAscii().constData();
    consign.breakdownId_    = tools::translate( "logistic", "breakdown id" ).toAscii().constData();
    consign.simTime_        = tools::translate( "logistic", "GDH" ).toAscii().constData();
    consign.unit_           = tools::translate( "logistic", "unit" ).toAscii().constData();
    consign.provider_       = tools::translate( "logistic", "provider" ).toAscii().constData();
    consign.equipment_      = tools::translate( "logistic", "equipment" ).toAscii().constData();
    consign.breakdown_      = tools::translate( "logistic", "breakdown" ).toAscii().constData();
    consign.state_          = tools::translate( "logistic", "state" ).toAscii().constData();
    SetHeader( consign );
}
