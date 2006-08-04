//*****************************************************************************
//
// $Created: NLD 2002-07-15 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/Agent.cpp $
// $Author: Nld $
// $Modtime: 8/07/05 15:31 $
// $Revision: 28 $
// $Workfile: Agent.cpp $
//
//*****************************************************************************

#include "astec_pch.h"
#include "Agent.h"

#include "App.h"
#include "AgentManager.h"
#include "World.h"
#include "RC.h"
#include "Net_Def.h"
#include "ASN_Messages.h"
#include "AgentKnowledge.h"
#include "ObjectKnowledge.h"
#include "AgentModel.h"
#include "ObjectManager.h"
#include "Object_ABC.h"
#include "Team.h"
#include "Gtia.h"
#include "Surface.h"
#include "TypeAutomate.h"
#include "TypePion.h"
#include "Experience.h"
#include "Tiredness.h"
#include "Morale.h"
#include "Population.h"
#include "PopulationConcentration.h"
#include "PopulationFlow.h"
#include "Resource.h"

using namespace DIN;

MIL_AgentID Agent::nMaxId_ = 5000000;

// -----------------------------------------------------------------------------
// Name: Agent constructor
// Created: APE 2005-02-16
// -----------------------------------------------------------------------------
Agent::Agent( const ASN1T_MsgAutomateCreation& asnMsg )
    : nAgentID_  ( asnMsg.oid_automate  )
    , bAutomate_ ( true )
	, nTC2_		 ( 0 )
	, nLogMaintenanceSuperior_ ( 0 )
	, nLogMedicalSuperior_ ( 0 )
	, nLogSupplySuperior_ ( 0 )
{
    Initialize();
 
    std::stringstream strTmp;
    strTmp << asnMsg.nom << " - [" << nAgentID_ << "]";
    sName_ = strTmp.str();
    
    pTypeAutomate_ = App::GetApp().GetAgentManager().FindTypeAutomate( asnMsg.type_automate );
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

    pGtia_ = App::GetApp().GetAgentManager().FindGtia( asnMsg.oid_groupe_connaissance );
    assert( pGtia_ );
}


// -----------------------------------------------------------------------------
// Name: Agent constructor
// Created: APE 2005-02-16
// -----------------------------------------------------------------------------
Agent::Agent( const ASN1T_MsgPionCreation& asnMsg )
    : nAgentID_  ( asnMsg.oid_pion  )
    , bAutomate_ ( false )
	, nTC2_		 ( 0 )
	, nLogMaintenanceSuperior_ ( 0 )
	, nLogMedicalSuperior_ ( 0 )
	, nLogSupplySuperior_ ( 0 )
{
    Initialize();
    
    std::stringstream strTmp;
    strTmp << asnMsg.nom << " - [" << nAgentID_ << "]";
    sName_ = strTmp.str();

    pTypeAutomate_ = 0;
    pTypePion_     = App::GetApp().GetAgentManager().FindTypePion( asnMsg.type_pion );

	if( pTypePion_ )
    {
        symbolName_      = pTypePion_->GetSymbol();
        levelSymbolName_ = pTypePion_->GetLevelSymbol();
    }

    pParent_ = App::GetApp().GetAgentManager().FindAgent( asnMsg.oid_automate );
    assert( pParent_ );
    pParent_->AddChild( *this );
}


//-----------------------------------------------------------------------------
// Name: Agent constructor
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
Agent::Agent( bool bGenerateId )
    : nAgentID_ ( bGenerateId ? ++nMaxId_ : 0 )
	, nTC2_		 ( 0 )
	, nLogMaintenanceSuperior_ ( 0 )
	, nLogMedicalSuperior_ ( 0 )
	, nLogSupplySuperior_ ( 0 )
{
    Initialize();
}

