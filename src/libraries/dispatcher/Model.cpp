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
#include "PopulationFlow.h"
#include "PopulationConcentration.h"
#include "PopulationKnowledge.h"
#include "Limit.h"
#include "Lima.h"
#include "Visitors.h"
#include "Fire.h"
#include "PopulationFire.h"
#include "FireEffect.h"
#include "Synchroniser.h"
#include "MT/MT_Logger/MT_Logger_lib.h"

#include "SimulationModel.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Model constructor
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
Model::Model()
    : pSimulationModel_( new SimulationModel() )
    , synching_( false )
{
    // NOTHING
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
// Name: Model::Receive
// Created: AGE 2007-07-05
// -----------------------------------------------------------------------------
void Model::Receive( const ASN1T_MsgsSimToClient& asnMsg )
{
    if( asnMsg.msg.t == T_MsgsSimToClient_msg_msg_control_send_current_state_begin )
    {
        Reset();
        MT_LOG_INFO_MSG( "Dispatcher - Initializing model" );
    }
    else if( asnMsg.msg.t == T_MsgsSimToClient_msg_msg_control_send_current_state_end )
        MT_LOG_INFO_MSG( "Dispatcher - Model initialized" );
    Update( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: Model::Update
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
void Model::Update( const ASN1T_MsgsSimToClient& asnMsg )
{
    switch( asnMsg.msg.t )
    {
        case T_MsgsSimToClient_msg_msg_limit_creation_request_ack:
        case T_MsgsSimToClient_msg_msg_limit_destruction_request_ack:
        case T_MsgsSimToClient_msg_msg_limit_update_request_ack:
        case T_MsgsSimToClient_msg_msg_lima_creation_request_ack:
        case T_MsgsSimToClient_msg_msg_lima_destruction_request_ack:
        case T_MsgsSimToClient_msg_msg_lima_update_request_ack:
            break; // NOTHING
        case T_MsgsSimToClient_msg_msg_unit_order_ack:
        case T_MsgsSimToClient_msg_msg_automat_order_ack:
        case T_MsgsSimToClient_msg_msg_population_order_ack:
        case T_MsgsSimToClient_msg_msg_frag_order_ack:
        case T_MsgsSimToClient_msg_msg_set_automat_mode_ack:
        case T_MsgsSimToClient_msg_msg_unit_magic_action_ack:
        case T_MsgsSimToClient_msg_msg_object_magic_action_ack:
        case T_MsgsSimToClient_msg_msg_population_magic_action_ack:
            break; // NOTHING
        case T_MsgsSimToClient_msg_msg_change_diplomacy_ack:                     sides_.Get( asnMsg.msg.u.msg_change_diplomacy_ack->oid_camp1 ).Update( *asnMsg.msg.u.msg_change_diplomacy_ack ); break;
        case T_MsgsSimToClient_msg_msg_change_diplomacy:                         sides_.Get( asnMsg.msg.u.msg_change_diplomacy->oid_camp1 ).Update( *asnMsg.msg.u.msg_change_diplomacy ); break;
        case T_MsgsSimToClient_msg_msg_automat_change_knowledge_group_ack:       automats_.Get( asnMsg.msg.u.msg_automat_change_knowledge_group_ack->oid ).Update( *asnMsg.msg.u.msg_automat_change_knowledge_group_ack ); break;
        case T_MsgsSimToClient_msg_msg_automat_change_logistic_links_ack:        automats_.Get( asnMsg.msg.u.msg_automat_change_logistic_links_ack->oid ).Update( *asnMsg.msg.u.msg_automat_change_logistic_links_ack ); break;
        case T_MsgsSimToClient_msg_msg_automat_change_logistic_links:            automats_.Get( asnMsg.msg.u.msg_automat_change_logistic_links->oid ).Update( *asnMsg.msg.u.msg_automat_change_logistic_links); break;
        case T_MsgsSimToClient_msg_msg_unit_change_superior_ack:                 agents_.Get( asnMsg.msg.u.msg_unit_change_superior_ack->oid ).Update( *asnMsg.msg.u.msg_unit_change_superior_ack ); break;
        case T_MsgsSimToClient_msg_msg_unit_change_superior:                     agents_.Get( asnMsg.msg.u.msg_unit_change_superior->oid ).Update( *asnMsg.msg.u.msg_unit_change_superior ); break;
        case T_MsgsSimToClient_msg_msg_log_supply_push_flow_ack:
        case T_MsgsSimToClient_msg_msg_log_supply_change_quotas_ack:
            break; // NOTHING
        case T_MsgsSimToClient_msg_msg_control_information:                  pSimulationModel_->Update( *asnMsg.msg.u.msg_control_information ); break;
        case T_MsgsSimToClient_msg_msg_control_begin_tick:                   pSimulationModel_->Update( asnMsg.msg.u.msg_control_begin_tick ); break;
        case T_MsgsSimToClient_msg_msg_control_end_tick:                     pSimulationModel_->Update( *asnMsg.msg.u.msg_control_end_tick ); break;
        case T_MsgsSimToClient_msg_msg_control_stop_ack:                     pSimulationModel_->Update_Stop( asnMsg.msg.u.msg_control_stop_ack ); break;
        case T_MsgsSimToClient_msg_msg_control_pause_ack:                    pSimulationModel_->Update_Pause( asnMsg.msg.u.msg_control_pause_ack ); break;
        case T_MsgsSimToClient_msg_msg_control_resume_ack:                   pSimulationModel_->Update_Resume( asnMsg.msg.u.msg_control_resume_ack ); break;
        case T_MsgsSimToClient_msg_msg_control_change_time_factor_ack:       pSimulationModel_->Update( *asnMsg.msg.u.msg_control_change_time_factor_ack ); break;
        case T_MsgsSimToClient_msg_msg_control_global_meteo_ack:
        case T_MsgsSimToClient_msg_msg_control_local_meteo_ack:
        case T_MsgsSimToClient_msg_msg_control_checkpoint_save_begin:
        case T_MsgsSimToClient_msg_msg_control_checkpoint_save_end:
        case T_MsgsSimToClient_msg_msg_control_checkpoint_set_frequency_ack:
        case T_MsgsSimToClient_msg_msg_control_checkpoint_save_now_ack:
        case T_MsgsSimToClient_msg_msg_control_send_current_state_begin:
        case T_MsgsSimToClient_msg_msg_control_send_current_state_end:
            break;  // $$$$ AGE 2007-04-18: messages vides...
        case T_MsgsSimToClient_msg_msg_limit_creation:                       CreateUpdate( limits_, asnMsg.msg.u.msg_limit_creation->oid, *asnMsg.msg.u.msg_limit_creation ); break;
        case T_MsgsSimToClient_msg_msg_limit_update:                         limits_.Get    ( asnMsg.msg.u.msg_limit_update->oid ).Update( *asnMsg.msg.u.msg_limit_update ); break;
        case T_MsgsSimToClient_msg_msg_limit_destruction:                    limits_.Destroy( asnMsg.msg.u.msg_limit_destruction ); break;
        case T_MsgsSimToClient_msg_msg_lima_creation:                        CreateUpdate( limas_, asnMsg.msg.u.msg_lima_creation->oid, *asnMsg.msg.u.msg_lima_creation ); break;
        case T_MsgsSimToClient_msg_msg_lima_update:                          limas_ .Get    ( asnMsg.msg.u.msg_lima_update->oid ).Update( *asnMsg.msg.u.msg_lima_update ); break;
        case T_MsgsSimToClient_msg_msg_lima_destruction:                     limas_ .Destroy( asnMsg.msg.u.msg_lima_destruction ); break;
        case T_MsgsSimToClient_msg_msg_unit_knowledge_creation:              CreateUpdate( agentKnowledges_, asnMsg.msg.u.msg_unit_knowledge_creation->oid, *asnMsg.msg.u.msg_unit_knowledge_creation ); break;
        case T_MsgsSimToClient_msg_msg_unit_knowledge_update:                agentKnowledges_.Get( asnMsg.msg.u.msg_unit_knowledge_update->oid ).Update( *asnMsg.msg.u.msg_unit_knowledge_update ); break;
        case T_MsgsSimToClient_msg_msg_unit_knowledge_destruction:           agentKnowledges_.Destroy( asnMsg.msg.u.msg_unit_knowledge_destruction->oid ); break;
        case T_MsgsSimToClient_msg_msg_side_creation:                        CreateUpdate( sides_          , asnMsg.msg.u.msg_side_creation->oid, *asnMsg.msg.u.msg_side_creation ); break;
        case T_MsgsSimToClient_msg_msg_knowledge_group_creation:             CreateUpdate( knowledgeGroups_, asnMsg.msg.u.msg_knowledge_group_creation->oid, *asnMsg.msg.u.msg_knowledge_group_creation ); break;
        case T_MsgsSimToClient_msg_msg_formation_creation:                   CreateUpdate( formations_     , asnMsg.msg.u.msg_formation_creation->oid, *asnMsg.msg.u.msg_formation_creation ); break;
        case T_MsgsSimToClient_msg_msg_unit_creation:                        CreateUpdate( agents_         , asnMsg.msg.u.msg_unit_creation    ->oid , *asnMsg.msg.u.msg_unit_creation     ); break;
        case T_MsgsSimToClient_msg_msg_unit_destruction:                     agents_.Destroy( asnMsg.msg.u.msg_unit_destruction ); break;
        case T_MsgsSimToClient_msg_msg_automat_creation :                    CreateUpdate( automats_       , asnMsg.msg.u.msg_automat_creation->oid, *asnMsg.msg.u.msg_automat_creation ); break;
        case T_MsgsSimToClient_msg_msg_unit_attributes:                      agents_.Get( asnMsg.msg.u.msg_unit_attributes->oid).Update( *asnMsg.msg.u.msg_unit_attributes ); break;
        case T_MsgsSimToClient_msg_msg_automat_attributes:                   automats_.Get( asnMsg.msg.u.msg_automat_attributes->oid).Update( *asnMsg.msg.u.msg_automat_attributes  ); break;
        case T_MsgsSimToClient_msg_msg_unit_pathfind:
            break; // $$$$ AGE 2007-04-18:
        case T_MsgsSimToClient_msg_msg_start_unit_fire:                      CreateUpdate( fires_, asnMsg.msg.u.msg_start_unit_fire->fire_oid, *asnMsg.msg.u.msg_start_unit_fire ); break;
        case T_MsgsSimToClient_msg_msg_stop_unit_fire:                       fires_.Destroy( asnMsg.msg.u.msg_stop_unit_fire->fire_oid ); break;
        case T_MsgsSimToClient_msg_msg_start_population_fire:                CreateUpdate( populationFires_, asnMsg.msg.u.msg_start_population_fire->fire_oid, *asnMsg.msg.u.msg_start_population_fire ); break;
        case T_MsgsSimToClient_msg_msg_stop_population_fire:                 populationFires_.Destroy( asnMsg.msg.u.msg_stop_population_fire->fire_oid ); break;
        case T_MsgsSimToClient_msg_msg_explosion:
        case T_MsgsSimToClient_msg_msg_report:
        case T_MsgsSimToClient_msg_msg_trace:
            break;  // $$$$ AGE 2007-04-18: Evenements, modèle client => rien, ou remanier
        case T_MsgsSimToClient_msg_msg_decisional_state:                     UpdateAnyAgent( asnMsg.msg.u.msg_decisional_state->oid, *asnMsg.msg.u.msg_decisional_state ); break;
        case T_MsgsSimToClient_msg_msg_start_fire_effect:                    CreateUpdate( fireEffects_, asnMsg.msg.u.msg_start_fire_effect->effect_oid, *asnMsg.msg.u.msg_start_fire_effect ); break;
        case T_MsgsSimToClient_msg_msg_stop_fire_effect:                     fireEffects_.Destroy( asnMsg.msg.u.msg_stop_fire_effect ); break;

        case T_MsgsSimToClient_msg_msg_unit_order:                           agents_     .Get( asnMsg.msg.u.msg_unit_order       ->oid ).Update( *asnMsg.msg.u.msg_unit_order                  ); break;
        case T_MsgsSimToClient_msg_msg_automat_order:                        automats_   .Get( asnMsg.msg.u.msg_automat_order    ->oid ).Update( *asnMsg.msg.u.msg_automat_order               ); break;
        case T_MsgsSimToClient_msg_msg_population_order:                     populations_.Get( asnMsg.msg.u.msg_population_order ->oid ).Update( *asnMsg.msg.u.msg_population_order            ); break;

        case T_MsgsSimToClient_msg_msg_object_creation:                      CreateUpdate( objects_, asnMsg.msg.u.msg_object_creation->oid, *asnMsg.msg.u.msg_object_creation ); break;
        case T_MsgsSimToClient_msg_msg_object_update:                        objects_.Get( asnMsg.msg.u.msg_object_update->oid ).Update( *asnMsg.msg.u.msg_object_update ); break;
        case T_MsgsSimToClient_msg_msg_object_destruction:                   objects_.Destroy( asnMsg.msg.u.msg_object_destruction ); break;
        case T_MsgsSimToClient_msg_msg_object_knowledge_creation:            CreateUpdate( objectKnowledges_, asnMsg.msg.u.msg_object_knowledge_creation->oid, *asnMsg.msg.u.msg_object_knowledge_creation ); break;
        case T_MsgsSimToClient_msg_msg_object_knowledge_update:              objectKnowledges_.Get( asnMsg.msg.u.msg_object_knowledge_update->oid ).Update( *asnMsg.msg.u.msg_object_knowledge_update ); break;
        case T_MsgsSimToClient_msg_msg_object_knowledge_destruction:         objectKnowledges_.Destroy( asnMsg.msg.u.msg_object_knowledge_destruction->oid ); break;

        case T_MsgsSimToClient_msg_msg_log_maintenance_handling_creation:    CreateUpdate( logConsignsMaintenance_, asnMsg.msg.u.msg_log_maintenance_handling_creation->oid_consigne, *asnMsg.msg.u.msg_log_maintenance_handling_creation ); break;
        case T_MsgsSimToClient_msg_msg_log_maintenance_handling_destruction: logConsignsMaintenance_.Destroy( asnMsg.msg.u.msg_log_maintenance_handling_destruction->oid_consigne ); break;
        case T_MsgsSimToClient_msg_msg_log_maintenance_handling_update:      logConsignsMaintenance_.Get( asnMsg.msg.u.msg_log_maintenance_handling_update->oid_consigne ).Update( *asnMsg.msg.u.msg_log_maintenance_handling_update ); break;
        case T_MsgsSimToClient_msg_msg_log_maintenance_state:                agents_.Get( asnMsg.msg.u.msg_log_maintenance_state->oid_pion ).Update( *asnMsg.msg.u.msg_log_maintenance_state ); break;

        case T_MsgsSimToClient_msg_msg_log_supply_handling_creation:        CreateUpdate( logConsignsSupply_, asnMsg.msg.u.msg_log_supply_handling_creation->oid_consigne, *asnMsg.msg.u.msg_log_supply_handling_creation ); break;
        case T_MsgsSimToClient_msg_msg_log_supply_handling_destruction:     logConsignsSupply_.Destroy( asnMsg.msg.u.msg_log_supply_handling_destruction->oid_consigne ); break;
        case T_MsgsSimToClient_msg_msg_log_supply_handling_update:          logConsignsSupply_.Get( asnMsg.msg.u.msg_log_supply_handling_update->oid_consigne ).Update( *asnMsg.msg.u.msg_log_supply_handling_update ); break;
        case T_MsgsSimToClient_msg_msg_log_supply_state:                    agents_.Get( asnMsg.msg.u.msg_log_medical_state->oid_pion ).Update( *asnMsg.msg.u.msg_log_supply_state); break;
        case T_MsgsSimToClient_msg_msg_log_supply_quotas:                   automats_.Get( asnMsg.msg.u.msg_log_supply_quotas->oid_automate ).Update( *asnMsg.msg.u.msg_log_supply_quotas ); break;

        case T_MsgsSimToClient_msg_msg_log_medical_handling_creation:       CreateUpdate( logConsignsMedical_, asnMsg.msg.u.msg_log_medical_handling_creation->oid_consigne, *asnMsg.msg.u.msg_log_medical_handling_creation ); break;
        case T_MsgsSimToClient_msg_msg_log_medical_handling_destruction:    logConsignsMedical_.Destroy( asnMsg.msg.u.msg_log_medical_handling_destruction->oid_consigne ); break;
        case T_MsgsSimToClient_msg_msg_log_medical_handling_update:         logConsignsMedical_.Get( asnMsg.msg.u.msg_log_medical_handling_update->oid_consigne ).Update( *asnMsg.msg.u.msg_log_medical_handling_update ); break;
        case T_MsgsSimToClient_msg_msg_log_medical_state:                   agents_.Get( asnMsg.msg.u.msg_log_medical_state->oid_pion ).Update( *asnMsg.msg.u.msg_log_medical_state ); break;

        case T_MsgsSimToClient_msg_msg_population_creation                       : CreateUpdate( populations_, asnMsg.msg.u.msg_population_creation->oid, *asnMsg.msg.u.msg_population_creation ); break;
        case T_MsgsSimToClient_msg_msg_population_update                         : populations_.Get( asnMsg.msg.u.msg_population_update->oid ).Update( *asnMsg.msg.u.msg_population_update ); break;
        case T_MsgsSimToClient_msg_msg_population_concentration_creation         : populations_.Get( asnMsg.msg.u.msg_population_concentration_creation->oid_population ).Update( *asnMsg.msg.u.msg_population_concentration_creation ); break;
        case T_MsgsSimToClient_msg_msg_population_concentration_destruction      : populations_.Get( asnMsg.msg.u.msg_population_concentration_destruction->oid_population ).Update( *asnMsg.msg.u.msg_population_concentration_destruction ); break;
        case T_MsgsSimToClient_msg_msg_population_concentration_update           : populations_.Get( asnMsg.msg.u.msg_population_concentration_update->oid_population ).Update( *asnMsg.msg.u.msg_population_concentration_update ); break;
        case T_MsgsSimToClient_msg_msg_population_flow_creation                  : populations_.Get( asnMsg.msg.u.msg_population_flow_creation->oid_population ).Update( *asnMsg.msg.u.msg_population_flow_creation ); break;
        case T_MsgsSimToClient_msg_msg_population_flow_destruction               : populations_.Get( asnMsg.msg.u.msg_population_flow_destruction->oid_population ).Update( *asnMsg.msg.u.msg_population_flow_destruction ); break;
        case T_MsgsSimToClient_msg_msg_population_flow_update                    : populations_.Get( asnMsg.msg.u.msg_population_flow_update->oid_population ).Update( *asnMsg.msg.u.msg_population_flow_update ); break;

        case T_MsgsSimToClient_msg_msg_population_knowledge_creation                  : CreateUpdate( populationKnowledges_, asnMsg.msg.u.msg_population_knowledge_creation->oid_connaissance, *asnMsg.msg.u.msg_population_knowledge_creation ); break;
        case T_MsgsSimToClient_msg_msg_population_knowledge_update                    : populationKnowledges_.Get( asnMsg.msg.u.msg_population_knowledge_update->oid_connaissance ).Update( *asnMsg.msg.u.msg_population_knowledge_update ); break;
        case T_MsgsSimToClient_msg_msg_population_knowledge_destruction               : populationKnowledges_.Destroy( asnMsg.msg.u.msg_population_knowledge_destruction->oid_connaissance ); break;
        case T_MsgsSimToClient_msg_msg_population_concentration_knowledge_creation    : populationKnowledges_.Get( asnMsg.msg.u.msg_population_concentration_knowledge_creation->oid_connaissance_population ).Update( *asnMsg.msg.u.msg_population_concentration_knowledge_creation ); break;
        case T_MsgsSimToClient_msg_msg_population_concentration_knowledge_update      : populationKnowledges_.Get( asnMsg.msg.u.msg_population_concentration_knowledge_update->oid_connaissance_population ).Update( *asnMsg.msg.u.msg_population_concentration_knowledge_update ); break;
        case T_MsgsSimToClient_msg_msg_population_concentration_knowledge_destruction : populationKnowledges_.Get( asnMsg.msg.u.msg_population_concentration_knowledge_destruction->oid_connaissance_population ).Update( *asnMsg.msg.u.msg_population_concentration_knowledge_destruction ); break;
        case T_MsgsSimToClient_msg_msg_population_flow_knowledge_creation             : populationKnowledges_.Get( asnMsg.msg.u.msg_population_flow_knowledge_creation->oid_connaissance_population ).Update( *asnMsg.msg.u.msg_population_flow_knowledge_creation ); break;
        case T_MsgsSimToClient_msg_msg_population_flow_knowledge_update               : populationKnowledges_.Get( asnMsg.msg.u.msg_population_flow_knowledge_update->oid_connaissance_population ).Update( *asnMsg.msg.u.msg_population_flow_knowledge_update ); break;
        case T_MsgsSimToClient_msg_msg_population_flow_knowledge_destruction          : populationKnowledges_.Get( asnMsg.msg.u.msg_population_flow_knowledge_destruction->oid_connaissance_population ).Update( *asnMsg.msg.u.msg_population_flow_knowledge_destruction ); break;

//        default:
//            assert( false );
    }
}

// -----------------------------------------------------------------------------
// Name: Model::CreateUpdate
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
template< typename T, typename P >
void Model::CreateUpdate( ModelsContainer< T >& container, unsigned id, const P& parameter )
{
    bool create = ! container.Find( id );
    T& object = container.Create( *this, id, parameter );
    if( synching_ )
        object.StartSynchronisation( create );
    object.Update( parameter );
}

// -----------------------------------------------------------------------------
// Name: Model::UpdateAnyAgent
// Created: ZEBRE 2007-06-21
// -----------------------------------------------------------------------------
template< typename T >
void Model::UpdateAnyAgent( unsigned id, const T& message )
{
    if( Agent* agent = agents_.Find( id ) )
        agent->Update( message );
    else if( Automat* automat = automats_.Find( id ) )
        automat->Update( message );
    else if( Population* popu = populations_.Find( id ) )
        popu->Update( message );
    else throw std::runtime_error( __FUNCTION__ " : Unknown entity" );
}

// -----------------------------------------------------------------------------
// Name: Model::Send
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void Model::Send( ClientPublisher_ABC& publisher ) const
{
    AsnMsgSimToClientControlSendCurrentStateBegin().Send( publisher );

    pSimulationModel_->Send( publisher );

    {
        CreationVisitor visitor( publisher );
        Accept( visitor );
    }
    {
        FullUpdateVisitor visitor( publisher );
        Accept( visitor );
    }

    AsnMsgSimToClientControlSendCurrentStateEnd().Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: Model::Accept
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void Model::Accept( ModelVisitor_ABC& visitor ) const
{
    sides_                 .Apply( std::mem_fun_ref( &Side::Accept                  ), visitor );
    agentKnowledges_       .Apply( std::mem_fun_ref( &AgentKnowledge::Accept        ), visitor );
    objectKnowledges_      .Apply( std::mem_fun_ref( &ObjectKnowledge::Accept       ), visitor );
    populationKnowledges_  .Apply( std::mem_fun_ref( &PopulationKnowledge::Accept   ), visitor );
    limits_                .Apply( std::mem_fun_ref( &Limit::Accept                 ), visitor );
    limas_                 .Apply( std::mem_fun_ref( &Lima ::Accept                 ), visitor );
    logConsignsMaintenance_.Apply( std::mem_fun_ref( &LogConsignMaintenance::Accept ), visitor );
    logConsignsSupply_     .Apply( std::mem_fun_ref( &LogConsignSupply     ::Accept ), visitor );
    logConsignsMedical_    .Apply( std::mem_fun_ref( &LogConsignMedical    ::Accept ), visitor );
    fires_                 .Apply( std::mem_fun_ref( &Fire                 ::Accept ), visitor );
    populationFires_       .Apply( std::mem_fun_ref( &PopulationFire       ::Accept ), visitor );
    fireEffects_           .Apply( std::mem_fun_ref( &FireEffect           ::Accept ), visitor );
}

// -----------------------------------------------------------------------------
// Name: Model::StartSynchronisation
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void Model::StartSynchronisation()
{
    synching_ = true;
    StartSynchVisitor visitor;
    Accept( visitor );
}

// -----------------------------------------------------------------------------
// Name: Model::EndSynchronisation
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void Model::EndSynchronisation( ClientPublisher_ABC& publisher )
{
    Synchroniser synch;
    EndSynchVisitor visitor( synch );
    Accept( visitor );
    synch.Commit( publisher, *this );
    synching_ = false;

    // $$$$ AGE 2007-04-24: Le commit va Update le Model et donc supprimer l'entité en question
    // $$$$ AGE 2007-04-24: c'est crado. Séparer l'envoi au client (dans le CommitDestruction() )
    // $$$$ AGE 2007-04-24: et la destruction (depuis la liste, séparément).
}

