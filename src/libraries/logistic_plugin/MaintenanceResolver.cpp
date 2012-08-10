// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "MaintenanceResolver.h"
#include "protocol/Protocol.h"

using namespace plugins::logistic;

// -----------------------------------------------------------------------------
// Name: MaintenanceConsignData constructor
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
MaintenanceConsignData::MaintenanceConsignData() 
    : tick_( noValue_ ), unitId_( noValue_ ), providerId_( noValue_ ), equipmentId_( noValue_ ), breakdownId_( noValue_ ), stateId_( noValue_ )
    , simTime_( noField_ ), unit_( noField_ ), provider_( noField_ ), equipment_( noField_ ), state_( noField_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MaintenanceConsignData::Write
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
void MaintenanceConsignData::Write( std::ofstream& output )
{
    //format: tick , GDH, unit , provider , equipment , breakdown , state
    //example: 1 , 20120312T240000 , 12, 1.1.4.BRB23 , 28, 1.2.BLT , 212, Char Leclerc , 3, en attente transporteurs
    output  << tick_         << separator_
            << simTime_      << separator_
            << unitId_       << subSeparator_   << unit_       << separator_
            << providerId_   << subSeparator_   << provider_   << separator_
            << equipmentId_  << subSeparator_   << equipment_  << separator_
            << breakdownId_  << separator_ 
            << stateId_      << subSeparator_   << state_      << std::endl;
}

// -----------------------------------------------------------------------------
// Name: MaintenanceResolver constructor
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
MaintenanceResolver::MaintenanceResolver( const std::string& name, const dispatcher::Model_ABC& model )
    : ConsignResolver_ABC( name, "tick , GDH, unit , provider , equipment , breakdown , state", model )
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

namespace
{
    void GetMaintenanceName( sword::LogMaintenanceHandlingUpdate::EnumLogMaintenanceHandlingStatus eMaintenance, std::string& name )
    {
        if( sword::LogMaintenanceHandlingUpdate::moving_to_supply == eMaintenance )
            name = "moving_to_supply";
        else if( sword::LogMaintenanceHandlingUpdate::waiting_for_transporter == eMaintenance )
            name = "waiting_for_transporter";
        else if( sword::LogMaintenanceHandlingUpdate::transporter_moving_to_supply == eMaintenance )
            name = "transporter_moving_to_supply";
        else if( sword::LogMaintenanceHandlingUpdate::transporter_loading == eMaintenance )
            name = "transporter_loading";
        else if( sword::LogMaintenanceHandlingUpdate::transporter_moving_back == eMaintenance )
            name = "transporter_moving_back";
        else if( sword::LogMaintenanceHandlingUpdate::transporter_unloading == eMaintenance )
            name = "transporter_unloading";
        else if( sword::LogMaintenanceHandlingUpdate::diagnosing == eMaintenance )
            name = "diagnosing";
        else if( sword::LogMaintenanceHandlingUpdate::searching_upper_levels == eMaintenance )
            name = "searching_upper_levels";
        else if( sword::LogMaintenanceHandlingUpdate::waiting_for_parts == eMaintenance )
            name = "waiting_for_parts";
        else if( sword::LogMaintenanceHandlingUpdate::waiting_for_repairer == eMaintenance )
            name = "waiting_for_repairer";
        else if( sword::LogMaintenanceHandlingUpdate::repairing == eMaintenance )
            name = "repairing";
        else if( sword::LogMaintenanceHandlingUpdate::moving_back == eMaintenance )
            name = "moving_back";
        else if( sword::LogMaintenanceHandlingUpdate::finished == eMaintenance )
            name = "finished";
    }
}

// -----------------------------------------------------------------------------
// Name: MaintenanceResolver::ManageMessage
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
void MaintenanceResolver::ManageMessage( const sword::SimToClient& message )
{
    if( message.message().has_log_maintenance_handling_creation() )
    {
        const ::sword::LogMaintenanceHandlingCreation& msg = message.message().log_maintenance_handling_creation();
        if( msg.has_request() )
        {
            MaintenanceConsignData* pConsignData = GetConsign< MaintenanceConsignData >( msg.request().id() );
            pConsignData->simTime_ = simTime_;
            if( msg.has_unit() )
            {
                pConsignData->unitId_ = msg.unit().id();
                GetAgentName( pConsignData->unitId_, pConsignData->unit_ );
            }
            if( msg.has_equipement() )
                pConsignData->equipmentId_ = msg.equipement().id();
            if( msg.has_breakdown() )
                pConsignData->breakdownId_ = msg.breakdown().id();
            if( msg.has_tick() )
                pConsignData->tick_ = msg.tick();
            pConsignData->Write( output_ );
        }
    }
    if( message.message().has_log_maintenance_handling_update() )
    {
        const ::sword::LogMaintenanceHandlingUpdate& msg = message.message().log_maintenance_handling_update();
        if( msg.has_request() )
        {
            MaintenanceConsignData* pConsignData = GetConsign< MaintenanceConsignData >( msg.request().id() );
            pConsignData->simTime_ = simTime_;
            if( msg.has_unit() )
            {
                pConsignData->unitId_  = msg.unit().id();
                GetAgentName( pConsignData->unitId_, pConsignData->unit_ );
            }
            if( msg.has_provider() )
            {
                pConsignData->providerId_ = msg.provider().id();
                GetAgentName( pConsignData->providerId_, pConsignData->provider_ );
            }
            if( msg.has_state() )
            {
                sword::LogMaintenanceHandlingUpdate::EnumLogMaintenanceHandlingStatus eState = msg.state();
                GetMaintenanceName( eState, pConsignData->state_ );
                pConsignData->stateId_ = eState;
            }
            pConsignData->Write( output_ );
        }
    }
}
