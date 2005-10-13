//*****************************************************************************
//
// $Created: NLD 2002-07-15 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_Agent.cpp $
// $Author: Nld $
// $Modtime: 8/07/05 15:31 $
// $Revision: 28 $
// $Workfile: MOS_Agent.cpp $
//
//*****************************************************************************

#include "MOS_Light2_pch.h"
#include "MOS_Agent.h"

#include "MOS_App.h"
#include "MOS_MOSServer.h"
#include "MOS_AgentManager.h"
#include "MOS_World.h"
#include "MOS_Report_ABC.h"
#include "MOS_RC.h"
#include "MOS_Trace.h"
#include "MOS_Net_Def.h"
#include "MOS_ASN_Messages.h"
#include "MOS_AgentKnowledge.h"
#include "MOS_ObjectKnowledge.h"
#include "MOS_AgentModel.h"
#include "MOS_ObjectManager.h"
#include "MOS_Object_ABC.h"
#include "MOS_Team.h"
#include "MOS_Gtia.h"
#include "MOS_Surface.h"
#include "MOS_TypeAutomate.h"
#include "MOS_TypePion.h"
#include "MOS_Experience.h"
#include "MOS_Tiredness.h"
#include "MOS_Morale.h"
#include "MOS_FireResult.h"
#include "MOS_Population.h"

using namespace DIN;

MIL_AgentID MOS_Agent::nMaxId_ = 5000000;

// -----------------------------------------------------------------------------
// Name: MOS_Agent constructor
// Created: APE 2005-02-16
// -----------------------------------------------------------------------------
MOS_Agent::MOS_Agent( const ASN1T_MsgAutomateCreation& asnMsg )
    : nAgentID_  ( asnMsg.oid_automate  )
    , bAutomate_ ( true )
	, nTC2_		 ( 0 )
	, nLogMaintenanceSuperior_ ( 0 )
	, nLogMedicalSuperior_ ( 0 )
	, nLogSupplySuperior_ ( 0 )
{
    Initialize();
    sName_ = asnMsg.nom;

    pTypeAutomate_ = MOS_App::GetApp().GetAgentManager().FindTypeAutomate( asnMsg.type_automate );
    assert( pTypeAutomate_ );
    pTypePion_ = &pTypeAutomate_->GetTypePC();
    if( pTypePion_ )
    {
        symbolName_      = pTypePion_->GetSymbol();
        levelSymbolName_ = pTypePion_->GetLevelSymbol();
    }

    if( asnMsg.m.oid_tc2Present )
		nTC2_ = asnMsg.oid_tc2;
    if( asnMsg.m.oid_maintenancePresent )
        nLogMaintenanceSuperior_ = asnMsg.oid_maintenance;
    if( asnMsg.m.oid_santePresent )
        nLogMedicalSuperior_ = asnMsg.oid_sante;
    if( asnMsg.m.oid_ravitaillementPresent )
        nLogSupplySuperior_ = asnMsg.oid_ravitaillement;

    pGtia_ = MOS_App::GetApp().GetAgentManager().FindGtia( asnMsg.oid_groupe_connaissance );
    assert( pGtia_ );
}


// -----------------------------------------------------------------------------
// Name: MOS_Agent constructor
// Created: APE 2005-02-16
// -----------------------------------------------------------------------------
MOS_Agent::MOS_Agent( const ASN1T_MsgPionCreation& asnMsg )
    : nAgentID_  ( asnMsg.oid_pion  )
    , bAutomate_ ( false )
	, nTC2_		 ( 0 )
	, nLogMaintenanceSuperior_ ( 0 )
	, nLogMedicalSuperior_ ( 0 )
	, nLogSupplySuperior_ ( 0 )
{
    Initialize();
	sName_ = asnMsg.nom;

    pTypeAutomate_ = 0;
    pTypePion_     = MOS_App::GetApp().GetAgentManager().FindTypePion( asnMsg.type_pion );

	if( pTypePion_ )
    {
        symbolName_      = pTypePion_->GetSymbol();
        levelSymbolName_ = pTypePion_->GetLevelSymbol();
    }

    pParent_ = MOS_App::GetApp().GetAgentManager().FindAgent( asnMsg.oid_automate );
    assert( pParent_ );
    pParent_->AddChild( *this );
}