// -----------------------------------------------------------------------------
// Name: Agent::Initialize
// Created: APE 2005-02-16
// -----------------------------------------------------------------------------
void Agent::Initialize()
{
    pExperience_                  = &Experience::veteran_;
    pTiredness_                   = &Tiredness::normal_;
    pMorale_                      = &Morale::bon_;
    pTypePion_                    = 0;
    pParent_                      = 0;
    bEmbraye_                     = false;
    nSpeed_                       = 0;
    nFightRateState_              = eEtatRapFor_None;
    nRulesOfEngagementState_      = eRoe_None;
    nRulesOfEngagementPopulationState_ = eRoePopulation_None;
    nCloseCombatState_            = eEtatCombatRencontre_None;
    bLoadingState_                = true;
    bHumanTransportersReady_      = true;
    pGtia_                        = 0;
    bNbcProtectionSuitWorn_       = false;
    bVisionSurfacesNeedUpdating_  = true;
    nRawOpState_                  = 100;
    nOpState_                     = eEtatOperationnel_Operationnel;
    nIndirectFireAvailability_    = eDisponibiliteAuTir_None;
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
    pSupplyData_                  = ( new Agent::T_SupplyData() );
    pTypeAutomate_				  = 0;
    bAggregated_                  = false;

    nOldPosture_     = eUnitPosture_PostureArret;
    nCurrentPosture_ = eUnitPosture_PostureArret;
    nPostureCompletionPourcentage_ = 0;
}


//-----------------------------------------------------------------------------
// Name: Agent destructor
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
Agent::~Agent()
{
    delete pMaintenanceData_;
    delete pMedicalData_;
    delete pSupplyData_;
    recognizedAgents_.clear();
    identifiedAgents_.clear();
    detectedAgents_  .clear();
    recordedAgents_  .clear();
}

//=============================================================================
// ATTRIBUTES
//=============================================================================

