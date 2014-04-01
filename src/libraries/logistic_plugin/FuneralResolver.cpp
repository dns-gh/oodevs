// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "FuneralResolver.h"
#include "ConsignWriter.h"
#include "NameResolver_ABC.h"
#include "clients_kernel/Tools.h"
#include <boost/lexical_cast.hpp>

using namespace plugins::logistic;

// -----------------------------------------------------------------------------
// Name: FuneralConsignData::WriteConsign
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
void FuneralConsignData::WriteConsign( ConsignWriter& output ) const
{
    output  << requestId_
            << tick_
            << simTime_
            << unit_
            << handlingUnit_
            << conveyingUnit_
            << rank_
            << packagingResource_
            << state_
            << stateEndTick_;
    output.End();
}

// -----------------------------------------------------------------------------
// Name: FuneralConsignData::ManageMessage
// Created: MMC 2012-08-21
// -----------------------------------------------------------------------------
bool FuneralConsignData::ManageMessage( const ::sword::LogFuneralHandlingCreation& msg,
        const NameResolver_ABC& nameResolver )
{
    if( msg.has_tick() )
        creationTick_ = boost::lexical_cast< std::string >( msg.tick() );
    if( msg.has_unit() )
    {
        int unitId = msg.unit().id();
        unitId_ = boost::lexical_cast< std::string >( unitId );
        nameResolver.GetAgentName( unitId, unit_ );
    }
    if( msg.has_rank() )
        nameResolver.GetRankName( msg.rank(), rank_ );
    return true;
}

// -----------------------------------------------------------------------------
// Name: FuneralConsignData::ManageMessage
// Created: MMC 2012-08-21
// -----------------------------------------------------------------------------
bool FuneralConsignData::ManageMessage( const ::sword::LogFuneralHandlingUpdate& msg,
        const NameResolver_ABC& nameResolver )
{
    if( msg.has_current_state_end_tick() )
    {
        int endTick = msg.current_state_end_tick();
        if( endTick > 0 )
            stateEndTick_ = boost::lexical_cast< std::string >( endTick );
        if( endTick <= GetTick() )
            stateEndTick_.clear();
    }
    if( msg.has_handling_unit() )
    {
        if( msg.handling_unit().has_automat() )
        {
            const uint32_t automatId = msg.handling_unit().automat().id();
            handlingUnitId_ = boost::lexical_cast< std::string >( automatId );
            nameResolver.GetAutomatName( automatId, handlingUnit_ );
        }
        else if( msg.handling_unit().has_formation() )
        {
            const uint32_t formationId = msg.handling_unit().formation().id();
            handlingUnitId_ = boost::lexical_cast< std::string >( formationId );
            nameResolver.GetFormationName( formationId, handlingUnit_ );
        }
    }
    if( msg.has_convoying_unit() )
    {
        int unitId = msg.convoying_unit().id();
        if( unitId > 0 )
        {
            conveyingUnitId_ = boost::lexical_cast< std::string >( unitId );
            nameResolver.GetAgentName( unitId, conveyingUnit_ );
        }
    }
    if( msg.has_packaging_resource() )
    {
        packagingResourceId_ = boost::lexical_cast< std::string >( msg.packaging_resource().id() );
        nameResolver.GetResourceName( msg.packaging_resource(), packagingResource_ );
    }
    if( msg.has_state() )
    {
        sword::LogFuneralHandlingUpdate::EnumLogFuneralHandlingStatus eState = msg.state();
        nameResolver.GetFuneralName( eState, state_ );
        stateId_ = boost::lexical_cast< std::string >( static_cast< int >( eState ) );
    }
    return true;
}

bool FuneralConsignData::DoUpdateConsign( const sword::SimToClient& message,
        const NameResolver_ABC& resolver )
{
    const auto& msg = message.message();
    if( msg.has_log_funeral_handling_creation() )
    {
        const auto& sub = msg.log_funeral_handling_creation();
        return ManageMessage( sub, resolver );
    }
    if( msg.has_log_funeral_handling_update() )
    {
        const auto& sub = msg.log_funeral_handling_update();
        return ManageMessage( sub, resolver );
    }
    return false;
}

std::string plugins::logistic::GetFuneralHeader()
{
    FuneralConsignData consign( tools::translate( "logistic", "request id" ).toStdString()  );
    consign.tick_                   = tools::translate( "logistic", "tick" ).toStdString();
    consign.creationTick_           = tools::translate( "logistic", "creation tick" ).toStdString();
    consign.stateEndTick_           = tools::translate( "logistic", "state end tick" ).toStdString();
    consign.simTime_                = tools::translate( "logistic", "GDH" ).toStdString();
    consign.unitId_                 = tools::translate( "logistic", "unit id" ).toStdString();
    consign.handlingUnitId_         = tools::translate( "logistic", "handling unit id" ).toStdString();
    consign.conveyingUnitId_        = tools::translate( "logistic", "conveying unit id" ).toStdString();
    consign.packagingResourceId_    = tools::translate( "logistic", "packaging resource id" ).toStdString();
    consign.stateId_                = tools::translate( "logistic", "state id" ).toStdString();
    consign.unit_                   = tools::translate( "logistic", "unit" ).toStdString();
    consign.handlingUnit_           = tools::translate( "logistic", "handling unit" ).toStdString();
    consign.conveyingUnit_          = tools::translate( "logistic", "conveying unit" ).toStdString();
    consign.rank_                   = tools::translate( "logistic", "rank" ).toStdString();
    consign.packagingResource_      = tools::translate( "logistic", "packaging resource" ).toStdString();
    consign.state_                  = tools::translate( "logistic", "state" ).toStdString();
    return consign.ToString();
}

