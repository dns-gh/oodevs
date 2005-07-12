//*****************************************************************************
// 
// $Created: NLD 2002-07-15 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_Agent.cpp $
// $Author: Nld $
// $Modtime: 27/04/05 17:28 $
// $Revision: 150 $
// $Workfile: MOS_Agent.cpp $
// 
//*****************************************************************************

#include "MOS_Light_Pch.h"

#include "MOS_Agent.h"

#ifndef MOS_USE_INLINE
#   include "MOS_Agent.inl"
#endif

#include "MOS_App.h"
#include "MOS_MainWindow.h"
#include "MOS_AgentCheckListItem_ABC.h"
#include "MOS_MOSServer.h"
#include "MOS_AgentManager.h"
#include "MOS_World.h"
#include "MOS_AgentEditor.h"
#include "MOS_WorldWidget.h"
#include "MOS_RC.h"
#include "MOS_Trace.h"
#include "MOS_Net_Def.h"
#include "MOS_ASN_Messages.h"
#include "MOS_History.h"
#include "MOS_Surface.h"
#include "MOS_Gtia.h"
#include "MOS_TypePion.h"
#include "MOS_TypeAutomate.h"


#include "MOS_AgentListView.h"
#include "MOS_ArmeListViewItem.h"
#include "MOS_PionListViewItem.h"
#include "MOS_AutomateListViewItem.h"

#include "MOS_AttrEditor.h"

#include "MOS_AgentKnowledge.h"
#include "MOS_ObjectKnowledge.h"
#include "MOS_TestManager.h"
#include "MOS_AgentTest.h"
#include "MOS_PionCheckListItem.h"
#include "MOS_AgentModel.h"

#include "MOS_DynaObjectManager.h"
#include "MOS_DynaObject.h"

#include "MOS_Team.h"

#include "MOS_Experience.h"
#include "MOS_Tiredness.h"
#include "MOS_Morale.h"

#include <QWidgetStack.h>
using namespace DIN;

// -----------------------------------------------------------------------------
// Name: MOS_Agent constructor
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
MOS_Agent::MOS_Agent( const ASN1T_MsgAutomateCreation& asnMsg )
    : nAgentID_                              ( asnMsg.oid_automate )
    , bAutomate_( true )
    , pParent_                               ( 0 )
    , nArme_                                 ( eTypeArmeUnknown )
    , rSpeed_                                ( 0. )
    , pAttrEditor_                           ( 0 )
    , rScrollPathFind_                       ( 0. )
    , pAgentPoint_                           ( 0 )
    , bMagicMove_                            ( false )
    , bNeedRedrawVision_                     ( true )    
    , nSpeed_                                ( 0 )
    , nNbOfficiers_( 0 )
    , nNbOfficiersKilled_( 0 )
    , nNbOfficiersInjured_( 0 )
    , nNbOfficiersMentalInjured_( 0 )
    , nNbOfficiersInMedicalChain_( 0 )
    , nNbOfficiersInMaintenanceChain_( 0 )
    , nNbOfficiersNBC_( 0 )
    , nNbSousOfficiers_( 0 )
    , nNbSousOfficiersKilled_( 0 )
    , nNbSousOfficiersInjured_( 0 )
    , nNbSousOfficiersMentalInjured_( 0 )
    , nNbSousOfficiersInMedicalChain_( 0 )
    , nNbSousOfficiersInMaintenanceChain_( 0 )
    , nNbSousOfficiersNBC_( 0 )
    , nNbMdrs_( 0 )
    , nNbMdrsKilled_( 0 )
    , nNbMdrsInjured_( 0 )
    , nNbMdrsMentalInjured_( 0 )
    , nNbMdrsInMedicalChain_( 0 )
    , nNbMdrsInMaintenanceChain_( 0 )
    , nNbMdrsNBC_( 0 )
    , nFightRateState_                       ( (E_ForceRatioState)-1         )
    , nRulesOfEngagementState_               ( (E_RulesOfEngagementState)-1 )
    , nCloseCombatState_                     ( (E_CloseCombatState)-1       )
    , pTypePion_                            ( 0 )
    , pTypeAutomate_                        ( 0 )
    , bLoadingState_                         ( true )
    , bStealthModeEnabled_                   ( false )
    , pGtia_                                 ( 0 )
    , bNbcProtectionSuitWorn_                ( false )
    , nContaminationState_                   ( 0 )
    , symbolName_                            ( "?" )
    , bEmbraye_                              ( false )
    , bJammed_                               ( false )
    , bBlackOut_                             ( false )
    , nPionRenforce_                         ( 0 )
    , pionTransportes_                       ()
    , nTransporteur_                         ( 0 )
    , pExperience_                           ( &MOS_Experience::veteran_ )
    , pTiredness_                            ( &MOS_Tiredness::normal_   )
    , pMorale_                               ( &MOS_Morale::bon_         )
    , bMaintenancePionLog_                   ( false )
    , bMaintenanceChainEnabled_              ( false )
    , nMaintenanceTempsBordee_               ( 8 )
    , maintenanceTacticalPriorities_         ()
    , maintenancePriorities_                 ( )
    , maintenanceDispoHaulers_()
    , maintenanceDispoRepairers_()
    , medicalConsigns_()
    , medicalConsignsHandled_()
    , bMedicalPionLog_()
    , bMedicalChainEnabled_()
    , nMedicalTempsBordee_()
    , medicalDispoReleveAmbulances_()
    , medicalDispoRamassageAmbulances_()
    , medicalDispoDoctors_()
    , medicalPriorities_()
    , medicalTacticalPriorities_         () 
    , supplyConsigns_()       
    , supplyConsignsHandled_()
    , bSupplyPionLog_( false )
    , bSupplyChainEnabled_()
    , supplyDispoTransporters_()
    , supplyDispoCommanders_()  
    , stocks_()
    , bRadarEnabled_( false )
    , bSurrendered_( false )
    , bPrisoner_( false )
    , bRefugeesManaged_( false )
    , nTC2ID_( 0 ) 
    , nLogMaintenanceSuperior_( 0 ) 
    , nLogMedicalSuperior_( 0 ) 
    , nLogSupplySuperior_( 0 ) 
{
    MOS_App::GetApp().GetAgentManager().RegisterListViewsForAgent( *this );
    MOS_App::GetApp().GetAgentManager().RegisterAgent( *this );

    sName_ = asnMsg.nom;

    if( asnMsg.m.oid_tc2Present )
        nTC2ID_ = asnMsg.oid_tc2;
    if( asnMsg.m.oid_maintenancePresent )
        nLogMaintenanceSuperior_ = asnMsg.oid_maintenance;
    if( asnMsg.m.oid_santePresent )
        nLogMedicalSuperior_ = asnMsg.oid_sante;
    if( asnMsg.m.oid_ravitaillementPresent )
        nLogSupplySuperior_ = asnMsg.oid_ravitaillement;

    pTypeAutomate_ = MOS_App::GetApp().GetAgentManager().FindTypeAutomate( asnMsg.type_automate );
    assert( pTypeAutomate_ );
    pTypePion_ = &pTypeAutomate_->GetTypePC();

    pGtia_ = MOS_App::GetApp().GetAgentManager().FindGtia( asnMsg.oid_groupe_connaissance );
    assert( pGtia_ );

    for( IT_AgentItemMap itItem = agentItemMap_.begin() ; itItem != agentItemMap_.end() ; ++itItem  )
        itItem->second = itItem->first->CreateAgent( *this );

    MOS_App::GetApp().GetTestManager().CreateAgentTest( *this );
    MOS_App::GetApp().GetMainWindow().GetHistory().AddAgent( *this );
}


// -----------------------------------------------------------------------------
// Name: MOS_Agent constructor
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
MOS_Agent::MOS_Agent( const ASN1T_MsgPionCreation& asnMsg )
    : nAgentID_                              ( asnMsg.oid_pion  )
    , bAutomate_                             ( false )
    , pParent_                               ( 0 )
    , nArme_                                 ( eTypeArmeUnknown )
    , rSpeed_                                ( 0. )
    , pAttrEditor_                           ( 0 )
    , rScrollPathFind_                       ( 0. )
    , pAgentPoint_                           ( 0 )
    , bMagicMove_                            ( false )
    , bNeedRedrawVision_                     ( true )    
    , nSpeed_                                ( 0 )
    , nNbOfficiers_( 0 )
    , nNbOfficiersKilled_( 0 )
    , nNbOfficiersInjured_( 0 )
    , nNbOfficiersMentalInjured_( 0 )
    , nNbOfficiersInMedicalChain_( 0 )
    , nNbOfficiersInMaintenanceChain_( 0 )
    , nNbOfficiersNBC_( 0 )
    , nNbSousOfficiers_( 0 )
    , nNbSousOfficiersKilled_( 0 )
    , nNbSousOfficiersInjured_( 0 )
    , nNbSousOfficiersMentalInjured_( 0 )
    , nNbSousOfficiersInMedicalChain_( 0 )
    , nNbSousOfficiersInMaintenanceChain_( 0 )
    , nNbSousOfficiersNBC_( 0 )
    , nNbMdrs_( 0 )
    , nNbMdrsKilled_( 0 )
    , nNbMdrsInjured_( 0 )
    , nNbMdrsMentalInjured_( 0 )
    , nNbMdrsInMedicalChain_( 0 )
    , nNbMdrsInMaintenanceChain_( 0 )
    , nNbMdrsNBC_( 0 )
    , nFightRateState_                       ( (E_ForceRatioState)-1         )
    , nRulesOfEngagementState_               ( (E_RulesOfEngagementState)-1 )
    , nCloseCombatState_                     ( (E_CloseCombatState)-1       )
    , pTypePion_                            ( 0 )
    , pTypeAutomate_                        ( 0 )
    , bLoadingState_                         ( true )
    , bStealthModeEnabled_                   ( false )
    , pGtia_                                 ( 0 )
    , bNbcProtectionSuitWorn_                ( false )
    , nContaminationState_                   ( 0 )
    , symbolName_                            ( "?" )
    , bEmbraye_                              ( false )
    , bJammed_                               ( false )
    , bBlackOut_                             ( false )
    , nPionRenforce_                         ( 0 )
    , pionTransportes_                       ()
    , nTransporteur_                         ( 0 )
    , pExperience_                           ( &MOS_Experience::veteran_ )
    , pTiredness_                            ( &MOS_Tiredness::normal_   )
    , pMorale_                               ( &MOS_Morale::bon_         )
    , bMaintenancePionLog_                   ( false )
    , bMaintenanceChainEnabled_              ( false )
    , nMaintenanceTempsBordee_               ( 8 )
    , maintenancePriorities_                 ( )
    , maintenanceDispoHaulers_()
    , maintenanceDispoRepairers_()
    , medicalConsigns_()
    , medicalConsignsHandled_()
    , bMedicalPionLog_()
    , bMedicalChainEnabled_()
    , nMedicalTempsBordee_()
    , medicalDispoReleveAmbulances_()
    , medicalDispoRamassageAmbulances_()
    , medicalDispoDoctors_()
    , medicalPriorities_()
    , supplyConsigns_()       
    , supplyConsignsHandled_()
    , bSupplyPionLog_( false )
    , bSupplyChainEnabled_()
    , supplyDispoTransporters_()
    , supplyDispoCommanders_()  
    , stocks_()
    , nTC2ID_( 0 ) 
    , nLogMaintenanceSuperior_( 0 ) 
    , nLogMedicalSuperior_( 0 ) 
    , nLogSupplySuperior_( 0 ) 
{
    MOS_App::GetApp().GetAgentManager().RegisterListViewsForAgent( *this );
    MOS_App::GetApp().GetAgentManager().RegisterAgent( *this );

    sName_ = asnMsg.nom;

    pTypeAutomate_ = 0;
    pTypePion_     = MOS_App::GetApp().GetAgentManager().FindTypePion( asnMsg.type_pion );
    assert( pTypePion_ );

    pParent_ = MOS_App::GetApp().GetAgentManager().FindAgent( asnMsg.oid_automate );
    assert( pParent_ );

    for( IT_AgentItemMap itItem = agentItemMap_.begin() ; itItem != agentItemMap_.end() ; ++itItem  )
        itItem->second = itItem->first->CreateAgent( *this );

    MOS_App::GetApp().GetTestManager().CreateAgentTest( *this );
    MOS_App::GetApp().GetMainWindow().GetHistory().AddAgent( *this );
}

