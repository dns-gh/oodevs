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
#include "ClientPublisher_ABC.h"
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
#include "Visitors.h"
#include "Fire.h"
#include "PopulationFire.h"
#include "FireEffect.h"
#include "Synchroniser.h"
#include "FolkModel.h"
#include "Report.h"
#include "UrbanObject.h"
#include "SimulationModel.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/ObjectTypes.h"
#include "MT/MT_Logger/MT_Logger_lib.h"
#include <boost/bind.hpp>

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Model constructor
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
Model::Model( const tools::ExerciseConfig& config )
    : simulation_( new SimulationModel() )
    , folk_( new FolkModel() )
    , compositeFactory_( new CompositeFactory() )
    , agentTypes_( new kernel::AgentTypes( config ) )
	, objectTypes_( new kernel::ObjectTypes( config ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Model destructor
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
Model::~Model()
{
    Reset();
}

// -----------------------------------------------------------------------------
// Name: Model::Reset
// Created: NLD 2006-10-05
// -----------------------------------------------------------------------------
void Model::Reset()
{
    simulation_->Reset();
    fireEffects_            .DeleteAll();
    populationFires_        .DeleteAll();
    fires_                  .DeleteAll();
    agentKnowledges_        .DeleteAll();
    objectKnowledges_       .DeleteAll();
    populationKnowledges_   .DeleteAll();
    logConsignsMaintenance_ .DeleteAll();
    logConsignsSupply_      .DeleteAll();
    logConsignsMedical_     .DeleteAll();
    objects_                .DeleteAll();
    populations_            .DeleteAll();
    agents_                 .DeleteAll();
    automats_               .DeleteAll();
    knowledgeGroups_        .DeleteAll();
    formations_             .DeleteAll();
    sides_                  .DeleteAll();
    urbanBlocks_            .DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: Model::Receive
// Created: AGE 2007-07-05
// -----------------------------------------------------------------------------
void Model::Receive( const ASN1T_MsgsSimToClient& asn )
{
    if( asn.msg.t == T_MsgsSimToClient_msg_msg_control_send_current_state_begin )
    {
        Reset();
        MT_LOG_INFO_MSG( "Dispatcher - Initializing model" );
    }
    else if( asn.msg.t == T_MsgsSimToClient_msg_msg_control_send_current_state_end )
        MT_LOG_INFO_MSG( "Dispatcher - Model initialized" );
    Update( asn );
}

// -----------------------------------------------------------------------------
// Name: Model::Update
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
void Model::Update( const ASN1T_MsgsSimToClient& asn )
{
    switch( asn.msg.t )
    {
        case T_MsgsSimToClient_msg_msg_unit_order_ack:
        case T_MsgsSimToClient_msg_msg_automat_order_ack:
        case T_MsgsSimToClient_msg_msg_population_order_ack:
        case T_MsgsSimToClient_msg_msg_frag_order_ack:
        case T_MsgsSimToClient_msg_msg_set_automat_mode_ack:
        case T_MsgsSimToClient_msg_msg_unit_magic_action_ack:
        case T_MsgsSimToClient_msg_msg_object_magic_action_ack:
        case T_MsgsSimToClient_msg_msg_population_magic_action_ack:
            break; // NOTHING
        case T_MsgsSimToClient_msg_msg_change_diplomacy_ack:                     sides_.Get( asn.msg.u.msg_change_diplomacy_ack->oid_camp1 ).Update( *asn.msg.u.msg_change_diplomacy_ack ); break;
        case T_MsgsSimToClient_msg_msg_change_diplomacy:                         sides_.Get( asn.msg.u.msg_change_diplomacy->oid_camp1 ).Update( *asn.msg.u.msg_change_diplomacy ); break;
        case T_MsgsSimToClient_msg_msg_automat_change_logistic_links:            automats_.Get( asn.msg.u.msg_automat_change_logistic_links->oid ).Update( *asn.msg.u.msg_automat_change_logistic_links ); break;
        case T_MsgsSimToClient_msg_msg_automat_change_knowledge_group:           automats_.Get( asn.msg.u.msg_automat_change_knowledge_group->oid ).Update( *asn.msg.u.msg_automat_change_knowledge_group ); break;
        case T_MsgsSimToClient_msg_msg_automat_change_superior:                  automats_.Get( asn.msg.u.msg_automat_change_superior->oid ).Update( *asn.msg.u.msg_automat_change_superior ); break;
        case T_MsgsSimToClient_msg_msg_automat_change_logistic_links_ack:
        case T_MsgsSimToClient_msg_msg_automat_change_knowledge_group_ack:
        case T_MsgsSimToClient_msg_msg_automat_change_superior_ack:
        case T_MsgsSimToClient_msg_msg_unit_change_superior_ack:
            break; // NOTHING
        case T_MsgsSimToClient_msg_msg_unit_change_superior:                     agents_.Get( asn.msg.u.msg_unit_change_superior->oid ).Update( *asn.msg.u.msg_unit_change_superior ); break;
        case T_MsgsSimToClient_msg_msg_log_supply_push_flow_ack:
        case T_MsgsSimToClient_msg_msg_log_supply_change_quotas_ack:
            break; // NOTHING
        case T_MsgsSimToClient_msg_msg_control_information:                  simulation_->Update( *asn.msg.u.msg_control_information ); break;
        case T_MsgsSimToClient_msg_msg_control_begin_tick:                   simulation_->Update( *asn.msg.u.msg_control_begin_tick ); break;
        case T_MsgsSimToClient_msg_msg_control_end_tick:                     simulation_->Update( *asn.msg.u.msg_control_end_tick ); break;
        case T_MsgsSimToClient_msg_msg_control_stop_ack:                     simulation_->Update_Stop( asn.msg.u.msg_control_stop_ack ); break;
        case T_MsgsSimToClient_msg_msg_control_pause_ack:                    simulation_->Update_Pause( asn.msg.u.msg_control_pause_ack ); break;
        case T_MsgsSimToClient_msg_msg_control_resume_ack:                   simulation_->Update_Resume( asn.msg.u.msg_control_resume_ack ); break;
        case T_MsgsSimToClient_msg_msg_control_change_time_factor_ack:       simulation_->Update( *asn.msg.u.msg_control_change_time_factor_ack ); break;
        case T_MsgsSimToClient_msg_msg_control_date_time_change_ack:
        case T_MsgsSimToClient_msg_msg_control_global_meteo_ack:
        case T_MsgsSimToClient_msg_msg_control_local_meteo_ack:
        case T_MsgsSimToClient_msg_msg_control_checkpoint_save_begin:
        case T_MsgsSimToClient_msg_msg_control_checkpoint_save_end:
        case T_MsgsSimToClient_msg_msg_control_checkpoint_set_frequency_ack:
        case T_MsgsSimToClient_msg_msg_control_checkpoint_save_now_ack:
        case T_MsgsSimToClient_msg_msg_control_send_current_state_begin:
        case T_MsgsSimToClient_msg_msg_control_send_current_state_end:
            break;  // $$$$ AGE 2007-04-18: messages vides...
        case T_MsgsSimToClient_msg_msg_unit_knowledge_creation:              CreateUpdate( agentKnowledges_, *asn.msg.u.msg_unit_knowledge_creation ); break;
        case T_MsgsSimToClient_msg_msg_unit_knowledge_update:                agentKnowledges_.Get( asn.msg.u.msg_unit_knowledge_update->oid ).Update( *asn.msg.u.msg_unit_knowledge_update ); break;
        case T_MsgsSimToClient_msg_msg_unit_knowledge_destruction:           Destroy( agentKnowledges_, asn.msg.u.msg_unit_knowledge_destruction->oid ); break;
        case T_MsgsSimToClient_msg_msg_side_creation:                        CreateUpdate( sides_, *asn.msg.u.msg_side_creation ); break;
        case T_MsgsSimToClient_msg_msg_knowledge_group_creation:             CreateUpdate( knowledgeGroups_, *asn.msg.u.msg_knowledge_group_creation ); break;
        case T_MsgsSimToClient_msg_msg_formation_creation:                   CreateUpdate( formations_, *asn.msg.u.msg_formation_creation ); break;
        case T_MsgsSimToClient_msg_msg_unit_creation:                        CreateUpdate( agents_, *asn.msg.u.msg_unit_creation ); break;
        case T_MsgsSimToClient_msg_msg_unit_environment_type:                agents_.Get( asn.msg.u.msg_unit_environment_type->oid ).Update( *asn.msg.u.msg_unit_environment_type ); break;
        case T_MsgsSimToClient_msg_msg_unit_destruction:                     Destroy( agents_, asn.msg.u.msg_unit_destruction ); break;
        case T_MsgsSimToClient_msg_msg_automat_creation :                    CreateUpdate( automats_, *asn.msg.u.msg_automat_creation ); break;
        case T_MsgsSimToClient_msg_msg_unit_attributes:                      agents_.Get( asn.msg.u.msg_unit_attributes->oid ).Update( *asn.msg.u.msg_unit_attributes ); break;
        case T_MsgsSimToClient_msg_msg_automat_attributes:                   automats_.Get( asn.msg.u.msg_automat_attributes->oid ).Update( *asn.msg.u.msg_automat_attributes  ); break;
        case T_MsgsSimToClient_msg_msg_unit_pathfind:                        agents_.Get( asn.msg.u.msg_unit_pathfind->oid ).Update( *asn.msg.u.msg_unit_pathfind ); break;
        case T_MsgsSimToClient_msg_msg_start_unit_fire:                      CreateUpdate( fires_, asn.msg.u.msg_start_unit_fire->fire_oid, *asn.msg.u.msg_start_unit_fire );
                                                                             agents_.Get( asn.msg.u.msg_start_unit_fire->firer_oid ).Update( *asn.msg.u.msg_start_unit_fire ); break;
        case T_MsgsSimToClient_msg_msg_stop_unit_fire:                       Destroy( fires_, asn.msg.u.msg_stop_unit_fire->fire_oid ); break;
        case T_MsgsSimToClient_msg_msg_start_population_fire:                CreateUpdate( populationFires_, asn.msg.u.msg_start_population_fire->fire_oid, *asn.msg.u.msg_start_population_fire ); break;
        case T_MsgsSimToClient_msg_msg_stop_population_fire:                 Destroy( populationFires_, asn.msg.u.msg_stop_population_fire->fire_oid ); break;
        case T_MsgsSimToClient_msg_msg_explosion:
            break;
        case T_MsgsSimToClient_msg_msg_report:                               CreateUpdate( reports_, asn.msg.u.msg_report->cr_oid, *asn.msg.u.msg_report ); break;
        case T_MsgsSimToClient_msg_msg_invalidate_report:                    Destroy( reports_, asn.msg.u.msg_invalidate_report->cr_oid ); break;
        case T_MsgsSimToClient_msg_msg_trace:
            break;  // $$$$ AGE 2007-04-18: Evenements, modèle client => rien, ou remanier
        case T_MsgsSimToClient_msg_msg_unit_detection:                       agents_.Get( asn.msg.u.msg_unit_detection->oid ).Update( *asn.msg.u.msg_unit_detection ); break;
        case T_MsgsSimToClient_msg_msg_object_detection:                     agents_.Get( asn.msg.u.msg_object_detection->oid ).Update( *asn.msg.u.msg_object_detection ); break;
        case T_MsgsSimToClient_msg_msg_decisional_state:                     UpdateAnyAgent( asn.msg.u.msg_decisional_state->oid, *asn.msg.u.msg_decisional_state ); break;
        case T_MsgsSimToClient_msg_msg_start_fire_effect:                    CreateUpdate( fireEffects_, asn.msg.u.msg_start_fire_effect->effect_oid, *asn.msg.u.msg_start_fire_effect ); break;
        case T_MsgsSimToClient_msg_msg_stop_fire_effect:                     Destroy( fireEffects_, asn.msg.u.msg_stop_fire_effect ); break;
        case T_MsgsSimToClient_msg_msg_unit_order:                           agents_     .Get( asn.msg.u.msg_unit_order       ->oid ).Update( *asn.msg.u.msg_unit_order ); break;
        case T_MsgsSimToClient_msg_msg_automat_order:                        automats_   .Get( asn.msg.u.msg_automat_order    ->oid ).Update( *asn.msg.u.msg_automat_order ); break;
        case T_MsgsSimToClient_msg_msg_population_order:                     populations_.Get( asn.msg.u.msg_population_order ->oid ).Update( *asn.msg.u.msg_population_order ); break;

        case T_MsgsSimToClient_msg_msg_object_creation:                      CreateUpdate( objects_, asn.msg.u.msg_object_creation->oid, *asn.msg.u.msg_object_creation ); break;
        case T_MsgsSimToClient_msg_msg_object_update:                        objects_.Get( asn.msg.u.msg_object_update->oid ).Update( *asn.msg.u.msg_object_update ); break;
        case T_MsgsSimToClient_msg_msg_object_destruction:                   Destroy( objects_, asn.msg.u.msg_object_destruction ); break;
        case T_MsgsSimToClient_msg_msg_object_knowledge_creation:            CreateUpdate( objectKnowledges_, asn.msg.u.msg_object_knowledge_creation->oid, *asn.msg.u.msg_object_knowledge_creation ); break;
        case T_MsgsSimToClient_msg_msg_object_knowledge_update:              objectKnowledges_.Get( asn.msg.u.msg_object_knowledge_update->oid ).Update( *asn.msg.u.msg_object_knowledge_update ); break;
        case T_MsgsSimToClient_msg_msg_object_knowledge_destruction:         Destroy( objectKnowledges_, asn.msg.u.msg_object_knowledge_destruction->oid ); break;

        case T_MsgsSimToClient_msg_msg_log_maintenance_handling_creation:    CreateUpdate( logConsignsMaintenance_, asn.msg.u.msg_log_maintenance_handling_creation->oid_consigne, *asn.msg.u.msg_log_maintenance_handling_creation ); break;
        case T_MsgsSimToClient_msg_msg_log_maintenance_handling_destruction: Destroy( logConsignsMaintenance_, asn.msg.u.msg_log_maintenance_handling_destruction->oid_consigne ); break;
        case T_MsgsSimToClient_msg_msg_log_maintenance_handling_update:      logConsignsMaintenance_.Get( asn.msg.u.msg_log_maintenance_handling_update->oid_consigne ).Update( *asn.msg.u.msg_log_maintenance_handling_update ); break;
        case T_MsgsSimToClient_msg_msg_log_maintenance_state:                agents_.Get( asn.msg.u.msg_log_maintenance_state->oid_pion ).Update( *asn.msg.u.msg_log_maintenance_state ); break;

        case T_MsgsSimToClient_msg_msg_log_supply_handling_creation:        CreateUpdate( logConsignsSupply_, asn.msg.u.msg_log_supply_handling_creation->oid_consigne, *asn.msg.u.msg_log_supply_handling_creation ); break;
        case T_MsgsSimToClient_msg_msg_log_supply_handling_destruction:     Destroy( logConsignsSupply_, asn.msg.u.msg_log_supply_handling_destruction->oid_consigne ); break;
        case T_MsgsSimToClient_msg_msg_log_supply_handling_update:          logConsignsSupply_.Get( asn.msg.u.msg_log_supply_handling_update->oid_consigne ).Update( *asn.msg.u.msg_log_supply_handling_update ); break;
        case T_MsgsSimToClient_msg_msg_log_supply_state:                    agents_.Get( asn.msg.u.msg_log_medical_state->oid_pion ).Update( *asn.msg.u.msg_log_supply_state); break;
        case T_MsgsSimToClient_msg_msg_log_supply_quotas:                   automats_.Get( asn.msg.u.msg_log_supply_quotas->oid_automate ).Update( *asn.msg.u.msg_log_supply_quotas ); break;

        case T_MsgsSimToClient_msg_msg_log_medical_handling_creation:       CreateUpdate( logConsignsMedical_, asn.msg.u.msg_log_medical_handling_creation->oid_consigne, *asn.msg.u.msg_log_medical_handling_creation ); break;
        case T_MsgsSimToClient_msg_msg_log_medical_handling_destruction:    Destroy( logConsignsMedical_, asn.msg.u.msg_log_medical_handling_destruction->oid_consigne ); break;
        case T_MsgsSimToClient_msg_msg_log_medical_handling_update:         logConsignsMedical_.Get( asn.msg.u.msg_log_medical_handling_update->oid_consigne ).Update( *asn.msg.u.msg_log_medical_handling_update ); break;
        case T_MsgsSimToClient_msg_msg_log_medical_state:                   agents_.Get( asn.msg.u.msg_log_medical_state->oid_pion ).Update( *asn.msg.u.msg_log_medical_state ); break;

        case T_MsgsSimToClient_msg_msg_population_creation                       : CreateUpdate( populations_, *asn.msg.u.msg_population_creation ); break;
        case T_MsgsSimToClient_msg_msg_population_update                         : populations_.Get( asn.msg.u.msg_population_update->oid ).Update( *asn.msg.u.msg_population_update ); break;
        case T_MsgsSimToClient_msg_msg_population_concentration_creation         : populations_.Get( asn.msg.u.msg_population_concentration_creation->oid_population ).Update( *asn.msg.u.msg_population_concentration_creation ); break;
        case T_MsgsSimToClient_msg_msg_population_concentration_destruction      : populations_.Get( asn.msg.u.msg_population_concentration_destruction->oid_population ).Update( *asn.msg.u.msg_population_concentration_destruction ); break;
        case T_MsgsSimToClient_msg_msg_population_concentration_update           : populations_.Get( asn.msg.u.msg_population_concentration_update->oid_population ).Update( *asn.msg.u.msg_population_concentration_update ); break;
        case T_MsgsSimToClient_msg_msg_population_flow_creation                  : populations_.Get( asn.msg.u.msg_population_flow_creation->oid_population ).Update( *asn.msg.u.msg_population_flow_creation ); break;
        case T_MsgsSimToClient_msg_msg_population_flow_destruction               : populations_.Get( asn.msg.u.msg_population_flow_destruction->oid_population ).Update( *asn.msg.u.msg_population_flow_destruction ); break;
        case T_MsgsSimToClient_msg_msg_population_flow_update                    : populations_.Get( asn.msg.u.msg_population_flow_update->oid_population ).Update( *asn.msg.u.msg_population_flow_update ); break;

        case T_MsgsSimToClient_msg_msg_population_knowledge_creation                  : CreateUpdate( populationKnowledges_, asn.msg.u.msg_population_knowledge_creation->oid_connaissance, *asn.msg.u.msg_population_knowledge_creation ); break;
        case T_MsgsSimToClient_msg_msg_population_knowledge_update                    : populationKnowledges_.Get( asn.msg.u.msg_population_knowledge_update->oid_connaissance ).Update( *asn.msg.u.msg_population_knowledge_update ); break;
        case T_MsgsSimToClient_msg_msg_population_knowledge_destruction               : Destroy( populationKnowledges_, asn.msg.u.msg_population_knowledge_destruction->oid_connaissance ); break;
        case T_MsgsSimToClient_msg_msg_population_concentration_knowledge_creation    : populationKnowledges_.Get( asn.msg.u.msg_population_concentration_knowledge_creation->oid_connaissance_population ).Update( *asn.msg.u.msg_population_concentration_knowledge_creation ); break;
        case T_MsgsSimToClient_msg_msg_population_concentration_knowledge_update      : populationKnowledges_.Get( asn.msg.u.msg_population_concentration_knowledge_update->oid_connaissance_population ).Update( *asn.msg.u.msg_population_concentration_knowledge_update ); break;
        case T_MsgsSimToClient_msg_msg_population_concentration_knowledge_destruction : populationKnowledges_.Get( asn.msg.u.msg_population_concentration_knowledge_destruction->oid_connaissance_population ).Update( *asn.msg.u.msg_population_concentration_knowledge_destruction ); break;
        case T_MsgsSimToClient_msg_msg_population_flow_knowledge_creation             : populationKnowledges_.Get( asn.msg.u.msg_population_flow_knowledge_creation->oid_connaissance_population ).Update( *asn.msg.u.msg_population_flow_knowledge_creation ); break;
        case T_MsgsSimToClient_msg_msg_population_flow_knowledge_update               : populationKnowledges_.Get( asn.msg.u.msg_population_flow_knowledge_update->oid_connaissance_population ).Update( *asn.msg.u.msg_population_flow_knowledge_update ); break;
        case T_MsgsSimToClient_msg_msg_population_flow_knowledge_destruction          : populationKnowledges_.Get( asn.msg.u.msg_population_flow_knowledge_destruction->oid_connaissance_population ).Update( *asn.msg.u.msg_population_flow_knowledge_destruction ); break;
        case T_MsgsSimToClient_msg_msg_folk_creation                                  : folk_->Update( *asn.msg.u.msg_folk_creation ); break;

        case T_MsgsSimToClient_msg_msg_urban_creation                                 : CreateUpdate( urbanBlocks_, *asn.msg.u.msg_urban_creation ); break;
//        default:
//            assert( false );//@TODO restore an exception, some messages aren't linked
    }
}

// -----------------------------------------------------------------------------
// Name: Model::CreateUpdate
// Created: SBO 2008-07-09
// -----------------------------------------------------------------------------
template< typename T, typename P >
void Model::CreateUpdate( tools::Resolver< T >& resolver, const P& parameter )
{
    CreateUpdate( resolver, parameter.oid, parameter );
}

// -----------------------------------------------------------------------------
// Name: Model::CreateUpdate
// Created: SBO 2008-07-09
// -----------------------------------------------------------------------------
template< typename T, typename P >
void Model::CreateUpdate( tools::Resolver< T >& resolver, unsigned id, const P& parameter )
{
    T* pElement = resolver.Find( id );
    if( !pElement )
    {
        pElement = new T( *this, parameter );
        AddExtensions( *pElement );
        resolver.Register( pElement->GetId(), *pElement );
    }
    pElement->Update( parameter );
}

// -----------------------------------------------------------------------------
// Name: Model::UpdateAnyAgent
// Created: ZEBRE 2007-06-21
// -----------------------------------------------------------------------------
template< typename T >
void Model::UpdateAnyAgent( unsigned id, const T& message )
{
    if( kernel::Agent_ABC* agent = agents_.Find( id ) )
        agent->Update( message );
    else if( kernel::Automat_ABC* automat = automats_.Find( id ) )
        automat->Update( message );
    else if( kernel::Population_ABC* popu = populations_.Find( id ) )
        popu->Update( message );
    else throw std::runtime_error( __FUNCTION__ " : Unknown entity" );
}

// -----------------------------------------------------------------------------
// Name: Model::Destroy
// Created: SBO 2008-07-09
// -----------------------------------------------------------------------------
template< typename T >
void Model::Destroy( tools::Resolver< T >& resolver, unsigned id )
{
    if( T* entity = resolver.Find( id ) )
    {
        resolver.Remove( id );
        delete entity;
    }
}

// -----------------------------------------------------------------------------
// Name: Model::Send
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void Model::Send( ClientPublisher_ABC& publisher ) const
{
    client::ControlSendCurrentStateBegin().Send( publisher );

    simulation_->Send( publisher );
    folk_->SendCreation( publisher );
    {
        CreationVisitor visitor( publisher );
        Accept( visitor );
    }
    {
        FullUpdateVisitor visitor( publisher );
        Accept( visitor );
    }

    client::ControlSendCurrentStateEnd().Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: Model::SendFirstTick
// Created: AGE 2007-10-15
// -----------------------------------------------------------------------------
void Model::SendFirstTick( ClientPublisher_ABC& publisher ) const
{
    simulation_->SendFirstTick( publisher );
}

// -----------------------------------------------------------------------------
// Name: Model::SendReplayInfo
// Created: AGE 2007-10-15
// -----------------------------------------------------------------------------
void Model::SendReplayInfo( ClientPublisher_ABC& publisher, unsigned totalTicks, ASN1T_EnumSimulationState status, unsigned factor ) const
{
    simulation_->SendReplayInfo( publisher, totalTicks, status, factor );
}

// -----------------------------------------------------------------------------
// Name: Model::Accept
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void Model::Accept( ModelVisitor_ABC& visitor ) const
{
    sides_                 .Apply( boost::bind( &Side::Accept, _1, boost::ref( visitor ) ) );
    agentKnowledges_       .Apply( boost::bind( &AgentKnowledge::Accept, _1, boost::ref( visitor ) ) );
    objectKnowledges_      .Apply( boost::bind( &ObjectKnowledge::Accept, _1, boost::ref( visitor ) ) );
    populationKnowledges_  .Apply( boost::bind( &PopulationKnowledge::Accept, _1, boost::ref( visitor ) ) );
    logConsignsMaintenance_.Apply( boost::bind( &LogConsignMaintenance::Accept, _1, boost::ref( visitor ) ) );
    logConsignsSupply_     .Apply( boost::bind( &LogConsignSupply::Accept, _1, boost::ref( visitor ) ) );
    logConsignsMedical_    .Apply( boost::bind( &LogConsignMedical::Accept, _1, boost::ref( visitor ) ) );
    fires_                 .Apply( boost::bind( &Fire::Accept, _1, boost::ref( visitor ) ) );
    populationFires_       .Apply( boost::bind( &PopulationFire::Accept, _1, boost::ref( visitor ) ) );
    fireEffects_           .Apply( boost::bind( &FireEffect::Accept, _1, boost::ref( visitor ) ) );
    reports_               .Apply( boost::bind( &Report::Accept, _1, boost::ref( visitor ) ) );
    urbanBlocks_           .Apply( boost::bind( &UrbanObject::Accept, _1, boost::ref( visitor ) ) );
}

// -----------------------------------------------------------------------------
// Name: tools::Resolver_ABC< kernel::AgentType >& Model::GetAgentTypes
// Created: AGE 2008-03-14
// -----------------------------------------------------------------------------
const tools::Resolver_ABC< kernel::AgentType >& Model::GetAgentTypes() const
{
    return *agentTypes_;
}

// -----------------------------------------------------------------------------
// Name: tools::Resolver_ABC< kernel::MissionType >& Model::GetMissionTypes
// Created: SBO 2008-05-21
// -----------------------------------------------------------------------------
const tools::Resolver_ABC< kernel::MissionType >& Model::GetMissionTypes() const
{
    return *agentTypes_;
}

// -----------------------------------------------------------------------------
// Name: tools::Resolver_ABC< kernel::FragOrderType >& Model::GetFragOrderTypes
// Created: AGE 2008-07-16
// -----------------------------------------------------------------------------
const tools::Resolver_ABC< kernel::FragOrderType >& Model::GetFragOrderTypes() const
{
    return *agentTypes_;
}

// -----------------------------------------------------------------------------
// Name: Model::GetFragOrderTypes
// Created: AGE 2008-07-16
// -----------------------------------------------------------------------------
const tools::Resolver_ABC< kernel::ObjectType, std::string >& Model::GetObjectTypes() const
{
	return *objectTypes_;
}

// -----------------------------------------------------------------------------
// Name: Model::RegisterFactory
// Created: SBO 2008-07-09
// -----------------------------------------------------------------------------
void Model::RegisterFactory( Factory_ABC& factory )
{
    compositeFactory_->Register( factory );
}

// -----------------------------------------------------------------------------
// Name: Model::UnregisterFactory
// Created: SBO 2008-07-09
// -----------------------------------------------------------------------------
void Model::UnregisterFactory( Factory_ABC& factory )
{
    compositeFactory_->Unregister( factory );
}
