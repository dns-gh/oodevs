// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"

#include "Model.h"
#include "Network_Def.h"
#include "Side.h"
#include "KnowledgeGroup.h"
#include "Formation.h"
#include "Automat.h"
#include "Agent.h"
#include "Object.h"
#include "AgentKnowledge.h"
#include "ObjectKnowledge.h"
#include "LogConsignMaintenance.h"
#include "LogConsignSupply.h"
#include "LogConsignMedical.h"
#include "Population.h"
#include "PopulationKnowledge.h"
#include "Limit.h"
#include "Lima.h"

#include "SimulationModel.h"

using namespace dispatcher;
using namespace DIN;

// -----------------------------------------------------------------------------
// Name: Model constructor
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
Model::Model( Dispatcher& dispatcher )
    : dispatcher_            ( dispatcher )
    , pSimulationModel_      ( 0 )
    , sides_                 ()
    , knowledgeGroups_       ()
    , formations_            ()
    , automats_              ()
    , agents_                ()
    , objects_               ()
    , agentKnowledges_       ()
    , objectKnowledges_      ()
    , logConsignsMaintenance_()
    , logConsignsSupply_     ()
    , logConsignsMedical_    ()
    , populations_           ()
    , populationKnowledges_  ()
    , limits_                ()
    , limas_                 ()
{
    pSimulationModel_ = new SimulationModel();
}

// -----------------------------------------------------------------------------
// Name: Model destructor
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
Model::~Model()
{
    delete pSimulationModel_;
}

// =============================================================================
// UPDATE
// =============================================================================

// -----------------------------------------------------------------------------
// Name: Model::Reset
// Created: NLD 2006-10-05
// -----------------------------------------------------------------------------
void Model::Reset()
{
    pSimulationModel_->Reset();

    limits_                 .Clear();
    limas_                  .Clear();
    agentKnowledges_        .Clear();
    objectKnowledges_       .Clear();
    populationKnowledges_   .Clear();
    logConsignsMaintenance_ .Clear();
    logConsignsSupply_      .Clear();
    logConsignsMedical_     .Clear();
    objects_                .Clear();
    populations_            .Clear();
    agents_                 .Clear();
    automats_               .Clear();
    knowledgeGroups_        .Clear();
    formations_             .Clear();
    sides_                  .Clear();
}

