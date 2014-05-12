// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Decision/DEC_RolePion_Decision.cpp $
// $Author: Nld $
// $Modtime: 23/06/05 10:28 $
// $Revision: 19 $
// $Workfile: DEC_RolePion_Decision.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_RolePion_Decision.h"
#include "ENT/ENT_Enums.h"
#include "Entities/Actions/PHY_ActionRecoLima.h"
#include "Entities/Agents/Units/Categories/PHY_RoePopulation.h"
#include "Entities/Agents/Roles/Communications/PHY_RolePion_Communications.h"
#include "Entities/Agents/Roles/Perception/PHY_RoleInterface_Perceiver.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/NBC/PHY_RoleInterface_NBC.h"
#include "Entities/Automates/DEC_AutomateDecision.h"
#include "Entities/Orders/MIL_LimaFunction.h"
#include "Entities/Orders/MIL_MissionType_ABC.h"
#include "Entities/Orders/MIL_Mission_ABC.h"
#include "Decision/DEC_Model_ABC.h"
#include "Decision/DEC_ActionFunctions.h"
#include "Decision/DEC_AgentFunctions.h"
#include "NetworkNotificationHandler_ABC.h"
#include "protocol/ClientSenders.h"

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_RolePion_Decision )

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision constructor
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
DEC_RolePion_Decision::DEC_RolePion_Decision( MIL_AgentPion& pion, unsigned int gcPause, unsigned int gcMult, sword::DEC_Logger* logger )
    : DEC_Decision( pion, gcPause, gcMult, logger )
    , nForceRatioState_         ( eForceRatioStateNone )
    , nRulesOfEngagementState_  ( eRoeStateFreeFire )
    , nCloseCombatState_        ( eCloseCombatStateNone )
    , nOperationalState_        ( eOpStateOperational )
    , nIndirectFireAvailability_( eFireAvailability_None )
    , pRoePopulation_           ( PHY_RoePopulation::Default() )
    , eEtatEchelon_             ( 0 )
    , eEtatDec_                 ( 0 )
    , eEtatDecPrudence_         ( 0 )
    , eEtatLima_                ( 0 )
    , eEtatNbc_                 ( 0 )
    , eEtatDestruction_         ( 0 )
    , eEtatFeu_                 ( 0 )
    , eEtatAmbiance_            ( 0 )
    , eEtatRadio_               ( 0 )
    , eEtatRadar_               ( 0 )
    , eEtatDeplacement_         ( 0 )
    , eEtatOrdreCoordination_   ( 0 )
    , eConsigneTir_             ( 0 )
    , eConsigneTirPopulation_   ( 0 )
    , eEtatSoutien_             ( 0 )
    , eEtatSituationEnnemi_     ( 0 )
    , eEtatPhaseMission_        ( 0 )
    , eTypeContact_             ( 0 )
    , eNiveauAction_            ( 0 )
    , bOrdreInterrompreMission_ ( false )
    , bOrdreDecrocher_          ( false )
    , bOrdreTenirSurLR_         ( false )
    , bOrdreTenir_              ( false )
    , bPasserSurLC_             ( false )
    , bDefenseStatique_Mobile_  ( false )
    , bStateHasChanged_         ( true )
    , pionEnEscorte_            ( 0 )
    , rTenir_                   ( 0.f )
    , pion_                     ( pion )
{
    const DEC_Model_ABC& model = pion.GetType().GetModel();
    try
    {
        SetModel( model );
    }
    catch( const std::exception& e )
    {
        throw MASA_EXCEPTION( tools::GetExceptionMsg( e ) );
    }
    // Parameters list for calling default behavior
    StartDefaultBehavior();
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision destructor
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
DEC_RolePion_Decision::~DEC_RolePion_Decision()
{
    // NOTHING
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::load
// Created: JVT 2005-04-01
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< DEC_Decision< MIL_AgentPion > >( *this )
        >> pEntity_
        >> nForceRatioState_
        >> nRulesOfEngagementState_
        >> nCloseCombatState_
        >> nOperationalState_
        >> nIndirectFireAvailability_;
    assert( pEntity_ );

    unsigned int nRoePopulationID;
    file >> nRoePopulationID;
    pRoePopulation_ = PHY_RoePopulation::Find( nRoePopulationID );
    assert( pRoePopulation_ );

    unsigned int nPionTypeID;
    file >> nPionTypeID;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::save
// Created: JVT 2005-04-01
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    unsigned roe  = pRoePopulation_->GetID(),
        type = pEntity_->GetType().GetID();
    file << boost::serialization::base_object< DEC_Decision< MIL_AgentPion > >( *this )
        << pEntity_
        << nForceRatioState_
        << nRulesOfEngagementState_
        << nCloseCombatState_
        << nOperationalState_
        << nIndirectFireAvailability_
        << roe
        << type;
}

// =============================================================================
// TOOLS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::EndCleanStateAfterCrash
// Created: LDC 2009-04-07
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::EndCleanStateAfterCrash()
{
    assert( pEntity_ );
    pEntity_->CancelAllActions();
    GetPion().GetRole< PHY_RoleInterface_Perceiver >().DisableAllPerceptions();
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GetGroupName
// Created: LDC 2011-09-01
// -----------------------------------------------------------------------------
std::string DEC_RolePion_Decision::GetGroupName()
{
    return GetAutomate().GetName();
}

// =============================================================================
// DEFAULT BEHAVIOR MANAGEMENT
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::StartMissionBehavior
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::StartMissionBehavior(const boost::shared_ptr< MIL_Mission_ABC > mission )
{
    const std::string& strBehavior = mission->GetType().GetDIABehavior();
    ActivateOrder( strBehavior, mission );
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::StopMissionBehavior
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::StopMissionBehavior( const boost::shared_ptr< MIL_Mission_ABC > mission )
{
    const std::string& strBehavior = mission->GetType().GetDIABehavior();
    StopMission( strBehavior );
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::StopMissionBehavior
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::NotifyHasChanged()
{
    bStateHasChanged_ = true;
    GetPion().Apply( &network::NetworkNotificationHandler_ABC::NotifyDataHasChanged );
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::NotifyRoePopulationChanged
// Created: NLD 2005-11-15
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::NotifyRoePopulationChanged( const PHY_RoePopulation& roe )
{
    assert( pRoePopulation_ );
    if( roe != *pRoePopulation_ )
    {
        pRoePopulation_   = &roe;
        NotifyHasChanged();
    }
}

// =============================================================================
// UPDATE
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::SendFullState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::SendFullState( client::UnitAttributes& msg ) const
{
    assert( pRoePopulation_ );
    msg().set_force_ratio               ( sword::ForceRatio_Value( nForceRatioState_ ) );
    msg().set_meeting_engagement        ( sword::EnumMeetingEngagementStatus( nCloseCombatState_ ) );
    msg().set_operational_state         ( sword::EnumOperationalStatus( nOperationalState_ ) );
    msg().set_indirect_fire_availability( sword::UnitAttributes::FireAvailability( nIndirectFireAvailability_ ) );
    msg().set_roe                       ( sword::RulesOfEngagement::Value( nRulesOfEngagementState_ ) );
    msg().set_roe_crowd                 ( pRoePopulation_->GetAsnID() );
    msg().set_decisional_model          ( model_->GetName() );
    msg().set_brain_debug               ( brainDebug_ );
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::SendChangedState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::SendChangedState( client::UnitAttributes& msg ) const
{
    if( bStateHasChanged_ )
        SendFullState( msg );
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GetAutomate
// Created: MGD 2010-02-25
// -----------------------------------------------------------------------------
MIL_Automate& DEC_RolePion_Decision::GetAutomate() const
{
    assert( pEntity_ );
    return pEntity_->GetAutomate();
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GetKnowledgeGroup
// Created: LDC 2011-05-05
// -----------------------------------------------------------------------------
boost::shared_ptr< MIL_KnowledgeGroup > DEC_RolePion_Decision::GetKnowledgeGroup() const
{
    return pEntity_->GetKnowledgeGroup();
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::RegisterSelf
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::RegisterSelf( sword::Brain& brain )
{
    brain.GetScriptRef( "myself" ) = static_cast< DEC_Decision_ABC* >( this );
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::RegisterSpecific
// Created: JSR 2013-02-18
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::RegisterSpecific( sword::Brain& brain, bool isMasalife, const std::string& groupName )
{
    if( isMasalife )
        brain.GetScriptRef( "InitMePlatoon" )(
            brain.GetScriptRef( "integration.ontology.types.body" ),
            brain.GetScriptRef( "myself" ),
            groupName );
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GeteEtatDec
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
int DEC_RolePion_Decision::GeteEtatDec() const
{
    return eEtatDec_;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::SeteEtatDec
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::SeteEtatDec( int value )
{
    eEtatDec_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GeteEtatDecPrudence
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
int DEC_RolePion_Decision::GeteEtatDecPrudence() const
{
    return eEtatDecPrudence_;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::SeteEtatDecPrudence
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::SeteEtatDecPrudence( int value )
{
    eEtatDecPrudence_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GeteEtatLima
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
int DEC_RolePion_Decision::GeteEtatLima() const
{
    return eEtatLima_;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::SeteEtatLima
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::SeteEtatLima( int value )
{
    eEtatLima_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GeteEtatNbc
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
int DEC_RolePion_Decision::GeteEtatNbc() const
{
    return eEtatNbc_;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::SeteEtatNbc
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::SeteEtatNbc( int value )
{
    eEtatNbc_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GeteEtatDestruction
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
int DEC_RolePion_Decision::GeteEtatDestruction() const
{
    return eEtatDestruction_;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::SeteEtatDestruction
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::SeteEtatDestruction( int value )
{
    eEtatDestruction_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GeteEtatFeu
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
int DEC_RolePion_Decision::GeteEtatFeu() const
{
    return eEtatFeu_;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::SeteEtatFeu
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::SeteEtatFeu( int value )
{
    eEtatFeu_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GeteEtatAmbiance
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
int DEC_RolePion_Decision::GeteEtatAmbiance() const
{
    return eEtatAmbiance_;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::SeteEtatAmbiance
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::SeteEtatAmbiance( int value )
{
    eEtatAmbiance_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GeteEtatRadio
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
int DEC_RolePion_Decision::GeteEtatRadio() const
{
    return eEtatRadio_;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::SeteEtatRadio
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::SeteEtatRadio( int value )
{
    eEtatRadio_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GeteEtatRadar
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
int DEC_RolePion_Decision::GeteEtatRadar() const
{
    return eEtatRadar_;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::SeteEtatRadar
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::SeteEtatRadar( int value )
{
    eEtatRadar_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GeteEtatDeplacement
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
int DEC_RolePion_Decision::GeteEtatDeplacement() const
{
    return eEtatDeplacement_;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::SeteEtatDeplacement
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::SeteEtatDeplacement( int value )
{
    eEtatDeplacement_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GeteEtatOrdreCoordination
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
int DEC_RolePion_Decision::GeteEtatOrdreCoordination() const
{
    return eEtatOrdreCoordination_;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::SeteEtatOrdreCoordination
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::SeteEtatOrdreCoordination( int value )
{
    eEtatOrdreCoordination_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GeteConsigneTir
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
int DEC_RolePion_Decision::GeteConsigneTir() const
{
    return eConsigneTir_;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::SeteConsigneTir
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::SeteConsigneTir( int value )
{
    eConsigneTir_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GeteConsigneTirPopulation
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
int DEC_RolePion_Decision::GeteConsigneTirPopulation() const
{
    return eConsigneTirPopulation_;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::SeteConsigneTirPopulation
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::SeteConsigneTirPopulation( int value )
{
    eConsigneTirPopulation_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GeteEtatEchelon
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
int DEC_RolePion_Decision::GeteEtatEchelon() const
{
    return eEtatEchelon_;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::SeteEtatEchelon
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::SeteEtatEchelon( int value )
{
    eEtatEchelon_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GeteEtatSoutien
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
int DEC_RolePion_Decision::GeteEtatSoutien() const
{
    return eEtatSoutien_;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::SeteEtatSoutien
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::SeteEtatSoutien( int value )
{
    eEtatSoutien_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GeteEtatSituationEnnemi
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
int DEC_RolePion_Decision::GeteEtatSituationEnnemi() const
{
    return eEtatSituationEnnemi_;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::SeteEtatSituationEnnemi
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::SeteEtatSituationEnnemi( int value )
{
    eEtatSituationEnnemi_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GeteEtatPhaseMission
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
int DEC_RolePion_Decision::GeteEtatPhaseMission() const
{
    return eEtatPhaseMission_;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::SeteEtatPhaseMission
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::SeteEtatPhaseMission( int value )
{
    eEtatPhaseMission_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GetbOrdreInterrompreMission
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
bool DEC_RolePion_Decision::GetbOrdreInterrompreMission() const
{
    return bOrdreInterrompreMission_;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::SetbOrdreInterrompreMission
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::SetbOrdreInterrompreMission( bool value )
{
    bOrdreInterrompreMission_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GetbOrdreDecrocher
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
bool DEC_RolePion_Decision::GetbOrdreDecrocher() const
{
    return bOrdreDecrocher_;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::SetbOrdreDecrocher
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::SetbOrdreDecrocher( bool value )
{
    bOrdreDecrocher_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GetbOrdreTenirSurLR
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
bool DEC_RolePion_Decision::GetbOrdreTenirSurLR() const
{
    return bOrdreTenirSurLR_;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::SetbOrdreTenirSurLR
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::SetbOrdreTenirSurLR( bool value )
{
    bOrdreTenirSurLR_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GetbOrdreTenir
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
bool DEC_RolePion_Decision::GetbOrdreTenir() const
{
    return bOrdreTenir_;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::SetbOrdreTenir
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::SetbOrdreTenir( bool value )
{
    bOrdreTenir_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GetbPasserSurLC
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
bool DEC_RolePion_Decision::GetbPasserSurLC() const
{
    return bPasserSurLC_;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::SetbPasserSurLC
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::SetbPasserSurLC( bool value )
{
    bPasserSurLC_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GetpionEnEscorte
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
DEC_Decision_ABC* DEC_RolePion_Decision::GetpionEnEscorte() const
{
    return pionEnEscorte_;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::SetpionEnEscorte
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::SetpionEnEscorte( DEC_Decision_ABC* value )
{
    pionEnEscorte_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GetitMvt
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Path_ABC > DEC_RolePion_Decision::GetitMvt() const
{
    return itMvt_;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::SetitMvt
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::SetitMvt( const boost::shared_ptr< DEC_Path_ABC >& value )
{
    itMvt_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GetobjectifEsquive
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_RolePion_Decision::GetobjectifEsquive() const
{
    return objectifEsquive_;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::SetobjectifEsquive
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::SetobjectifEsquive( MT_Vector2D* value )
{
    if( value )
        objectifEsquive_.reset( new MT_Vector2D( *value ) );
    else
        objectifEsquive_.reset();
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GeteniEnCours
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Agent > DEC_RolePion_Decision::GeteniEnCours() const
{
    return eniEnCours_;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::SeteniEnCours
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::SeteniEnCours( boost::shared_ptr< DEC_Knowledge_Agent > id )
{
    eniEnCours_ = id;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GetmissionPrecedente
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
std::string DEC_RolePion_Decision::GetmissionPrecedente() const
{
    return missionPrecedente_;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::SetmissionPrecedente
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::SetmissionPrecedente( const std::string& value )
{
    missionPrecedente_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GetrTenir
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
float DEC_RolePion_Decision::GetrTenir() const
{
    return rTenir_;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::SetrTenir
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::SetrTenir( float value )
{
    rTenir_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GeteTypeContact
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
int DEC_RolePion_Decision::GeteTypeContact() const
{
    return eTypeContact_;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::SeteTypeContact
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::SeteTypeContact( int value )
{
    eTypeContact_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GeteNiveauAction
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
int DEC_RolePion_Decision::GeteNiveauAction() const
{
    return eNiveauAction_;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::SeteNiveauAction
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::SeteNiveauAction( int value )
{
    eNiveauAction_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GetbDefenseStatique_Mobile
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
bool DEC_RolePion_Decision::GetbDefenseStatique_Mobile() const
{
    return bDefenseStatique_Mobile_;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::SetbDefenseStatique_Mobile
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::SetbDefenseStatique_Mobile( bool value )
{
    bDefenseStatique_Mobile_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GetselUnitesEnAppui
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
const std::vector< DEC_Decision_ABC* >* DEC_RolePion_Decision::GetselUnitesEnAppui() const
{
    return &pionsEnAppui_;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::AddToselUnitesEnAppui
// Created: LDC 2009-07-29
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::AddToselUnitesEnAppui( DEC_Decision_ABC* pPion )
{
    pionsEnAppui_.push_back( pPion );
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::RemoveFromselUnitesEnAppui
// Created: LDC 2009-07-29
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::RemoveFromselUnitesEnAppui( DEC_Decision_ABC* pPion )
{
    for ( std::vector< DEC_Decision_ABC* >::iterator it = pionsEnAppui_.begin(); it != pionsEnAppui_.end(); ++it )
        if( *it == pPion )
        {
            pionsEnAppui_.erase( it );
            return;
        }
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::IsNeutralized
// Created: LDC 2009-06-18
// -----------------------------------------------------------------------------
bool DEC_RolePion_Decision::IsNeutralized() const
{
    return GetPion().IsNeutralized();
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::IsMoving
// Created: LDC 2009-07-29
// -----------------------------------------------------------------------------
bool DEC_RolePion_Decision::IsMoving() const
{
    return( GetPion().GetRole< PHY_RoleInterface_Location >().GetCurrentSpeed() != 0. );
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::IsContaminated
// Created: LDC 2009-06-18
// -----------------------------------------------------------------------------
bool DEC_RolePion_Decision::IsContaminated() const
{
    return GetPion().GetRole< nbc::PHY_RoleInterface_NBC >().IsContaminated();
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::IsDecontaminationFinished
// Created: LGY 2013-01-07
// -----------------------------------------------------------------------------
bool DEC_RolePion_Decision::IsDecontaminationFinished() const
{
    return GetPion().GetRole< nbc::PHY_RoleInterface_NBC >().GetDecontaminationState() == 1.;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::IsJammed
// Created: MMC 2012-07-03
// -----------------------------------------------------------------------------
bool DEC_RolePion_Decision::IsJammed() const
{
    return GetPion().CallRole( &PHY_RolePion_Communications::IsJammed, false );
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GetPosition
// Created: LDC 2009-06-19
// -----------------------------------------------------------------------------
const MT_Vector2D* DEC_RolePion_Decision::GetPosition() const
{
    return DEC_AgentFunctions::GetPosition( GetPion() ).get();
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GetPion
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
MIL_AgentPion& DEC_RolePion_Decision::GetPion() const
{
    assert( pEntity_ );
    return *pEntity_;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GetOrderManager
// Created: NMI 2013-11-15
// -----------------------------------------------------------------------------
MIL_OrderManager_ABC& DEC_RolePion_Decision::GetOrderManager() const
{
    return GetPion().GetOrderManager();
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::Clean
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::Clean()
{
    bStateHasChanged_ = false;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GetRoePopulation
// Created: NLD 2005-11-16
// -----------------------------------------------------------------------------
const PHY_RoePopulation& DEC_RolePion_Decision::GetRoePopulation() const
{
    assert( pRoePopulation_ );
    return *pRoePopulation_;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::IsPC
// Created: SBO 2009-07-29
// -----------------------------------------------------------------------------
bool DEC_RolePion_Decision::IsPC() const
{
    return DEC_AgentFunctions::IsPC( GetPion() );
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::IsTransported
// Created: SBO 2009-07-29
// -----------------------------------------------------------------------------
bool DEC_RolePion_Decision::IsTransported() const
{
    return DEC_AgentFunctions::IsTransported( GetPion() );
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::IsFlying
// Created: SBO 2009-07-29
// -----------------------------------------------------------------------------
bool DEC_RolePion_Decision::IsFlying() const
{
    return DEC_AgentFunctions::IsFlying( GetPion() );
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::IsInCity
// Created: NMI 2014-05-12
// -----------------------------------------------------------------------------
bool DEC_RolePion_Decision::IsInCity() const
{
    return DEC_AgentFunctions::IsInCity( GetPion() );
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GetOperationalState
// Created: NMI 2013-06-12
// ----------------------------------------------------------------------
double DEC_RolePion_Decision::GetOperationalState() const
{
    return DEC_AgentFunctions::GetOperationalState( GetPion() );
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GetMajorOperationalState
// Created: SBO 2009-07-29
// ----------------------------------------------------------------------
double DEC_RolePion_Decision::GetMajorOperationalState() const
{
    return DEC_AgentFunctions::GetMajorOperationalState( GetPion() );
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::IsAutomateEngaged
// Created: SBO 2009-07-29
// -----------------------------------------------------------------------------
bool DEC_RolePion_Decision::IsAutomateEngaged() const
{
    return DEC_AgentFunctions::IsAutomateEngaged( GetPion() );
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::IsDead
// Created: SBO 2009-07-29
// -----------------------------------------------------------------------------
bool DEC_RolePion_Decision::IsDead() const
{
    return DEC_AgentFunctions::IsDead( GetPion() );
}

// =============================================================================
// NOTIFICATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::NotifyIndirectFireAvailabilityChanged
// Created: NLD 2005-10-19
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::NotifyIndirectFireAvailabilityChanged( E_FireAvailability nState )
{
    if( nIndirectFireAvailability_ != nState )
    {
        nIndirectFireAvailability_ = nState;
        NotifyHasChanged();
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::NotifyForceRatioStateChanged
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::NotifyForceRatioStateChanged( E_ForceRatioState nState )
{
    if( nForceRatioState_ != nState )
    {
        nForceRatioState_ = nState;
        NotifyHasChanged();
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::NotifyRulesOfEngagementStateChanged
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::NotifyRulesOfEngagementStateChanged( E_RulesOfEngagementState nState )
{
    if( nRulesOfEngagementState_ != nState )
    {
        nRulesOfEngagementState_ = nState;
        NotifyHasChanged();
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GetRulesOfEngagementState
// Created: DDA 2010-02-03
// -----------------------------------------------------------------------------
int DEC_RolePion_Decision::GetRulesOfEngagementState() const
{
    return nRulesOfEngagementState_;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::NotifyCloseCombatStateChanged
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::NotifyCloseCombatStateChanged( E_CloseCombatState nState )
{
    if( nCloseCombatState_ != nState )
    {
        nCloseCombatState_ = nState;
        NotifyHasChanged();
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::NotifyOperationalStateChanged
// Created: NLD 2005-07-26
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::NotifyOperationalStateChanged( E_OperationalState nState )
{
    if( nOperationalState_ != nState )
    {
        nOperationalState_ = nState;
        if( nOperationalState_ == eOpStateFullyDestroyed && pion_.IsPC() )
            MIL_Report::PostEvent( pion_, report::eRC_DestructionPC );
        if( nOperationalState_ == eOpStateTacticallyDestroyed )
            MIL_Report::PostEvent( pion_, report::eRC_TacticallyDestroyed );
        NotifyHasChanged();
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GetName
// Created: LDC 2009-04-09
// -----------------------------------------------------------------------------
std::string DEC_RolePion_Decision::GetName() const
{
    return pion_.GetName();
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GetDecAutomate
// Created: LDC 2009-04-09
// -----------------------------------------------------------------------------
DEC_AutomateDecision* DEC_RolePion_Decision::GetDecAutomate()
{
    return dynamic_cast< DEC_AutomateDecision* >( &pion_.GetAutomate().GetDecision() );
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GetDIAType
// Created: LDC 2009-07-09
// -----------------------------------------------------------------------------
const std::string& DEC_RolePion_Decision::GetDIAType() const
{
    return DEC_Decision< MIL_AgentPion >::GetDIAType();
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GetPionsWithPC
// Created: HBD 2010-05-27
// -----------------------------------------------------------------------------
std::vector< DEC_Decision_ABC* > DEC_RolePion_Decision::GetPionsWithPC()
{
    return DEC_AgentFunctions::GetPionsWithPC( GetPion() );
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::Reload
// Created:AHC 2012-01-12
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::Reload( bool doInitBrain )
{
    DEC_Decision< MIL_AgentPion >::Reload( doInitBrain );
    NotifyHasChanged();
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GetCommunicationPionsWithPC
// Created: MMC 2012-07-03
// -----------------------------------------------------------------------------
std::vector< DEC_Decision_ABC* > DEC_RolePion_Decision::GetCommunicationPionsWithPC()
{
    return DEC_AgentFunctions::GetCommunicationPionsWithPC( GetPion() );
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::ActivateBrainDebug
// Created: SLI 2013-06-21
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::ActivateBrainDebug( bool activate )
{
    DEC_Decision< MIL_AgentPion >::ActivateBrainDebug( activate );
    NotifyHasChanged();
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::PostStartMission
// Created: MMC 2013-10-15
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::PostStartMission()
{
    internalMissionActions_.push_back( DEC_ActionFunctions::StartAction< PHY_ActionRecoLima, const MIL_LimaFunction& >( pion_, MIL_LimaFunction::LR_ ) );
    internalMissionActions_.push_back( DEC_ActionFunctions::StartAction< PHY_ActionRecoLima, const MIL_LimaFunction& >( pion_, MIL_LimaFunction::LIA_ ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::PostStopMission
// Created: MMC 2013-10-15
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::PostStopMission()
{
    for( auto it = internalMissionActions_.begin(); it != internalMissionActions_.end(); ++it )
        DEC_ActionFunctions::StopAction( pion_, *it );
    internalMissionActions_.clear();
}
