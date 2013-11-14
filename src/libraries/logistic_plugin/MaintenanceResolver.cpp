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
bool MaintenanceConsignData::ManageMessage( const ::sword::LogMaintenanceHandlingCreation& msg, 
        const NameResolver_ABC& nameResolver )
{
    PushState();
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
    return true;
}

// -----------------------------------------------------------------------------
// Name: MaintenanceConsignData::ManageMessage
// Created: MMC 2012-08-21
// -----------------------------------------------------------------------------
bool MaintenanceConsignData::ManageMessage( const ::sword::LogMaintenanceHandlingUpdate& msg, 
        const NameResolver_ABC& nameResolver )
{
    auto& state = PushState();
    if( msg.has_current_state_end_tick() )
    {
        int entTick = msg.current_state_end_tick();
        if( entTick > 0 )
            stateEndTick_ = boost::lexical_cast< std::string >( entTick );
        if( entTick <= GetTick() )
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
        const uint32_t providerId = msg.provider().id();
        providerId_ = boost::lexical_cast< std::string >( providerId );
        nameResolver.GetAgentName( providerId, provider_ );
        state.handlerId_ = providerId;
    }
    if( msg.has_state() )
    {
        sword::LogMaintenanceHandlingUpdate::EnumLogMaintenanceHandlingStatus eState = msg.state();
        nameResolver.GetMaintenanceName( eState, state_ );
        stateId_ = boost::lexical_cast< std::string >( static_cast< int >( eState ) );
        state.status_ = eState;
    }
    return true;
}

bool MaintenanceConsignData::DoUpdateConsign( const sword::SimToClient& message,
        const NameResolver_ABC& resolver )
{
    if( message.message().has_log_maintenance_handling_creation() )
        return ManageMessage( message.message().log_maintenance_handling_creation(), resolver );
    if( message.message().has_log_maintenance_handling_update() )
        return ManageMessage( message.message().log_maintenance_handling_update(), resolver );
    return false;
}

std::string plugins::logistic::GetMaintenanceHeader()
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
    return consign.ToString();
}

