// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Populations/DEC_PopulationDecision.cpp $
// $Author: Nld $
// $Modtime: 23/06/05 10:31 $
// $Revision: 20 $
// $Workfile: DEC_PopulationDecision.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_PopulationDecision.h"
#include "MIL_PopulationType.h"
#include "Decision/DEC_Model_ABC.h"
#include "Decision/DEC_Tools.h"
#include "Entities/Orders/MIL_MissionType_ABC.h"
#include "protocol/ClientSenders.h"

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_PopulationDecision )

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision constructor
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
DEC_PopulationDecision::DEC_PopulationDecision( MIL_Population& population, unsigned int gcPause, unsigned int gcMult, sword::DEC_Logger* logger )
    : DEC_Decision( population, gcPause, gcMult, logger )
    , rDominationState_    ( 0. )
    , rLastDominationState_( 0. )
    , bStateHasChanged_    ( true )
{
    const DEC_Model_ABC& model = population.GetType().GetModel();
    SetModel( model );
    //GetVariable( nDIANameIdx_    ).SetValue( population.GetName() ); // $$$$ LDC: FIXME Use member data
    StartDefaultBehavior();
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision destructor
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
DEC_PopulationDecision::~DEC_PopulationDecision()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::load
// Created: JVT 2005-04-05
// -----------------------------------------------------------------------------
void DEC_PopulationDecision::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< DEC_Decision< MIL_Population  > >( *this );
    file >> pEntity_;
    file >> rDominationState_;
    file >> rLastDominationState_;
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::save
// Created: JVT 2005-04-05
// -----------------------------------------------------------------------------
void DEC_PopulationDecision::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    assert( pEntity_ );
    file << boost::serialization::base_object< DEC_Decision< MIL_Population  > >( *this );
    file << pEntity_;
    file << rDominationState_;
    file << rLastDominationState_;
}