//-----------------------------------------------------------------------------
// Name: MOS_Agent constructor
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
MOS_Agent::MOS_Agent( bool bGenerateId )
    : nAgentID_ ( bGenerateId ? ++nMaxId_ : 0 )
	, nTC2_		 ( 0 )
	, nLogMaintenanceSuperior_ ( 0 )
	, nLogMedicalSuperior_ ( 0 )
	, nLogSupplySuperior_ ( 0 )
{
    Initialize();
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::Initialize
// Created: APE 2005-02-16
// -----------------------------------------------------------------------------
void MOS_Agent::Initialize()
{
    pExperience_                  = &MOS_Experience::veteran_;
    pTiredness_                   = &MOS_Tiredness::normal_;
    pMorale_                      = &MOS_Morale::bon_;
    pTypePion_                    = 0;
    pParent_                      = 0;
    bEmbraye_                     = false;
    nSpeed_                       = 0;
    nFightRateState_              = (E_ForceRatioState)-1;
    nRulesOfEngagementState_      = (E_RulesOfEngagementState)-1;
    nCloseCombatState_            = (E_CloseCombatState)-1;
    bLoadingState_                = true;
    bHumanTransportersReady_      = true;
    pGtia_                        = 0;
    bNbcProtectionSuitWorn_       = false;
    bVisionSurfacesNeedUpdating_  = true;
    nRawOpState_                  = 100;
    nOpState_                     = (E_OperationalState)-1;
    nContamination_               = 0;
    symbolName_                   = "?";
    levelSymbolName_              = "a";
    bCommJammed_                  = false;
    bRadioSilence_                = false;
    nPionRenforce_                = 0;
    nTransporteur_                = 0;
    bRadarEnabled_                = false;
    bPrisoner_                    = false;
    bSurrendered_                 = false;
    bRefugeesManaged_             = false;
    pMedicalData_                 = 0;
    pMaintenanceData_             = 0;
    pSupplyData_                  = 0;
    pTypeAutomate_				  = 0;
}


//-----------------------------------------------------------------------------
// Name: MOS_Agent destructor
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
MOS_Agent::~MOS_Agent()
{
    MT_DELETEOWNED( reportVector_ );
    delete pMaintenanceData_;
    delete pMedicalData_;
    delete pSupplyData_;
}

//=============================================================================
// ATTRIBUTES
//=============================================================================

// -----------------------------------------------------------------------------
// Name: mos_agent::OnLogisticLinksUpdated
// Created: NLD 2005-07-08
// -----------------------------------------------------------------------------
void MOS_Agent::OnLogisticLinksUpdated( const ASN1T_MsgChangeLiensLogistiquesAck& asnMsg )
{
    if( asnMsg.m.oid_tc2Present )
		nTC2_ = asnMsg.oid_tc2;
    if( asnMsg.m.oid_maintenancePresent )
        nLogMaintenanceSuperior_ = asnMsg.oid_maintenance;
    if( asnMsg.m.oid_santePresent )
        nLogMedicalSuperior_ = asnMsg.oid_sante;
    if( asnMsg.m.oid_ravitaillementPresent )
        nLogSupplySuperior_ = asnMsg.oid_ravitaillement;
}

//-----------------------------------------------------------------------------
// Name: MOS_Agent::OnReceiveMsgUnitInterVisibility
// Created: NLD 2003-03-17
//-----------------------------------------------------------------------------
void MOS_Agent::OnReceiveMsgUnitInterVisibility( DIN::DIN_Input& input )
{
    MIL_AgentID     nAgentID;
    uint8           nVisType;

    input >> nAgentID;
    input >> nVisType;
    MOS_Agent* pAgent = MOS_App::GetApp().GetAgentManager().FindAgent( nAgentID );
    assert( pAgent );

    recognizedAgents_.erase( pAgent );
    identifiedAgents_.erase( pAgent );
    detectedAgents_  .erase( pAgent );

    switch( nVisType )
    {
        case eVisTypeRecognized:
            recognizedAgents_.insert( pAgent );
            break;
        case eVisTypeIdentified:
            identifiedAgents_.insert( pAgent );
            break;
        case eVisTypeDetected:
            detectedAgents_.insert( pAgent );
            break;
        case eVisTypeInvisible:
            break;

        default:
            assert( false );
    }
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::OnReceiveMsgPopulationConcentrationInterVisibility
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
void MOS_Agent::OnReceiveMsgPopulationConcentrationInterVisibility( DIN::DIN_Input& input )
{
    uint32  nPopulationID;
    uint32  nConcentrationID;
    uint8   nVisType;

    input >> nPopulationID;
    input >> nConcentrationID;
    input >> nVisType;
    MOS_Population* pPopulation = MOS_App::GetApp().GetAgentManager().FindPopulation( nPopulationID );
    assert( pPopulation );
    const MOS_PopulationConcentration* pConcentration = pPopulation->FindConcentration( nConcentrationID );
    assert( pConcentration );

    concentrationsPerceived_.erase( pConcentration );
    switch( nVisType )
    {
        case eVisTypeInvisible: break;
        case eVisTypeRecognized:
        case eVisTypeDetected  :
        case eVisTypeIdentified:
            concentrationsPerceived_.insert( pConcentration ); break;
        default:
            assert( false );
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_Agent::OnReceiveMsgPopulationFlowInterVisibility
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
void MOS_Agent::OnReceiveMsgPopulationFlowInterVisibility( DIN::DIN_Input& input )
{
    uint32  nPopulationID;
    uint32  nFlowID;

    input >> nPopulationID;
    input >> nFlowID;
    MOS_Population* pPopulation = MOS_App::GetApp().GetAgentManager().FindPopulation( nPopulationID );
    assert( pPopulation );

    const MOS_PopulationFlux* pFlow = pPopulation->FindFlow( nFlowID );
    assert( pFlow );

    uint32 nNbrPoints;
    input >> nNbrPoints;

    T_PointVector shape;
    for( uint i = 0; i < nNbrPoints; ++i )
    {
        MT_Vector2D vTmp;
        input >> vTmp;
        shape.push_back( vTmp );
    }

    flowsPerceived_.erase( pFlow );
    if( !shape.empty() )
        flowsPerceived_.insert( std::make_pair( pFlow, shape )  );
}

//-----------------------------------------------------------------------------
// Name: MOS_Agent::OnReceiveMsgObjectInterVisibility
// Created: NLD 2003-03-17
//-----------------------------------------------------------------------------
void MOS_Agent::OnReceiveMsgObjectInterVisibility( DIN::DIN_Input& input )
{
    uint32  nObjectID;
    uint8   nObjectVisType;

    input >> nObjectID;
    input >> nObjectVisType;
    MOS_Object_ABC* pObject = MOS_App::GetApp().GetObjectManager().FindObject( nObjectID );
    assert( pObject );

    objectsPerceived_.erase( pObject );

    switch( nObjectVisType )
    {
    case eVisTypeInvisible: break;
    case eVisTypeRecognized:
    case eVisTypeDetected  :
    case eVisTypeIdentified:
        objectsPerceived_.insert( pObject ); break;
    default:
        assert( false );
    }
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::OnReceiveDebugDrawPointsMsg
// Created: NLD 2005-03-22
// -----------------------------------------------------------------------------
void MOS_Agent::OnReceiveDebugDrawPointsMsg( DIN_Input& msg )
{
    uint32 nTmp;
    msg >> nTmp;

    reportPoints_.clear();
    reportPoints_.reserve( nTmp );
    for( uint i = 0; i < nTmp; ++i )
    {
        MT_Vector2D vPos;
        msg >> vPos;
        reportPoints_.push_back( vPos );
    }
}

//-----------------------------------------------------------------------------
// Name: MOS_Agent::OnAttributeUpdated
// Created: NLD 2003-02-27
//-----------------------------------------------------------------------------
void MOS_Agent::OnAttributeUpdated( const ASN1T_MsgUnitAttributes& asnMsg )
{
    if( asnMsg.m.positionPresent )
    {
        MT_Vector2D vNewPos;
        MOS_App::GetApp().GetWorld().MosToSimMgrsCoord( (const char*)asnMsg.position.data, vNewPos );
        if( vNewPos != vPos_ )
        {
            vPos_ = vNewPos;
            oldPathVector_.push_back( vPos_ );
            bVisionSurfacesNeedUpdating_ = true;
        }
    }

    if( asnMsg.m.etat_operationnel_brutPresent )
        nRawOpState_ = asnMsg.etat_operationnel_brut;

    if( asnMsg.m.etat_operationnelPresent )
        nOpState_ = (E_OperationalState)asnMsg.etat_operationnel;

    if( asnMsg.m.pions_renforcantPresent )
    {
        renforts_.clear();
        for( uint i = 0; i < asnMsg.pions_renforcant.n; ++i )
            renforts_.push_back( asnMsg.pions_renforcant.elem[i] );
    }

    if( asnMsg.m.pion_renforcePresent )
        nPionRenforce_ = asnMsg.pion_renforce;

    if( asnMsg.m.pions_transportesPresent )
    {
        pionTransportes_.clear();
        for( uint i = 0; i < asnMsg.pions_transportes.n; ++i )
            pionTransportes_.push_back( asnMsg.pions_transportes.elem[i] );
    }

    if( asnMsg.m.pion_transporteurPresent )
        nTransporteur_ = asnMsg.pion_transporteur;

    if( asnMsg.m.posture_newPresent )
        nCurrentPosture_ = (E_UnitPosture)asnMsg.posture_new;

    if( asnMsg.m.posture_oldPresent )
        nOldPosture_ = (E_UnitPosture)asnMsg.posture_old;

    if( asnMsg.m.posture_pourcentagePresent )
        nPostureCompletionPourcentage_ = asnMsg.posture_pourcentage;

    if( asnMsg.m.etat_automatePresent )
        bEmbraye_ = ( asnMsg.etat_automate == EnumAutomateState::embraye );

    if( asnMsg.m.mortPresent ) 
        bDead_ = asnMsg.mort;

    if( asnMsg.m.neutralisePresent )
        bNeutralized_ = asnMsg.neutralise;

    if( asnMsg.m.rapport_de_forcePresent )
        nFightRateState_ = (E_ForceRatioState)asnMsg.rapport_de_force;

    if( asnMsg.m.regles_d_engagementPresent )
        nRulesOfEngagementState_ = (E_RulesOfEngagementState)asnMsg.regles_d_engagement;

    if( asnMsg.m.combat_de_rencontrePresent )
        nCloseCombatState_ = (E_CloseCombatState)asnMsg.combat_de_rencontre;

    if( asnMsg.m.embarquePresent )
        bLoadingState_ = asnMsg.embarque;

    if( asnMsg.m.transporteurs_disponiblesPresent )
        bHumanTransportersReady_ = asnMsg.transporteurs_disponibles;

    if( asnMsg.m.en_tenue_de_protection_nbcPresent )
        bNbcProtectionSuitWorn_ = asnMsg.en_tenue_de_protection_nbc;

    if( asnMsg.m.mode_furtif_actifPresent )
        bStealthModeEnabled_ = asnMsg.mode_furtif_actif;

    if( asnMsg.m.etat_contaminationPresent )
        nContamination_ = asnMsg.etat_contamination;

    if( asnMsg.m.contamine_par_agents_nbcPresent )
    {
        contaminatingNbcAgents_.clear();
        for( uint i = 0; i < asnMsg.contamine_par_agents_nbc.n; ++i )
            contaminatingNbcAgents_.push_back( asnMsg.contamine_par_agents_nbc.elem[i] );
    }

    if( asnMsg.m.moralPresent )
        pMorale_ = MOS_Morale::Find( asnMsg.moral );

    if( asnMsg.m.experiencePresent )
    {
        bVisionSurfacesNeedUpdating_ = true;
        pExperience_ = MOS_Experience::Find( asnMsg.experience );
    }

    if( asnMsg.m.fatiguePresent )
    {
        bVisionSurfacesNeedUpdating_ = true;
        pTiredness_ = MOS_Tiredness::Find( asnMsg.fatigue );
    }

    if( asnMsg.m.vitessePresent )
        nSpeed_ = asnMsg.vitesse;

    if( asnMsg.m.hauteurPresent )
        nAltitude_ = asnMsg.hauteur;

    if( asnMsg.m.directionPresent )
        nDirection_ = asnMsg.direction;

    if( asnMsg.m.communications_brouilleesPresent )
        bCommJammed_ = asnMsg.communications_brouillees;

    if( asnMsg.m.silence_radioPresent )
        bRadioSilence_ = asnMsg.silence_radio;

    if( asnMsg.m.radar_actifPresent  )
        bRadarEnabled_ = asnMsg.radar_actif;

    if( asnMsg.m.prisonnierPresent )
        bPrisoner_ = asnMsg.prisonnier;

    if( asnMsg.m.renduPresent )
        bSurrendered_ = asnMsg.rendu;

    if( asnMsg.m.refugie_pris_en_comptePresent )
        bRefugeesManaged_ = asnMsg.refugie_pris_en_compte;
}


//-----------------------------------------------------------------------------
// Name: MOS_Agent::OnAttributeUpdated
// Created: NLD 2003-03-06
//-----------------------------------------------------------------------------
void MOS_Agent::OnAttributeUpdated( const ASN1T_MsgUnitDotations& asnMsg )
{
    bool bWasOutOfGas = this->IsOutOfGas();
    composition_.SetComposition( asnMsg );
    bool bNowOutOfGas = this->IsOutOfGas();

    if( ! bWasOutOfGas && bNowOutOfGas )
        MOS_App::GetApp().NotifyAgentOutOfGas( *this );
    if( bWasOutOfGas && ! bNowOutOfGas )
        MOS_App::GetApp().NotifyAgentRefueled( *this );
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::OnReceiveMsgLogMaintenanceEtat
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void MOS_Agent::OnReceiveMsgLogMaintenanceEtat( const ASN1T_MsgLogMaintenanceEtat& asn )
{
    if( ! pMaintenanceData_ )
        pMaintenanceData_ = new T_MaintenanceData();

    T_MaintenanceData& data = * pMaintenanceData_;

    if( asn.m.chaine_activeePresent )
        data.bChainEnabled_ = asn.chaine_activee;
    if( asn.m.temps_de_bordeePresent )
        data.nTempsBordee_ = 8 + asn.temps_de_bordee * 4;

    if( asn.m.prioritesPresent )
    {
        data.priorities_.resize( asn.priorites.n );
        for( uint i = 0; i < asn.priorites.n; ++i )
            data.priorities_[i] = asn.priorites.elem[i];
    }   
    if( asn.m.priorites_tactiquesPresent )
    {
        data.tacticalPriorities_.resize( asn.priorites_tactiques.n );
        for( uint i = 0; i < asn.priorites_tactiques.n; ++i )
            data.tacticalPriorities_[i] = asn.priorites_tactiques.elem[i];
    }
    if( asn.m.disponibilites_remorqueursPresent )
    {
        data.dispoHaulers_.resize( asn.disponibilites_remorqueurs.n );
        for( uint i = 0; i < asn.disponibilites_remorqueurs.n; ++i )
            data.dispoHaulers_[i] = std::make_pair( asn.disponibilites_remorqueurs.elem[i].type_equipement, asn.disponibilites_remorqueurs.elem[i].pourcentage_disponibilite );
    }
    if( asn.m.disponibilites_reparateursPresent )
    {
        data.dispoRepairers_.resize( asn.disponibilites_reparateurs.n );
        for( uint i = 0; i < asn.disponibilites_reparateurs.n; ++i )
            data.dispoRepairers_[i] = std::make_pair( asn.disponibilites_reparateurs.elem[i].type_equipement, asn.disponibilites_reparateurs.elem[i].pourcentage_disponibilite );
    }
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::OnReceiveMsgLogMedicalEtat
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void MOS_Agent::OnReceiveMsgLogMedicalEtat( const ASN1T_MsgLogSanteEtat& asn )
{
    if( ! pMedicalData_ )
        pMedicalData_ = new T_MedicalData();

    T_MedicalData& data = * pMedicalData_;

    if( asn.m.chaine_activeePresent )
        data.bChainEnabled_ = asn.chaine_activee;

    if( asn.m.temps_de_bordeePresent )
        data.nTempsBordee_ = 8 + 4 * asn.temps_de_bordee;

    if( asn.m.prioritesPresent )
    {
        data.priorities_.resize( asn.priorites.n );
        for( uint i = 0; i < asn.priorites.n; ++i )
            data.priorities_[i] = asn.priorites.elem[i];
    }
    if( asn.m.priorites_tactiquesPresent )
    {
        data.tacticalPriorities_.resize( asn.priorites_tactiques.n );
        for( uint i = 0; i < asn.priorites_tactiques.n; ++i )
            data.tacticalPriorities_[i] = asn.priorites_tactiques.elem[i];
    }

    if( asn.m.disponibilites_ambulances_ramassagePresent )
    {
        data.dispoRamassageAmbulances_.resize( asn.disponibilites_ambulances_ramassage.n );
        for( uint i = 0; i < asn.disponibilites_ambulances_ramassage.n; ++i )
            data.dispoRamassageAmbulances_[i] = std::make_pair( asn.disponibilites_ambulances_ramassage.elem[i].type_equipement, asn.disponibilites_ambulances_ramassage.elem[i].pourcentage_disponibilite );
    }
    if( asn.m.disponibilites_ambulances_relevePresent )
    {
        data.dispoReleveAmbulances_.resize( asn.disponibilites_ambulances_releve.n );
        for( uint i = 0; i < asn.disponibilites_ambulances_releve.n; ++i )
            data.dispoReleveAmbulances_[i] = std::make_pair( asn.disponibilites_ambulances_releve.elem[i].type_equipement, asn.disponibilites_ambulances_releve.elem[i].pourcentage_disponibilite );
    }
    if( asn.m.disponibilites_medecinsPresent )
    {
        data.dispoDoctors_.resize( asn.disponibilites_medecins.n );
        for( uint i = 0; i < asn.disponibilites_medecins.n; ++i )
            data.dispoDoctors_[i] = std::make_pair( asn.disponibilites_medecins.elem[i].type_equipement, asn.disponibilites_medecins.elem[i].pourcentage_disponibilite );
    }
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::OnReceiveMsgLogSupplyEtat
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void MOS_Agent::OnReceiveMsgLogSupplyEtat( const ASN1T_MsgLogRavitaillementEtat& asn )
{
    if( ! pSupplyData_ )
        pSupplyData_ = new T_SupplyData();

    T_SupplyData& data = *pSupplyData_;
    if( asn.m.chaine_activeePresent )
        data.bChainEnabled_ = asn.chaine_activee;

    if( asn.m.disponibilites_transporteurs_convoisPresent )
    {
        data.dispoTransporters_.resize( asn.disponibilites_transporteurs_convois.n );
        for( uint i = 0; i < asn.disponibilites_transporteurs_convois.n; ++i )
            data.dispoTransporters_[i] = std::make_pair( asn.disponibilites_transporteurs_convois.elem[i].type_equipement, asn.disponibilites_transporteurs_convois.elem[i].pourcentage_disponibilite );
    }
    if( asn.m.disponibilites_chefs_convoisPresent )
    {
        data.dispoCommanders_.resize( asn.disponibilites_chefs_convois.n );
        for( uint i = 0; i < asn.disponibilites_chefs_convois.n; ++i )
            data.dispoCommanders_[i] = std::make_pair( asn.disponibilites_chefs_convois.elem[i].type_equipement, asn.disponibilites_chefs_convois.elem[i].pourcentage_disponibilite );
    }

    if( asn.m.stocksPresent )
    {
        uint nSize = asn.stocks.n;
        while( nSize > 0 )
        {
            ASN1T_DotationStock& value = asn.stocks.elem[ --nSize ];
            data.stocks_[ value.ressource_id ] = value.quantite_disponible;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::OnReceiveMsgLogSupplyQuotas
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void MOS_Agent::OnReceiveMsgLogSupplyQuotas( const ASN1T_MsgLogRavitaillementQuotas& asn )
{
    if( ! pSupplyData_ )
        pSupplyData_ = new T_SupplyData();

    T_SupplyData& data = *pSupplyData_;

    data.quotas_.resize( asn.quotas.n );
    for( uint i = 0; i < asn.quotas.n; ++i )
        data.quotas_[ i ] = std::make_pair( asn.quotas.elem[i].ressource_id, asn.quotas.elem[i].quota_disponible );
}


//-----------------------------------------------------------------------------
// Name: MOS_Agent::UpdatePathFind
/** Updates the pathfind vector to reflect the agent's progress along its path.
*/
// Created: FBD 03-01-29
//-----------------------------------------------------------------------------
void MOS_Agent::UpdatePathFind()
{
    if( pathFindVector_.size() <= 1 )
        return;

    // We have to determine where along the path is located the unit. To do this, we
    // compute the distance from the agent to each segment of the path, and take
    // the closest segment. The path is then truncated to discard all the points
    // before that segment (since we're supposed to have passed them).

    MT_Float rMinDistance = 999999999.0;
    IT_PointVector itLastValidPoint = pathFindVector_.end();

    // Iterate on the path vector.
    IT_PointVector itPreviousPoint = pathFindVector_.begin();
    for( IT_PointVector it = pathFindVector_.begin() + 1; it != pathFindVector_.end() ; ++it )
    {
        MT_Line curLine( *itPreviousPoint, *it );
        MT_Vector2D vNewPos = curLine.ClosestPointOnLine( vPos_ );
        MT_Float rSqrDistance = vPos_.SquareDistance( vNewPos );
        if( rSqrDistance < rMinDistance )
        {
            itLastValidPoint = itPreviousPoint;
            rMinDistance = rSqrDistance;
        }

        // If the distance is less than 0.1 km, we can consider that this is good enough.
        if( rSqrDistance < (0.1 * 0.1) )
            break;

        itPreviousPoint = it;
    }

    // Check and handle an improbable result.
    if( itLastValidPoint == pathFindVector_.end() )
    {
        pathFindVector_.clear();
        return;
    }

    // Move the points we want to keep to the begining of the vector, and trucate the end.
    std::copy( itLastValidPoint, pathFindVector_.end(), pathFindVector_.begin() );
    pathFindVector_.resize( pathFindVector_.end() - itLastValidPoint );

}


//-----------------------------------------------------------------------------
// Name: MOS_Agent::ClearOldPath
// Created: FBD 03-01-29
//-----------------------------------------------------------------------------
void MOS_Agent::ClearOldPath()
{
    oldPathVector_.clear();
}


//-----------------------------------------------------------------------------
// Name: MOS_Agent::OnReceiveMsgWaitForOrderConduite
// Created: NLD 2003-04-04
//-----------------------------------------------------------------------------
void MOS_Agent::OnReceiveMsgWaitForOrderConduite( const ASN1T_MsgAttenteOrdreConduite& asnMsg )
{
    MOS_RC& rc = *new MOS_RC( *this );
    rc.Initialize( asnMsg );
    reportVector_.push_back( &rc );

    MOS_App::GetApp().NotifyReportCreated( *this, rc );
}




//-----------------------------------------------------------------------------
// Name: MOS_Agent::OnReceiveMsgPathFind
// Created: NLD 2003-01-28
//-----------------------------------------------------------------------------
void MOS_Agent::OnReceiveMsgPathFind( const ASN1T_MsgUnitPathFind& asnMsg )
{
    pathFindVector_.clear();
    for( uint i = 0; i < asnMsg.itineraire.vecteur_point.n; ++i )
    {
        MT_Vector2D vPos;
        MOS_App::GetApp().GetWorld().MosToSimMgrsCoord( (const char*)asnMsg.itineraire.vecteur_point.elem[i].data, vPos );
        pathFindVector_.push_back( vPos );
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_Agent::OnReceiveMsgVisionCones
// Created: NLD 2003-02-12
//-----------------------------------------------------------------------------
void MOS_Agent::OnReceiveMsgVisionCones( DIN_Input& msg )
{
    for( CIT_SurfaceVector itSurface = surfaces_.begin(); itSurface != surfaces_.end(); ++itSurface )
        delete *itSurface;
    surfaces_.clear();

    uint32 nNbrSurfaces;
    msg >> nNbrSurfaces;
    for( uint i = 0; i < nNbrSurfaces; ++i )
        surfaces_.push_back( new MOS_Surface( msg ) );

    msg >> rElongationFactor_;

    bVisionSurfacesNeedUpdating_ = true;
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::GetElongationFactor
// Created: AGE 2005-03-18
// -----------------------------------------------------------------------------
MT_Float MOS_Agent::GetElongationFactor() const
{
    return rElongationFactor_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::OnSuperiorChanged
// Created: NLD 2003-10-09
// -----------------------------------------------------------------------------
void MOS_Agent::OnSuperiorChanged( MOS_Agent& newSuperior )
{
    assert( pParent_ );
    IT_AgentVector it = std::find( pParent_->children_.begin(), pParent_->children_.end(), this );
    assert( it != pParent_->children_.end() );
    pParent_->children_.erase( it );
    pParent_ = &newSuperior;
    newSuperior.AddChild( *this );
}


// -----------------------------------------------------------------------------
// Name: MOS_Agent::OnSuperiorChanged
/** @param  superior
*/
// Created: APE 2004-10-29
// -----------------------------------------------------------------------------
void MOS_Agent::OnSuperiorChanged( MOS_Gtia& superior )
{
    assert( pParent_ == 0 );
    pGtia_ = &superior;

    for( IT_AgentVector it = children_.begin(); it != children_.end(); ++it )
        (*it)->SetGtia( *pGtia_ );
}


// -----------------------------------------------------------------------------
// Name: MOS_Agent::ReadODB
/** @param  archive
*/
// Created: APE 2004-08-30
// -----------------------------------------------------------------------------
void MOS_Agent::ReadODB( MOS_InputArchive& archive, bool bAutomata )
{

    if( bAutomata )
        archive.Section( "Automate" );
    else
        archive.Section( "Pion" );

    archive.ReadAttribute( "type", strCategory_ );

    if( bAutomata )
    {
        pTypeAutomate_ = MOS_App::GetApp().GetAgentManager().FindTypeAutomate( strCategory_ );
        if( pTypeAutomate_ )
            pTypePion_ = & pTypeAutomate_->GetTypePC();
    }
    else
        pTypePion_ = MOS_App::GetApp().GetAgentManager().FindTypePion( strCategory_ );
    if( pTypePion_ )
    {
        symbolName_      = pTypePion_->GetSymbol();
        levelSymbolName_ = pTypePion_->GetLevelSymbol();
    };

    archive.ReadAttribute( "id", nAgentID_ );
    if( nAgentID_ > nMaxId_ )
        nMaxId_ = nAgentID_;

    if( ! archive.ReadField( "Nom", sName_, MOS_InputArchive::eNothing ) )
        sName_ = strCategory_;

    archive.ReadField( "Embraye", bEmbraye_, MOS_InputArchive::eNothing );

    std::string strPos;
    archive.ReadField( "Position", strPos );
    MOS_App::GetApp().GetWorld().MosToSimMgrsCoord( strPos, vPos_ );

    bAutomate_ = bAutomata;

    archive.Section( "LiensHierarchiques" );
    if( bAutomate_ )
    {
        uint nGtia;
        archive.ReadField( "GroupeConnaissance", nGtia );
        pGtia_ = MOS_App::GetApp().GetAgentManager().FindGtia( nGtia );
        assert( pGtia_ != 0 );
        if( archive.Section( "TC2", MOS_InputArchive::eNothing ) )
        {
			archive.ReadAttribute( "automate", nTC2_ );
            archive.EndSection();
        }
        if( archive.Section( "Logistique", MOS_InputArchive::eNothing ) )
        {
			if( archive.Section( "Maintenance", MOS_InputArchive::eNothing ) )
			{
				archive.ReadAttribute( "automate", nLogMaintenanceSuperior_ );
				archive.EndSection();
			}
			if( archive.Section( "Sante", MOS_InputArchive::eNothing ) )
			{
				archive.ReadAttribute( "automate", nLogMedicalSuperior_ );
				archive.EndSection();
			}
			if( archive.Section( "Ravitaillement", MOS_InputArchive::eNothing ) )
			{
				archive.ReadAttribute( "automate", nLogSupplySuperior_ );
				archive.EndSection();
			}
            archive.EndSection();
        }
    }
    else
    {
        uint nParent;
        archive.ReadField( "Automate", nParent );
        pParent_ = MOS_App::GetApp().GetAgentManager().FindAgent( (MIL_AgentID)nParent );
        assert( pParent_ != 0 );
        pParent_->AddChild( *this );
        pGtia_ = & pParent_->GetGtia();
    }
    archive.EndSection();

    archive.EndSection();
}


// -----------------------------------------------------------------------------
// Name: MOS_Agent::WriteODB
/** @param  archive
*/
// Created: APE 2004-08-30
// -----------------------------------------------------------------------------
void MOS_Agent::WriteODB( MT_XXmlOutputArchive& archive )
{
    if( bAutomate_ )
        archive.Section( "Automate" );
    else
        archive.Section( "Pion" );

    archive.WriteAttribute( "type", strCategory_ );
    archive.WriteAttribute( "id", nAgentID_ );
    archive.WriteField( "Nom", sName_ );

    std::string strPos;
    MOS_App::GetApp().GetWorld().SimToMosMgrsCoord( vPos_, strPos );
    archive.WriteField( "Position", strPos );

    if( bAutomate_ )
    {
		archive.WriteField( "Embraye", bEmbraye_ );
        archive.Section( "LiensHierarchiques" );
        archive.WriteField( "Armee", pGtia_->GetTeam().GetName() );
        archive.WriteField( "GroupeConnaissance", pGtia_->GetID() );
        if( nTC2_ != (uint)0 )
        {
            archive.Section( "TC2" );
            archive.WriteAttribute( "automate", nTC2_ );
            archive.EndSection();
		}
        if( nLogMaintenanceSuperior_ != (uint)0
			||nLogMedicalSuperior_ != (uint)0
			||nLogSupplySuperior_ != (uint)0 )
        {
	            archive.Section( "Logistique" );
				if( nLogMaintenanceSuperior_ != (uint)0 )
				{
					archive.Section( "Maintenance" );
					archive.WriteAttribute( "automate", nLogMaintenanceSuperior_ );
					archive.EndSection();
				}
				if( nLogMedicalSuperior_ != (uint)0 )
				{
					archive.Section( "Sante" );
					archive.WriteAttribute( "automate", nLogMedicalSuperior_ );
					archive.EndSection();
				}
				if( nLogSupplySuperior_ != (uint)0 )
				{
					archive.Section( "Ravitaillement" );
					archive.WriteAttribute( "automate", nLogSupplySuperior_ );
					archive.Section( "Quotas" );
					archive.EndSection();
					archive.EndSection();
				}
				archive.EndSection();
		}
		archive.EndSection();
    }
    else
    {
        archive.Section( "LiensHierarchiques" );
        archive.WriteField( "Automate", pParent_->GetAgentID() );
        archive.EndSection();
    }
    
    archive.EndSection();
}


// -----------------------------------------------------------------------------
// Name: MOS_Agent::GetModel
// Created: AGN 2003-12-22
// -----------------------------------------------------------------------------
const MOS_AgentModel* MOS_Agent::GetModelPion() const
{
    assert( pTypePion_ );
    return &pTypePion_->GetModel();
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::GetModel
// Created: AGN 2003-12-22
// -----------------------------------------------------------------------------
const MOS_AgentModel* MOS_Agent::GetModelAutomate() const
{
    if( !pTypeAutomate_ )
        return 0;
    return &pTypeAutomate_->GetModel();
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::AddConsign
// Created: AGE 2005-03-31
// -----------------------------------------------------------------------------
void MOS_Agent::AddConsign( MOS_LogSupplyConsign& consign )
{
    requestedSupplies_.insert( &consign );
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::RemoveConsign
// Created: AGE 2005-03-31
// -----------------------------------------------------------------------------
void MOS_Agent::RemoveConsign( MOS_LogSupplyConsign& consign )
{
    requestedSupplies_.erase( &consign );
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::AddConsign
// Created: AGE 2005-03-31
// -----------------------------------------------------------------------------
void MOS_Agent::AddConsign( MOS_LogMaintenanceConsign& consign )
{
    requestedMaintenances_.insert( &consign );
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::RemoveConsign
// Created: AGE 2005-03-31
// -----------------------------------------------------------------------------
void MOS_Agent::RemoveConsign( MOS_LogMaintenanceConsign& consign )
{
    requestedMaintenances_.erase( &consign );
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::AddConsign
// Created: AGE 2005-03-31
// -----------------------------------------------------------------------------
void MOS_Agent::AddConsign( MOS_LogMedicalConsign& consign )
{
    requestedMedical_.insert( &consign );
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::RemoveConsign
// Created: AGE 2005-03-31
// -----------------------------------------------------------------------------
void MOS_Agent::RemoveConsign( MOS_LogMedicalConsign& consign )
{
    requestedMedical_.erase( &consign );
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::HandleConsign
// Created: AGE 2005-03-31
// -----------------------------------------------------------------------------
void MOS_Agent::HandleConsign( MOS_LogSupplyConsign& consign )
{
    handledSupplies_.insert( &consign );
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::TerminateConsign
// Created: AGE 2005-03-31
// -----------------------------------------------------------------------------
void MOS_Agent::TerminateConsign( MOS_LogSupplyConsign& consign )
{
    handledSupplies_.erase( &consign );
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::HandleConsign
// Created: AGE 2005-03-31
// -----------------------------------------------------------------------------
void MOS_Agent::HandleConsign( MOS_LogMaintenanceConsign& consign )
{
    handledMaintenances_.insert( &consign );
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::TerminateConsign
// Created: AGE 2005-03-31
// -----------------------------------------------------------------------------
void MOS_Agent::TerminateConsign( MOS_LogMaintenanceConsign& consign )
{
    handledMaintenances_.erase( &consign );
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::HandleConsign
// Created: AGE 2005-03-31
// -----------------------------------------------------------------------------
void MOS_Agent::HandleConsign( MOS_LogMedicalConsign& consign )
{
    handledMedical_.insert( &consign );
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::TerminateConsign
// Created: AGE 2005-03-31
// -----------------------------------------------------------------------------
void MOS_Agent::TerminateConsign( MOS_LogMedicalConsign& consign )
{
    handledMedical_.erase( &consign );
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::OnReceiveMsgStopFire
// Created: SBO 2005-08-30
// -----------------------------------------------------------------------------
void MOS_Agent::OnReceiveMsgStopFire( const ASN1T_FireResult& asnMsg )
{
    MOS_FireResult& fireResult = *new MOS_FireResult();
    fireResult.Initialize( asnMsg );
    fireResults_.push_back( &fireResult );
    if( fireResults_.size() > 20 )
        fireResults_.erase( fireResults_.begin() );
}
    
// -----------------------------------------------------------------------------
// Name: MOS_Agent::DeleteAllFireResults
// Created: SBO 2005-08-30
// -----------------------------------------------------------------------------
void MOS_Agent::DeleteAllFireResults()
{
    for( CIT_FireResults it = fireResults_.begin(); it != fireResults_.end(); ++it )
        delete *it;
    fireResults_.clear();
}
