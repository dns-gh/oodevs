// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "MedicalResolver.h"
#include "protocol/Protocol.h"

using namespace plugins::logistic;

// -----------------------------------------------------------------------------
// Name: MedicalConsignData constructor
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
MedicalConsignData::MedicalConsignData() 
    : tick_( noValue_ ), unitId_( noValue_ ), providerId_( noValue_ ), nbc_( noValue_ ), mental_( noValue_ ), stateId_( noValue_ ) 
    , simTime_( noField_ ), unit_( noField_ ), provider_( noField_ ), rank_( noField_ ), wound_( noField_ ), state_( noField_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MedicalConsignData::Write
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
void MedicalConsignData::Write( std::ofstream& output )
{
    //format: tick , GDH, unit , provider , rank , wound , nbc , mental , state
    //example: 1 , 20120312T240000 , 12, 1.1.4.BRB23 , 28, 1.2.BLT , officer , U2 , 0 , 0 , 3, en traitement
    output  << tick_         << separator_
            << simTime_      << separator_
            << unitId_       << subSeparator_    << unit_     << separator_
            << providerId_   << subSeparator_    << provider_ << separator_
            << rank_         << separator_
            << wound_        << separator_
            << nbc_          << separator_
            << mental_       << separator_
            << stateId_      << subSeparator_    << state_    << subSeparator_ << std::endl;
}

// -----------------------------------------------------------------------------
// Name: MedicalResolver constructor
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
MedicalResolver::MedicalResolver( const std::string& name, const dispatcher::Model_ABC& model ) 
    : ConsignResolver_ABC( name, "tick , GDH, unit , provider , rank , wound , nbc , mental , state", model )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MedicalResolver destructor
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
MedicalResolver::~MedicalResolver()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MedicalResolver::IsManageable
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
bool MedicalResolver::IsManageable( const sword::SimToClient& message )
{
    return      message.message().has_log_medical_handling_creation()
            ||  message.message().has_log_medical_handling_update();
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

    void GetWoundName( sword::EnumHumanWound eWound, std::string& name )
    {
        if( sword::unwounded == eWound )
            name = "healthy";
        else if( sword::dead == eWound )
            name = "dead";
        else if( sword::wounded_urgency_1 == eWound )
            name = "u1";
        else if( sword::wounded_urgency_2 == eWound )
            name = "u2";
        else if( sword::wounded_urgency_3 == eWound )
            name = "u3";
        else if( sword:: wounded_extreme_urgency == eWound )
            name = "ue";
    }

    void GetMedicalName( sword::LogMedicalHandlingUpdate::EnumLogMedicalHandlingStatus eMedical, std::string name )
    {
        if( sword::LogMedicalHandlingUpdate::waiting_for_evacuation == eMedical )
            name = "waiting_for_evacuation";
        else if( sword::LogMedicalHandlingUpdate::evacuation_ambulance_moving_in == eMedical )
            name = "evacuation_ambulance_moving_in";
        else if( sword::LogMedicalHandlingUpdate::evacuation_ambulance_loading == eMedical )
            name = "evacuation_ambulance_loading";
        else if( sword::LogMedicalHandlingUpdate::waiting_for_evacuation_loading_completion == eMedical )
            name = "waiting_for_evacuation_loading_completion";
        else if( sword::LogMedicalHandlingUpdate::evacuation_ambulance_moving_out == eMedical )
            name = "evacuation_ambulance_moving_out";
        else if( sword::LogMedicalHandlingUpdate::evacuation_ambulance_unloading == eMedical )
            name = "evacuation_ambulance_unloading";
        else if( sword::LogMedicalHandlingUpdate::waiting_for_diagnostic == eMedical )
            name = "waiting_for_diagnostic";
        else if( sword::LogMedicalHandlingUpdate::diagnosing == eMedical )
            name = "diagnosing";
        else if( sword::LogMedicalHandlingUpdate::looking_for_triage == eMedical )
            name = "looking_for_triage";
        else if( sword::LogMedicalHandlingUpdate::waiting_for_triage == eMedical )
            name = "waiting_for_triage";
        else if( sword::LogMedicalHandlingUpdate::triaging == eMedical )
            name = "triaging";
        else if( sword::LogMedicalHandlingUpdate::looking_for_medical_attention == eMedical )
            name = "looking_for_medical_attention";
        else if( sword::LogMedicalHandlingUpdate::waiting_for_medical_attention == eMedical )
            name = "waiting_for_medical_attention";
        else if( sword::LogMedicalHandlingUpdate::receiving_medical_attention == eMedical )
            name = "receiving_medical_attention";
        else if( sword::LogMedicalHandlingUpdate::resting == eMedical )
            name = "resting";
        if( sword::LogMedicalHandlingUpdate::waiting_for_collection == eMedical )
            name = "waiting_for_collection";
        else if( sword::LogMedicalHandlingUpdate::collection_ambulance_loading == eMedical )
            name = "collection_ambulance_loading";
        else if( sword::LogMedicalHandlingUpdate::waiting_for_collection_loading_completion == eMedical )
            name = "waiting_for_collection_loading_completion";
        else if( sword::LogMedicalHandlingUpdate::collection_ambulance_moving_in == eMedical )
            name = "collection_ambulance_moving_in";
        else if( sword::LogMedicalHandlingUpdate::collection_ambulance_unloading == eMedical )
            name = "collection_ambulance_unloading";
        else if( sword::LogMedicalHandlingUpdate::finished == eMedical )
            name = "finished";
    }
}

// -----------------------------------------------------------------------------
// Name: MedicalResolver::ManageMessage
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
void MedicalResolver::ManageMessage( const sword::SimToClient& message )
{
    if( message.message().has_log_medical_handling_creation() )
    {
        const ::sword::LogMedicalHandlingCreation& msg = message.message().log_medical_handling_creation();
        if( msg.has_request() )
        {
            MedicalConsignData* pConsignData = GetConsign< MedicalConsignData >( static_cast< int >( msg.request().id() ) );
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
            if( msg.has_wound() )
                GetWoundName( msg.wound(), pConsignData->wound_ );
            if( msg.has_mental_wound() )
                msg.mental_wound() ? 1 : 0;
            if( msg.has_nbc_contaminated() )
                msg.nbc_contaminated() ? 1 : 0;
            pConsignData->Write( output_ );
        }
    }
    if( message.message().has_log_medical_handling_update() )
    {
        const ::sword::LogMedicalHandlingUpdate& msg = message.message().log_medical_handling_update();
        if( msg.has_request() )
        {
            MedicalConsignData* pConsignData = GetConsign< MedicalConsignData >( static_cast< int >( msg.request().id() ) );
            pConsignData->simTime_ = simTime_;
            if( msg.has_current_state_end_tick() )
                pConsignData->tick_ = msg.current_state_end_tick();
            if( msg.has_unit() )
            {
                pConsignData->unitId_ = msg.unit().id();
                GetAgentName( pConsignData->unitId_, pConsignData->unit_ );
            }
            if( msg.has_provider() )
            {
                pConsignData->providerId_ = msg.provider().id();
                GetAgentName( pConsignData->providerId_, pConsignData->provider_ );
            }
            if( msg.has_state() )
            {
                sword::LogMedicalHandlingUpdate::EnumLogMedicalHandlingStatus eState = msg.state();
                GetMedicalName( eState, pConsignData->state_ );
                pConsignData->stateId_ = eState;
            }
            if( msg.has_wound() )
                GetWoundName( msg.wound(), pConsignData->wound_ );
            if( msg.has_mental_wound() )
                pConsignData->mental_ = msg.mental_wound() ? 1 : 0;
            if( msg.has_nbc_contaminated() )
                pConsignData->nbc_ = msg.nbc_contaminated() ? 1 : 0;
            pConsignData->Write( output_ );
        }
    }
}