// -----------------------------------------------------------------------------
// Name: Model::Update
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
void Model::Update( const ASN1T_MsgsOutSim& asnMsg )
{
    switch( asnMsg.msg.t )
    {
//        case T_MsgsOutSim_msg_msg_limit_creation_ack:
//        case T_MsgsOutSim_msg_msg_limit_destruction_ack:
//        case T_MsgsOutSim_msg_msg_limit_update_ack:
//        case T_MsgsOutSim_msg_msg_lima_creation_ack:
//        case T_MsgsOutSim_msg_msg_lima_destruction_ack:
//        case T_MsgsOutSim_msg_msg_lima_update_ack:
//        case T_MsgsOutSim_msg_msg_pion_order_ack 7
//        case T_MsgsOutSim_msg_msg_order_conduite_ack 8
//        case T_MsgsOutSim_msg_msg_automate_order_ack 9
//        case T_MsgsOutSim_msg_msg_population_order_ack 10
//        case T_MsgsOutSim_msg_msg_set_automate_mode_ack 11
//        case T_MsgsOutSim_msg_msg_unit_magic_action_ack 12
//        case T_MsgsOutSim_msg_msg_object_magic_action_ack 13
//        case T_MsgsOutSim_msg_msg_population_magic_action_ack 14
        case T_MsgsOutSim_msg_msg_change_diplomatie_ack:                sides_.Get( asnMsg.msg.u.msg_change_diplomatie_ack->oid_camp1 ).Update( *asnMsg.msg.u.msg_change_diplomatie_ack ); break;
        case T_MsgsOutSim_msg_msg_change_diplomatie:                    sides_.Get( asnMsg.msg.u.msg_change_diplomatie->oid_camp1 ).Update( *asnMsg.msg.u.msg_change_diplomatie ); break;
        case T_MsgsOutSim_msg_msg_change_groupe_connaissance_ack:       automats_.Get( asnMsg.msg.u.msg_change_groupe_connaissance_ack->oid_automate ).Update( *asnMsg.msg.u.msg_change_groupe_connaissance_ack ); break;
        case T_MsgsOutSim_msg_msg_change_liens_logistiques_ack:         automats_.Get( asnMsg.msg.u.msg_change_liens_logistiques_ack->oid_automate ).Update( *asnMsg.msg.u.msg_change_liens_logistiques_ack ); break;
        case T_MsgsOutSim_msg_msg_change_liens_logistiques:             automats_.Get( asnMsg.msg.u.msg_change_liens_logistiques->oid_automate ).Update( *asnMsg.msg.u.msg_change_liens_logistiques ); break;
        case T_MsgsOutSim_msg_msg_change_automate_ack:                  agents_.Get( asnMsg.msg.u.msg_change_automate_ack->oid_pion ).Update( *asnMsg.msg.u.msg_change_automate_ack ); break;   
        case T_MsgsOutSim_msg_msg_change_automate:                      agents_.Get( asnMsg.msg.u.msg_change_automate->oid_pion ).Update( *asnMsg.msg.u.msg_change_automate ); break;
//        case T_MsgsOutSim_msg_msg_log_ravitaillement_pousser_flux_ack 19
//        case T_MsgsOutSim_msg_msg_log_ravitaillement_change_quotas_ack 20

        case T_MsgsOutSim_msg_msg_ctrl_info:                            pSimulationModel_->Update( *asnMsg.msg.u.msg_ctrl_info ); break;
        case T_MsgsOutSim_msg_msg_ctrl_begin_tick:                      pSimulationModel_->Update( asnMsg.msg.u.msg_ctrl_begin_tick ); break;
        case T_MsgsOutSim_msg_msg_ctrl_end_tick:                        pSimulationModel_->Update( *asnMsg.msg.u.msg_ctrl_end_tick ); break;
        case T_MsgsOutSim_msg_msg_ctrl_stop_ack:                        pSimulationModel_->Update_Stop( asnMsg.msg.u.msg_ctrl_stop_ack ); break;
        case T_MsgsOutSim_msg_msg_ctrl_pause_ack:                       pSimulationModel_->Update_Pause( asnMsg.msg.u.msg_ctrl_pause_ack ); break;
        case T_MsgsOutSim_msg_msg_ctrl_resume_ack:                      pSimulationModel_->Update_Resume( asnMsg.msg.u.msg_ctrl_resume_ack ); break;
        case T_MsgsOutSim_msg_msg_ctrl_change_time_factor_ack:          pSimulationModel_->Update( *asnMsg.msg.u.msg_ctrl_change_time_factor_ack ); break;
//        case T_MsgsOutSim_msg_msg_ctrl_meteo_globale_ack:               OnReceiveMsgCtrlMeteoGlobalAck        (); break;
//        case T_MsgsOutSim_msg_msg_ctrl_meteo_locale_ack:                OnReceiveMsgCtrlMeteoLocalAck         (); break;
//        case T_MsgsOutSim_msg_msg_ctrl_checkpoint_save_begin:           OnReceiveMsgCheckPointSaveBegin       (); break;
//        case T_MsgsOutSim_msg_msg_ctrl_checkpoint_save_end:             OnReceiveMsgCheckPointSaveEnd         (); break;
//        case T_MsgsOutSim_msg_msg_ctrl_checkpoint_load_begin:           OnReceiveMsgCheckPointLoadBegin       (); break;
//        case T_MsgsOutSim_msg_msg_ctrl_checkpoint_load_end:             OnReceiveMsgCheckPointLoadEnd         (); break;
//        case T_MsgsOutSim_msg_msg_ctrl_checkpoint_set_frequency_ack:    pSimulationModel_->Update( asnMsg.msg.u.msg_ctrl_checkpoint_set_frequency_ack ); break;
//        case T_MsgsOutSim_msg_msg_ctrl_checkpoint_save_now_ack:         OnReceiveMsgCheckPointSaveNowAck      (); break;
        case T_MsgsOutSim_msg_msg_ctrl_send_current_state_begin:        /*NOTHING*/ break;
        case T_MsgsOutSim_msg_msg_ctrl_send_current_state_end:          /*NOTHING*/ break;

        case T_MsgsOutSim_msg_msg_limit_creation:                       limits_.Create ( *this, asnMsg.msg.u.msg_limit_creation->oid, *asnMsg.msg.u.msg_limit_creation ); break;
        case T_MsgsOutSim_msg_msg_limit_update:                         limits_.Get    ( asnMsg.msg.u.msg_limit_update->oid ).Update( *asnMsg.msg.u.msg_limit_update ); break;
        case T_MsgsOutSim_msg_msg_limit_destruction:                    limits_.Destroy( asnMsg.msg.u.msg_limit_destruction ); break;
        case T_MsgsOutSim_msg_msg_lima_creation:                        limas_ .Create ( *this, asnMsg.msg.u.msg_lima_creation->oid, *asnMsg.msg.u.msg_lima_creation ); break;
        case T_MsgsOutSim_msg_msg_lima_update:                          limas_ .Get    ( asnMsg.msg.u.msg_lima_update->oid ).Update( *asnMsg.msg.u.msg_lima_update ); break;
        case T_MsgsOutSim_msg_msg_lima_destruction:                     limas_ .Destroy( asnMsg.msg.u.msg_lima_destruction ); break;

        case T_MsgsOutSim_msg_msg_unit_knowledge_creation:              agentKnowledges_.Create( *this, asnMsg.msg.u.msg_unit_knowledge_creation->oid_connaissance, *asnMsg.msg.u.msg_unit_knowledge_creation ); break;
        case T_MsgsOutSim_msg_msg_unit_knowledge_update:                agentKnowledges_.Get( asnMsg.msg.u.msg_unit_knowledge_update->oid_connaissance ).Update( *asnMsg.msg.u.msg_unit_knowledge_update ); break;
        case T_MsgsOutSim_msg_msg_unit_knowledge_destruction:           agentKnowledges_.Destroy( asnMsg.msg.u.msg_unit_knowledge_destruction->oid_connaissance ); break;

        case T_MsgsOutSim_msg_msg_side_creation:                        sides_          .Create( *this, asnMsg.msg.u.msg_side_creation->oid, *asnMsg.msg.u.msg_side_creation ); break;
        case T_MsgsOutSim_msg_msg_knowledge_group_creation:             knowledgeGroups_.Create( *this, asnMsg.msg.u.msg_knowledge_group_creation->oid, *asnMsg.msg.u.msg_knowledge_group_creation ); break;
        case T_MsgsOutSim_msg_msg_formation_creation:                   formations_     .Create( *this, asnMsg.msg.u.msg_formation_creation->oid, *asnMsg.msg.u.msg_formation_creation ); break;
        case T_MsgsOutSim_msg_msg_pion_creation:                        agents_         .Create( *this, asnMsg.msg.u.msg_pion_creation    ->oid_pion    , *asnMsg.msg.u.msg_pion_creation     ); break;
        case T_MsgsOutSim_msg_msg_automate_creation:                    automats_       .Create( *this, asnMsg.msg.u.msg_automate_creation->oid_automate, *asnMsg.msg.u.msg_automate_creation ); break;

        case T_MsgsOutSim_msg_msg_unit_attributes:                      agents_.Get( asnMsg.msg.u.msg_unit_attributes->oid_pion ).Update( *asnMsg.msg.u.msg_unit_attributes ); break;
        case T_MsgsOutSim_msg_msg_automate_attributes:                  automats_.Get( asnMsg.msg.u.msg_automate_attributes->oid_automate ).Update( *asnMsg.msg.u.msg_automate_attributes  ); break;

//        case T_MsgsOutSim_msg_msg_unit_pathfind:                        OnReceiveMsgUnitPathFind              ( *message.u.msg_unit_pathfind                       ); break;

//        case T_MsgsOutSim_msg_msg_start_pion_fire:                      OnReceiveMsgStartPionFire             ( *message.u.msg_start_pion_fire                     ); break;
//        case T_MsgsOutSim_msg_msg_stop_pion_fire:                       OnReceiveMsgStopPionFire              ( *message.u.msg_stop_pion_fire                      ); break;
//        case T_MsgsOutSim_msg_msg_start_population_fire:                OnReceiveMsgStartPopulationFire       ( *message.u.msg_start_population_fire               ); break;
//        case T_MsgsOutSim_msg_msg_stop_population_fire:                 OnReceiveMsgStopPopulationFire        ( *message.u.msg_stop_population_fire                ); break;

//        case T_MsgsOutSim_msg_msg_explosion:                            OnReceiveMsgExplosion                 ( *message.u.msg_explosion                           ); break;
//        case T_MsgsOutSim_msg_msg_cr:                                   OnReceiveMsgCR                        ( *message.u.msg_cr                                  ); break;
//        case T_MsgsOutSim_msg_msg_start_fire_effect:                    OnReceiveMsgStartFireEffect           ( *message.u.msg_start_fire_effect ); break;
//        case T_MsgsOutSim_msg_msg_stop_fire_effect:                     OnReceiveMsgStopFireEffect            ( message.u.msg_stop_fire_effect ); break;

//        case T_MsgsOutSim_msg_msg_pion_order_management:                     OnReceiveMsgOrderManagement           ( *message.u.msg_order_management                    ); break;
//        case T_MsgsOutSim_msg_msg_automate_order_management:                 OnReceiveMsgOrderManagement           ( *message.u.msg_order_management                    ); break;
//        case T_MsgsOutSim_msg_msg_population_order_management:               OnReceiveMsgOrderManagement           ( *message.u.msg_order_management                    ); break;

//        case T_MsgsOutSim_msg_msg_automate_mrt:                         OnReceiveMsgAutomateMRT               ( *message.u.msg_automate_mrt                        ); break;
//        case T_MsgsOutSim_msg_msg_pion_order:                           OnReceiveMsgPionOrder                 ( *message.u.msg_pion_order ); break;

        case T_MsgsOutSim_msg_msg_object_creation:                      objects_.Create( *this, asnMsg.msg.u.msg_object_creation->oid, *asnMsg.msg.u.msg_object_creation ); break;
        case T_MsgsOutSim_msg_msg_object_update:                        objects_.Get( asnMsg.msg.u.msg_object_update->oid ).Update( *asnMsg.msg.u.msg_object_update ); break;
        case T_MsgsOutSim_msg_msg_object_destruction:                   objects_.Destroy( asnMsg.msg.u.msg_object_destruction ); break;
        case T_MsgsOutSim_msg_msg_object_knowledge_creation:            objectKnowledges_.Create( *this, asnMsg.msg.u.msg_object_knowledge_creation->oid_connaissance, *asnMsg.msg.u.msg_object_knowledge_creation ); break;
        case T_MsgsOutSim_msg_msg_object_knowledge_update:              objectKnowledges_.Get( asnMsg.msg.u.msg_object_knowledge_update->oid_connaissance ).Update( *asnMsg.msg.u.msg_object_knowledge_update ); break;
        case T_MsgsOutSim_msg_msg_object_knowledge_destruction:         objectKnowledges_.Destroy( asnMsg.msg.u.msg_object_knowledge_destruction->oid_connaissance ); break;

        case T_MsgsOutSim_msg_msg_log_maintenance_traitement_equipement_creation:    logConsignsMaintenance_.Create( *this, asnMsg.msg.u.msg_log_maintenance_traitement_equipement_creation->oid_consigne, *asnMsg.msg.u.msg_log_maintenance_traitement_equipement_creation ); break;
        case T_MsgsOutSim_msg_msg_log_maintenance_traitement_equipement_destruction: logConsignsMaintenance_.Destroy( asnMsg.msg.u.msg_log_maintenance_traitement_equipement_destruction->oid_consigne ); break;
        case T_MsgsOutSim_msg_msg_log_maintenance_traitement_equipement_update:      logConsignsMaintenance_.Get( asnMsg.msg.u.msg_log_maintenance_traitement_equipement_update->oid_consigne ).Update( *asnMsg.msg.u.msg_log_maintenance_traitement_equipement_update ); break;
        case T_MsgsOutSim_msg_msg_log_maintenance_etat:                              agents_.Get( asnMsg.msg.u.msg_log_maintenance_etat->oid_pion ).Update( *asnMsg.msg.u.msg_log_maintenance_etat ); break;

        case T_MsgsOutSim_msg_msg_log_ravitaillement_traitement_creation:    logConsignsSupply_.Create ( *this, asnMsg.msg.u.msg_log_ravitaillement_traitement_creation->oid_consigne, *asnMsg.msg.u.msg_log_ravitaillement_traitement_creation ); break;
        case T_MsgsOutSim_msg_msg_log_ravitaillement_traitement_destruction: logConsignsSupply_.Destroy( asnMsg.msg.u.msg_log_ravitaillement_traitement_destruction->oid_consigne ); break;
        case T_MsgsOutSim_msg_msg_log_ravitaillement_traitement_update:      logConsignsSupply_.Get( asnMsg.msg.u.msg_log_ravitaillement_traitement_update->oid_consigne ).Update( *asnMsg.msg.u.msg_log_ravitaillement_traitement_update ); break;
        case T_MsgsOutSim_msg_msg_log_ravitaillement_etat:                   agents_.Get( asnMsg.msg.u.msg_log_ravitaillement_etat->oid_pion ).Update( *asnMsg.msg.u.msg_log_ravitaillement_etat ); break;
        //case T_MsgsOutSim_msg_msg_log_ravitaillement_quotas:                 OnReceiveMsgLogRavitaillementQuotas               (  *message.u.msg_log_ravitaillement_quotas ); break;

        case T_MsgsOutSim_msg_msg_log_sante_traitement_humain_creation:    logConsignsMedical_.Create ( *this, asnMsg.msg.u.msg_log_sante_traitement_humain_creation->oid_consigne, *asnMsg.msg.u.msg_log_sante_traitement_humain_creation ); break;
        case T_MsgsOutSim_msg_msg_log_sante_traitement_humain_destruction: logConsignsMedical_.Destroy( asnMsg.msg.u.msg_log_sante_traitement_humain_destruction->oid_consigne ); break;
        case T_MsgsOutSim_msg_msg_log_sante_traitement_humain_update:      logConsignsMedical_.Get( asnMsg.msg.u.msg_log_sante_traitement_humain_update->oid_consigne ).Update( *asnMsg.msg.u.msg_log_sante_traitement_humain_update ); break;
        case T_MsgsOutSim_msg_msg_log_sante_etat:                           agents_.Get( asnMsg.msg.u.msg_log_sante_etat->oid_pion ).Update( *asnMsg.msg.u.msg_log_sante_etat ); break;

        case T_MsgsOutSim_msg_msg_population_creation                       : populations_.Create( *this, asnMsg.msg.u.msg_population_creation->oid_population, *asnMsg.msg.u.msg_population_creation ); break;
        case T_MsgsOutSim_msg_msg_population_update                         : populations_.Get( asnMsg.msg.u.msg_population_update->oid_population ).Update( *asnMsg.msg.u.msg_population_update ); break;
        case T_MsgsOutSim_msg_msg_population_concentration_creation         : populations_.Get( asnMsg.msg.u.msg_population_concentration_creation->oid_population ).Update( *asnMsg.msg.u.msg_population_concentration_creation ); break;
        case T_MsgsOutSim_msg_msg_population_concentration_destruction      : populations_.Get( asnMsg.msg.u.msg_population_concentration_destruction->oid_population ).Update( *asnMsg.msg.u.msg_population_concentration_destruction ); break;
        case T_MsgsOutSim_msg_msg_population_concentration_update           : populations_.Get( asnMsg.msg.u.msg_population_concentration_update->oid_population ).Update( *asnMsg.msg.u.msg_population_concentration_update ); break;
        case T_MsgsOutSim_msg_msg_population_flux_creation                  : populations_.Get( asnMsg.msg.u.msg_population_flux_creation->oid_population ).Update( *asnMsg.msg.u.msg_population_flux_creation ); break;
        case T_MsgsOutSim_msg_msg_population_flux_destruction               : populations_.Get( asnMsg.msg.u.msg_population_flux_destruction->oid_population ).Update( *asnMsg.msg.u.msg_population_flux_destruction ); break;
        case T_MsgsOutSim_msg_msg_population_flux_update                    : populations_.Get( asnMsg.msg.u.msg_population_flux_update->oid_population ).Update( *asnMsg.msg.u.msg_population_flux_update ); break;

        case T_MsgsOutSim_msg_msg_population_knowledge_creation                  : populationKnowledges_.Create( *this, asnMsg.msg.u.msg_population_knowledge_creation->oid_connaissance, *asnMsg.msg.u.msg_population_knowledge_creation ); break;
        case T_MsgsOutSim_msg_msg_population_knowledge_update                    : populationKnowledges_.Get( asnMsg.msg.u.msg_population_knowledge_update->oid_connaissance ).Update( *asnMsg.msg.u.msg_population_knowledge_update ); break;
        case T_MsgsOutSim_msg_msg_population_knowledge_destruction               : populationKnowledges_.Destroy( asnMsg.msg.u.msg_population_knowledge_destruction->oid_connaissance ); break; 
        case T_MsgsOutSim_msg_msg_population_concentration_knowledge_creation    : populationKnowledges_.Get( asnMsg.msg.u.msg_population_concentration_knowledge_creation->oid_connaissance_population ).Update( *asnMsg.msg.u.msg_population_concentration_knowledge_creation ); break;
        case T_MsgsOutSim_msg_msg_population_concentration_knowledge_update      : populationKnowledges_.Get( asnMsg.msg.u.msg_population_concentration_knowledge_update->oid_connaissance_population ).Update( *asnMsg.msg.u.msg_population_concentration_knowledge_update ); break;
        case T_MsgsOutSim_msg_msg_population_concentration_knowledge_destruction : populationKnowledges_.Get( asnMsg.msg.u.msg_population_concentration_knowledge_destruction->oid_connaissance_population ).Update( *asnMsg.msg.u.msg_population_concentration_knowledge_destruction ); break;
        case T_MsgsOutSim_msg_msg_population_flux_knowledge_creation             : populationKnowledges_.Get( asnMsg.msg.u.msg_population_flux_knowledge_creation->oid_connaissance_population ).Update( *asnMsg.msg.u.msg_population_flux_knowledge_creation ); break;
        case T_MsgsOutSim_msg_msg_population_flux_knowledge_update               : populationKnowledges_.Get( asnMsg.msg.u.msg_population_flux_knowledge_update->oid_connaissance_population ).Update( *asnMsg.msg.u.msg_population_flux_knowledge_update ); break;
        case T_MsgsOutSim_msg_msg_population_flux_knowledge_destruction          : populationKnowledges_.Get( asnMsg.msg.u.msg_population_flux_knowledge_destruction->oid_connaissance_population ).Update( *asnMsg.msg.u.msg_population_flux_knowledge_destruction ); break;

//        default:
//            assert( false );
    }
}