//-----------------------------------------------------------------------------
// Name: MOS_Agent destructor
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
MOS_Agent::~MOS_Agent()
{
    MOS_App::GetApp().GetMainWindow().NotifyDeleteAgent( *this );
    MOS_App::GetApp().GetAgentManager().UnregisterAgent( *this );
    DeleteListView();

    pAttrEditor_ = 0;

    MOS_App::GetApp().GetTestManager().DeleteAgentTest( *this );

    MOS_App::GetApp().GetMainWindow().GetHistory().DelAgent( *this );
}

//-----------------------------------------------------------------------------
// Name: MOS_agent::DeleteListView
// Created: FBD 03-01-03
//-----------------------------------------------------------------------------
void MOS_Agent::DeleteListView()
{
    // Les pions seront détruits par l'automate
    if( IsAutomate() )
    {
        for( IT_AgentItemMap itItem = agentItemMap_.begin() ; itItem != agentItemMap_.end() ; ++itItem  )
        {
            if( itItem->first->GetType() == eListViewSelector )
            {
                MOS_AgentCheckListItem_ABC* pAgent = static_cast< MOS_AgentCheckListItem_ABC* >( itItem->second );
                if( pAgent )
                    itItem->first->DeleteAgent( *pAgent );
            }
            else
            {
                MOS_AgentListViewItem_ABC* pAgent = static_cast< MOS_AgentListViewItem_ABC* >( itItem->second );
                if( pAgent )
                    itItem->first->DeleteAgent( *pAgent );
            }
            itItem->second = 0;
        }
    }
    else
    {
        for( IT_AgentItemMap itItem = agentItemMap_.begin() ; itItem != agentItemMap_.end() ; ++itItem  )
        {
            if( itItem->first->GetType() == eListViewSelector )
            {
                MOS_AgentCheckListItem_ABC* pAgent = static_cast< MOS_AgentCheckListItem_ABC* >( itItem->second );
                if( pAgent )
                    itItem->first->UnregisterAgent( *pAgent );
            }
            else
            {
                MOS_AgentListViewItem_ABC* pAgent = static_cast< MOS_AgentListViewItem_ABC* >( itItem->second );
                if( pAgent )
                    itItem->first->UnregisterAgent( *pAgent );
            }
            itItem->second = 0;

        }
    }
}

