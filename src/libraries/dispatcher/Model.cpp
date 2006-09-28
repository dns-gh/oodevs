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
#include "Automat.h"
#include "Agent.h"
#include "Object.h"
#include "KnowledgeAgent.h"
#include "SimulationModel.h"

using namespace dispatcher;
using namespace DIN;

// -----------------------------------------------------------------------------
// Name: Model constructor
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
Model::Model( Dispatcher& dispatcher )
    : dispatcher_      ( dispatcher )
    , pSimulationModel_( 0 )
    , sides_           ()
    , knowledgeGroups_ ()
    , automats_        ()
    , agents_          ()
    , objects_         ()
    , knowledgesAgent_ ()
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
// Name: Model::Update
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
void Model::Update( const ASN1T_MsgsOutSim& asnMsg )
{
    switch( asnMsg.msg.t )
    {
        case T_MsgsOutSim_msg_msg_ctrl_info:                            pSimulationModel_->Update( *asnMsg.msg.u.msg_ctrl_info ); break;
//        case T_MsgsOutSim_msg_msg_ctrl_begin_tick:                      OnReceiveMsgCtrlBeginTick             (  message.u.msg_ctrl_begin_tick                     ); break;
//        case T_MsgsOutSim_msg_msg_ctrl_end_tick:                        OnReceiveMsgCtrlEndTick               ( *message.u.msg_ctrl_end_tick                       ); break;
//        case T_MsgsOutSim_msg_msg_ctrl_stop_ack:                        break;
//        case T_MsgsOutSim_msg_msg_ctrl_pause_ack:                       OnReceiveMsgCtrlPauseAck              ( message.u.msg_ctrl_pause_ack                       ); break;
//        case T_MsgsOutSim_msg_msg_ctrl_resume_ack:                      OnReceiveMsgCtrlResumeAck             ( message.u.msg_ctrl_resume_ack                      ); break;
//        case T_MsgsOutSim_msg_msg_ctrl_change_time_factor_ack:          OnReceiveMsgCtrlChangeTimeFactorAck   ( *message.u.msg_ctrl_change_time_factor_ack         ); break;
//        case T_MsgsOutSim_msg_msg_ctrl_meteo_globale_ack:               OnReceiveMsgCtrlMeteoGlobalAck        (); break;
//        case T_MsgsOutSim_msg_msg_ctrl_meteo_locale_ack:                OnReceiveMsgCtrlMeteoLocalAck         (); break;
//        case T_MsgsOutSim_msg_msg_ctrl_checkpoint_save_begin:           OnReceiveMsgCheckPointSaveBegin       (); break;
//        case T_MsgsOutSim_msg_msg_ctrl_checkpoint_save_end:             OnReceiveMsgCheckPointSaveEnd         (); break;
//        case T_MsgsOutSim_msg_msg_ctrl_checkpoint_load_begin:           OnReceiveMsgCheckPointLoadBegin       (); break;
//        case T_MsgsOutSim_msg_msg_ctrl_checkpoint_load_end:             OnReceiveMsgCheckPointLoadEnd         (); break;
//        case T_MsgsOutSim_msg_msg_ctrl_checkpoint_set_frequency_ack:    OnReceiveMsgCheckPointSetFrequencyAck (); break;
//        case T_MsgsOutSim_msg_msg_ctrl_checkpoint_save_now_ack:         OnReceiveMsgCheckPointSaveNowAck      (); break;
        case T_MsgsOutSim_msg_msg_ctrl_send_current_state_begin:        /*NOTHING*/ break;
        case T_MsgsOutSim_msg_msg_ctrl_send_current_state_end:          /*NOTHING*/ break;

//        case T_MsgsOutSim_msg_msg_limit_creation:                       OnReceiveMsgLimitCreation             ( *message.u.msg_limit_creation                      ); break;
//        case T_MsgsOutSim_msg_msg_limit_destruction:                    OnReceiveMsgLimitDestruction          ( message.u.msg_limit_destruction                    ); break;
//        case T_MsgsOutSim_msg_msg_lima_creation:                        OnReceiveMsgLimaCreation              ( *message.u.msg_lima_creation                       ); break;
//        case T_MsgsOutSim_msg_msg_lima_destruction:                     OnReceiveMsgLimaDestruction           ( message.u.msg_lima_destruction                     ); break;

        case T_MsgsOutSim_msg_msg_unit_knowledge_creation:              knowledgesAgent_.Create( *this, asnMsg.msg.u.msg_unit_knowledge_creation->oid_connaissance, *asnMsg.msg.u.msg_unit_knowledge_creation ); break;
        case T_MsgsOutSim_msg_msg_unit_knowledge_update:                knowledgesAgent_.Get( asnMsg.msg.u.msg_unit_knowledge_update->oid_connaissance ).Update( *asnMsg.msg.u.msg_unit_knowledge_update ); break;
        case T_MsgsOutSim_msg_msg_unit_knowledge_destruction:           knowledgesAgent_.Destroy( asnMsg.msg.u.msg_unit_knowledge_destruction->oid_connaissance ); break;
//
        case T_MsgsOutSim_msg_msg_unit_attributes:                    agents_.Get( asnMsg.msg.u.msg_unit_attributes->oid_pion ).Update( *asnMsg.msg.u.msg_unit_attributes ); break;
        case T_MsgsOutSim_msg_msg_unit_dotations:                     agents_.Get( asnMsg.msg.u.msg_unit_dotations ->oid_pion ).Update( *asnMsg.msg.u.msg_unit_dotations  ); break;

//        case T_MsgsOutSim_msg_msg_unit_pathfind:                        OnReceiveMsgUnitPathFind              ( *message.u.msg_unit_pathfind                       ); break;

//        case T_MsgsOutSim_msg_msg_start_pion_fire:                      OnReceiveMsgStartPionFire             ( *message.u.msg_start_pion_fire                     ); break;
//        case T_MsgsOutSim_msg_msg_stop_pion_fire:                       OnReceiveMsgStopPionFire              ( *message.u.msg_stop_pion_fire                      ); break;
//        case T_MsgsOutSim_msg_msg_start_population_fire:                OnReceiveMsgStartPopulationFire       ( *message.u.msg_start_population_fire               ); break;
//        case T_MsgsOutSim_msg_msg_stop_population_fire:                 OnReceiveMsgStopPopulationFire        ( *message.u.msg_stop_population_fire                ); break;

//        case T_MsgsOutSim_msg_msg_explosion:                            OnReceiveMsgExplosion                 ( *message.u.msg_explosion                           ); break;
//        case T_MsgsOutSim_msg_msg_cr:                                   OnReceiveMsgCR                        ( *message.u.msg_cr                                  ); break;
//        case T_MsgsOutSim_msg_msg_start_fire_effect:                    OnReceiveMsgStartFireEffect           ( *message.u.msg_start_fire_effect ); break;
//        case T_MsgsOutSim_msg_msg_stop_fire_effect:                     OnReceiveMsgStopFireEffect            ( message.u.msg_stop_fire_effect ); break;

//        case T_MsgsOutSim_msg_msg_order_management:                     OnReceiveMsgOrderManagement           ( *message.u.msg_order_management                    ); break;
//        case T_MsgsOutSim_msg_msg_automate_mrt:                         OnReceiveMsgAutomateMRT               ( *message.u.msg_automate_mrt                        ); break;
//        case T_MsgsOutSim_msg_msg_pion_order:                           OnReceiveMsgPionOrder                 ( *message.u.msg_pion_order ); break;

        case T_MsgsOutSim_msg_msg_object_creation:                      objects_.Create( *this, asnMsg.msg.u.msg_object_creation->oid, *asnMsg.msg.u.msg_object_creation ); break;
        case T_MsgsOutSim_msg_msg_object_update:                        objects_.Get( asnMsg.msg.u.msg_object_update->oid ).Update( *asnMsg.msg.u.msg_object_update ); break;
        case T_MsgsOutSim_msg_msg_object_destruction:                   objects_.Destroy( asnMsg.msg.u.msg_object_destruction ); break;
//        case T_MsgsOutSim_msg_msg_object_knowledge_creation:            OnReceiveMsgObjectKnowledgeCreation   ( *message.u.msg_object_knowledge_creation           ); break;
//        case T_MsgsOutSim_msg_msg_object_knowledge_update:              OnReceiveMsgObjectKnowledgeUpdate     ( *message.u.msg_object_knowledge_update             ); break;
//        case T_MsgsOutSim_msg_msg_object_knowledge_destruction:         OnReceiveMsgObjectKnowledgeDestruction( *message.u.msg_object_knowledge_destruction        ); break;
//
//        case T_MsgsOutSim_msg_msg_change_automate:                      OnReceiveMsgChangeAutomate            ( *message.u.msg_change_automate ); break;
//
        case T_MsgsOutSim_msg_msg_pion_creation:                        agents_  .Create( *this, asnMsg.msg.u.msg_pion_creation    ->oid_pion    , *asnMsg.msg.u.msg_pion_creation     ); break;
        case T_MsgsOutSim_msg_msg_automate_creation:                    automats_.Create( *this, asnMsg.msg.u.msg_automate_creation->oid_automate, *asnMsg.msg.u.msg_automate_creation ); break;
        case T_MsgsOutSim_msg_msg_change_diplomatie:                    sides_.Get( asnMsg.msg.u.msg_change_diplomatie->oid_camp1 ).Update( *asnMsg.msg.u.msg_change_diplomatie ); break;
//
//        case T_MsgsOutSim_msg_msg_log_maintenance_traitement_equipement_creation:    OnReceiveMsgLogMaintenanceTraitementEquipementCreation   ( *message.u.msg_log_maintenance_traitement_equipement_creation ); break;
//        case T_MsgsOutSim_msg_msg_log_maintenance_traitement_equipement_destruction: OnReceiveMsgLogMaintenanceTraitementEquipementDestruction( *message.u.msg_log_maintenance_traitement_equipement_destruction ); break;
//        case T_MsgsOutSim_msg_msg_log_maintenance_traitement_equipement_update:      OnReceiveMsgLogMaintenanceTraitementEquipementUpdate     ( *message.u.msg_log_maintenance_traitement_equipement_update ); break;
//        case T_MsgsOutSim_msg_msg_log_maintenance_etat:                              OnReceiveMsgLogMaintenanceEtat( *message.u.msg_log_maintenance_etat ); break;
//
//        case T_MsgsOutSim_msg_msg_log_ravitaillement_traitement_creation:    OnReceiveMsgLogRavitaillementTraitementCreation   ( *message.u.msg_log_ravitaillement_traitement_creation ); break;
//        case T_MsgsOutSim_msg_msg_log_ravitaillement_traitement_destruction: OnReceiveMsgLogRavitaillementTraitementDestruction( *message.u.msg_log_ravitaillement_traitement_destruction ); break;
//        case T_MsgsOutSim_msg_msg_log_ravitaillement_traitement_update:      OnReceiveMsgLogRavitaillementTraitementUpdate     ( *message.u.msg_log_ravitaillement_traitement_update ); break;
//        case T_MsgsOutSim_msg_msg_log_ravitaillement_etat:                   OnReceiveMsgLogRavitaillementEtat( *message.u.msg_log_ravitaillement_etat ); break;
//        case T_MsgsOutSim_msg_msg_log_ravitaillement_quotas:                 OnReceiveMsgLogRavitaillementQuotas               (  *message.u.msg_log_ravitaillement_quotas ); break;
//
//        case T_MsgsOutSim_msg_msg_log_sante_traitement_humain_creation:    OnReceiveMsgLogSanteTraitementHumainCreation   ( *message.u.msg_log_sante_traitement_humain_creation ); break;
//        case T_MsgsOutSim_msg_msg_log_sante_traitement_humain_destruction: OnReceiveMsgLogSanteTraitementHumainDestruction( *message.u.msg_log_sante_traitement_humain_destruction ); break;
//        case T_MsgsOutSim_msg_msg_log_sante_traitement_humain_update:      OnReceiveMsgLogSanteTraitementHumainUpdate     ( *message.u.msg_log_sante_traitement_humain_update ); break;
//        case T_MsgsOutSim_msg_msg_log_sante_etat:                          OnReceiveMsgLogSanteEtat( *message.u.msg_log_sante_etat ); break;

//        case T_MsgsOutSim_msg_msg_population_creation                       : OnMsgPopulationCreation                ( *message.u.msg_population_creation ); break;
//        case T_MsgsOutSim_msg_msg_population_update                         : OnMsgPopulationUpdate                  ( *message.u.msg_population_update ); break;
//        case T_MsgsOutSim_msg_msg_population_concentration_creation         : OnMsgPopulationConcentrationCreation   ( *message.u.msg_population_concentration_creation ); break;
//        case T_MsgsOutSim_msg_msg_population_concentration_destruction      : OnMsgPopulationConcentrationDestruction( *message.u.msg_population_concentration_destruction ); break;
//        case T_MsgsOutSim_msg_msg_population_concentration_update           : OnMsgPopulationConcentrationUpdate     ( *message.u.msg_population_concentration_update ); break;
//        case T_MsgsOutSim_msg_msg_population_flux_creation                  : OnMsgPopulationFluxCreation            ( *message.u.msg_population_flux_creation ); break;
//        case T_MsgsOutSim_msg_msg_population_flux_destruction               : OnMsgPopulationFluxDestruction         ( *message.u.msg_population_flux_destruction ); break;
//        case T_MsgsOutSim_msg_msg_population_flux_update                    : OnMsgPopulationFluxUpdate              ( *message.u.msg_population_flux_update ); break;
//
//        case T_MsgsOutSim_msg_msg_population_knowledge_creation                  : OnReceiveMsgPopulationKnowledgeCreation                ( *message.u.msg_population_knowledge_creation                  ); break;
//        case T_MsgsOutSim_msg_msg_population_knowledge_update                    : OnReceiveMsgPopulationKnowledgeUpdate                  ( *message.u.msg_population_knowledge_update                    ); break;
//        case T_MsgsOutSim_msg_msg_population_knowledge_destruction               : OnReceiveMsgPopulationKnowledgeDestruction             ( *message.u.msg_population_knowledge_destruction               ); break;
//        case T_MsgsOutSim_msg_msg_population_concentration_knowledge_creation    : OnReceiveMsgPopulationConcentrationKnowledgeCreation   ( *message.u.msg_population_concentration_knowledge_creation    ); break;
//        case T_MsgsOutSim_msg_msg_population_concentration_knowledge_update      : OnReceiveMsgPopulationConcentrationKnowledgeUpdate     ( *message.u.msg_population_concentration_knowledge_update      ); break;
//        case T_MsgsOutSim_msg_msg_population_concentration_knowledge_destruction : OnReceiveMsgPopulationConcentrationKnowledgeDestruction( *message.u.msg_population_concentration_knowledge_destruction ); break;
//        case T_MsgsOutSim_msg_msg_population_flux_knowledge_creation             : OnReceiveMsgPopulationFlowKnowledgeCreation            ( *message.u.msg_population_flux_knowledge_creation             ); break;
//        case T_MsgsOutSim_msg_msg_population_flux_knowledge_update               : OnReceiveMsgPopulationFlowKnowledgeUpdate              ( *message.u.msg_population_flux_knowledge_update               ); break;
//        case T_MsgsOutSim_msg_msg_population_flux_knowledge_destruction          : OnReceiveMsgPopulationFlowKnowledgeDestruction         ( *message.u.msg_population_flux_knowledge_destruction          ); break;
    }
}