// -----------------------------------------------------------------------------
// Name: Model::Update
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
void Model::Update( uint nMsgID, DIN::DIN_Input& msg )
{
    switch( nMsgID )
    {
//        case eMsgEnableUnitVisionCones                  : break;
//        case eMsgDisableUnitVisionCones                 : break;
//        case eMsgUnitMagicAction                        : break;
//        case eMsgEnableProfiling                        : break;
//        case eMsgDisableProfiling                       : break;
//        case eMsgUnitVisionCones                        : break;
//        case eMsgTrace                                  : break;
        case eMsgInit                                   : pSimulationModel_->Update_MsgInit( msg ); break;
//        case eMsgProfilingValues                        : break;
//        case eMsgUnitInterVisibility                    : break;
//        case eMsgObjectInterVisibility                  : break;
//        case eMsgPopulationConcentrationInterVisibility : break;
//        case eMsgPopulationFlowInterVisibility          : break;
//        case eMsgDebugDrawPoints                        : break;
//        case eMsgEnvironmentType                        : break;
//        case eMsgPopulationCollision                    : break;
        default:
            ;
    }
}

// -----------------------------------------------------------------------------
// Name: Model::Send
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void Model::Send( Publisher_ABC& publisher ) const
{
    AsnMsgInClientCtrlSendCurrentStateBegin().Send( publisher );

    pSimulationModel_->Send( publisher );

    sides_.Apply( /*std::bind2nd( */std::mem_fun_ref( &Side::SendCreation ), publisher /*)*/ ); //$$$$ booouh std::bind2nd sucks
    sides_.Apply( std::mem_fun_ref( &Side      ::SendFullUpdate ), publisher );

    // Logistic
    logConsignsMaintenance_.Apply( std::mem_fun_ref( &LogConsignMaintenance::SendCreation   ), publisher );
    logConsignsSupply_     .Apply( std::mem_fun_ref( &LogConsignSupply     ::SendCreation   ), publisher );
    logConsignsMedical_    .Apply( std::mem_fun_ref( &LogConsignMedical    ::SendCreation   ), publisher );
    logConsignsMaintenance_.Apply( std::mem_fun_ref( &LogConsignMaintenance::SendFullUpdate ), publisher );
    logConsignsSupply_     .Apply( std::mem_fun_ref( &LogConsignSupply     ::SendFullUpdate ), publisher );
    logConsignsMedical_    .Apply( std::mem_fun_ref( &LogConsignMedical    ::SendFullUpdate ), publisher );

    // Knowledges
    agentKnowledges_.Apply( std::mem_fun_ref( &AgentKnowledge::SendCreation   ), publisher );
    agentKnowledges_.Apply( std::mem_fun_ref( &AgentKnowledge::SendFullUpdate ), publisher );

    objectKnowledges_.Apply( std::mem_fun_ref( &ObjectKnowledge::SendCreation   ), publisher );
    objectKnowledges_.Apply( std::mem_fun_ref( &ObjectKnowledge::SendFullUpdate ), publisher );

    populationKnowledges_.Apply( std::mem_fun_ref( &PopulationKnowledge::SendCreation   ), publisher );
    populationKnowledges_.Apply( std::mem_fun_ref( &PopulationKnowledge::SendFullUpdate ), publisher );

    // Tactical lines
    limits_.Apply( std::mem_fun_ref( &Limit::SendCreation ), publisher );
    limas_ .Apply( std::mem_fun_ref( &Lima ::SendCreation ), publisher );
    
    AsnMsgInClientCtrlSendCurrentStateEnd().Send( publisher );
}
