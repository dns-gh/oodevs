// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Automates/DEC_AutomateDecision.cpp $
// $Author: Nld $
// $Modtime: 23/06/05 10:31 $
// $Revision: 20 $
// $Workfile: DEC_AutomateDecision.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_AutomateDecision.h"
#include "MIL_AutomateType.h"
#include "Decision/DEC_Model_ABC.h"
#include "Decision/DEC_Tools.h"
#include "Entities/Orders/MIL_MissionType_ABC.h"
#include "Entities/Agents/Units/Categories/PHY_RoePopulation.h"
#include "Decision/DEC_AutomateFunctions.h"
#include "protocol/ClientSenders.h"

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_AutomateDecision )

template< typename Archive >
void save_construct_data( Archive& archive, const DEC_AutomateDecision* role, const unsigned int /*version*/ )
{
    archive << role->pEntity_;
    archive << role->gcPause_;
    archive << role->gcMult_;
    archive << role->logger_;
}

template< typename Archive >
void load_construct_data( Archive& archive, DEC_AutomateDecision* role, const unsigned int /*version*/ )
{
    MIL_Automate* automate;
    unsigned int gcPause;
    unsigned int gcMult;
    sword::DEC_Logger* logger;
    archive >> automate;
    archive >> gcPause;
    archive >> gcMult;
    archive >> logger;
    ::new( role )DEC_AutomateDecision( *automate, gcPause, gcMult, logger );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision constructor
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
DEC_AutomateDecision::DEC_AutomateDecision( MIL_Automate& automate, unsigned int gcPause, unsigned int gcMult, sword::DEC_Logger* logger )
    : DEC_Decision            ( automate, gcMult, gcPause, logger )
    , nRulesOfEngagementState_( eRoeStateFreeFire )
    , pRoePopulation_         ( PHY_RoePopulation::Default() )
    , nCloseCombatState_      ( eCloseCombatStateNone )
    , nOperationalState_      ( eOpStateOperational )
    , fuseau_                 ( 0 )
    , ePhaseMission_          ( 0 )
    , eEtatEchelon_           ( 0 )
    , eEtatDec_               ( 0 )
    , eEtatLima_              ( 0 )
    , rDestruction_           ( 0 )
    , bStateHasChanged_       ( true )
    , bOrdreAttendre_         ( false )
    , bOrdrePoursuivre_       ( false )
    , bOrdreRalentir_         ( false )
    , bOrdreDecrocher_        ( false )
    , bOrdreTenirSurLR_       ( false )
    , bOrdreTenir_            ( false )
{
    const DEC_Model_ABC& model = automate.GetType().GetModel();
    try
    {
        SetModel( model );
    }
    catch( const std::exception& e )
    {
        throw MASA_EXCEPTION( tools::GetExceptionMsg( e ) );
    }
    StartDefaultBehavior();
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision destructor
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
DEC_AutomateDecision::~DEC_AutomateDecision()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::load
// Created: JVT 2005-04-05
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    unsigned int nRoePopulationID;
    file >> boost::serialization::base_object< DEC_Decision< MIL_Automate > >( *this );
    file >> pEntity_;
    file >> nRulesOfEngagementState_;
    file >> nCloseCombatState_;
    file >> nOperationalState_;
    file >> nRoePopulationID;
    pRoePopulation_ = PHY_RoePopulation::Find( nRoePopulationID );
    unsigned int nID;
    file >> nID;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::save
// Created: JVT 2005-04-05
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    assert( pEntity_ );
    unsigned int nRoePopulationID = pRoePopulation_->GetID();
    unsigned id = pEntity_->GetType().GetID();
    file << boost::serialization::base_object< DEC_Decision< MIL_Automate > >( *this );
    file << pEntity_;
    file << nRulesOfEngagementState_;
    file << nCloseCombatState_;
    file << nOperationalState_;
    file << nRoePopulationID;
    file << id;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::EndCleanStateAfterCrash
// Created: NLD 2005-06-23
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::EndCleanStateAfterCrash()
{
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::GetGroupName
// Created: LDC 2011-09-01
// -----------------------------------------------------------------------------
std::string DEC_AutomateDecision::GetGroupName()
{
    return GetAutomate().GetName();
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::StartMissionMrtBehavior
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::StartMissionMrtBehavior( const boost::shared_ptr< MIL_Mission_ABC > mission )
{
    const std::string& strBehavior = mission->GetType().GetDIABehavior( MIL_MissionType_ABC::ePhaseMRT );

    ActivateOrder( strBehavior, mission );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::StopMissionMrtBehavior
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::StopMissionMrtBehavior( const boost::shared_ptr< MIL_Mission_ABC > mission )
{
    const std::string& strBehavior = mission->GetType().GetDIABehavior( MIL_MissionType_ABC::ePhaseMRT );
    StopMission( strBehavior );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::StartMissionConduiteBehavior
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::StartMissionConduiteBehavior( const boost::shared_ptr< MIL_Mission_ABC > mission )
{
    const std::string& strBehavior = mission->GetType().GetDIABehavior( MIL_MissionType_ABC::ePhaseCDT );
    ActivateOrder( strBehavior, mission );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::StopMissionConduiteBehavior
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::StopMissionConduiteBehavior( const boost::shared_ptr< MIL_Mission_ABC > mission )
{
    const std::string& strBehavior = mission->GetType().GetDIABehavior( MIL_MissionType_ABC::ePhaseCDT );
    StopMission( strBehavior );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::SendFullState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::SendFullState( client::AutomatAttributes& msg ) const
{
    msg().set_meeting_engagement( sword::EnumMeetingEngagementStatus( nCloseCombatState_ ) );
    msg().set_operational_state ( sword::EnumOperationalStatus( nOperationalState_ ) );
    msg().set_roe               ( sword::RulesOfEngagement::Value( nRulesOfEngagementState_ ) );
    msg().set_decisional_model  ( model_->GetName() );
    msg().set_brain_debug       ( brainDebug_ );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::SendChangedState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::SendChangedState( client::AutomatAttributes& msg ) const
{
    if( bStateHasChanged_ )
        SendFullState( msg );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::GetDecAutomate
// Created: LDC 2009-04-10
// Modified: NMI 2013-09-03
// -----------------------------------------------------------------------------
DEC_AutomateDecision* DEC_AutomateDecision::GetDecAutomate()
{
    return this;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::GetName
// Created: LDC 2009-04-10
// -----------------------------------------------------------------------------
std::string DEC_AutomateDecision::GetName() const
{
    return pEntity_->GetName();
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::Clean
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::Clean()
{
    bStateHasChanged_ = false;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::HasStateChanged
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
bool DEC_AutomateDecision::HasStateChanged() const
{
    return bStateHasChanged_;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::NotifyCloseCombatStateChanged
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::NotifyCloseCombatStateChanged( E_CloseCombatState nState )
{
    if( nCloseCombatState_ != nState )
    {
        nCloseCombatState_ = nState;
        bStateHasChanged_  = true;
    }
}

DEC_Decision_ABC::E_Kind DEC_AutomateDecision::GetKind() const
{
    return eAutomate;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::GetAutomate
// Created: NLD 2004-10-26
// -----------------------------------------------------------------------------
MIL_Automate& DEC_AutomateDecision::GetAutomate() const
{
    assert( pEntity_ );
    return *pEntity_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Automate_Decision::GetOrderManager
// Created: NMI 2013-11-15
// -----------------------------------------------------------------------------
MIL_OrderManager_ABC& DEC_AutomateDecision::GetOrderManager() const
{
    return GetAutomate().GetOrderManager();
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::GetKnowledgeGroup
// Created: LDC 2011-05-05
// -----------------------------------------------------------------------------
boost::shared_ptr< MIL_KnowledgeGroup > DEC_AutomateDecision::GetKnowledgeGroup() const
{
    return pEntity_->GetKnowledgeGroup();
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::RegisterSelf
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::RegisterSelf( sword::Brain& brain )
{
    brain.GetScriptRef( "myself" ) = (DEC_Decision_ABC*)this;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::RegisterSpecific
// Created: JSR 2013-02-18
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::RegisterSpecific( sword::Brain& brain, bool isMasalife, const std::string& groupName )
{
    if( isMasalife )
        brain.GetScriptRef( "InitMeAutomat" )(
            brain.GetScriptRef( "integration.ontology.types.body" ),
            brain.GetScriptRef( "myself" ),
            groupName );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::GeteEtatPhaseMission
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
int DEC_AutomateDecision::GeteEtatPhaseMission() const
{
    return ePhaseMission_;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::SeteEtatPhaseMission
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::SeteEtatPhaseMission( int value )
{
    ePhaseMission_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::GeteEtatLima
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
int DEC_AutomateDecision::GeteEtatLima() const
{
    return eEtatLima_;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::SeteEtatLima
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::SeteEtatLima( int value )
{
    eEtatLima_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::GeteEtatDec
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
int DEC_AutomateDecision::GeteEtatDec() const
{
    return eEtatDec_;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::SeteEtatDec
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::SeteEtatDec( int value )
{
    eEtatDec_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::GetrDestruction
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
float DEC_AutomateDecision::GetrDestruction() const
{
    return rDestruction_;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::SetrDestruction
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::SetrDestruction( float value )
{
    rDestruction_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::GeteEtatEchelon
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
int DEC_AutomateDecision::GeteEtatEchelon() const
{
    return eEtatEchelon_;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::SeteEtatEchelon
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::SeteEtatEchelon( int value )
{
    eEtatEchelon_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::Getfuseau
// Created: LDC 2009-05-19
// $$$$ LDC USed only for ada, i.e. in the ge40 pion. Should be removed and rewritten.
// -----------------------------------------------------------------------------
const MIL_Fuseau* DEC_AutomateDecision::Getfuseau() const
{
    return fuseau_;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::Setfuseau
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::Setfuseau( MIL_Fuseau* value )
{
    fuseau_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::Getzone
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
TER_Localisation* DEC_AutomateDecision::Getzone() const
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::Setzone
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::Setzone( TER_Localisation* /*value*/ )
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::GetbOrdreAttendre
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
bool DEC_AutomateDecision::GetbOrdreAttendre() const
{
    return bOrdreAttendre_;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::SetbOrdreAttendre
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::SetbOrdreAttendre( bool value )
{
    bOrdreAttendre_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::GetbOrdrePoursuivre
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
bool DEC_AutomateDecision::GetbOrdrePoursuivre() const
{
    return bOrdrePoursuivre_;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::SetbOrdrePoursuivre
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::SetbOrdrePoursuivre( bool value )
{
    bOrdrePoursuivre_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::GetbOrdreRalentir
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
bool DEC_AutomateDecision::GetbOrdreRalentir() const
{
    return bOrdreRalentir_;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::SetbOrdreRalentir
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::SetbOrdreRalentir( bool value )
{
    bOrdreRalentir_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::GetbOrdreDecrocher
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
bool DEC_AutomateDecision::GetbOrdreDecrocher() const
{
    return bOrdreDecrocher_;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::SetbOrdreDecrocher
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::SetbOrdreDecrocher( bool value )
{
    bOrdreDecrocher_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::GetbOrdreTenirSurLR
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
bool DEC_AutomateDecision::GetbOrdreTenirSurLR() const
{
    return bOrdreTenirSurLR_;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::SetbOrdreTenirSurLR
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::SetbOrdreTenirSurLR( bool value )
{
    bOrdreTenirSurLR_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::GetbOrdreTenir
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
bool DEC_AutomateDecision::GetbOrdreTenir() const
{
    return bOrdreTenir_;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::SetbOrdreTenir
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::SetbOrdreTenir( bool value )
{
    bOrdreTenir_ = value;
}

// -----------------------------------------------------------------------------
// Name: std::vector< DEC_Decision_ABC* > DEC_AutomateDecision::GetPionsWithoutPC
// Created: LDC 2009-06-15
// -----------------------------------------------------------------------------
std::vector< DEC_Decision_ABC* > DEC_AutomateDecision::GetPionsWithPC()
{
    return DEC_AutomateFunctions::GetPionsWithPC( this );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::IsPionNeutralized
// Created: LDC 2009-06-15
// -----------------------------------------------------------------------------
bool DEC_AutomateDecision::IsPionNeutralized( DEC_Decision_ABC* pPion )
{
    return DEC_AutomateFunctions::IsPionNeutralized( this, pPion );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::GetDIAType
// Created: LDC 2009-07-09
// -----------------------------------------------------------------------------
const std::string& DEC_AutomateDecision::GetDIAType() const
{
    return DEC_Decision< MIL_Automate >::GetDIAType();
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::IsAutomateEngaged
// Created: HBD 2010-05-28
// -----------------------------------------------------------------------------
bool DEC_AutomateDecision::IsAutomateEngaged() const
{
    return pEntity_->IsEngaged();
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::NotifyRulesOfEngagementStateChanged
// Created: LDC 2011-08-05
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::NotifyRulesOfEngagementStateChanged( E_RulesOfEngagementState state )
{
    if( nRulesOfEngagementState_ != state )
    {
        nRulesOfEngagementState_ = state;
        bStateHasChanged_  = true;
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::NotifyRulesOfEngagementPopulationStateChanged
// Created: LGY 2012-06-22
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::NotifyRulesOfEngagementPopulationStateChanged( const PHY_RoePopulation& roe )
{
    if( roe != *pRoePopulation_ )
    {
        pRoePopulation_   = &roe;
        bStateHasChanged_  = true;
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::GetRoePopulation
// Created: LGY 2012-06-22
// -----------------------------------------------------------------------------
const PHY_RoePopulation& DEC_AutomateDecision::GetRoePopulation() const
{
    return *pRoePopulation_;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::AddReconnoiteredPoint
// Created: LMT 2011-08-16
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::AddReconnoiteredPointBy( DEC_Decision_ABC* pPion, MT_Vector2D& point )
{
    listReconnoitringPoint_[point] = pPion;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::PointIsReconnoiteredByMeOrNoOne
// Created: LMT 2011-08-16
// -----------------------------------------------------------------------------
bool DEC_AutomateDecision::PointIsReconnoiteredByMeOrNoOne( DEC_Decision_ABC* pPion,MT_Vector2D& point ) const
{
    std::map< MT_Vector2D, DEC_Decision_ABC* >::const_iterator it = listReconnoitringPoint_.find( point );
    if( it != listReconnoitringPoint_.end() )
    {
        if( it->second != pPion )
            return false;
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::Reload
// Created:AHC 2012-01-24
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::Reload( bool force, bool doInitBrain )
{
    DEC_Decision< MIL_Automate >::Reload( force, doInitBrain );
    bStateHasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::GetRulesOfEngagementState
// Created: LGY 2012-06-26
// -----------------------------------------------------------------------------
int DEC_AutomateDecision::GetRulesOfEngagementState() const
{
    return nRulesOfEngagementState_;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::ActivateBrainDebug
// Created: SLI 2013-06-21
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::ActivateBrainDebug( bool activate )
{
    DEC_Decision< MIL_Automate >::ActivateBrainDebug( activate );
    bStateHasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::PostStartMission
// Created: MMC 2013-10-15
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::PostStartMission()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::PostStopMission
// Created: MMC 2013-10-15
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::PostStopMission()
{
    // NOTHING
}