//=============================================================================
// ATTRIBUTES
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MOS_Agent::OnReceiveMsgUnitInterVisibility
// Created: NLD 2003-03-17
//-----------------------------------------------------------------------------
void MOS_Agent::OnReceiveMsgUnitInterVisibility( DIN::DIN_Input& input )
{
    MIL_AgentID     nAgentID;
    uint8           nVisType;
    uint8           nMaxVisType;
    bool            bRecordMode;

    input >> nAgentID;
    input >> nVisType;
    input >> nMaxVisType;
    input >> bRecordMode;
    MOS_Agent* pAgent = MOS_App::GetApp().GetAgentManager().FindAgent( nAgentID );
    if( ! pAgent )
        return;

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
    MOS_DynaObject* pObject = MOS_App::GetApp().GetDynaObjectManager().FindDynaObject( nObjectID );
    if( !pObject )
        return;

    objectsPerceived_           .erase( pObject );

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
            MOS_App::GetApp().GetMainWindow().GetAgentEditor().NotifyAgentHasMoved( *this, vPos_ );
            if( pAttrEditor_ != 0 )
                pAttrEditor_->NotifyAgentHasMoved( *this, vPos_ );
        }
    }

    if( asnMsg.m.etat_opPresent )
    {
        nOpState_ = asnMsg.etat_op;
        if( pAttrEditor_ )
            pAttrEditor_->SetOpState( nOpState_ );
    }

    if( asnMsg.m.pions_renforcantPresent )
    {
        renforts_.clear();
        for( uint i = 0; i < asnMsg.pions_renforcant.n; ++i )
            renforts_.push_back( asnMsg.pions_renforcant.elem[i] );
        if( pAttrEditor_ )
            pAttrEditor_->SetRenforts( renforts_ );
    }

    if( asnMsg.m.pion_renforcePresent )
    {
        nPionRenforce_ = asnMsg.pion_renforce;
        if( pAttrEditor_ )
            pAttrEditor_->SetPionRenforce( nPionRenforce_ );
    }

    if( asnMsg.m.pions_transportesPresent )
    {
        pionTransportes_.clear();
        for( uint i = 0; i < asnMsg.pions_transportes.n; ++i )
            pionTransportes_.push_back( asnMsg.pions_transportes.elem[i] );
        if( pAttrEditor_ )
            pAttrEditor_->SetPionTransportes( pionTransportes_ );
    }

    if( asnMsg.m.moralPresent )
    {
        pMorale_ = MOS_Morale::Find( asnMsg.moral );
        assert( pMorale_ );
        if( pAttrEditor_ )
            pAttrEditor_->SetMorale( *pMorale_ );
    }

    if( asnMsg.m.experiencePresent )
    {
        bNeedRedrawVision_ = true;
        pExperience_ = MOS_Experience::Find( asnMsg.experience );
        assert( pExperience_ );
        if( pAttrEditor_ )
            pAttrEditor_->SetExperience( *pExperience_);
    }

    if( asnMsg.m.fatiguePresent )
    {
        bNeedRedrawVision_ = true;
        pTiredness_ = MOS_Tiredness::Find( asnMsg.fatigue );
        assert( pTiredness_ );
        if( pAttrEditor_ )
            pAttrEditor_->SetTiredness( *pTiredness_ );
    }

    if( asnMsg.m.pion_transporteurPresent )
    {
        nTransporteur_ = asnMsg.pion_transporteur;
        if( pAttrEditor_ )
            pAttrEditor_->SetTransporteur( nTransporteur_ );
    }

    if( asnMsg.m.posture_newPresent )
    {
        nCurrentPosture_ = (E_PostureType)asnMsg.posture_new;
        bNeedRedrawVision_ = true;
    }
    if( asnMsg.m.posture_oldPresent )
    {
        nOldPosture_ = (E_PostureType)asnMsg.posture_old;
        bNeedRedrawVision_ = true;
    }
    if( asnMsg.m.posture_pourcentagePresent )
    {
        nPostureCompletionPourcentage_ = asnMsg.posture_pourcentage;
        bNeedRedrawVision_ = true;
    }

    if( pAttrEditor_ != 0 )
        pAttrEditor_->SetPosture( nOldPosture_, nCurrentPosture_, nPostureCompletionPourcentage_ );

    if( asnMsg.m.etat_automatePresent )
    {
        bEmbraye_ = ( asnMsg.etat_automate == EnumAutomateState::embraye );
        if( pAttrEditor_ )
            pAttrEditor_->SetAutomateMode( bEmbraye_ );
    }

    if( asnMsg.m.etatPresent )
    {
        nState_ = (E_AgentState)asnMsg.etat;
        if( pAttrEditor_ )
            pAttrEditor_->SetAgentState( nState_ );
    }
    
    if( asnMsg.m.rapport_de_forcePresent )
    {
        nFightRateState_ = (E_ForceRatioState)asnMsg.rapport_de_force;
        if( pAttrEditor_ )
            pAttrEditor_->SetFightRateState( nFightRateState_ );
    }

    if( asnMsg.m.regles_d_engagementPresent )
    {
        nRulesOfEngagementState_ = (E_RulesOfEngagementState)asnMsg.regles_d_engagement;
        if( pAttrEditor_ )
            pAttrEditor_->SetRulesOfEngagementState( nRulesOfEngagementState_ );
    }

    if( asnMsg.m.combat_de_rencontrePresent )
    {
        nCloseCombatState_ = (E_CloseCombatState)asnMsg.combat_de_rencontre;
        if( pAttrEditor_ )
            pAttrEditor_->SetCloseCombatState( nCloseCombatState_ );
    }

    if( asnMsg.m.embarquePresent )
    {
        bLoadingState_ = asnMsg.embarque;
        if( pAttrEditor_ )
            pAttrEditor_->SetLoadingState( bLoadingState_ );
    }

    if( asnMsg.m.mode_furtif_actifPresent )
    {
        bStealthModeEnabled_ = asnMsg.mode_furtif_actif;
        if( pAttrEditor_ )
            pAttrEditor_->SetStealthModeEnabled( bStealthModeEnabled_ );
    }

    if( asnMsg.m.en_tenue_de_protection_nbcPresent )
    {
        bNbcProtectionSuitWorn_ = asnMsg.en_tenue_de_protection_nbc;
        if( pAttrEditor_ )
            pAttrEditor_->SetNbcProtectionSuitWorn( bNbcProtectionSuitWorn_ );
    }

    if( asnMsg.m.etat_contaminationPresent )
    {
        nContaminationState_ = asnMsg.etat_contamination;
        if( pAttrEditor_ ) 
            pAttrEditor_->SetContaminationState( nContaminationState_ );
    }

    if( asnMsg.m.contamine_par_agents_nbcPresent )
    {
        nbcAgentContaminating_.clear();
        for( uint i = 0; i < asnMsg.contamine_par_agents_nbc.n; ++i )
            nbcAgentContaminating_.push_back( asnMsg.contamine_par_agents_nbc.elem[i] );

        if( pAttrEditor_ )
            pAttrEditor_->SetNbcAgentsContaminating( nbcAgentContaminating_ );
    }

    if( asnMsg.m.vitessePresent )
    {
        nSpeed_ = asnMsg.vitesse;
        if( pAttrEditor_ != 0 )
            pAttrEditor_->SetSpeed( nSpeed_ );
    }   
    
    if( asnMsg.m.hauteurPresent )
    {
        nAltitude_ = asnMsg.hauteur;
        if( pAttrEditor_ != 0 )    
            pAttrEditor_->SetAltitude( nAltitude_ );
    }

    if( asnMsg.m.directionPresent )
    {
        nDirection_ = asnMsg.direction;
        if( pAttrEditor_ != 0 )    
            pAttrEditor_->SetDirection( nDirection_ );
    }

    if( asnMsg.m.communications_brouilleesPresent )
    {
        bJammed_ = asnMsg.communications_brouillees;
         if( pAttrEditor_ != 0 )    
            pAttrEditor_->SetJammedState( bJammed_ );
    }

    if( asnMsg.m.silence_radioPresent )
    {
        bBlackOut_ = asnMsg.silence_radio;
        if( pAttrEditor_ != 0 )    
            pAttrEditor_->SetBlackOutState( bBlackOut_ );
    }

    if( asnMsg.m.radar_actifPresent  )
    {
        bRadarEnabled_ = asnMsg.radar_actif;
        if( pAttrEditor_ != 0 )    
            pAttrEditor_->SetRadarEnabled( bRadarEnabled_ );
    }

    if( asnMsg.m.prisonnierPresent )
    {
        bPrisoner_ = asnMsg.prisonnier;
        if( pAttrEditor_ != 0 )    
            pAttrEditor_->SetPrisonerState( bPrisoner_ );
    }

    if( asnMsg.m.renduPresent )
    {
        bSurrendered_ = asnMsg.rendu;
        if( pAttrEditor_ != 0 )    
            pAttrEditor_->SetSurrenderedState( bSurrendered_ );
    }

    if( asnMsg.m.refugie_pris_en_comptePresent )
    {
        bRefugeesManaged_ = asnMsg.refugie_pris_en_compte;
        if( pAttrEditor_ != 0 )    
            pAttrEditor_->SetRefugeesManagedState( bRefugeesManaged_ );
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_Agent::OnAttributeUpdated
// Created: NLD 2003-03-06
//-----------------------------------------------------------------------------
void MOS_Agent::OnAttributeUpdated( const ASN1T_MsgUnitDotations& asnMsg )
{
    OnAttributeUpdated_Personnel          ( asnMsg );
    OnAttributeUpdated_Equipement         ( asnMsg );
    OnAttributeUpdated_Ressources         ( asnMsg );


    if( asnMsg.m.prets_equipementPresent )
    {
        lends_.clear();
        for( uint i = 0; i < asnMsg.prets_equipement.n; ++i )
        {
            const ASN1T_PretEquipement& pret = asnMsg.prets_equipement.elem[i];
            T_Lend lend;
            lend.pLent_             = MOS_App::GetApp().GetAgentManager().FindAgent( pret.oid_pion_emprunteur );
            lend.nComposanteTypeID_ = pret.type_equipement;
            lend.nNbr_              = pret.nombre;
            lends_.push_back( lend );
        }
        if( pAttrEditor_ != 0 )    
            pAttrEditor_->SetLends( lends_ );
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_Agent::OnAttributeUpdated_Personnel
// Created: AGN 03-06-04
//-----------------------------------------------------------------------------
void MOS_Agent::OnAttributeUpdated_Personnel( const ASN1T_MsgUnitDotations& asnMsg )
{
    if( asnMsg.m.dotation_eff_personnelPresent != 1 )
        return;

    uint nSize = asnMsg.dotation_eff_personnel.n;
    while( nSize > 0 )
    {
        ASN1T_DotationPersonnel& dot = asnMsg.dotation_eff_personnel.elem[ --nSize ];
        switch( dot.rang  )
        {
            case EnumHumanRank::officier:
            {
                nNbOfficiers_                   = dot.nb_total;
                nNbOfficiersKilled_             = dot.nb_morts;
                nNbOfficiersInjured_            = dot.nb_blesses;
                nNbOfficiersMentalInjured_      = dot.nb_blesses_mentaux;
                nNbOfficiersInMedicalChain_      = dot.nb_dans_chaine_sante;
                nNbOfficiersInMaintenanceChain_ = dot.nb_utilises_pour_maintenance;
                nNbOfficiersNBC_                = dot.nb_contamines_nbc;
                break;
            }

            case EnumHumanRank::sous_officer:
            {
                nNbSousOfficiers_                   = dot.nb_total;
                nNbSousOfficiersKilled_             = dot.nb_morts;
                nNbSousOfficiersInjured_            = dot.nb_blesses;
                nNbSousOfficiersMentalInjured_      = dot.nb_blesses_mentaux;
                nNbSousOfficiersInMedicalChain_      = dot.nb_dans_chaine_sante;
                nNbSousOfficiersInMaintenanceChain_ = dot.nb_utilises_pour_maintenance;
                nNbSousOfficiersNBC_                = dot.nb_contamines_nbc;
                break;
            }

            case EnumHumanRank::mdr:
            {
                nNbMdrs_                   = dot.nb_total;
                nNbMdrsKilled_             = dot.nb_morts;
                nNbMdrsInjured_            = dot.nb_blesses;
                nNbMdrsMentalInjured_      = dot.nb_blesses_mentaux;
                nNbMdrsInMedicalChain_      = dot.nb_dans_chaine_sante;
                nNbMdrsInMaintenanceChain_ = dot.nb_utilises_pour_maintenance;
                nNbMdrsNBC_                = dot.nb_contamines_nbc;
                break;
            }
            default:
                assert( false );
        }
    }

    if( pAttrEditor_ != 0 )
    {
        pAttrEditor_->ResetHumains();
        pAttrEditor_->AddHumains( "Total"             , nNbOfficiers_                  , nNbSousOfficiers_                  , nNbMdrs_ );
        pAttrEditor_->AddHumains( "Morts"             , nNbOfficiersKilled_            , nNbSousOfficiersKilled_            , nNbMdrsKilled_ );
        pAttrEditor_->AddHumains( "Blessés"           , nNbOfficiersInjured_           , nNbSousOfficiersInjured_           , nNbMdrsInjured_ );
        pAttrEditor_->AddHumains( "Cas psychiatriques", nNbOfficiersMentalInjured_     , nNbSousOfficiersMentalInjured_     , nNbMdrsMentalInjured_ );
        pAttrEditor_->AddHumains( "Contaminés"        , nNbOfficiersNBC_               , nNbSousOfficiersNBC_               , nNbMdrsNBC_ );
        pAttrEditor_->AddHumains( "Santé"             , nNbOfficiersInMedicalChain_     , nNbSousOfficiersInMedicalChain_     , nNbMdrsInMedicalChain_ );
        pAttrEditor_->AddHumains( "Maintenance"       , nNbOfficiersInMaintenanceChain_, nNbSousOfficiersInMaintenanceChain_, nNbMdrsInMaintenanceChain_ );
    }
}

//-----------------------------------------------------------------------------
// Name: MOS_Agent::OnAttributeUpdated_Equipement
// Created: AGN 03-06-04
//-----------------------------------------------------------------------------
void MOS_Agent::OnAttributeUpdated_Equipement( const ASN1T_MsgUnitDotations& asnMsg )
{
    if ( asnMsg.m.dotation_eff_materielPresent != 1 )
        return;

    uint nSize = asnMsg.dotation_eff_materiel.n;
    while ( nSize > 0 )
    {
        ASN1T_DotationEquipement& value = asnMsg.dotation_eff_materiel.elem[ --nSize ];
        MIL_AgentID equipId = value.type_equipement;
        equipement_[ equipId ].nNbrAvailable_       = value.nb_disponibles;
        equipement_[ equipId ].nNbrUnavailable_     = value.nb_indisponibles;
        equipement_[ equipId ].nNbrReparable_       = value.nb_reparables;
        equipement_[ equipId ].nNbrInMaintenance_   = value.nb_dans_chaine_maintenance;
        equipement_[ equipId ].nNbrPrisoner_        = value.nb_prisonniers;        
    }

    if ( pAttrEditor_ )
    {
        pAttrEditor_->ResetEquipments();
        for ( IT_EquipementQty_Map it = equipement_.begin(); it != equipement_.end(); ++it )
            pAttrEditor_->AddEquipment( it->first, it->second );        
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_Agent::OnAttributeUpdated_Ressources
// Created: AGN 03-06-04
//-----------------------------------------------------------------------------
void MOS_Agent::OnAttributeUpdated_Ressources( const ASN1T_MsgUnitDotations& asnMsg )
{
    if( asnMsg.m.dotation_eff_ressourcePresent != 1 )
        return;

    uint nSize = asnMsg.dotation_eff_ressource.n;
    while( nSize > 0 )
    {
        ASN1T_DotationRessource& value = asnMsg.dotation_eff_ressource.elem[ --nSize ];
        MIL_AgentID equipId = value.ressource_id;
        uint qty = value.quantite_disponible;
        ressources_[ equipId ] = qty;
    }
    
    if( pAttrEditor_ != 0 )
    {
        pAttrEditor_->ResetRessources();
        for( IT_RessourceQty_Map it = ressources_.begin(); it != ressources_.end(); ++it )
            pAttrEditor_->AddRessource( it->first, it->second );        
    }
}


//=============================================================================
// DRAW
//=============================================================================

//----------------------------------------------------------------- ------------
// Name: MOS_Agent::Draw
// Created: NLD 2002-11-28
//-----------------------------------------------------------------------------
void MOS_Agent::Draw()
{
    MOS_AgentListView& listView = MOS_App::GetApp().GetMainWindow().GetAgentEditor().GetAgentListView();

/*    if( bDrawKnowledge_ )
    {
        for( IT_KnowledgeMap it = knowledgeMap_.begin(); it != knowledgeMap_.end(); ++it )
        {
            if( listView.GetSelectedAgent()->GetAgentID() != it->second->GetRealId() )
                it->second->Draw( color_ );
        }
    }*/

    const MOS_Team& team = GetTeam();
    if( ! MOS_App::GetApp().GetMainWindow().GetAgentEditor().MustBeDrawn( team ) && listView.GetSelectedAgent() != this )
        return;

    static MT_Float rCrossSize = 100;

    if( listView.GetSelectedAgent() == this )
    {
        for( CIT_PointVector itPoint = debugPointsToDraw_.begin(); itPoint != debugPointsToDraw_.end(); ++itPoint )
            GFX_Tools::CreateGLCross( *itPoint, rCrossSize, 4.0, GFX_Color( 255, 0, 255 ) );
    }

    UpdatePathFind();

    if( ( listView.IsDrawItineraire() && listView.GetSelectedAgent() == this ) || listView.IsDrawAllItineraire() )
    {
        if( listView.IsDrawOldPath() )
            DrawOldPathFind();
        DrawPathFind();
    }


    MT_Float rSize = 600.;
    GFX_Color color = team.GetColor();
    if( listView.GetSelectedAgent() == this )
        color.AddRGB( 80, 200, 80 );  // Gives a brighter color to the selected unit

    if( pParent_ != 0 && listView.GetSelectedAgent() == pParent_ )
        color.AddRGB( 60, 130, 60 );  // Gives a brighter color to the selected unit

    GFX_Tools::CreateGLAgentShadow( vPos_, rSize, 4., 8., color , true, symbolName_, nOpState_ );
    
    GFX_Tools::CreateGLCross( vPos_, rCrossSize, 4.0, color );

    if( !nbcAgentContaminating_.empty() )
        GFX_Tools::CreateGLCross ( vPos_, rCrossSize, 6.0, GFX_Color( 0, 255, 0) );

    if( bNbcProtectionSuitWorn_ )
        GFX_Tools::CreateGLCircle( vPos_, rCrossSize, GFX_Color( 255, 255, 255  ) );
}

//-----------------------------------------------------------------------------
// Name: MOS_Agent::UpdatePathFind
// Created: FBD 03-01-29
//-----------------------------------------------------------------------------
void MOS_Agent::UpdatePathFind()
{
    MT_Float rMinDistance = 999999999.0;
    MT_Vector2D* pAgentPoint = 0;

    if( pathFindVector_.size() >= 2 )
    {
        MT_Vector2D* pLastPoint = 0;
        for( IT_PointVector itPoint = pathFindVector_.begin() ; itPoint != pathFindVector_.end() ; ++itPoint )
        {
            MT_Vector2D* pCurPoint = &*itPoint;
            if( pLastPoint )
            {
                MT_Line curLine( *pLastPoint, *pCurPoint );
                MT_Vector2D vNewPos = curLine.ClosestPointOnLine( vPos_ );
                MT_Float rDistance = vPos_.Distance( vNewPos );
                if( rDistance < rMinDistance )
                {
                    pAgentPoint = pLastPoint;
                    rMinDistance = rDistance;
                }
            }
            pLastPoint = pCurPoint;
        }
    }

    if( pAgentPoint )
    {
        pAgentPoint_ = pAgentPoint;
    }

    if( pAgentPoint_ && oldPathFindVector_.size() )
    {
        MT_Vector2D* pFound = 0;

        T_PointVector& oldPathFindVector = *oldPathFindVector_.rbegin();
        for( IT_PointVector itOldPoint = oldPathFindVector.begin() ; itOldPoint != oldPathFindVector.end() ; ++itOldPoint )
        {
            if( *itOldPoint == *pAgentPoint_ )
                pFound = &*itOldPoint;
        }
        if( pFound == 0 )
            oldPathFindVector.push_back( *pAgentPoint_ );
    }
}
//-----------------------------------------------------------------------------
// Name: MOS_Agent::ClearPathFind
// Created: FBD 03-01-29
//-----------------------------------------------------------------------------
void MOS_Agent::ClearPathFind()
{
    oldPathFindVector_.clear();
    oldPathFindVector_.push_back( T_PointVector() );
}


//-----------------------------------------------------------------------------
// Name: MOS_Agent::NotifyMagicalMove
// Created: FBD 03-01-29
//-----------------------------------------------------------------------------
void MOS_Agent::NotifyMagicalMove()
{
    bMagicMove_ = true;
    SaveOldPathFind();
    pathFindVector_.clear();
    pAgentPoint_ = 0;
}

//-----------------------------------------------------------------------------
// Name: MOS_Agent::DrawOldPathFind
// Created: FBD 03-01-29
//-----------------------------------------------------------------------------
void MOS_Agent::DrawOldPathFind()
{
    for( RIT_PathFindVector itPathFind = oldPathFindVector_.rbegin() ; itPathFind != oldPathFindVector_.rend() ; ++itPathFind )
    {
        T_PointVector& oldPathFindVector = *itPathFind;
        if( oldPathFindVector.size() >= 1 )
        {
            GFX_Color colorPathFind( 255., 255., 0., 1.0 );
            MT_Vector2D* pLastPoint = 0;

            for( IT_PointVector itPoint = oldPathFindVector.begin() ; itPoint != oldPathFindVector.end() ; ++itPoint )
            {
                MT_Vector2D* pCurPoint = &*itPoint;
                if( pLastPoint )
                {
                    GFX_Tools::CreateGLLine( *pLastPoint, *pCurPoint, 5., GFX_Color( 0., 0., 0., 0.7 ) );
                    GFX_Tools::CreateGLLine( *pLastPoint, *pCurPoint, 2., colorPathFind );
                }
                pLastPoint = pCurPoint;
            }

            if( itPathFind == oldPathFindVector_.rbegin() )
            {
                GFX_Tools::CreateGLLine( *pLastPoint, vPos_, 5., GFX_Color( 0., 0., 0., 0.7 ) );
                GFX_Tools::CreateGLLine( *pLastPoint, vPos_, 2., colorPathFind );
            }
        }
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_Agent::DrawPathFind
// Created: FBD 03-01-29
//-----------------------------------------------------------------------------
void MOS_Agent::DrawPathFind()
{
    MT_Float rLenPosAgent = 0;
    if( pathFindVector_.size() >= 2 )
    {
        int nDraw = 0;
        GFX_Color colorPathFind( 255., 255., 255., 1.0 );

        MT_Float rSegmentSize = 400.0;
        rScrollPathFind_ += 80; // $$$$ AGE 2005-02-08: make it time dependent
        if( rScrollPathFind_ >= rSegmentSize * 2.0 )
        {
            rScrollPathFind_ -= rSegmentSize * 2.0;
        }

        MT_Float rPosStartDraw = rScrollPathFind_;
        MT_Float rPosEndDraw = rPosStartDraw + rSegmentSize;
        MT_Float rCurPos = 0.0;
        IT_PointVector itPoint = pathFindVector_.begin();
        MT_Vector2D* pLastPoint = &*itPoint;
        ++itPoint;
        while (  itPoint != pathFindVector_.end() )
        {
            MT_Vector2D* pCurPoint = &*itPoint;
            if( pLastPoint == pAgentPoint_ && nDraw == 0)
            {
                nDraw = 1;
                rLenPosAgent =  vPos_.Distance( *pLastPoint ) + rCurPos;
            }

            MT_Float rSize =  (*pCurPoint).Distance( *pLastPoint );
            MT_Vector2D vDir( MT_Vector2D( *pCurPoint - *pLastPoint ).Normalize() );
            MT_Vector2D vCurPos1 = *pLastPoint;
            MT_Vector2D vCurPos2 = *pLastPoint + ( vDir * rSize );

            // Si Fin Vecteur < à Début Draw
            if( rCurPos + rSize <= rPosStartDraw )
            {
                // on passe au vecteur suivant
                rCurPos += rSize;
                pLastPoint = pCurPoint;
                ++itPoint;
                if( nDraw == 1 )
                    nDraw = 2;

            }
            else
            // Si début vecteur > à début draw && fin vecteur < & fin Draw ( tout le vecteur est draw )
            if( rCurPos >= rPosStartDraw && rCurPos + rSize <= rPosEndDraw )
            {
                // On trace tous le vecteur pour draw
                if( nDraw == 2 )
                    GFX_Tools::CreateGLLine( vCurPos1, vCurPos2, 2., colorPathFind );
                else
                if( nDraw == 1 )
                {
                    MT_Float rSize2 = rLenPosAgent - rCurPos;
                    vCurPos1 = *pLastPoint + ( vDir * rSize2 );
                    GFX_Tools::CreateGLLine( vCurPos1, vCurPos2, 2., colorPathFind );
                    nDraw = 2;
                }

                // On passe au vecteur suivant
                ++itPoint;
                pLastPoint = pCurPoint;
                rCurPos += rSize;
                if( nDraw == 1 )
                    nDraw = 2;
            }
            else
            // Si début vecteur > à début draw && fin vecteur > à fin Draw
            if( rCurPos >= rPosStartDraw  && rCurPos + rSize >= rPosEndDraw )
            {
                // on calcul la fin du draw dans le vecteur
                MT_Float rSize2 = rPosEndDraw - rCurPos;
                vCurPos2 = *pLastPoint + ( vDir * rSize2 );

                // On trace la fin du draw
                // On trace tous le vecteur pour draw
                if( nDraw == 2 )
                    GFX_Tools::CreateGLLine( vCurPos1, vCurPos2, 2., colorPathFind );
                else
                if( nDraw == 1 && rLenPosAgent < rPosEndDraw )
                {
                    MT_Float rSize2 = rLenPosAgent - rCurPos;
                    vCurPos1 = *pLastPoint + ( vDir * rSize2 );
                    GFX_Tools::CreateGLLine( vCurPos1, vCurPos2, 2., colorPathFind );
                    nDraw = 2;
                }

                // On passe au draw suivant
                rPosStartDraw += rSegmentSize * 2.;
                rPosEndDraw = rPosStartDraw + rSegmentSize;
            }
            else
            // Si début vecteur < à début draw && fin vecteur < à fin Draw
            if( rCurPos <= rPosStartDraw  && rCurPos + rSize <= rPosEndDraw )
            {
                // On calcul le début du draw dans le vecteur
                MT_Float rSize2 = rPosStartDraw - rCurPos;
                vCurPos1 = *pLastPoint + ( vDir * rSize2 );

                // On trace le début du draw
                if( nDraw == 2 )
                    GFX_Tools::CreateGLLine( vCurPos1, vCurPos2, 2., colorPathFind );
                else
                if( nDraw == 1 && rLenPosAgent > rPosStartDraw )
                {
                    MT_Float rSize2 = rLenPosAgent - rCurPos;
                    vCurPos1 = *pLastPoint + ( vDir * rSize2 );
                    GFX_Tools::CreateGLLine( vCurPos1, vCurPos2, 2., colorPathFind );
                    nDraw = 2;
                }

                // On passe au vecteur suivant
                ++itPoint;
                pLastPoint = pCurPoint;
                rCurPos += rSize;
                if( nDraw == 1 )
                    nDraw = 2;
            }
            else
            // si début vecteur < a début draw && fin vecteur > fin draw
            if( rCurPos <= rPosStartDraw  && rCurPos + rSize >= rPosEndDraw )
            {
                // On calcul le début du draw dans le vecteur
                MT_Float rSize2 = rPosStartDraw - rCurPos;
                vCurPos1 = *pLastPoint + ( vDir * rSize2 );

                // On calcul la fin du draw dans le vecteur
                MT_Float rSize3 = rPosEndDraw - rCurPos;
                vCurPos2 = *pLastPoint + ( vDir * rSize3 );

                // On trace le draw
                if( nDraw == 2 )
                    GFX_Tools::CreateGLLine( vCurPos1, vCurPos2, 2., colorPathFind );
                else
                if( nDraw == 1  )
                {
                    if( rLenPosAgent < rPosStartDraw )
                    {
                        GFX_Tools::CreateGLLine( vCurPos1, vCurPos2, 2., colorPathFind );
                        nDraw = 2;
                    }
                    else
                    if( rLenPosAgent >= rPosStartDraw && rLenPosAgent <= rPosEndDraw )
                    {
                        MT_Float rSize2 = rLenPosAgent - rCurPos;
                        vCurPos1 = *pLastPoint + ( vDir * rSize2 );
                        GFX_Tools::CreateGLLine( vCurPos1, vCurPos2, 2., colorPathFind );
                        nDraw = 2;
                    }
                }

                // On passe au draw suivant
                rPosStartDraw += rSegmentSize * 2.;
                rPosEndDraw = rPosStartDraw + rSegmentSize;
            }
        }
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_Agent::IsInside
// Created: FBD 03-01-07
//-----------------------------------------------------------------------------
bool MOS_Agent::IsInside( const MT_Vector2D& vPos ) const
{
    return GFX_Tools::IsInsideGLAgent( vPos_, 600. , symbolName_, vPos );
}


//-----------------------------------------------------------------------------
// Name: MOS_Agent::RegisterListView
// Created: FBD 02-12-20
//-----------------------------------------------------------------------------
void MOS_Agent::RegisterListView( MOS_AgentListView& listview )
{
    agentItemMap_.insert( std::make_pair( &listview, (QListViewItem*)0 ) );
}

//-----------------------------------------------------------------------------
// Name: MOS_Agent::DeleteListView
// Created: FBD 03-01-14
//-----------------------------------------------------------------------------
void MOS_Agent::DeleteListView( MOS_AgentListView& listview )
{
    IT_AgentItemMap itItem = agentItemMap_.find( &listview );
    if( itItem != agentItemMap_.end() )
    {
        if( IsAutomate() )
        {
            if( itItem->first->GetType() == eListViewSelector )
            {
                MOS_AgentCheckListItem_ABC* pAgent = static_cast< MOS_AgentCheckListItem_ABC* >( itItem->second );
                if( pAgent )
                    itItem->first->DeleteAgent( *pAgent );
            }
            else
            {
                MOS_AgentListViewItem_ABC* pAgent = static_cast< MOS_AgentListViewItem_ABC* >( itItem->second );
                if( pAgent )
                    itItem->first->DeleteAgent( *pAgent );
            }
        }
        else
        {
            if( itItem->first->GetType() == eListViewSelector )
            {
                MOS_AgentCheckListItem_ABC* pAgent = static_cast< MOS_AgentCheckListItem_ABC* >( itItem->second );
                if( pAgent )
                    itItem->first->UnregisterAgent( *pAgent );
            }
            else
            {
                MOS_AgentListViewItem_ABC* pAgent = static_cast< MOS_AgentListViewItem_ABC* >( itItem->second );
                if( pAgent )
                    itItem->first->UnregisterAgent( *pAgent );
            }
        }
        itItem->second = 0;
    }
    UnregisterListView( listview );
}
//-----------------------------------------------------------------------------
// Name: MOS_Agent::UnregisterListView
// Created: FBD 02-12-20
//-----------------------------------------------------------------------------
void MOS_Agent::UnregisterListView( MOS_AgentListView& listview )
{
    IT_AgentItemMap itItem = agentItemMap_.find( &listview );
    agentItemMap_.erase( itItem );
}

//-----------------------------------------------------------------------------
// Name: MOS_Agent::SetDefaultName
// Created: FBD 02-12-18
//-----------------------------------------------------------------------------
void MOS_Agent::SetDefaultName()
{
    sName_ = QString( "Agent %1" ).arg(  MOS_App::GetApp().GetAgentManager().GetAgentList().size() );
}


//-----------------------------------------------------------------------------
// Name: MOS_Agent::SetName
// Created: FBD 03-01-02
//-----------------------------------------------------------------------------
void MOS_Agent::SetName( const std::string sText )
{
    sName_ = sText;
    for( IT_AgentItemMap itItem = agentItemMap_.begin() ; itItem != agentItemMap_.end() ; ++itItem  )
    {
        if( itItem->first->GetType() == eListViewSelector )
        {
            MOS_AgentCheckListItem_ABC* pAgent = static_cast< MOS_AgentCheckListItem_ABC* >( itItem->second );
            pAgent->SetName( sName_ );
        }
        else
        {
            MOS_AgentListViewItem_ABC* pAgent = static_cast< MOS_AgentListViewItem_ABC* >( itItem->second );
            pAgent->SetName( sName_ );

        }
    }
}

//-----------------------------------------------------------------------------
// Name: MOS_Agent::SetArme
// Created: FBD 03-01-06
// Last modified: JVT 03-11-26
//-----------------------------------------------------------------------------
/*void MOS_Agent::SetArme()
{
    switch ( nType_ )
    {
        // Automates

        case eAgentTypeAutomateInfanterie : nArme_ = eTypeArmeInfanterie; bAutomate_ = true;  break;
        case eAgentTypeAutomateBlinde     : nArme_ = eTypeArmeBlinde;     bAutomate_ = true;  break;
        case eAgentTypeAutomateGenie      : nArme_ = eTypeArmeGenie;      bAutomate_ = true;  break;
        case eAgentTypeAutomate3D         : nArme_ = eTypeArme3D;         bAutomate_ = true;  break;
        case eAgentTypeAutomateLogTC2     : nArme_ = eTypeArmeLog;        bAutomate_ = true;  break;
        case eAgentTypeAutomateLogBSD     : nArme_ = eTypeArmeLog;        bAutomate_ = true;  break;
        case eAgentTypeAutomateALAT       : nArme_ = eTypeArmeAlat;       bAutomate_ = true;  break;
        case eAgentTypeAutomateRens       : nArme_ = eTypeArmeRens;       bAutomate_ = true;  break;
        case eAgentTypeAutomateNBC        : nArme_ = eTypeArmeNBC;        bAutomate_ = true;  break;

        case eAgentTypePionInfanterie     : nArme_ = eTypeArmeInfanterie; bAutomate_ = false; break;
        case eAgentTypePionBlinde         : nArme_ = eTypeArmeBlinde;     bAutomate_ = false; break;
        case eAgentTypePionGenie          : nArme_ = eTypeArmeGenie;      bAutomate_ = false; break;
        case eAgentTypePion3D             : nArme_ = eTypeArme3D;         bAutomate_ = false; break;
        case eAgentTypePionLog            : nArme_ = eTypeArmeLog;        bAutomate_ = false; break;
        case eAgentTypePionALAT           : nArme_ = eTypeArmeAlat;       bAutomate_ = false; break;
        case eAgentTypePionRens           : nArme_ = eTypeArmeRens;       bAutomate_ = false; break;
        case eAgentTypePionNBC            : nArme_ = eTypeArmeNBC;        bAutomate_ = false; break;
            
        default:
            assert( false );
    }
}
*/

//-----------------------------------------------------------------------------
// Name: MOS_Agent::GetPosAround
// Created: FBD 03-01-15
//-----------------------------------------------------------------------------
MT_Vector2D MOS_Agent::GetPosAround()
{
    return MT_Vector2D( vPos_ + MT_Vector2D( ( (MT_Float)( rand() % 20 ) ) , ( (MT_Float)( rand() % 20 ) ) ) );
}

//-----------------------------------------------------------------------------
// Name: MOS_Agent::RegisterListViewItem
// Created: FBD 03-01-09
//-----------------------------------------------------------------------------
void MOS_Agent::RegisterListViewItem( MOS_AgentListView* pListView, QListViewItem* pItem )
{
    IT_AgentItemMap itItem = agentItemMap_.find( pListView );
    assert( itItem != agentItemMap_.end() );
    itItem->second = pItem;

}
//-----------------------------------------------------------------------------
// Name: MOS_Agent::UnregisterListviewItem
// Created: FBD 03-01-09
//-----------------------------------------------------------------------------
void MOS_Agent::UnregisterListViewItem( MOS_AgentListView* pListView, QListViewItem* pItem )
{
    IT_AgentItemMap itItem = agentItemMap_.find( pListView );
    assert( itItem != agentItemMap_.end() );
    assert( itItem->second == pItem );
    itItem->second = 0;
}


//-----------------------------------------------------------------------------
// Name: MOS_Agent::OnReceiveMsgWaitForOrderConduite
// Created: NLD 2003-04-04
//-----------------------------------------------------------------------------
void MOS_Agent::OnReceiveMsgWaitForOrderConduite( const ASN1T_MsgAttenteOrdreConduite& asnMsg )
{
    MOS_RC& RC = *new MOS_RC( *this );
    RegisterRC( RC );
    RC.Initialize( asnMsg );
}


//-----------------------------------------------------------------------------
// Name: MOS_Agent::OnReceiveMsgCR
// Created: FBD 03-01-27
//-----------------------------------------------------------------------------
void MOS_Agent::OnReceiveMsgCR( const ASN1T_MsgCR& asnMsg )
{
    MOS_RC& RC = *new MOS_RC( *this );
    RegisterRC( RC );
    RC.Initialize( asnMsg );
}

//-----------------------------------------------------------------------------
// Name: MOS_Agent::RegisterRC
// Created: FBD 03-01-27
//-----------------------------------------------------------------------------
void MOS_Agent::RegisterRC( MOS_RC& rc )
{
    IT_RCVector itRC = std::find( RCVector_.begin() , RCVector_.end() , &rc );
    assert( itRC == RCVector_.end() );
    RCVector_.push_back( &rc );
}

//-----------------------------------------------------------------------------
// Name: MOS_Agent::UnregisterRC
// Created: FBD 03-01-27
//-----------------------------------------------------------------------------
void MOS_Agent::UnregisterRC( MOS_RC& rc )
{
    IT_RCVector itRC = std::find( RCVector_.begin() , RCVector_.end() , &rc );
    assert( itRC != RCVector_.end() );
    RCVector_.erase( itRC );

}

//-----------------------------------------------------------------------------
// Name: MOS_Agent::OnReceiveTraceMsg
// Created: FBD 03-01-27
//-----------------------------------------------------------------------------
void MOS_Agent::OnReceiveTraceMsg( DIN_Input& msg )
{
    MOS_Trace& Trace = *new MOS_Trace( *this );
    RegisterTrace( Trace );
    Trace.Initialize( msg );
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::OnReceiveDebugDrawPointsMsg
// Created: NLD 2005-03-22
// -----------------------------------------------------------------------------
void MOS_Agent::OnReceiveDebugDrawPointsMsg( DIN_Input& msg )
{
    debugPointsToDraw_.clear();
    uint32 nTmp;
    msg >> nTmp;
    for( uint i = 0; i < nTmp; ++i )
    {
        MT_Vector2D vPos;
        msg >> vPos;
        debugPointsToDraw_.push_back( vPos );
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_Agent::RegisterTrace
// Created: FBD 03-01-27
//-----------------------------------------------------------------------------
void MOS_Agent::RegisterTrace( MOS_Trace& Trace )
{
    IT_TraceVector itTrace = std::find( TraceVector_.begin() , TraceVector_.end() , &Trace );
    assert( itTrace == TraceVector_.end() );
    TraceVector_.push_back( &Trace );
}
//-----------------------------------------------------------------------------
// Name: MOS_Agent::UnregisterTrace
// Created: FBD 03-01-27
//-----------------------------------------------------------------------------
void MOS_Agent::UnregisterTrace( MOS_Trace& Trace )
{
    IT_TraceVector itTrace = std::find( TraceVector_.begin() , TraceVector_.end() , &Trace );
    assert( itTrace != TraceVector_.end() );
    TraceVector_.erase( itTrace );

}

//-----------------------------------------------------------------------------
// Name: MOS_Agent::SaveOldPathFind
// Created: FBD 03-01-31
//-----------------------------------------------------------------------------
void MOS_Agent::SaveOldPathFind()
{
    if( oldPathFindVector_.size() )
    {
        oldPathFindVector_.rbegin()->push_back( vPos_ );
    }
    oldPathFindVector_.push_back( T_PointVector() );
}

//-----------------------------------------------------------------------------
// Name: MOS_Agent::OnReceiveMsgPathFind
// Created: NLD 2003-01-28
//-----------------------------------------------------------------------------
void MOS_Agent::OnReceiveMsgPathFind( const ASN1T_MsgUnitPathFind& asnMsg )
{
    SaveOldPathFind();

    pathFindVector_.clear();
    for( uint i = 0; i < asnMsg.itineraire.vecteur_point.n; ++i )
    {
        MT_Vector2D vPos;

        MOS_App::GetApp().GetWorld().MosToSimMgrsCoord( (const char*)asnMsg.itineraire.vecteur_point.elem[i].data, vPos );

        if( i == 0 )
            vPos_ + vPos;

        pathFindVector_.push_back( vPos );
    }
}

//-----------------------------------------------------------------------------
// Name: SIM_Agent::DrawVisionAgents
// Created: JVT 02-11-27
//-----------------------------------------------------------------------------
void MOS_Agent::DrawVision()
{
    const MOS_Team& team = GetTeam();

    MOS_AgentListView& listView = MOS_App::GetApp().GetMainWindow().GetAgentEditor().GetAgentListView();
    if( ! MOS_App::GetApp().GetMainWindow().GetAgentEditor().MustBeDrawn( team ) && listView.GetSelectedAgent() != this )
        return;

    if( !listView.IsDrawAllVision()  )
        return;

    MT_Float rSizearrow = 100.;

    GFX_Color recordColor( 255, 55, 255, 0.5 );
    recordColor.SetGLColor();
    for( CIT_AgentConstPtrSet itDetect = recordedAgents_.begin(); itDetect != recordedAgents_.end(); ++itDetect )
        if( (*itDetect)->GetTeam().GetID() != team.GetID() )
            GFX_Tools::CreateGLArrow( GetPos(), (*itDetect)->GetPos(), recordColor, 3., rSizearrow );

    GFX_Color color( 255, 255, 255, 1. );
    color.SetGLColor();

    for( CIT_AgentConstPtrSet itIdent = identifiedAgents_.begin(); itIdent != identifiedAgents_.end(); ++itIdent )
        if( (*itIdent)->GetTeam().GetID() != team.GetID() )
            GFX_Tools::CreateGLArrow( GetPos(), (*itIdent)->GetPos(), color, 1., rSizearrow );

    color.SetBlue( 170 );
    color.SetGLColor();
    for( CIT_AgentConstPtrSet itReco = recognizedAgents_.begin(); itReco != recognizedAgents_.end(); ++itReco )
        if( (*itReco)->GetTeam().GetID() != team.GetID() )
            GFX_Tools::CreateGLArrow( GetPos(), (*itReco)->GetPos(), color, 1., rSizearrow );

    color.SetBlue( 85 );
    color.SetGLColor();
    for( CIT_AgentConstPtrSet itDetect = detectedAgents_.begin(); itDetect != detectedAgents_.end(); ++itDetect )
        if( (*itDetect)->GetTeam().GetID() != team.GetID() )
            GFX_Tools::CreateGLArrow( GetPos(), (*itDetect)->GetPos(), color, 1., rSizearrow );

    color.SetBlue( 255 );
    color.SetGLColor();
    for( CIT_ObjectConstPtrSet itObject = objectsPerceived_.begin(); itObject != objectsPerceived_.end(); ++itObject )
        GFX_Tools::CreateGLArrow( GetPos(), (*itObject)->GetCenter(), color, 1., rSizearrow );   
}

//-----------------------------------------------------------------------------
// Name: MOS_Agent::DrawVisionCone
// Created: JVT 02-11-27
//-----------------------------------------------------------------------------
void MOS_Agent::DrawVisionCone()
{
    const MOS_Team& team = GetTeam();
    MOS_AgentListView& listView = MOS_App::GetApp().GetMainWindow().GetAgentEditor().GetAgentListView();

    if( ! MOS_App::GetApp().GetMainWindow().GetAgentEditor().MustBeDrawn( team ) && listView.GetSelectedAgent() != this )
        return;

    if( !listView.IsDrawAllCone()  )
        return;

    GFX_Color color( team.GetColor() );
    

    for( CIT_SurfaceVector itSurface = surfaces_.begin(); itSurface != surfaces_.end(); ++itSurface )
        (**itSurface).Draw( *this, color );
}


//-----------------------------------------------------------------------------
// Name: MOS_Agent::DrawVisionSurface
// Created: JVT 02-11-27
// Last modified: JVT 03-09-26
//-----------------------------------------------------------------------------
void MOS_Agent::DrawVisionSurface()
{
    const MOS_Team& team = GetTeam();
    const MOS_AgentEditor& editor = MOS_App::GetApp().GetMainWindow().GetAgentEditor();

    if( ! ( editor.MustBeDrawn( team ) && editor.GetAgentListView().IsDrawAllCone() ) )
        return;

    GetVisionSurfaces( surfaceVision_ );

    GFX_Color color( 255, 255, 255, .8 );

    for( CIT_VisionResultMap it = surfaceVision_.begin(); it != surfaceVision_.end(); ++it )
    {
        if ( it->second == eNotSeen )
            continue;

        color.SetBlue ( it->second == eIdentification ? 255 : it->second == eRecognition ? 170 : 85 );

        MT_Vector2D bl = ( it->first );
        MOS_App::GetApp().GetRawVisionData().AlignToCell( bl, MOS_RawVisionData::eBottomLeft );

        MT_Vector2D tr = ( it->first );
        MOS_App::GetApp().GetRawVisionData().AlignToCell( tr, MOS_RawVisionData::eTopRight );

        GFX_Tools::CreateGLRectPoly( MT_Rect( bl.rX_, bl.rY_, tr.rX_, tr.rY_ ), color );
    }
}
//-----------------------------------------------------------------------------
// Name: MOS_Agent::GetVisionSurfaces
// Created: FBD 03-02-12
// Modified: JVT 2004-09-27 
//-----------------------------------------------------------------------------
void MOS_Agent::GetVisionSurfaces( T_VisionResultMap& res )
{
    if ( !( bNeedRedrawVision_ || MOS_App::GetApp().GetRawVisionData().IsMeteoUpdated() ) )
        return;

    res.clear();
    for ( CIT_SurfaceVector itSurface = surfaces_.begin(); itSurface != surfaces_.end(); ++itSurface )
        (*itSurface)->UpdateVisionMatrice( *this, res );
    bNeedRedrawVision_ = false;
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

    bNeedRedrawVision_ = true;
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::OnLogisticLinksChanged
// Created: NLD 2005-07-08
// -----------------------------------------------------------------------------
void MOS_Agent::OnLogisticLinksChanged( const ASN1T_MsgChangeLiensLogistiquesAck& asnMsg )
{
    if( asnMsg.m.oid_tc2Present )
        nTC2ID_ = asnMsg.oid_tc2;
    if( asnMsg.m.oid_maintenancePresent )
        nLogMaintenanceSuperior_ = asnMsg.oid_maintenance;
    if( asnMsg.m.oid_santePresent )
        nLogMedicalSuperior_ = asnMsg.oid_sante;
    if( asnMsg.m.oid_ravitaillementPresent )
        nLogSupplySuperior_ = asnMsg.oid_ravitaillement;

    if( pAttrEditor_ )
    {
        pAttrEditor_->SetTC2( nTC2ID_ );
        pAttrEditor_->SetLogMaintenanceSuperior( nLogMaintenanceSuperior_ );
        pAttrEditor_->SetLogMedicalSuperior    ( nLogMedicalSuperior_ );
        pAttrEditor_->SetLogSupplySuperior     ( nLogSupplySuperior_ );
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_Agent::SetAttributeEditor
// Created: AGN 03-06-03
//-----------------------------------------------------------------------------
void MOS_Agent::SetAttributeEditor( MOS_AttrEditor* pAttrEditor )
{
	if( pAttrEditor_ )
    {
        pAttrEditor_->Reset();

        for( CIT_MaintenanceConsignSet it = maintenanceConsigns_.begin(); it != maintenanceConsigns_.end(); ++it )
            pAttrEditor_->RemoveMaintenanceConsign( **it );

        for( it = maintenanceConsignsHandled_.begin(); it != maintenanceConsignsHandled_.end(); ++it )
            pAttrEditor_->RemoveHandledMaintenanceConsign( **it );

        for( CIT_MedicalConsignSet it = medicalConsigns_.begin(); it != medicalConsigns_.end(); ++it )
            pAttrEditor_->RemoveMedicalConsign( **it );

        for( it = medicalConsignsHandled_.begin(); it != medicalConsignsHandled_.end(); ++it )
            pAttrEditor_->RemoveHandledMedicalConsign( **it );

        for( CIT_SupplyConsignSet it = supplyConsigns_.begin(); it != supplyConsigns_.end(); ++it )
            pAttrEditor_->RemoveSupplyConsign( **it );

        for( it = supplyConsignsHandled_.begin(); it != supplyConsignsHandled_.end(); ++it )
            pAttrEditor_->RemoveHandledSupplyConsign( **it );
    }

    pAttrEditor_ = pAttrEditor;

    if( pAttrEditor_ != 0 )
    {
        pAttrEditor_->NotifyAgentHasMoved( *this, vPos_ );
        pAttrEditor_->SetSpeed( nSpeed_ );
        pAttrEditor_->SetAltitude( nAltitude_ );
        pAttrEditor_->SetDirection( nDirection_ );

        pAttrEditor_->SetLends( lends_ );

        pAttrEditor_->AddHumains( "Total"             , nNbOfficiers_                  , nNbSousOfficiers_                  , nNbMdrs_ );
        pAttrEditor_->AddHumains( "Morts"             , nNbOfficiersKilled_            , nNbSousOfficiersKilled_            , nNbMdrsKilled_ );
        pAttrEditor_->AddHumains( "Blessés"           , nNbOfficiersInjured_           , nNbSousOfficiersInjured_           , nNbMdrsInjured_ );
        pAttrEditor_->AddHumains( "Cas psychiatriques", nNbOfficiersMentalInjured_     , nNbSousOfficiersMentalInjured_     , nNbMdrsMentalInjured_ );
        pAttrEditor_->AddHumains( "Contaminés"        , nNbOfficiersNBC_               , nNbSousOfficiersNBC_               , nNbMdrsNBC_ );
        pAttrEditor_->AddHumains( "Santé"             , nNbOfficiersInMedicalChain_     , nNbSousOfficiersInMedicalChain_     , nNbMdrsInMedicalChain_ );
        pAttrEditor_->AddHumains( "Maintenance"       , nNbOfficiersInMaintenanceChain_, nNbSousOfficiersInMaintenanceChain_, nNbMdrsInMaintenanceChain_ );

        for( IT_EquipementQty_Map itEquipement = equipement_.begin(); itEquipement != equipement_.end(); ++itEquipement )
            pAttrEditor_->AddEquipment( itEquipement->first, itEquipement->second );
        for( IT_RessourceQty_Map itRessource = ressources_.begin(); itRessource != ressources_.end(); ++itRessource )
            pAttrEditor_->AddRessource( itRessource->first, itRessource->second );

        pAttrEditor_->SetPosture            ( nOldPosture_, nCurrentPosture_, nPostureCompletionPourcentage_ );
        pAttrEditor_->SetAgentState         ( nState_ );
        pAttrEditor_->SetLoadingState       ( bLoadingState_ );
        pAttrEditor_->SetStealthModeEnabled ( bStealthModeEnabled_ );
        pAttrEditor_->SetOpState            ( nOpState_ );
        pAttrEditor_->SetRenforts           ( renforts_ );
        pAttrEditor_->SetPionRenforce       ( nPionRenforce_ );
        pAttrEditor_->SetPionTransportes    ( pionTransportes_ );
        pAttrEditor_->SetTransporteur       ( nTransporteur_ );
        pAttrEditor_->SetMorale             ( *pMorale_ );
        pAttrEditor_->SetExperience         ( *pExperience_ );
        pAttrEditor_->SetTiredness          ( *pTiredness_ );

        if( IsAutomate() )
        {
            pAttrEditor_->SetAutomateMode( bEmbraye_ );
            pAttrEditor_->SetTC2( nTC2ID_ );
            pAttrEditor_->SetLogMaintenanceSuperior( nLogMaintenanceSuperior_ );
            pAttrEditor_->SetLogMedicalSuperior    ( nLogMedicalSuperior_ );
            pAttrEditor_->SetLogSupplySuperior     ( nLogSupplySuperior_ );
        }

        if( nFightRateState_ != (E_ForceRatioState)-1 )
            pAttrEditor_->SetFightRateState( nFightRateState_ );
        if( nRulesOfEngagementState_ != (E_RulesOfEngagementState)-1 )
            pAttrEditor_->SetRulesOfEngagementState( nRulesOfEngagementState_ );
        if( nCloseCombatState_ != (E_CloseCombatState)-1 )
            pAttrEditor_->SetCloseCombatState( nCloseCombatState_ );

        pAttrEditor_->SetNbcProtectionSuitWorn ( bNbcProtectionSuitWorn_ );
        pAttrEditor_->SetNbcAgentsContaminating( nbcAgentContaminating_  );
        pAttrEditor_->SetContaminationState    ( nContaminationState_    );
        pAttrEditor_->SetJammedState           ( bJammed_                );
        pAttrEditor_->SetBlackOutState         ( bBlackOut_              );
        pAttrEditor_->SetRadarEnabled          ( bRadarEnabled_          );
        pAttrEditor_->SetPrisonerState         ( bPrisoner_              );
        pAttrEditor_->SetSurrenderedState      ( bSurrendered_           );
        pAttrEditor_->SetRefugeesManagedState  ( bRefugeesManaged_       );

        if( bMaintenancePionLog_ )
        {
            pAttrEditor_->SetLogMaintenanceChaineEnabled( bMaintenanceChainEnabled_ );
            pAttrEditor_->SetLogMaintenanceTempsBordee  ( nMaintenanceTempsBordee_ );
            pAttrEditor_->SetLogMaintenancePriorites    ( maintenancePriorities_ );
            pAttrEditor_->SetLogMaintenanceTacticalPriorites( maintenanceTacticalPriorities_ );
            pAttrEditor_->ResetLogMaintenanceDispoHaulers();
            pAttrEditor_->ResetLogMaintenanceDispoRepairers();
            for( CIT_DisponibiliteLogMoyenVector it = maintenanceDispoHaulers_.begin(); it != maintenanceDispoHaulers_.end(); ++it )
                pAttrEditor_->AddLogMaintenanceDispoHauler( it->first, it->second );
            for( it = maintenanceDispoRepairers_.begin(); it != maintenanceDispoRepairers_.end(); ++it )
                pAttrEditor_->AddLogMaintenanceDispoRepairer( it->first, it->second );
        }
        for( CIT_MaintenanceConsignSet it = maintenanceConsigns_.begin(); it != maintenanceConsigns_.end(); ++it )
            pAttrEditor_->AddMaintenanceConsign( **it );

        for( it = maintenanceConsignsHandled_.begin(); it != maintenanceConsignsHandled_.end(); ++it )
            pAttrEditor_->AddHandledMaintenanceConsign( **it );

        if( bMedicalPionLog_ )
        {
            pAttrEditor_->SetLogMedicalChaineEnabled( bMedicalChainEnabled_ );
            pAttrEditor_->SetLogMedicalTempsBordee  ( nMedicalTempsBordee_ );
            pAttrEditor_->SetLogMedicalPriorites    ( medicalPriorities_ );
            pAttrEditor_->SetLogMedicalTacticalPriorites( medicalTacticalPriorities_ );
            pAttrEditor_->ResetLogMedicalDispoReleveAmbulances();
            pAttrEditor_->ResetLogMedicalDispoRamassageAmbulances();  
            pAttrEditor_->ResetLogMedicalDispoDoctors();

            for( CIT_DisponibiliteLogMoyenVector it = medicalDispoReleveAmbulances_.begin(); it != medicalDispoReleveAmbulances_.end(); ++it )
                pAttrEditor_->AddLogMedicalDispoReleveAmbulances( it->first, it->second );
            for( CIT_DisponibiliteLogMoyenVector it = medicalDispoRamassageAmbulances_.begin(); it != medicalDispoRamassageAmbulances_.end(); ++it )
                pAttrEditor_->AddLogMedicalDispoRamassageAmbulances( it->first, it->second );  
            for( CIT_DisponibiliteLogMoyenVector it = medicalDispoDoctors_.begin(); it != medicalDispoDoctors_.end(); ++it )
                pAttrEditor_->AddLogMedicalDispoDoctors( it->first, it->second );
        }
        for( CIT_MedicalConsignSet it = medicalConsigns_.begin(); it != medicalConsigns_.end(); ++it )
            pAttrEditor_->AddMedicalConsign( **it );

        for( it = medicalConsignsHandled_.begin(); it != medicalConsignsHandled_.end(); ++it )
            pAttrEditor_->AddHandledMedicalConsign( **it );

        if( bSupplyPionLog_ )
        {
            pAttrEditor_->SetLogSupplyChaineEnabled( bSupplyChainEnabled_ );
            pAttrEditor_->ResetLogSupplyDispoTransporters();
            pAttrEditor_->ResetLogSupplyDispoCommanders();

            for( CIT_DisponibiliteLogMoyenVector it = supplyDispoTransporters_.begin(); it != supplyDispoTransporters_.end(); ++it )
                pAttrEditor_->AddLogSupplyDispoTransporter( it->first, it->second );
            for( CIT_DisponibiliteLogMoyenVector it = supplyDispoCommanders_.begin(); it != supplyDispoCommanders_.end(); ++it )
                pAttrEditor_->AddLogSupplyDispoCommander( it->first, it->second );
            pAttrEditor_->ResetLogSupplyStocks();
            for( IT_RessourceQty_Map it = stocks_.begin(); it != stocks_.end(); ++it )
                pAttrEditor_->AddLogSupplyStock( it->first, it->second );        

            pAttrEditor_->ResetLogSupplyQuotas();
            for( CIT_QuotasVector it = quotas_.begin(); it != quotas_.end(); ++it )
                pAttrEditor_->AddLogSupplyQuota( it->first, it->second );        
        }
        for( CIT_SupplyConsignSet it = supplyConsigns_.begin(); it != supplyConsigns_.end(); ++it )
            pAttrEditor_->AddSupplyConsign( **it );

        for( it = supplyConsignsHandled_.begin(); it != supplyConsignsHandled_.end(); ++it )
            pAttrEditor_->AddHandledSupplyConsign( **it );        
    }
}



// -----------------------------------------------------------------------------
// Name: MOS_Agent::OnAutomateChanged
// Created: NLD 2003-10-09
// -----------------------------------------------------------------------------
void MOS_Agent::OnAutomateChanged( MOS_Agent& newSuperior )
{
    assert( pParent_ );

    MOS_Agent& oldSuperior = *pParent_;

    CIT_AgentItemMap itAgentItem; 

    // Remove old ones
    for( itAgentItem = oldSuperior.agentItemMap_.begin(); itAgentItem != oldSuperior.agentItemMap_.end(); ++itAgentItem )
    {
        MOS_AgentListView& agentListView = *itAgentItem->first;

        if( !itAgentItem->second )
            continue;

        MOS_ListViewItem_ABC& listViewItem = static_cast< MOS_ListViewItem_ABC& >( *itAgentItem->second );

        QListViewItem* pItem = listViewItem.firstChild();
        while( pItem )
        {
            MOS_Agent* pAgent = 0;
            if( pItem->rtti() == 1 )       //$$$ CRADE
                pAgent = &static_cast< MOS_AgentCheckListItem_ABC* >( pItem )->GetAgent();
            else if( pItem->rtti() == 0 ) 
                pAgent = &static_cast< MOS_AgentListViewItem_ABC* >( pItem )->GetAgent();
            else
                assert( false );

            if( pAgent->GetAgentID() == GetAgentID() )
            {
                QListViewItem* pItemTmp = pItem;
                pItem = pItem->nextSibling();
                
                UnregisterListViewItem( &agentListView, pItemTmp );
                agentListView.UnregisterAgent( *pItemTmp );

                delete pItemTmp;
                continue;
            }
            pItem = pItem->nextSibling();
        }
    }

    // Add new ones
    for( itAgentItem = newSuperior.agentItemMap_.begin(); itAgentItem != newSuperior.agentItemMap_.end(); ++itAgentItem )
    {
        MOS_AgentListView& agentListView = *itAgentItem->first;

        if( !itAgentItem->second )
            continue;

        QListViewItem* pNewPionItem = 0;
        if( agentListView.GetType() == eListViewSelector ) 
        {
            QCheckListItem* pItemTmp = static_cast< QCheckListItem* >( itAgentItem->second );
            pNewPionItem = new MOS_PionCheckListItem( *this, *pItemTmp, *pItemTmp );
        }
        else
            pNewPionItem = new MOS_PionListViewItem( *this, *itAgentItem->second, *itAgentItem->second );

        agentListView.RegisterAgent( *pNewPionItem );
        RegisterListViewItem( &agentListView, pNewPionItem );
    }
    pParent_ = &newSuperior;
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::OnKnowledgeGroupChanged
// Created: NLD 2003-10-09
// -----------------------------------------------------------------------------
void MOS_Agent::OnKnowledgeGroupChanged( MOS_Gtia& newGtia )
{
    pGtia_ = &newGtia;
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::GetTeam
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
MOS_Team& MOS_Agent::GetTeam() const
{
    return const_cast< MOS_Team& >( GetGtia().GetTeam() );
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::GetGtia
/** @return 
*/
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
MOS_Gtia& MOS_Agent::GetGtia() const
{
    if( pGtia_ )
        return *pGtia_;

    if( pParent_ )
        return pParent_->GetGtia();

    assert( false );
    return *(MOS_Gtia*)0;
}

// =============================================================================
// LOGISTIC - MAINTENANCE
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MOS_Agent::AddMaintenanceConsign
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void MOS_Agent::AddMaintenanceConsign( MOS_LogMaintenanceConsign& consign )
{
    maintenanceConsigns_.insert( &consign );
    if( pAttrEditor_ )
        pAttrEditor_->AddMaintenanceConsign( consign );
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::RemoveMaintenanceConsign
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void MOS_Agent::RemoveMaintenanceConsign( MOS_LogMaintenanceConsign& consign )
{
    maintenanceConsigns_.erase( &consign );
    if( pAttrEditor_ )
        pAttrEditor_->RemoveMaintenanceConsign( consign );
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::AddHandledMaintenanceConsign
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void MOS_Agent::AddHandledMaintenanceConsign( MOS_LogMaintenanceConsign& consign )
{
    maintenanceConsignsHandled_.insert( &consign );
    if( pAttrEditor_ )
        pAttrEditor_->AddHandledMaintenanceConsign( consign );
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::RemoveHandledMaintenanceConsign
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void MOS_Agent::RemoveHandledMaintenanceConsign( MOS_LogMaintenanceConsign& consign )
{
    maintenanceConsignsHandled_.erase( &consign );
    if( pAttrEditor_ )
        pAttrEditor_->RemoveHandledMaintenanceConsign( consign );
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::OnReceiveMsgLogMaintenanceEtat
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void MOS_Agent::OnReceiveMsgLogMaintenanceEtat( const ASN1T_MsgLogMaintenanceEtat& asn )
{
    bMaintenancePionLog_ = true;
    if( asn.m.chaine_activeePresent )
        bMaintenanceChainEnabled_ = asn.chaine_activee;
    if( asn.m.temps_de_bordeePresent )
    {
        switch( asn.temps_de_bordee )
        {
            case EnumTempsBordee::temps_8_heures : nMaintenanceTempsBordee_ =  8; break;
            case EnumTempsBordee::temps_12_heures: nMaintenanceTempsBordee_ = 12; break;
            case EnumTempsBordee::temps_16_heures: nMaintenanceTempsBordee_ = 16; break;
            case EnumTempsBordee::temps_20_heures: nMaintenanceTempsBordee_ = 20; break;
            case EnumTempsBordee::temps_24_heures: nMaintenanceTempsBordee_ = 24; break;
            default:
                assert( false );
        }
    }
    if( asn.m.prioritesPresent )
    {
        maintenancePriorities_.clear();
        for( uint i = 0; i < asn.priorites.n; ++i )
            maintenancePriorities_.push_back( asn.priorites.elem[i] );
    }
    if( asn.m.priorites_tactiquesPresent )
    {
        maintenanceTacticalPriorities_.clear();
        for( uint i = 0; i < asn.priorites_tactiques.n; ++i )
        {
            uint nTmp = asn.priorites_tactiques.elem[i];
            MOS_Agent* pAutomate = MOS_App::GetApp().GetAgentManager().FindAgent( nTmp );
            assert( pAutomate );
            maintenanceTacticalPriorities_.push_back( pAutomate );
        }
    }
    if( asn.m.disponibilites_remorqueursPresent )
    {
        maintenanceDispoHaulers_.clear();
        for( uint i = 0; i < asn.disponibilites_remorqueurs.n; ++i )
            maintenanceDispoHaulers_.push_back( std::make_pair( asn.disponibilites_remorqueurs.elem[i].type_equipement, asn.disponibilites_remorqueurs.elem[i].pourcentage_disponibilite ) );
    }
    if( asn.m.disponibilites_reparateursPresent )
    {
        maintenanceDispoRepairers_.clear();
        for( uint i = 0; i < asn.disponibilites_reparateurs.n; ++i )
            maintenanceDispoRepairers_.push_back( std::make_pair( asn.disponibilites_reparateurs.elem[i].type_equipement, asn.disponibilites_reparateurs.elem[i].pourcentage_disponibilite ) );
    }

    if( pAttrEditor_ )
    {
        pAttrEditor_->SetLogMaintenanceChaineEnabled( bMaintenanceChainEnabled_ );
        pAttrEditor_->SetLogMaintenanceTempsBordee  ( nMaintenanceTempsBordee_ );
        pAttrEditor_->SetLogMaintenancePriorites    ( maintenancePriorities_ );
        pAttrEditor_->SetLogMaintenanceTacticalPriorites( maintenanceTacticalPriorities_ );
        pAttrEditor_->ResetLogMaintenanceDispoHaulers();
        pAttrEditor_->ResetLogMaintenanceDispoRepairers();
        for( CIT_DisponibiliteLogMoyenVector it = maintenanceDispoHaulers_.begin(); it != maintenanceDispoHaulers_.end(); ++it )
            pAttrEditor_->AddLogMaintenanceDispoHauler( it->first, it->second );
        for( it = maintenanceDispoRepairers_.begin(); it != maintenanceDispoRepairers_.end(); ++it )
            pAttrEditor_->AddLogMaintenanceDispoRepairer( it->first, it->second );
    }
}


// =============================================================================
// LOGISTIC - MEDICAL
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MOS_Agent::AddMedicalConsign
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void MOS_Agent::AddMedicalConsign( MOS_LogMedicalConsign& consign )
{
    medicalConsigns_.insert( &consign );
    if( pAttrEditor_ )
        pAttrEditor_->AddMedicalConsign( consign );
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::RemoveMedicalConsign
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void MOS_Agent::RemoveMedicalConsign( MOS_LogMedicalConsign& consign )
{
    medicalConsigns_.erase( &consign );
    if( pAttrEditor_ )
        pAttrEditor_->RemoveMedicalConsign( consign );
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::AddHandledMedicalConsign
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void MOS_Agent::AddHandledMedicalConsign( MOS_LogMedicalConsign& consign )
{
    medicalConsignsHandled_.insert( &consign );
    if( pAttrEditor_ )
        pAttrEditor_->AddHandledMedicalConsign( consign );
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::RemoveHandledMedicalConsign
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void MOS_Agent::RemoveHandledMedicalConsign( MOS_LogMedicalConsign& consign )
{
    medicalConsignsHandled_.erase( &consign );
    if( pAttrEditor_ )
        pAttrEditor_->RemoveHandledMedicalConsign( consign );
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::OnReceiveMsgLogMedicalEtat
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void MOS_Agent::OnReceiveMsgLogMedicalEtat( const ASN1T_MsgLogSanteEtat& asn )
{
    bMedicalPionLog_ = true;
    if( asn.m.chaine_activeePresent )
        bMedicalChainEnabled_ = asn.chaine_activee;
    if( asn.m.temps_de_bordeePresent )
    {
        switch( asn.temps_de_bordee )
        {
            case EnumTempsBordee::temps_8_heures : nMedicalTempsBordee_ =  8; break;
            case EnumTempsBordee::temps_12_heures: nMedicalTempsBordee_ = 12; break;
            case EnumTempsBordee::temps_16_heures: nMedicalTempsBordee_ = 16; break;
            case EnumTempsBordee::temps_20_heures: nMedicalTempsBordee_ = 20; break;
            case EnumTempsBordee::temps_24_heures: nMedicalTempsBordee_ = 24; break;
            default:
                assert( false );
        }
    }

    if( asn.m.prioritesPresent )
    {
        medicalPriorities_.clear();
        for( uint i = 0; i < asn.priorites.n; ++i )
            medicalPriorities_.push_back( asn.priorites.elem[i] );
    }
    
    if( asn.m.priorites_tactiquesPresent )
    {
        medicalTacticalPriorities_.clear();
        for( uint i = 0; i < asn.priorites_tactiques.n; ++i )
        {
            uint nTmp = asn.priorites_tactiques.elem[i];
            MOS_Agent* pAutomate = MOS_App::GetApp().GetAgentManager().FindAgent( nTmp );
            assert( pAutomate );
            medicalTacticalPriorities_.push_back( pAutomate );
        }
    }

    if( asn.m.disponibilites_ambulances_ramassagePresent )
    {
        medicalDispoRamassageAmbulances_.clear();
        for( uint i = 0; i < asn.disponibilites_ambulances_ramassage.n; ++i )
            medicalDispoRamassageAmbulances_.push_back( std::make_pair( asn.disponibilites_ambulances_ramassage.elem[i].type_equipement, asn.disponibilites_ambulances_ramassage.elem[i].pourcentage_disponibilite ) );        
    }
    if( asn.m.disponibilites_ambulances_relevePresent )
    {
        medicalDispoReleveAmbulances_.clear();
        for( uint i = 0; i < asn.disponibilites_ambulances_releve.n; ++i )
            medicalDispoReleveAmbulances_.push_back( std::make_pair( asn.disponibilites_ambulances_releve.elem[i].type_equipement, asn.disponibilites_ambulances_releve.elem[i].pourcentage_disponibilite ) );        
    }
    if( asn.m.disponibilites_medecinsPresent )
    {
        medicalDispoDoctors_.clear();
        for( uint i = 0; i < asn.disponibilites_medecins.n; ++i )
            medicalDispoDoctors_.push_back( std::make_pair( asn.disponibilites_medecins.elem[i].type_equipement, asn.disponibilites_medecins.elem[i].pourcentage_disponibilite ) );        
    }

    if( pAttrEditor_ )
    {
        pAttrEditor_->SetLogMedicalChaineEnabled    ( bMedicalChainEnabled_ );
        pAttrEditor_->SetLogMedicalTempsBordee      ( nMedicalTempsBordee_ );
        pAttrEditor_->SetLogMedicalPriorites        ( medicalPriorities_ );
        pAttrEditor_->SetLogMedicalTacticalPriorites( medicalTacticalPriorities_ );

        pAttrEditor_->ResetLogMedicalDispoReleveAmbulances();
        pAttrEditor_->ResetLogMedicalDispoRamassageAmbulances();  
        pAttrEditor_->ResetLogMedicalDispoDoctors();

        for( CIT_DisponibiliteLogMoyenVector it = medicalDispoReleveAmbulances_.begin(); it != medicalDispoReleveAmbulances_.end(); ++it )
            pAttrEditor_->AddLogMedicalDispoReleveAmbulances( it->first, it->second );
        for( CIT_DisponibiliteLogMoyenVector it = medicalDispoRamassageAmbulances_.begin(); it != medicalDispoRamassageAmbulances_.end(); ++it )
            pAttrEditor_->AddLogMedicalDispoRamassageAmbulances( it->first, it->second );  
        for( CIT_DisponibiliteLogMoyenVector it = medicalDispoDoctors_.begin(); it != medicalDispoDoctors_.end(); ++it )
            pAttrEditor_->AddLogMedicalDispoDoctors( it->first, it->second );        
    }
}


// =============================================================================
// LOGISTIC - RAV
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MOS_Agent::AddSupplyConsign
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void MOS_Agent::AddSupplyConsign( MOS_LogSupplyConsign& consign )
{
    supplyConsigns_.insert( &consign );
    if( pAttrEditor_ )
        pAttrEditor_->AddSupplyConsign( consign );
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::RemoveSupplyConsign
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void MOS_Agent::RemoveSupplyConsign( MOS_LogSupplyConsign& consign )
{
    supplyConsigns_.erase( &consign );
    if( pAttrEditor_ )
        pAttrEditor_->RemoveSupplyConsign( consign );
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::AddHandledSupplyConsign
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void MOS_Agent::AddHandledSupplyConsign( MOS_LogSupplyConsign& consign )
{
    supplyConsignsHandled_.insert( &consign );
    if( pAttrEditor_ )
        pAttrEditor_->AddHandledSupplyConsign( consign );
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::RemoveHandledSupplyConsign
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void MOS_Agent::RemoveHandledSupplyConsign( MOS_LogSupplyConsign& consign )
{
    supplyConsignsHandled_.erase( &consign );
    if( pAttrEditor_ )
        pAttrEditor_->RemoveHandledSupplyConsign( consign );
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::OnReceiveMsgLogSupplyEtat
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void MOS_Agent::OnReceiveMsgLogSupplyEtat( const ASN1T_MsgLogRavitaillementEtat& asn )
{
    bSupplyPionLog_ = true;
    if( asn.m.chaine_activeePresent )
    {
        bSupplyChainEnabled_ = asn.chaine_activee;
        if( pAttrEditor_ )
            pAttrEditor_->SetLogSupplyChaineEnabled( bSupplyChainEnabled_ );
    }

    
    if( asn.m.disponibilites_transporteurs_convoisPresent )
    {
        supplyDispoTransporters_.clear();
        for( uint i = 0; i < asn.disponibilites_transporteurs_convois.n; ++i )
            supplyDispoTransporters_.push_back( std::make_pair( asn.disponibilites_transporteurs_convois.elem[i].type_equipement, asn.disponibilites_transporteurs_convois.elem[i].pourcentage_disponibilite ) );        
        if( pAttrEditor_ )
        {
            pAttrEditor_->ResetLogSupplyDispoTransporters();
            for( CIT_DisponibiliteLogMoyenVector it = supplyDispoTransporters_.begin(); it != supplyDispoTransporters_.end(); ++it )
                pAttrEditor_->AddLogSupplyDispoTransporter( it->first, it->second );
        }
    }
    if( asn.m.disponibilites_chefs_convoisPresent )
    {
        supplyDispoCommanders_.clear();
        for( uint i = 0; i < asn.disponibilites_chefs_convois.n; ++i )
            supplyDispoCommanders_.push_back( std::make_pair( asn.disponibilites_chefs_convois.elem[i].type_equipement, asn.disponibilites_chefs_convois.elem[i].pourcentage_disponibilite ) );        
        if( pAttrEditor_ )
        {
            pAttrEditor_->ResetLogSupplyDispoCommanders();
            for( CIT_DisponibiliteLogMoyenVector it = supplyDispoCommanders_.begin(); it != supplyDispoCommanders_.end(); ++it )
                pAttrEditor_->AddLogSupplyDispoCommander( it->first, it->second ); 
        }
    }

    if( asn.m.stocksPresent )
    {
        uint nSize = asn.stocks.n;
        while( nSize > 0 )
        {
            ASN1T_DotationStock& value = asn.stocks.elem[ --nSize ];
            MIL_AgentID equipId = value.ressource_id;
            uint qty = value.quantite_disponible;
            stocks_[ equipId ] = qty;
        }    
        if( pAttrEditor_ )
        {
            pAttrEditor_->ResetLogSupplyStocks();
            for( IT_RessourceQty_Map it = stocks_.begin(); it != stocks_.end(); ++it )
                pAttrEditor_->AddLogSupplyStock( it->first, it->second );        
        }
    }
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::OnReceiveMsgLogSupplyQuotas
// Created: NLD 2005-02-01
// -----------------------------------------------------------------------------
void MOS_Agent::OnReceiveMsgLogSupplyQuotas( const ASN1T_MsgLogRavitaillementQuotas& asn )
{
    bSupplyPionLog_ = true;
    quotas_.clear();
    for( uint i = 0; i < asn.quotas.n; ++i )
        quotas_.push_back( std::make_pair( asn.quotas.elem[i].ressource_id, asn.quotas.elem[i].quota_disponible ) );
    if( pAttrEditor_ )
    {
        pAttrEditor_->ResetLogSupplyQuotas();
        for( CIT_QuotasVector it = quotas_.begin(); it != quotas_.end(); ++it )
            pAttrEditor_->AddLogSupplyQuota( it->first, it->second );        
    }
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
