// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "MedicalResolver.h"
#include "NameResolver_ABC.h"
#include "ConsignWriter.h"
#include "clients_kernel/Tools.h"
#include <boost/lexical_cast.hpp>

using namespace plugins::logistic;

// -----------------------------------------------------------------------------
// Name: MedicalConsignData::WriteConsign
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
void MedicalConsignData::WriteConsign( ConsignWriter& output ) const
{
    output  << requestId_
            << tick_
            << simTime_
            << unit_
            << provider_
            << rank_
            << wound_
            << nbc_
            << mental_
            << state_
            << stateEndTick_;
    output.End();
}

// -----------------------------------------------------------------------------
// Name: MedicalConsignData::ManageMessage
// Created: MMC 2012-08-21
// -----------------------------------------------------------------------------
bool MedicalConsignData::ManageMessage( const ::sword::LogMedicalHandlingCreation& msg,
        const NameResolver_ABC& nameResolver )
{
    if( msg.has_tick() )
        creationTick_ = boost::lexical_cast< std::string >( msg.tick() );
    if( msg.has_unit() )
    {
        unitId_ = boost::lexical_cast< std::string >( msg.unit().id() );
        nameResolver.GetAgentName( msg.unit().id(), unit_ );
    }
    if( msg.has_rank() )
        nameResolver.GetRankName( msg.rank(), rank_ );
    if( msg.has_wound() )
        nameResolver.GetWoundName( msg.wound(), wound_ );
    std::string strYes = tools::translate( "logistic", "yes" ).toStdString();
    std::string strNo = tools::translate( "logistic", "no" ).toStdString();
    if( msg.has_mental_wound() )
        mental_ = msg.mental_wound() ? strYes : strNo;
    if( msg.has_nbc_contaminated() )
        nbc_ = msg.nbc_contaminated() ? strYes : strNo;
    return true;
}

// -----------------------------------------------------------------------------
// Name: MedicalConsignData::ManageMessage
// Created: MMC 2012-08-21
// -----------------------------------------------------------------------------
bool MedicalConsignData::ManageMessage( const ::sword::LogMedicalHandlingUpdate& msg,
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
    if( msg.has_unit() )
    {
        unitId_ = boost::lexical_cast< std::string >( msg.unit().id() );
        nameResolver.GetAgentName( msg.unit().id(), unit_ );
    }
    if( msg.has_provider() )
    {
        const uint32_t providerId = msg.provider().id();
        providerId_ = boost::lexical_cast< std::string >( providerId );
        nameResolver.GetAgentName( msg.provider().id(), provider_ );
    }
    if( msg.has_state() )
    {
        sword::LogMedicalHandlingUpdate::EnumLogMedicalHandlingStatus eState = msg.state();
        nameResolver.GetMedicalName( eState, state_ );
        stateId_ = boost::lexical_cast< std::string >( static_cast< int >( eState ) );
    }
    std::string strYes = tools::translate( "logistic", "yes" ).toStdString();
    std::string strNo = tools::translate( "logistic", "no" ).toStdString();
    if( msg.has_wound() )
        nameResolver.GetWoundName( msg.wound(), wound_ );
    if( msg.has_mental_wound() )
        mental_ = msg.mental_wound() ? strYes : strNo;
    if( msg.has_nbc_contaminated() )
        nbc_ = msg.nbc_contaminated() ? strYes : strNo;
    return true;
}

// -----------------------------------------------------------------------------
// Name: MedicalConsignData::ManageMessage
// Created: MMC 2012-08-21
// -----------------------------------------------------------------------------
bool MedicalConsignData::ManageMessage( const ::sword::LogMedicalHandlingDestruction& msg,
        const NameResolver_ABC& nameResolver )
{
    if( msg.has_unit() )
    {
        unitId_ = boost::lexical_cast< std::string >( msg.unit().id() );
        nameResolver.GetAgentName( msg.unit().id(), unit_ );
    }
    state_ = tools::translate( "logistic", "instruction finished" ).toAscii().constData();
    return true;
}

// -----------------------------------------------------------------------------
// Name: MedicalResolver::ManageMessage
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
bool MedicalConsignData::DoUpdateConsign( const sword::SimToClient& message,
        const NameResolver_ABC& resolver )
{
    const auto& msg = message.message();
    if( msg.has_log_medical_handling_creation() )
    {
        const auto& sub = msg.log_medical_handling_creation();
        return ManageMessage( sub, resolver );
    }
    if( msg.has_log_medical_handling_update() )
    {
        const auto& sub = msg.log_medical_handling_update();
        return ManageMessage( sub, resolver );
    }
    if( msg.has_log_medical_handling_destruction() )
        return ManageMessage( msg.log_medical_handling_destruction(), resolver );
    return false;
}

std::string plugins::logistic::GetMedicalHeader()
{
    MedicalConsignData consign( tools::translate( "logistic", "request id" ).toStdString() );
    consign.tick_           = tools::translate( "logistic", "tick" ).toStdString();
    consign.creationTick_   = tools::translate( "logistic", "creation tick" ).toStdString();
    consign.stateEndTick_   = tools::translate( "logistic", "state end tick" ).toStdString();
    consign.simTime_        = tools::translate( "logistic", "GDH" ).toStdString();
    consign.unitId_         = tools::translate( "logistic", "unit id" ).toStdString();
    consign.providerId_     = tools::translate( "logistic", "provider id" ).toStdString();
    consign.stateId_        = tools::translate( "logistic", "state id" ).toStdString();
    consign.rank_           = tools::translate( "logistic", "rank" ).toStdString();
    consign.wound_          = tools::translate( "logistic", "wound" ).toStdString();
    consign.nbc_            = tools::translate( "logistic", "nbc" ).toStdString();
    consign.mental_         = tools::translate( "logistic", "mental" ).toStdString();
    consign.unit_           = tools::translate( "logistic", "unit" ).toStdString();
    consign.provider_       = tools::translate( "logistic", "provider" ).toStdString();
    consign.state_          = tools::translate( "logistic", "state" ).toStdString();
    return consign.ToString();
}