// -----------------------------------------------------------------------------
// Name: mos_agent::OnLogisticLinksUpdated
// Created: NLD 2005-07-08
// -----------------------------------------------------------------------------
void Agent::OnLogisticLinksUpdated( const ASN1T_MsgChangeLiensLogistiquesAck& asnMsg )
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
// Name: Agent::OnReceiveMsgUnitInterVisibility
// Created: NLD 2003-03-17
//-----------------------------------------------------------------------------
void Agent::OnReceiveMsgUnitInterVisibility( DIN::DIN_Input& input )
{
    MIL_AgentID     nAgentID;
    uint8           nVisType;
    uint8           nMaxVisType;
    bool            bRecordMode;

    input >> nAgentID;
    input >> nVisType;
    input >> nMaxVisType;
    input >> bRecordMode;
    Agent* pAgent = App::GetApp().GetAgentManager().FindAgent( nAgentID );
    assert( pAgent );

    recognizedAgents_.erase( pAgent );
    identifiedAgents_.erase( pAgent );
    detectedAgents_  .erase( pAgent );
    recordedAgents_  .erase( pAgent );

    if( bRecordMode )
        recordedAgents_.insert( pAgent );
    else
    {
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
}

// -----------------------------------------------------------------------------
// Name: Agent::OnReceiveMsgPopulationCollision
// Created: NLD 2005-10-28
// -----------------------------------------------------------------------------
void Agent::OnReceiveMsgPopulationCollision( DIN::DIN_Input& msg )
{
    uint32  nPopulationID;

    msg >> nPopulationID;
    Population* pPopulation = App::GetApp().GetAgentManager().FindPopulation( nPopulationID );
    assert( pPopulation );

    populationCollisions_.erase( pPopulation );

    sPopulationCollision collision;

    uint32 nTmp;
    msg >> nTmp;
    for( uint i = 0; i < nTmp; ++i )
    {
        uint32 nConcentrationID;
        msg >> nConcentrationID;

        const PopulationConcentration* pConcentration = pPopulation->FindConcentration( nConcentrationID );
        assert( pConcentration );
        collision.concentrations_.insert( pConcentration );
    }

    msg >> nTmp;
    for( uint i = 0; i < nTmp; ++i )
    {
        uint32 nFlowID;
        msg >> nFlowID;

        const PopulationFlow* pFlow = pPopulation->FindFlow( nFlowID );
        assert( pFlow );
        collision.flows_.insert( pFlow );
    }

    if( !collision.flows_.empty() || !collision.concentrations_.empty() )
        populationCollisions_.insert( std::make_pair( pPopulation, collision ) );
}

// -----------------------------------------------------------------------------
// Name: Agent::OnReceiveMsgPopulationConcentrationInterVisibility
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
void Agent::OnReceiveMsgPopulationConcentrationInterVisibility( DIN::DIN_Input& input )
{
    uint32  nPopulationID;
    uint32  nConcentrationID;
    uint8   nVisType;

    input >> nPopulationID;
    input >> nConcentrationID;
    input >> nVisType;
    Population* pPopulation = App::GetApp().GetAgentManager().FindPopulation( nPopulationID );
    assert( pPopulation );
    const PopulationConcentration* pConcentration = pPopulation->FindConcentration( nConcentrationID );
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
// Name: Agent::OnReceiveMsgPopulationFlowInterVisibility
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
void Agent::OnReceiveMsgPopulationFlowInterVisibility( DIN::DIN_Input& input )
{
    uint32  nPopulationID;
    uint32  nFlowID;

    input >> nPopulationID;
    input >> nFlowID;
    Population* pPopulation = App::GetApp().GetAgentManager().FindPopulation( nPopulationID );
    assert( pPopulation );

    const PopulationFlow* pFlow = pPopulation->FindFlow( nFlowID );
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
// Name: Agent::OnReceiveMsgObjectInterVisibility
// Created: NLD 2003-03-17
//-----------------------------------------------------------------------------
void Agent::OnReceiveMsgObjectInterVisibility( DIN::DIN_Input& input )
{
    uint32  nObjectID;
    uint8   nObjectVisType;

    input >> nObjectID;
    input >> nObjectVisType;
    Object_ABC* pObject = App::GetApp().GetObjectManager().FindObject( nObjectID );
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

//-----------------------------------------------------------------------------
// Name: Agent::OnAttributeUpdated
// Created: NLD 2003-02-27
//-----------------------------------------------------------------------------
void Agent::OnAttributeUpdated( const ASN1T_MsgUnitAttributes& asnMsg )
{
    if( asnMsg.m.positionPresent )
    {
        MT_Vector2D vNewPos;
        App::GetApp().GetWorld().MosToSimMgrsCoord( (const char*)asnMsg.position.data, vNewPos );
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
        nOpState_ = (E_EtatOperationnel)asnMsg.etat_operationnel;

    if( asnMsg.m.disponibilite_au_tir_indirectPresent )
        nIndirectFireAvailability_  = (E_DisponibiliteAuTir)asnMsg.disponibilite_au_tir_indirect;

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
        nFightRateState_ = (E_EtatRapFor)asnMsg.rapport_de_force;

    if( asnMsg.m.roePresent )
        nRulesOfEngagementState_ = (E_Roe)asnMsg.roe;

    if( asnMsg.m.roe_populationPresent )
        nRulesOfEngagementPopulationState_ = (E_RoePopulation)asnMsg.roe_population;

    if( asnMsg.m.combat_de_rencontrePresent )
        nCloseCombatState_ = (E_EtatCombatRencontre)asnMsg.combat_de_rencontre;

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
        pMorale_ = Morale::Find( asnMsg.moral );

    if( asnMsg.m.experiencePresent )
    {
        bVisionSurfacesNeedUpdating_ = true;
        pExperience_ = Experience::Find( asnMsg.experience );
    }

    if( asnMsg.m.fatiguePresent )
    {
        bVisionSurfacesNeedUpdating_ = true;
        pTiredness_ = Tiredness::Find( asnMsg.fatigue );
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
// Name: Agent::OnAttributeUpdated
// Created: NLD 2003-03-06
//-----------------------------------------------------------------------------
void Agent::OnAttributeUpdated( const ASN1T_MsgUnitDotations& asnMsg )
{
    bool bWasOutOfGas = this->IsOutOfGas();
    composition_.SetComposition( asnMsg );
    bool bNowOutOfGas = this->IsOutOfGas();

    if( ! bWasOutOfGas && bNowOutOfGas )
        App::GetApp().NotifyAgentOutOfGas( *this );
    if( bWasOutOfGas && ! bNowOutOfGas )
        App::GetApp().NotifyAgentRefueled( *this );
}

// -----------------------------------------------------------------------------
// Name: Agent::OnReceiveMsgLogMaintenanceEtat
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void Agent::OnReceiveMsgLogMaintenanceEtat( const ASN1T_MsgLogMaintenanceEtat& asn )
{
    if( ! pMaintenanceData_ )
        pMaintenanceData_ = new T_MaintenanceData();

    T_MaintenanceData& data = * pMaintenanceData_;

    if( asn.m.chaine_activeePresent )
        data.bChainEnabled_ = asn.chaine_activee;
    if( asn.m.regime_travailPresent )
        data.nWorkRate_ = asn.regime_travail;

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
        {   
            T_LogisticAvailability& dataDispo = data.dispoHaulers_[i];
            dataDispo.nEquipmentType_   = asn.disponibilites_remorqueurs.elem[i].type_equipement;
            dataDispo.nNbrTotal_        = asn.disponibilites_remorqueurs.elem[i].nbr_total;
            dataDispo.nNbrAvailable_    = asn.disponibilites_remorqueurs.elem[i].nbr_disponibles;
            dataDispo.nNbrWorking_      = asn.disponibilites_remorqueurs.elem[i].nbr_au_travail;
            dataDispo.nNbrLent_         = asn.disponibilites_remorqueurs.elem[i].nbr_pretes;
            dataDispo.nNbrResting_      = std::numeric_limits< uint >::max();
            if( asn.disponibilites_remorqueurs.elem[i].m.nbr_au_reposPresent )
                dataDispo.nNbrResting_ = asn.disponibilites_remorqueurs.elem[i].nbr_au_repos;
        }
    }
    if( asn.m.disponibilites_reparateursPresent )
    {
        data.dispoRepairers_.resize( asn.disponibilites_reparateurs.n );
        for( uint i = 0; i < asn.disponibilites_reparateurs.n; ++i )
        {   
            T_LogisticAvailability& dataDispo = data.dispoRepairers_[i];
            dataDispo.nEquipmentType_   = asn.disponibilites_reparateurs.elem[i].type_equipement;
            dataDispo.nNbrTotal_        = asn.disponibilites_reparateurs.elem[i].nbr_total;
            dataDispo.nNbrAvailable_    = asn.disponibilites_reparateurs.elem[i].nbr_disponibles;
            dataDispo.nNbrWorking_      = asn.disponibilites_reparateurs.elem[i].nbr_au_travail;
            dataDispo.nNbrLent_         = asn.disponibilites_reparateurs.elem[i].nbr_pretes;
            dataDispo.nNbrResting_      = std::numeric_limits< uint >::max();
            if( asn.disponibilites_reparateurs.elem[i].m.nbr_au_reposPresent )
                dataDispo.nNbrResting_ = asn.disponibilites_reparateurs.elem[i].nbr_au_repos;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: Agent::OnReceiveMsgLogMedicalEtat
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void Agent::OnReceiveMsgLogMedicalEtat( const ASN1T_MsgLogSanteEtat& asn )
{
    if( ! pMedicalData_ )
        pMedicalData_ = new T_MedicalData();

    T_MedicalData& data = * pMedicalData_;

    if( asn.m.chaine_activeePresent )
        data.bChainEnabled_ = asn.chaine_activee;

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
        {   
            T_LogisticAvailability& dataDispo = data.dispoRamassageAmbulances_[i];
            dataDispo.nEquipmentType_   = asn.disponibilites_ambulances_ramassage.elem[i].type_equipement;
            dataDispo.nNbrTotal_        = asn.disponibilites_ambulances_ramassage.elem[i].nbr_total;
            dataDispo.nNbrAvailable_    = asn.disponibilites_ambulances_ramassage.elem[i].nbr_disponibles;
            dataDispo.nNbrWorking_      = asn.disponibilites_ambulances_ramassage.elem[i].nbr_au_travail;
            dataDispo.nNbrLent_         = asn.disponibilites_ambulances_ramassage.elem[i].nbr_pretes;
            dataDispo.nNbrResting_      = std::numeric_limits< uint >::max();
            if( asn.disponibilites_ambulances_ramassage.elem[i].m.nbr_au_reposPresent )
                dataDispo.nNbrResting_ = asn.disponibilites_ambulances_ramassage.elem[i].nbr_au_repos;
        }
    }
    if( asn.m.disponibilites_ambulances_relevePresent )
    {
        data.dispoReleveAmbulances_.resize( asn.disponibilites_ambulances_releve.n );
        for( uint i = 0; i < asn.disponibilites_ambulances_releve.n; ++i )
        {   
            T_LogisticAvailability& dataDispo = data.dispoReleveAmbulances_[i];
            dataDispo.nEquipmentType_   = asn.disponibilites_ambulances_releve.elem[i].type_equipement;
            dataDispo.nNbrTotal_        = asn.disponibilites_ambulances_releve.elem[i].nbr_total;
            dataDispo.nNbrAvailable_    = asn.disponibilites_ambulances_releve.elem[i].nbr_disponibles;
            dataDispo.nNbrWorking_      = asn.disponibilites_ambulances_releve.elem[i].nbr_au_travail;
            dataDispo.nNbrLent_         = asn.disponibilites_ambulances_releve.elem[i].nbr_pretes;
            dataDispo.nNbrResting_      = std::numeric_limits< uint >::max();
            if( asn.disponibilites_ambulances_releve.elem[i].m.nbr_au_reposPresent )
                dataDispo.nNbrResting_ = asn.disponibilites_ambulances_releve.elem[i].nbr_au_repos;
        }
    }
    if( asn.m.disponibilites_medecinsPresent )
    {
        data.dispoDoctors_.resize( asn.disponibilites_medecins.n );
        for( uint i = 0; i < asn.disponibilites_medecins.n; ++i )
        {   
            T_LogisticAvailability& dataDispo = data.dispoDoctors_[i];
            dataDispo.nEquipmentType_   = asn.disponibilites_medecins.elem[i].type_equipement;
            dataDispo.nNbrTotal_        = asn.disponibilites_medecins.elem[i].nbr_total;
            dataDispo.nNbrAvailable_    = asn.disponibilites_medecins.elem[i].nbr_disponibles;
            dataDispo.nNbrWorking_      = asn.disponibilites_medecins.elem[i].nbr_au_travail;
            dataDispo.nNbrLent_         = asn.disponibilites_medecins.elem[i].nbr_pretes;
            dataDispo.nNbrResting_      = std::numeric_limits< uint >::max();
            if( asn.disponibilites_medecins.elem[i].m.nbr_au_reposPresent )
                dataDispo.nNbrResting_ = asn.disponibilites_medecins.elem[i].nbr_au_repos;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: Agent::OnReceiveMsgLogSupplyEtat
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void Agent::OnReceiveMsgLogSupplyEtat( const ASN1T_MsgLogRavitaillementEtat& asn )
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
        {   
            T_LogisticAvailability& dataDispo = data.dispoTransporters_[i];
            dataDispo.nEquipmentType_   = asn.disponibilites_transporteurs_convois.elem[i].type_equipement;
            dataDispo.nNbrTotal_        = asn.disponibilites_transporteurs_convois.elem[i].nbr_total;
            dataDispo.nNbrAvailable_    = asn.disponibilites_transporteurs_convois.elem[i].nbr_disponibles;
            dataDispo.nNbrWorking_      = asn.disponibilites_transporteurs_convois.elem[i].nbr_au_travail;
            dataDispo.nNbrLent_         = asn.disponibilites_transporteurs_convois.elem[i].nbr_pretes;
            dataDispo.nNbrResting_      = std::numeric_limits< uint >::max();
            if( asn.disponibilites_transporteurs_convois.elem[i].m.nbr_au_reposPresent )
                dataDispo.nNbrResting_ = asn.disponibilites_transporteurs_convois.elem[i].nbr_au_repos;
        }
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
// Name: Agent::OnReceiveMsgLogSupplyQuotas
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void Agent::OnReceiveMsgLogSupplyQuotas( const ASN1T_MsgLogRavitaillementQuotas& asn )
{
    if( !pSupplyData_ && asn.quotas.n == 0 )
        return;

    if( ! pSupplyData_ )
        pSupplyData_ = new T_SupplyData();

    T_SupplyData& data = *pSupplyData_;

    data.quotas_.resize( asn.quotas.n );
    for( uint i = 0; i < asn.quotas.n; ++i )
        data.quotas_[ i ] = std::make_pair( asn.quotas.elem[i].ressource_id, asn.quotas.elem[i].quota_disponible );
}


//-----------------------------------------------------------------------------
// Name: Agent::UpdatePathFind
/** Updates the pathfind vector to reflect the agent's progress along its path.
*/
// Created: FBD 03-01-29
//-----------------------------------------------------------------------------
void Agent::UpdatePathFind()
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
// Name: Agent::ClearOldPath
// Created: FBD 03-01-29
//-----------------------------------------------------------------------------
void Agent::ClearOldPath()
{
    oldPathVector_.clear();
}







//-----------------------------------------------------------------------------
// Name: Agent::OnReceiveMsgPathFind
// Created: NLD 2003-01-28
//-----------------------------------------------------------------------------
void Agent::OnReceiveMsgPathFind( const ASN1T_MsgUnitPathFind& asnMsg )
{
    pathFindVector_.clear();
    for( uint i = 0; i < asnMsg.itineraire.vecteur_point.n; ++i )
    {
        MT_Vector2D vPos;
        App::GetApp().GetWorld().MosToSimMgrsCoord( (const char*)asnMsg.itineraire.vecteur_point.elem[i].data, vPos );
        pathFindVector_.push_back( vPos );
    }
}


//-----------------------------------------------------------------------------
// Name: Agent::OnReceiveMsgVisionCones
// Created: NLD 2003-02-12
//-----------------------------------------------------------------------------
void Agent::OnReceiveMsgVisionCones( DIN_Input& msg )
{
    for( CIT_SurfaceVector itSurface = surfaces_.begin(); itSurface != surfaces_.end(); ++itSurface )
        delete *itSurface;
    surfaces_.clear();

    uint32 nNbrSurfaces;
    msg >> nNbrSurfaces;
    for( uint i = 0; i < nNbrSurfaces; ++i )
        surfaces_.push_back( new Surface( msg ) );

    msg >> rElongationFactor_;

    bVisionSurfacesNeedUpdating_ = true;
}

// -----------------------------------------------------------------------------
// Name: Agent::GetElongationFactor
// Created: AGE 2005-03-18
// -----------------------------------------------------------------------------
MT_Float Agent::GetElongationFactor() const
{
    return rElongationFactor_;
}

// -----------------------------------------------------------------------------
// Name: Agent::GetPopulationCollisionDensity
// Created: NLD 2005-10-28
// -----------------------------------------------------------------------------
MT_Float Agent::GetPopulationCollisionDensity() const
{
    MT_Float rDensity = 0.;
    for( CIT_PopulationCollisionMap itCol = populationCollisions_.begin(); itCol != populationCollisions_.end(); ++itCol )
    {
        const sPopulationCollision& data = itCol->second;

        for( CIT_ConcentrationSet it = data.concentrations_.begin(); it != data.concentrations_.end(); ++it )
            rDensity = std::max( rDensity, (**it).GetDensity() );

        for( CIT_FlowSet it = data.flows_.begin(); it != data.flows_.end(); ++it )
            rDensity = std::max( rDensity, (**it).GetDensity() );
    }
    return rDensity;
}

// -----------------------------------------------------------------------------
// Name: Agent::OnSuperiorChanged
// Created: NLD 2003-10-09
// -----------------------------------------------------------------------------
void Agent::OnSuperiorChanged( Agent& newSuperior )
{
    assert( pParent_ );
    IT_AgentVector it = std::find( pParent_->children_.begin(), pParent_->children_.end(), this );
    assert( it != pParent_->children_.end() );
    pParent_->children_.erase( it );
    pParent_ = &newSuperior;
    newSuperior.AddChild( *this );
}


// -----------------------------------------------------------------------------
// Name: Agent::OnSuperiorChanged
// Created: APE 2004-10-29
// -----------------------------------------------------------------------------
void Agent::OnSuperiorChanged( Gtia& superior )
{
    assert( pParent_ == 0 );
    pGtia_ = &superior;

    for( IT_AgentVector it = children_.begin(); it != children_.end(); ++it )
        (*it)->SetGtia( *pGtia_ );
}


// -----------------------------------------------------------------------------
// Name: Agent::ReadODB
// Created: APE 2004-08-30
// -----------------------------------------------------------------------------
void Agent::ReadODB( InputArchive& archive, bool bAutomata )
{

    if( bAutomata )
        archive.Section( "Automate" );
    else
        archive.Section( "Pion" );

    archive.ReadAttribute( "type", strCategory_ );

    if( bAutomata )
    {
        pTypeAutomate_ = App::GetApp().GetAgentManager().FindTypeAutomate( strCategory_ );
        if( pTypeAutomate_ )
            pTypePion_ = & pTypeAutomate_->GetTypePC();
    }
    else
        pTypePion_ = App::GetApp().GetAgentManager().FindTypePion( strCategory_ );
    if( pTypePion_ )
    {
        symbolName_      = pTypePion_->GetSymbol();
        levelSymbolName_ = pTypePion_->GetLevelSymbol();
    };

    archive.ReadAttribute( "id", nAgentID_ );
    if( nAgentID_ > nMaxId_ )
        nMaxId_ = nAgentID_;

    if( ! archive.ReadField( "Nom", sName_, InputArchive::eNothing ) )
        sName_ = strCategory_;

    archive.ReadField( "Embraye", bEmbraye_, InputArchive::eNothing );

    std::string strPos;
    archive.ReadField( "Position", strPos );
    App::GetApp().GetWorld().MosToSimMgrsCoord( strPos, vPos_ );

    bAutomate_ = bAutomata;

    archive.Section( "LiensHierarchiques" );
    if( bAutomate_ )
    {
        uint nGtia;
        archive.ReadField( "GroupeConnaissance", nGtia );
        pGtia_ = App::GetApp().GetAgentManager().FindGtia( nGtia );
        assert( pGtia_ != 0 );
        if( archive.Section( "TC2", InputArchive::eNothing ) )
        {
			archive.ReadAttribute( "automate", nTC2_ );
            archive.EndSection();
        }
        if( archive.Section( "Logistique", InputArchive::eNothing ) )
        {
			if( archive.Section( "Maintenance", InputArchive::eNothing ) )
			{
				archive.ReadAttribute( "automate", nLogMaintenanceSuperior_ );
				archive.EndSection();
			}
			if( archive.Section( "Sante", InputArchive::eNothing ) )
			{
				archive.ReadAttribute( "automate", nLogMedicalSuperior_ );
				archive.EndSection();
			}
			if( archive.Section( "Ravitaillement", InputArchive::eNothing ) )
			{
				archive.ReadAttribute( "automate", nLogSupplySuperior_ );
                if( archive.BeginList( "Quotas", InputArchive::eNothing ) )
			    {
                    while( archive.NextListElement() )
                    {
                        archive.Section( "Dotation" );
                        archive.BeginList( "Categories" );
                        while( archive.NextListElement() )
                        {
                            archive.Section( "Categorie" );
                            std::string strName;
                            archive.ReadAttribute( "nom", strName );
                            uint quota;
                            archive.ReadAttribute( "quota", quota );
                            pSupplyData_->quotas_.push_back( std::pair< uint, uint >( App::GetApp().GetRessourceID( strName ), quota ) );
                            archive.EndSection(); //Categorie
                        }
                        archive.EndList(); //Categories
                        archive.EndSection(); //Dotation
                    }
                    archive.EndList(); //Quotas
                }
				archive.EndSection();
			}
            archive.EndSection();
        }
    }
    else
    {
        uint nParent;
        archive.ReadField( "Automate", nParent );
        pParent_ = App::GetApp().GetAgentManager().FindAgent( (MIL_AgentID)nParent );
        assert( pParent_ != 0 );
        pParent_->AddChild( *this );
        pGtia_ = & pParent_->GetGtia();
    }
    archive.EndSection();

    archive.EndSection();

    InitializeStocks();
}


// -----------------------------------------------------------------------------
// Name: Agent::WriteODB
// Created: APE 2004-08-30
// -----------------------------------------------------------------------------
void Agent::WriteODB( MT_XXmlOutputArchive& archive )
{
    if( bAutomate_ )
        archive.Section( "Automate" );
    else
        archive.Section( "Pion" );

    archive.WriteAttribute( "type", strCategory_ );
    archive.WriteAttribute( "id", nAgentID_ );
    archive.WriteField( "Nom", sName_ );

    std::string strPos;
    App::GetApp().GetWorld().SimToMosMgrsCoord( vPos_, strPos );
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
                    if ( pSupplyData_->quotas_.size() > 0 )
                    {
                        for( Agent::CIT_LogisticStockAvailabilities itQuota = pSupplyData_->quotas_.begin(); itQuota != pSupplyData_->quotas_.end(); ++itQuota )
                        {
                            const Resource& resource = App::GetApp().GetResource( itQuota->first );
                            archive.Section( "Dotation" );
                            archive.WriteAttribute( "nom", resource.GetDotationName() );
                            archive.Section( "Categories" );
                            archive.Section( "Categorie" );
                            archive.WriteAttribute( "nom", resource.GetName() );
                            archive.WriteAttribute( "quota", itQuota->second );
                            archive.EndSection();
                            archive.EndSection();
                            archive.EndSection();
                        }
                    }
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
        archive.WriteField( "Automate", pParent_->GetID() );
        archive.EndSection();
    }
    
    archive.EndSection();
}


// -----------------------------------------------------------------------------
// Name: Agent::GetModel
// Created: AGN 2003-12-22
// -----------------------------------------------------------------------------
const AgentModel* Agent::GetModelPion() const
{
    assert( pTypePion_ );
    return &pTypePion_->GetModel();
}

// -----------------------------------------------------------------------------
// Name: Agent::GetModel
// Created: AGN 2003-12-22
// -----------------------------------------------------------------------------
const AgentModel* Agent::GetModelAutomate() const
{
    if( !pTypeAutomate_ )
        return 0;
    return &pTypeAutomate_->GetModel();
}

// -----------------------------------------------------------------------------
// Name: Agent::AddConsign
// Created: AGE 2005-03-31
// -----------------------------------------------------------------------------
void Agent::AddConsign( LogSupplyConsign& consign )
{
    requestedSupplies_.insert( &consign );
}

// -----------------------------------------------------------------------------
// Name: Agent::RemoveConsign
// Created: AGE 2005-03-31
// -----------------------------------------------------------------------------
void Agent::RemoveConsign( LogSupplyConsign& consign )
{
    requestedSupplies_.erase( &consign );
}

// -----------------------------------------------------------------------------
// Name: Agent::AddConsign
// Created: AGE 2005-03-31
// -----------------------------------------------------------------------------
void Agent::AddConsign( LogMaintenanceConsign& consign )
{
    requestedMaintenances_.insert( &consign );
}

// -----------------------------------------------------------------------------
// Name: Agent::RemoveConsign
// Created: AGE 2005-03-31
// -----------------------------------------------------------------------------
void Agent::RemoveConsign( LogMaintenanceConsign& consign )
{
    requestedMaintenances_.erase( &consign );
}

// -----------------------------------------------------------------------------
// Name: Agent::AddConsign
// Created: AGE 2005-03-31
// -----------------------------------------------------------------------------
void Agent::AddConsign( LogMedicalConsign& consign )
{
    requestedMedical_.insert( &consign );
}

// -----------------------------------------------------------------------------
// Name: Agent::RemoveConsign
// Created: AGE 2005-03-31
// -----------------------------------------------------------------------------
void Agent::RemoveConsign( LogMedicalConsign& consign )
{
    requestedMedical_.erase( &consign );
}

// -----------------------------------------------------------------------------
// Name: Agent::HandleConsign
// Created: AGE 2005-03-31
// -----------------------------------------------------------------------------
void Agent::HandleConsign( LogSupplyConsign& consign )
{
    handledSupplies_.insert( &consign );
}

// -----------------------------------------------------------------------------
// Name: Agent::TerminateConsign
// Created: AGE 2005-03-31
// -----------------------------------------------------------------------------
void Agent::TerminateConsign( LogSupplyConsign& consign )
{
    handledSupplies_.erase( &consign );
}

// -----------------------------------------------------------------------------
// Name: Agent::HandleConsign
// Created: AGE 2005-03-31
// -----------------------------------------------------------------------------
void Agent::HandleConsign( LogMaintenanceConsign& consign )
{
    handledMaintenances_.insert( &consign );
}

// -----------------------------------------------------------------------------
// Name: Agent::TerminateConsign
// Created: AGE 2005-03-31
// -----------------------------------------------------------------------------
void Agent::TerminateConsign( LogMaintenanceConsign& consign )
{
    handledMaintenances_.erase( &consign );
}

// -----------------------------------------------------------------------------
// Name: Agent::HandleConsign
// Created: AGE 2005-03-31
// -----------------------------------------------------------------------------
void Agent::HandleConsign( LogMedicalConsign& consign )
{
    handledMedical_.insert( &consign );
}

// -----------------------------------------------------------------------------
// Name: Agent::TerminateConsign
// Created: AGE 2005-03-31
// -----------------------------------------------------------------------------
void Agent::TerminateConsign( LogMedicalConsign& consign )
{
    handledMedical_.erase( &consign );
}

// -----------------------------------------------------------------------------
// Name: Agent::InitializeStocks
// Created: HME 2005-11-03
// -----------------------------------------------------------------------------
void Agent::InitializeStocks()
{
    if ( pTypePion_ == 0 )
        return;
    if ( pSupplyData_ )
        pSupplyData_->stocks_ = pTypePion_->GetStocks();
    else
    {
        pSupplyData_ = new T_SupplyData();
        pSupplyData_->stocks_ = pTypePion_->GetStocks();
    }
}

// -----------------------------------------------------------------------------
// Name: Agent::Aggregate
// Created: HME 2005-11-30
// -----------------------------------------------------------------------------
void  Agent::SetAggregation( bool bAgg )
{
    bAggregated_ = bAgg;
}

// -----------------------------------------------------------------------------
// Name: Agent::GetAggregatedPos
// Created: HME 2006-01-20
// -----------------------------------------------------------------------------
const MT_Vector2D Agent::GetAggregatedPos() const
{
    if( bAutomate_ && bAggregated_ && children_.size() > 0 )
    {
        MT_Vector2D bary = vPos_;
        for( CIT_AgentVector it = children_.begin(); it != children_.end(); ++it )
            bary = bary + (*it)->GetPos();
        bary = bary / (children_.size() + 1);
        return bary;
    }
    else if( (! bAutomate_) && pParent_ && pParent_->IsEmbraye() && pParent_->IsAggregated() )
        return pParent_->GetAggregatedPos();
    else
        return vPos_;
}