void DEC_PopulationDecision::SetModel( const DEC_Model_ABC& model )
{
    model_ = &model;
    assert( pEntity_ );
    InitBrain( model.GetScriptFile(), model.GetDIAType(), model.GetIncludePath(),
               GetGroupName(), model.IsMasalife(), false, model.GetIntegrationDir() );
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::EndCleanStateAfterCrash
// Created: NLD 2005-06-23
// -----------------------------------------------------------------------------
void DEC_PopulationDecision::EndCleanStateAfterCrash()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::GetGroupName
// Created: LDC 2011-09-01
// -----------------------------------------------------------------------------
std::string DEC_PopulationDecision::GetGroupName()
{
    return "none";
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::RegisterUserFunctions
// Created: LDC 2009-04-09
// -----------------------------------------------------------------------------
void DEC_PopulationDecision::RegisterUserFunctions( sword::Brain& )
{
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::StartMissionBehavior
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
void DEC_PopulationDecision::StartMissionBehavior( const boost::shared_ptr< MIL_Mission_ABC > mission )
{
    const std::string& strBehavior = mission->GetType().GetDIABehavior();
    ActivateOrder( strBehavior, mission );
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::StopMissionBehavior
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
void DEC_PopulationDecision::StopMissionBehavior( const boost::shared_ptr< MIL_Mission_ABC > mission )
{
    const std::string& strBehavior = mission->GetType().GetDIABehavior();
    StopMission( strBehavior );
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::SendFullState
// Created: NLD 2006-02-22
// -----------------------------------------------------------------------------
void DEC_PopulationDecision::SendFullState( client::CrowdUpdate& msg ) const
{
    msg().set_domination( static_cast< unsigned int >( rDominationState_ * 100. ) );
    msg().set_decisional_model( model_->GetName() );
    msg().set_brain_debug( brainDebug_ );
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::SendChangedState
// Created: NLD 2006-02-22
// -----------------------------------------------------------------------------
void DEC_PopulationDecision::SendChangedState( client::CrowdUpdate& msg )
{
    if( bStateHasChanged_ )
        SendFullState( msg );
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::GetDominationState
// Created: NLD 2006-02-22
// -----------------------------------------------------------------------------
double DEC_PopulationDecision::GetDominationState() const
{
    return rDominationState_;
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::GetPopulation
// Created: NLD 2004-10-26
// -----------------------------------------------------------------------------
MIL_Population& DEC_PopulationDecision::GetPopulation() const
{
    assert( pEntity_ );
    return *pEntity_;
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::GetKnowledgeGroup
// Created: NMI 2014-08-25
// -----------------------------------------------------------------------------
boost::shared_ptr< MIL_KnowledgeGroup > DEC_PopulationDecision::GetKnowledgeGroup() const
{
    return pEntity_->GetKnowledgeGroup();
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::NotifyDominationStateChanged
// Created: NLD 2006-02-22
// -----------------------------------------------------------------------------
void DEC_PopulationDecision::NotifyDominationStateChanged( double rValue )
{
    assert( rValue >= 0. && rValue <= 1. );
    if( rDominationState_ == rValue )
        return;

    rDominationState_ = rValue;

    static const double rDeltaPercentageForNetwork = 0.05;
    if( fabs( rLastDominationState_ - rDominationState_ ) > rDeltaPercentageForNetwork || rDominationState_ == 0. || rDominationState_ == 1. )
    {
        bStateHasChanged_ = true;
        rLastDominationState_ = rDominationState_;
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::Clean
// Created: NLD 2006-02-22
// -----------------------------------------------------------------------------
void DEC_PopulationDecision::Clean()
{
    bStateHasChanged_ = false;
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::HasStateChanged
// Created: NLD 2006-02-22
// -----------------------------------------------------------------------------
bool DEC_PopulationDecision::HasStateChanged() const
{
    return bStateHasChanged_;
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::GetDecAutomate
// Created: LDC 2009-04-10
// -----------------------------------------------------------------------------
DEC_AutomateDecision* DEC_PopulationDecision::GetDecAutomate()
{
    throw MASA_EXCEPTION( "DEC_GetAutomate unexpected on population" );
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::GetName
// Created: LDC 2009-04-09
// -----------------------------------------------------------------------------
std::string DEC_PopulationDecision::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::GetOrderManager
// Created: NMI 2013-11-15
// -----------------------------------------------------------------------------
MIL_OrderManager_ABC& DEC_PopulationDecision::GetOrderManager() const
{
    return GetPopulation().GetOrderManager();
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::RegisterSelf
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_PopulationDecision::RegisterSelf( sword::Brain& brain )
{
    brain.GetScriptRef( "myself" ) = (DEC_Decision_ABC*)this;
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::RegisterSpecific
// Created: JSR 2013-02-18
// -----------------------------------------------------------------------------
void DEC_PopulationDecision::RegisterSpecific( sword::Brain& brain, bool isMasalife, const std::string& groupName )
{
    if( isMasalife )
        brain.GetScriptRef( "InitMeCrowd" )(
            brain.GetScriptRef( "integration.ontology.types.body" ),
            brain.GetScriptRef( "myself" ),
            groupName );
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::Reload
// Created: AHC 2012-01-25
// -----------------------------------------------------------------------------
void DEC_PopulationDecision::Reload( bool force, bool doInitBrain )
{
    DEC_Decision< MIL_Population >::Reload( force, doInitBrain );
    bStateHasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::ActivateBrainDebug
// Created: SLI 2013-06-21
// -----------------------------------------------------------------------------
void DEC_PopulationDecision::ActivateBrainDebug( bool activate )
{
    DEC_Decision< MIL_Population >::ActivateBrainDebug( activate );
    bStateHasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::PostStartMission
// Created: MMC 2013-10-15
// -----------------------------------------------------------------------------
void DEC_PopulationDecision::PostStartMission()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::PostStopMission
// Created: MMC 2013-10-15
// -----------------------------------------------------------------------------
void DEC_PopulationDecision::PostStopMission()
{
    // NOTHING
}

