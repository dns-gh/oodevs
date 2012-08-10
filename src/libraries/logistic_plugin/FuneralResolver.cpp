// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "FuneralResolver.h"
#include "protocol/Protocol.h"

using namespace plugins::logistic;

// -----------------------------------------------------------------------------
// Name: FuneralConsignData constructor
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
FuneralConsignData::FuneralConsignData() 
    : tick_( noValue_ ), unitId_( noValue_ ), handlingUnitId_( noValue_ ), conveyingUnitId_( noValue_ ), stateId_( noValue_ )
    , simTime_( noField_ ), unit_( noField_ ), handlingUnit_( noField_ ), conveyingUnit_( noField_ ), rank_( noField_ )
    , packagingResource_( noField_ ), state_( noField_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FuneralConsignData::Write
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
void FuneralConsignData::Write( std::ofstream& output )
{
    //format: tick , GDH, unit , handling_unit , conveying_unit , rank , packaging_resource, state
    //example: 1 , 20120312T240000 , 12, 1.1.4.BRB23 , 28, 1.2.BLT , 22, 1.2.TR323.TR1, officer , 3, conteneur mortuaire, 5, en attente transporteurs
    output  << tick_                 << separator_ 
            << simTime_              << separator_
            << unitId_               << subSeparator_    << unit_                << separator_
            << handlingUnitId_       << subSeparator_    << handlingUnit_        << separator_
            << conveyingUnitId_      << subSeparator_    << conveyingUnit_       << separator_
            << rank_                 << separator_
            << packagingResourceId_  << subSeparator_    << packagingResource_   << separator_
            << stateId_              << subSeparator_    << state_               << std::endl;
}

// -----------------------------------------------------------------------------
// Name: FuneralResolver constructor
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
FuneralResolver::FuneralResolver( const std::string& name, const dispatcher::Model_ABC& model ) 
    : ConsignResolver_ABC( name, "tick , GDH, unit , handling_unit , conveying_unit , rank , packaging_resource, state", model )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FuneralResolver destructor
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
FuneralResolver::~FuneralResolver()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FuneralResolver::IsManageable
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
bool FuneralResolver::IsManageable( const sword::SimToClient& message )
{
    return     message.message().has_log_funeral_handling_creation()
            || message.message().has_log_funeral_handling_update();
}

namespace
{
    void GetRankName( sword::EnumHumanRank eRank, std::string& name )
    {
        if( sword::officer == eRank )
            name = "officer";
        else if( sword::sub_officer == eRank )
            name = "sub_officer";
        else if( sword::trooper == eRank )
            name = "trooper";
    }

    void GetFuneralName( sword::LogFuneralHandlingUpdate::EnumLogFuneralHandlingStatus eFuneral, std::string name )
    {
        if( sword::LogFuneralHandlingUpdate::waiting_for_handling == eFuneral )
            name = "waiting_for_handling";
        else if( sword::LogFuneralHandlingUpdate::transporting_unpackaged == eFuneral )
            name = "transporting_unpackaged";
        else if( sword::LogFuneralHandlingUpdate::waiting_for_packaging == eFuneral )
            name = "waiting_for_packaging";
        else if( sword::LogFuneralHandlingUpdate::packaging == eFuneral )
            name = "packaging";
        else if( sword::LogFuneralHandlingUpdate::waiting_for_transporter == eFuneral )
            name = "waiting_for_transporter";
        else if( sword::LogFuneralHandlingUpdate::transporting_packaged == eFuneral )
            name = "transporting_packaged";
        else if( sword::LogFuneralHandlingUpdate::finished == eFuneral )
            name = "finished";
    }
}

// -----------------------------------------------------------------------------
// Name: FuneralResolver::ManageMessage
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
void FuneralResolver::ManageMessage( const sword::SimToClient& message )
{
    if( message.message().has_log_funeral_handling_creation() )
    {
        const ::sword::LogFuneralHandlingCreation& msg = message.message().log_funeral_handling_creation();
        if( msg.has_request() )
        {
            FuneralConsignData* pConsignData = GetConsign< FuneralConsignData >( msg.request().id() );
            pConsignData->simTime_ = simTime_;
            if( msg.has_unit() )
            {
                pConsignData->unitId_ = msg.unit().id();
                GetAgentName( pConsignData->unitId_, pConsignData->unit_ );
            }
            if( msg.has_tick() )
                pConsignData->tick_ = msg.tick();
            if( msg.has_rank() )
                GetRankName( msg.rank(), pConsignData->rank_ );
            pConsignData->Write( output_ );
        }
    }
    if( message.message().has_log_funeral_handling_update() )
    {
        const ::sword::LogFuneralHandlingUpdate& msg = message.message().log_funeral_handling_update();
        if( msg.has_request() )
        {
            FuneralConsignData* pConsignData = GetConsign< FuneralConsignData >( msg.request().id() );
            pConsignData->simTime_ = simTime_;
            if( msg.has_handling_unit() )
            {
                if( msg.handling_unit().has_automat() )
                {
                    pConsignData->handlingUnitId_ = static_cast< int >( msg.handling_unit().automat().id() );
                    GetAutomatName( pConsignData->handlingUnitId_, pConsignData->handlingUnit_ );
                }
                else if( msg.handling_unit().has_formation() )
                {
                    pConsignData->handlingUnitId_ = static_cast< int >( msg.handling_unit().formation().id() );
                    GetFormationName( pConsignData->handlingUnitId_, pConsignData->handlingUnit_ );
                }
            }
            if( msg.has_convoying_unit() )
            {
                pConsignData->conveyingUnitId_ = msg.convoying_unit().id();
                GetAgentName( pConsignData->conveyingUnitId_, pConsignData->conveyingUnit_ );
            }
            if( msg.has_packaging_resource() )
                pConsignData->packagingResourceId_ = msg.packaging_resource().id();
            if( msg.has_state() )
            {
                sword::LogFuneralHandlingUpdate::EnumLogFuneralHandlingStatus eState = msg.state();
                GetFuneralName( eState, pConsignData->state_ );
                pConsignData->stateId_ = eState;
            }
            pConsignData->Write( output_ );
        }
    }
}