// -----------------------------------------------------------------------------
// Name: Model::OnReceiveMsgCreateSide
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
void Model::OnReceiveMsgCreateSide( DIN_Input& msg )
{
    unsigned long nID;
    msg >> nID;
    sides_.Create( *this, nID, nID, msg ); 
}
    
// -----------------------------------------------------------------------------
// Name: Model::OnReceiveMsgCreateKnowledgeGroup
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
void Model::OnReceiveMsgCreateKnowledgeGroup( DIN_Input& msg )
{
    unsigned long nSideID;
    unsigned long nID;

    msg >> nSideID;
    msg >> nID;

    Side& side = sides_.Get( nSideID );
    knowledgeGroups_.Create( *this, nID, nID, side );
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
        case eMsgKnowledgeGroup                         : OnReceiveMsgCreateKnowledgeGroup( msg ); break;
        case eMsgArmy                                   : OnReceiveMsgCreateSide( msg ); break;

            
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

    /*
    workspace.GetLimaManager  ().SendStateToNewClient();
    workspace.GetLimitManager ().SendStateToNewClient();
    */

    sides_          .Apply( /*std::bind2nd( */std::mem_fun_ref( &Side::SendCreation ), publisher /*)*/ ); //$$$$ booouh std::bind2nd sucks
    knowledgeGroups_.Apply( std::mem_fun_ref( &KnowledgeGroup::SendCreation ), publisher );
    automats_       .Apply( std::mem_fun_ref( &Automat       ::SendCreation ), publisher );
    agents_         .Apply( std::mem_fun_ref( &Agent         ::SendCreation ), publisher );
//$$$ POPULATION
    objects_        .Apply( std::mem_fun_ref( &Object        ::SendCreation ), publisher );


    sides_          .Apply( std::mem_fun_ref( &Side   ::SendFullUpdate ), publisher );
//    automats_       .Apply( std::mem_fun_ref( &Automat::SendFullUpdate ), publisher ); //$$$ Séparation pions/automates
    agents_         .Apply( std::mem_fun_ref( &Agent  ::SendFullUpdate ), publisher );
//$$$ POPULATION
    objects_        .Apply( std::mem_fun_ref( &Object ::SendFullUpdate ), publisher );

    // Knowledges
    knowledgesAgent_.Apply( std::mem_fun_ref( &KnowledgeAgent::SendCreation   ), publisher );
    knowledgesAgent_.Apply( std::mem_fun_ref( &KnowledgeAgent::SendFullUpdate ), publisher );
    /*

    // Knowledge
    for( CIT_ArmyMap itArmy = armies_.begin(); itArmy != armies_.end(); ++itArmy )
        itArmy->second->SendKnowledge();
//    for( CIT_AutomateMap itAutomate = automates_.begin(); itAutomate != automates_.end(); ++itAutomate )
//        itAutomate->second->SendKnowledge );
    for( CIT_PionMap itPion = pions_.begin(); itPion != pions_.end(); ++itPion )
        itPion->second->SendKnowledge();
    */

    /*
    // Armies / knowledge groups
    for( CIT_ArmyMap itArmy = armies_.begin(); itArmy != armies_.end(); ++itArmy )
        itArmy->second->SendCreation();
    for( CIT_ArmyMap itArmy = armies_.begin(); itArmy != armies_.end(); ++itArmy )
        itArmy->second->SendFullState();
*/
    
    AsnMsgInClientCtrlSendCurrentStateEnd().Send( publisher );
}
