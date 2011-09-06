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
#include "Agent.h"
#include "AgentKnowledge.h"
#include "Automat.h"
#include "Config.h"
#include "Fire.h"
#include "FireEffect.h"
#include "FolkModel.h"
#include "Formation.h"
#include "KnowledgeGroup.h"
#include "LogConsignMaintenance.h"
#include "LogConsignSupply.h"
#include "LogConsignMedical.h"
#include "MeteoModel.h"
#include "Object.h"
#include "ObjectKnowledge.h"
#include "Inhabitant.h"
#include "Population.h"
#include "PopulationFlow.h"
#include "PopulationConcentration.h"
#include "PopulationFire.h"
#include "PopulationKnowledge.h"
#include "Report.h"
#include "Side.h"
#include "SimulationModel.h"
#include "UrbanKnowledge.h"
#include "UrbanObject.h"
#include "Visitors.h"
#include "MemoryLogger_ABC.h"
#include "EntityPublisher.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/ClientSenders.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/FormationLevels.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/StaticModel.h"
#include "MT_Tools/MT_Logger.h"
#include <boost/bind.hpp>
#include <boost/filesystem.hpp>
#include <xeumeuleu/xml.hpp>
#pragma warning( disable : 4503 4355 )

inline std::ostream& operator<<( std::ostream& os, const QString& s )
{
    return os << s.toStdString();
}

inline xml::xostream& operator<<( xml::xostream& xos, const QString& s )
{
    return xos << s.toStdString();
}

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Model constructor
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
Model::Model( const Config& config, const kernel::StaticModel& staticModel, MemoryLogger_ABC& logger )
    : staticModel_     ( staticModel )
    , logger_          ( logger )
    , simulation_      ( new SimulationModel( config ) )
    , compositeFactory_( new CompositeFactory() )
    , folk_            ( new FolkModel() )
    , meteoModel_      ( new MeteoModel( staticModel.coordinateConverter_, config, *this ) )
    , config_          ( config )
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
    fireEffects_           .DeleteAll();
    populationFires_       .DeleteAll();
    fires_                 .DeleteAll();
    agentKnowledges_       .DeleteAll();
    objectKnowledges_      .DeleteAll();
    populationKnowledges_  .DeleteAll();
    logConsignsMaintenance_.DeleteAll();
    logConsignsSupply_     .DeleteAll();
    logConsignsMedical_    .DeleteAll();
    objects_               .DeleteAll();
    inhabitants_           .DeleteAll();
    populations_           .DeleteAll();
    agents_                .DeleteAll();
    automats_              .DeleteAll();
    knowledgeGroups_       .DeleteAll();
    formations_            .DeleteAll();
    sides_                 .DeleteAll();
    urbanBlocks_           .DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: Model::Receive
// Created: AGE 2007-07-05
// -----------------------------------------------------------------------------
void Model::Receive( const sword::SimToClient& wrapper )
{
    if( wrapper.message().has_control_send_current_state_begin() )
    {
        Reset();
        MT_LOG_INFO_MSG( "Dispatcher - Initializing model" );
    }
    else if( wrapper.message().has_control_send_current_state_end() )
        MT_LOG_INFO_MSG( "Dispatcher - Model initialized" );
    try
    {
        Update( wrapper );
    }
    catch( std::runtime_error& e )
    {
        MT_LOG_ERROR_MSG( e.what() );
    }
}

// -----------------------------------------------------------------------------
// Name: Model::Update
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
void Model::Update( const sword::SimToClient& wrapper )
{
    if( wrapper.message().has_order_ack() ||
        wrapper.message().has_frag_order_ack() ||
        wrapper.message().has_set_automat_mode_ack() ||
        wrapper.message().has_unit_magic_action_ack() ||
        wrapper.message().has_object_magic_action_ack() ||
        wrapper.message().has_crowd_magic_action_ack() )
        { // NOTHING
        }
    else if( wrapper.message().has_change_diplomacy_ack() )
        sides_.Get( wrapper.message().change_diplomacy_ack().party1().id() ).Update( wrapper.message().change_diplomacy_ack() );
    else if( wrapper.message().has_change_diplomacy() )
        sides_.Get( wrapper.message().change_diplomacy().party1().id() ).Update( wrapper.message().change_diplomacy() );
    else if( wrapper.message().has_automat_change_logistic_links() )
    {
        if( wrapper.message().automat_change_logistic_links().requester().has_automat() )
            automats_.Get( wrapper.message().automat_change_logistic_links().requester().automat().id() ).Update( wrapper.message().automat_change_logistic_links() );
        else
            formations_.Get( wrapper.message().automat_change_logistic_links().requester().formation().id() ).Update( wrapper.message().automat_change_logistic_links() );
    }
    else if( wrapper.message().has_automat_change_knowledge_group() )
        automats_.Get( wrapper.message().automat_change_knowledge_group().automat().id() ).Update( wrapper.message().automat_change_knowledge_group() );
    else if( wrapper.message().has_automat_change_superior() )
        automats_.Get( wrapper.message().automat_change_superior().automat().id() ).Update( wrapper.message().automat_change_superior() );
    else if( wrapper.message().has_automat_change_logistic_links_ack() ||
        wrapper.message().has_automat_change_knowledge_group_ack() ||
        wrapper.message().has_automat_change_superior_ack() ||
        wrapper.message().has_unit_change_superior_ack() )
        { // NOTHING
        }
    else if( wrapper.message().has_unit_change_superior() )
        agents_.Get( wrapper.message().unit_change_superior().unit().id() ).Update( wrapper.message().unit_change_superior() );
    else if( wrapper.message().has_log_supply_push_flow_ack() ||
        wrapper.message().has_log_supply_pull_flow_ack() ||
        wrapper.message().has_log_supply_change_quotas_ack() )
        { // NOTHING
        }
    else if( wrapper.message().has_control_information() )
        simulation_->Update( wrapper.message().control_information() );
    else if( wrapper.message().has_control_begin_tick() )
        simulation_->Update( wrapper.message().control_begin_tick() );
    else if( wrapper.message().has_control_end_tick() )
    {
        const sword::ControlEndTick& message = wrapper.message().control_end_tick();
        logger_.Update( message );
        simulation_->Update( message );
    }
    else if( wrapper.message().has_control_stop_ack() )
        simulation_->Update_Stop( wrapper.message().control_stop_ack() );
    else if( wrapper.message().has_control_pause_ack() )
        simulation_->Update_Pause( wrapper.message().control_pause_ack() );
    else if( wrapper.message().has_control_resume_ack() )
        simulation_->Update_Resume( wrapper.message().control_resume_ack() );
    else if( wrapper.message().has_control_change_time_factor_ack() )
        simulation_->Update( wrapper.message().control_change_time_factor_ack() );
    else if( wrapper.message().has_control_date_time_change_ack() ||
        wrapper.message().has_control_global_weather_ack() ||
        wrapper.message().has_control_local_weather_ack() ||
        wrapper.message().has_control_checkpoint_save_begin() ||
        wrapper.message().has_control_checkpoint_save_end() ||
        wrapper.message().has_control_checkpoint_set_frequency_ack() ||
        wrapper.message().has_control_checkpoint_save_now_ack() ||
        wrapper.message().has_control_send_current_state_begin() ||
        wrapper.message().has_control_send_current_state_end() )
        { // NOTHING // $$$$ AGE 2007-04-18: messages vides...
        }
    else if( wrapper.message().has_control_checkpoint_save_delete() )
        DeleteCheckpoint( wrapper.message().control_checkpoint_save_delete().name() );
    else if( wrapper.message().has_unit_knowledge_creation() )
        CreateUpdate< AgentKnowledge >( agentKnowledges_, wrapper.message().unit_knowledge_creation().knowledge().id(), wrapper.message().unit_knowledge_creation() );
    else if( wrapper.message().has_unit_knowledge_update() )
        agentKnowledges_.Get( wrapper.message().unit_knowledge_update().knowledge().id() ).Update( wrapper.message().unit_knowledge_update() );
    else if( wrapper.message().has_unit_knowledge_destruction() )
        Destroy( agentKnowledges_, wrapper.message().unit_knowledge_destruction().knowledge().id(), wrapper.message().unit_knowledge_destruction() );
    else if( wrapper.message().has_party_creation() )
        CreateUpdate< Side >( sides_, wrapper.message().party_creation().party().id(), wrapper.message().party_creation() );
    else if( wrapper.message().has_knowledge_group_creation() )
        CreateUpdate< KnowledgeGroup >( knowledgeGroups_, wrapper.message().knowledge_group_creation().knowledge_group().id(), wrapper.message().knowledge_group_creation() );
    else if( wrapper.message().has_knowledge_group_destruction() )
        Destroy( knowledgeGroups_, wrapper.message().knowledge_group_destruction().knowledge_group().id(), wrapper.message().knowledge_group_destruction() );
    else if( wrapper.message().has_knowledge_group_update() )
        knowledgeGroups_.Get( wrapper.message().knowledge_group_update().knowledge_group().id() ).Update( wrapper.message().knowledge_group_update() );
    else if( wrapper.message().has_formation_creation() )
        CreateUpdate< Formation >( formations_, wrapper.message().formation_creation().formation().id(), wrapper.message().formation_creation(), staticModel_.levels_ );
    else if( wrapper.message().has_formation_update() )
        formations_.Get( wrapper.message().formation_update().formation().id() ).Update( wrapper.message().formation_update() );
    else if( wrapper.message().has_formation_destruction() )
        Destroy( formations_, wrapper.message().formation_destruction().formation().id(), wrapper.message().formation_destruction() );
    else if( wrapper.message().has_unit_creation() )
        CreateUpdate< Agent >( agents_, wrapper.message().unit_creation().unit().id(), wrapper.message().unit_creation(), staticModel_.types_ );
    else if( wrapper.message().has_unit_environment_type() )
        agents_.Get( wrapper.message().unit_environment_type().unit().id() ).Update( wrapper.message().unit_environment_type() );
    else if( wrapper.message().has_unit_destruction() )
        Destroy( agents_, wrapper.message().unit_destruction().unit().id(), wrapper.message().unit_destruction() );
    else if( wrapper.message().has_automat_creation() )
        CreateUpdate< Automat >( automats_, wrapper.message().automat_creation().automat().id(), wrapper.message().automat_creation() );
    else if( wrapper.message().has_automat_destruction() )
        Destroy( automats_, wrapper.message().automat_destruction().automat().id(), wrapper.message().automat_destruction() );
    else if( wrapper.message().has_unit_attributes() )
        agents_.Get( wrapper.message().unit_attributes().unit().id() ).Update( wrapper.message().unit_attributes() );
    else if( wrapper.message().has_automat_attributes() )
        automats_.Get( wrapper.message().automat_attributes().automat().id() ).Update( wrapper.message().automat_attributes() );
    else if( wrapper.message().has_unit_pathfind() )
        agents_.Get( wrapper.message().unit_pathfind().unit().id() ).Update( wrapper.message().unit_pathfind() );
    else if( wrapper.message().has_start_unit_fire() )
    {
        CreateUpdate< Fire >( fires_, wrapper.message().start_unit_fire().fire().id(), wrapper.message().start_unit_fire() );
        agents_.Get( wrapper.message().start_unit_fire().firing_unit().id() ).Update( wrapper.message().start_unit_fire() );
    }
    else if( wrapper.message().has_stop_unit_fire() )
        Destroy( fires_, wrapper.message().stop_unit_fire().fire().id(), wrapper.message().stop_unit_fire() );
    else if( wrapper.message().has_start_crowd_fire() )
        CreateUpdate< PopulationFire >( populationFires_, wrapper.message().start_crowd_fire().fire().id(), wrapper.message().start_crowd_fire() );
    else if( wrapper.message().has_stop_crowd_fire() )
        Destroy( populationFires_, wrapper.message().stop_crowd_fire().fire().id(), wrapper.message().stop_crowd_fire() );
    else if( wrapper.message().has_explosion() )
    {} // $$$$ merge


    else if( wrapper.message().has_report() )
        CreateUpdate< Report >( reports_, wrapper.message().report().report().id(), wrapper.message().report() );
    else if( wrapper.message().has_invalidate_report() )
        Destroy( reports_, wrapper.message().invalidate_report().report().id(), wrapper.message().invalidate_report() );
    else if( wrapper.message().has_trace() )
    {
        // $$$$ AGE 2007-04-18: Evenements, modèle client => rien, ou remanier
    }
    else if( wrapper.message().has_unit_detection() )
        agents_.Get( wrapper.message().unit_detection().observer().id() ).Update( wrapper.message().unit_detection() );
    else if( wrapper.message().has_object_detection() )
        agents_.Get( wrapper.message().object_detection().observer().id() ).Update( wrapper.message().object_detection() );
    else if( wrapper.message().has_decisional_state() )
        UpdateAnyAgent( TaskerToId( wrapper.message().decisional_state().source() ), wrapper.message().decisional_state() );
    else if( wrapper.message().has_start_fire_effect() )
        CreateUpdate< FireEffect >( fireEffects_, wrapper.message().start_fire_effect().fire_effect().id(), wrapper.message().start_fire_effect() );
    else if( wrapper.message().has_stop_fire_effect() )
        Destroy( fireEffects_, wrapper.message().stop_fire_effect().fire_effect().id(), wrapper.message().stop_fire_effect() );
    else if( wrapper.message().has_unit_order() )
        agents_.Get( wrapper.message().unit_order().tasker().id() ).Update( wrapper.message().unit_order() );
    else if( wrapper.message().has_automat_order() )
        automats_.Get( wrapper.message().automat_order().tasker().id() ).Update( wrapper.message().automat_order() );
    else if( wrapper.message().has_crowd_order() )
        populations_.Get( wrapper.message().crowd_order().tasker().id() ).Update( wrapper.message().crowd_order() );

    else if( wrapper.message().has_object_creation() )
        CreateUpdate< Object >( objects_, wrapper.message().object_creation().object().id(), wrapper.message().object_creation(), staticModel_.objectTypes_ );
    else if( wrapper.message().has_object_update() )
    {
        if( objects_.Find( wrapper.message().object_update().object().id() ) )
            objects_.Get( wrapper.message().object_update().object().id() ).Update( wrapper.message().object_update() );
        else if( urbanBlocks_.Find( wrapper.message().object_update().object().id() ) )
            urbanBlocks_.Get( wrapper.message().object_update().object().id() ).Update( wrapper.message().object_update() );
    }
    else if( wrapper.message().has_object_destruction() )
        Destroy( objects_, wrapper.message().object_destruction().object().id(), wrapper.message().object_destruction() );
    else if( wrapper.message().has_object_knowledge_creation() )
        CreateUpdate< ObjectKnowledge >( objectKnowledges_, wrapper.message().object_knowledge_creation().knowledge().id(), wrapper.message().object_knowledge_creation() );
    else if( wrapper.message().has_object_knowledge_update() )
        objectKnowledges_.Get( wrapper.message().object_knowledge_update().knowledge().id() ).Update( wrapper.message().object_knowledge_update() );
    else if( wrapper.message().has_object_knowledge_destruction() )
        Destroy( objectKnowledges_, wrapper.message().object_knowledge_destruction().knowledge().id(), wrapper.message().object_knowledge_destruction() );

    else if( wrapper.message().has_log_maintenance_handling_creation() )
        CreateUpdate< LogConsignMaintenance >( logConsignsMaintenance_, wrapper.message().log_maintenance_handling_creation().request().id(), wrapper.message().log_maintenance_handling_creation() );
    else if( wrapper.message().has_log_maintenance_handling_destruction() )
        Destroy( logConsignsMaintenance_, wrapper.message().log_maintenance_handling_destruction().request().id(), wrapper.message().log_maintenance_handling_destruction() );
    else if( wrapper.message().has_log_maintenance_handling_update() )
        logConsignsMaintenance_.Get( wrapper.message().log_maintenance_handling_update().request().id() ).Update( wrapper.message().log_maintenance_handling_update() );
    else if( wrapper.message().has_log_maintenance_state() )
        agents_.Get( wrapper.message().log_maintenance_state().unit().id() ).Update( wrapper.message().log_maintenance_state() );

    else if( wrapper.message().has_log_supply_handling_creation() )
        CreateUpdate< LogConsignSupply >( logConsignsSupply_, wrapper.message().log_supply_handling_creation().request().id(), wrapper.message().log_supply_handling_creation() );
    else if( wrapper.message().has_log_supply_handling_destruction() )
        Destroy( logConsignsSupply_, wrapper.message().log_supply_handling_destruction().request().id(), wrapper.message().log_supply_handling_destruction() );
    else if( wrapper.message().has_log_supply_handling_update() )
        logConsignsSupply_.Get( wrapper.message().log_supply_handling_update().request().id() ).Update( wrapper.message().log_supply_handling_update() );
    else if( wrapper.message().has_log_supply_state() )
        agents_.Get( wrapper.message().log_supply_state().unit().id() ).Update( wrapper.message().log_supply_state() );
    else if( wrapper.message().has_log_supply_quotas() )
    {
        if( wrapper.message().log_supply_quotas().supplied().has_automat() )
            automats_.Get( wrapper.message().log_supply_quotas().supplied().automat().id() ).Update( wrapper.message().log_supply_quotas() );
        else
            formations_.Get( wrapper.message().log_supply_quotas().supplied().formation().id() ).Update( wrapper.message().log_supply_quotas() );
    }
    else if( wrapper.message().has_log_medical_handling_creation() )
        CreateUpdate< LogConsignMedical >( logConsignsMedical_, wrapper.message().log_medical_handling_creation().request().id(),  wrapper.message().log_medical_handling_creation() );
    else if( wrapper.message().has_log_medical_handling_destruction() )
        Destroy( logConsignsMedical_, wrapper.message().log_medical_handling_destruction().request().id(), wrapper.message().log_medical_handling_destruction() );
    else if( wrapper.message().has_log_medical_handling_update() )
        logConsignsMedical_.Get( wrapper.message().log_medical_handling_update().request().id() ).Update( wrapper.message().log_medical_handling_update() );
    else if( wrapper.message().has_log_medical_state() )
        agents_.Get( wrapper.message().log_medical_state().unit().id() ).Update( wrapper.message().log_medical_state() );


    else if( wrapper.message().has_population_creation() )
        CreateUpdate< Inhabitant >( inhabitants_, wrapper.message().population_creation().id().id(), wrapper.message().population_creation() );
    else if( wrapper.message().has_population_update() )
        inhabitants_.Get( wrapper.message().population_update().id().id() ).Update( wrapper.message().population_update() );

    else if( wrapper.message().has_crowd_creation() )
        CreateUpdate< Population >( populations_, wrapper.message().crowd_creation().crowd().id(), wrapper.message().crowd_creation() );
    else if( wrapper.message().has_crowd_destruction() )
        Destroy( populations_, wrapper.message().crowd_destruction().crowd().id(), wrapper.message().crowd_destruction() );
    else if( wrapper.message().has_crowd_update() )
        populations_.Get( wrapper.message().crowd_update().crowd().id() ).Update( wrapper.message().crowd_update() );
    else if( wrapper.message().has_crowd_concentration_creation() )
        populations_.Get( wrapper.message().crowd_concentration_creation().crowd().id() ).Update( wrapper.message().crowd_concentration_creation() );
    else if( wrapper.message().has_crowd_concentration_destruction() )
        populations_.Get( wrapper.message().crowd_concentration_destruction().crowd().id() ).Update( wrapper.message().crowd_concentration_destruction() );
    else if( wrapper.message().has_crowd_concentration_update() )
        populations_.Get( wrapper.message().crowd_concentration_update().crowd().id() ).Update( wrapper.message().crowd_concentration_update() );
    else if( wrapper.message().has_crowd_flow_creation() )
        populations_.Get( wrapper.message().crowd_flow_creation().crowd().id() ).Update( wrapper.message().crowd_flow_creation() );
    else if( wrapper.message().has_crowd_flow_destruction() )
        populations_.Get( wrapper.message().crowd_flow_destruction().crowd().id() ).Update( wrapper.message().crowd_flow_destruction() );
    else if( wrapper.message().has_crowd_flow_update() )
        populations_.Get( wrapper.message().crowd_flow_update().crowd().id() ).Update( wrapper.message().crowd_flow_update() );

    else if( wrapper.message().has_crowd_knowledge_creation() )
        CreateUpdate< PopulationKnowledge >( populationKnowledges_, wrapper.message().crowd_knowledge_creation().knowledge().id(), wrapper.message().crowd_knowledge_creation() );
    else if( wrapper.message().has_crowd_knowledge_update() )
        populationKnowledges_.Get( wrapper.message().crowd_knowledge_update().knowledge().id() ).Update( wrapper.message().crowd_knowledge_update() );
    else if( wrapper.message().has_crowd_knowledge_destruction() )
        Destroy( populationKnowledges_, wrapper.message().crowd_knowledge_destruction().knowledge().id(), wrapper.message().crowd_knowledge_destruction() );
    else if( wrapper.message().has_crowd_concentration_knowledge_creation() )
        populationKnowledges_.Get( wrapper.message().crowd_concentration_knowledge_creation().crowd().id() ).Update( wrapper.message().crowd_concentration_knowledge_creation() );
    else if( wrapper.message().has_crowd_concentration_knowledge_update() )
        populationKnowledges_.Get( wrapper.message().crowd_concentration_knowledge_update().crowd().id() ).Update( wrapper.message().crowd_concentration_knowledge_update() );
    else if( wrapper.message().has_crowd_concentration_knowledge_destruction() )
        populationKnowledges_.Get( wrapper.message().crowd_concentration_knowledge_destruction().crowd().id() ).Update( wrapper.message().crowd_concentration_knowledge_destruction() );
    else if( wrapper.message().has_crowd_flow_knowledge_creation() )
        populationKnowledges_.Get( wrapper.message().crowd_flow_knowledge_creation().crowd().id() ).Update( wrapper.message().crowd_flow_knowledge_creation() );
    else if( wrapper.message().has_crowd_flow_knowledge_update() )
        populationKnowledges_.Get( wrapper.message().crowd_flow_knowledge_update().crowd().id() ).Update( wrapper.message().crowd_flow_knowledge_update() );
    else if( wrapper.message().has_crowd_flow_knowledge_destruction() )
        populationKnowledges_.Get( wrapper.message().crowd_flow_knowledge_destruction().crowd().id() ).Update( wrapper.message().crowd_flow_knowledge_destruction() );
    else if( wrapper.message().has_folk_creation() )
        folk_->Update( wrapper.message().folk_creation() );

    else if( wrapper.message().has_urban_creation() )
        CreateUpdate< UrbanObject >( urbanBlocks_, wrapper.message().urban_creation().object().id(), wrapper.message().urban_creation() );
    else if( wrapper.message().has_urban_update() )
        urbanBlocks_.Get( wrapper.message().urban_update().object().id() ).Update( wrapper.message().urban_update() );

    else if( wrapper.message().has_urban_knowledge_creation() )
        CreateUpdate< UrbanKnowledge >( urbanKnowledges_, wrapper.message().urban_knowledge_creation().knowledge().id(), wrapper.message().urban_knowledge_creation() );
    else if( wrapper.message().has_urban_knowledge_update() )
        urbanKnowledges_.Get( wrapper.message().urban_knowledge_update().knowledge().id() ).Update( wrapper.message().urban_knowledge_update() );
    else if( wrapper.message().has_urban_knowledge_destruction() )
        Destroy( urbanKnowledges_, wrapper.message().urban_knowledge_destruction().knowledge().id(), wrapper.message().urban_knowledge_destruction() );
    else if( wrapper.message().has_control_global_weather() )
        meteoModel_->OnReceiveMsgGlobalMeteo( wrapper.message().control_global_weather() );
    else if( wrapper.message().has_control_local_weather_creation() )
        meteoModel_->OnReceiveMsgLocalMeteoCreation( wrapper.message().control_local_weather_creation() );
    else if( wrapper.message().has_control_local_weather_destruction() )
        meteoModel_->OnReceiveMsgLocalMeteoDestruction( wrapper.message().control_local_weather_destruction() );
//        default:
//            assert( false );//@TODO restore an exception, some messages aren't linked
//    }
}

// -----------------------------------------------------------------------------
// Name: Model::CreateUpdate
// Created: SBO 2008-07-09
// -----------------------------------------------------------------------------
template< typename Concrete, typename Base, typename Message >
void Model::CreateUpdate( tools::Resolver< Base >& resolver, const Message& message )
{
    CreateUpdate< Concrete, Base, Message >( resolver, message.id().id(), message );
}

// -----------------------------------------------------------------------------
// Name: Model::CreateUpdate2
// Created: LGY 2010-07-07
// -----------------------------------------------------------------------------
template< typename Concrete, typename Base, typename Message >
void Model::CreateUpdate2( tools::Resolver< Base >& resolver, const Message& message )
{
    CreateUpdate< Concrete, Base, Message >( resolver, message.oid(), message );
}

// -----------------------------------------------------------------------------
// Name: Model::CreateUpdate
// Created: MGD 2009-12-22
// -----------------------------------------------------------------------------
template< typename Concrete, typename Base, typename Message >
void Model::CreateUpdate( tools::Resolver< Base >& resolver, unsigned id, const Message& message )
{
    Concrete* pElement = static_cast< Concrete* >( resolver.Find( id ) );
    if( !pElement )
    {
        pElement = new Concrete( *this, message );
        AddExtensions( *pElement );
        resolver.Register( pElement->GetId(), *pElement );
    }
    pElement->Update( message );
}

// -----------------------------------------------------------------------------
// Name: Model::CreateUpdate
// Created: MGD 2009-12-22
// -----------------------------------------------------------------------------
template< typename Concrete, typename Base, typename Message, typename Parameter >
void Model::CreateUpdate( tools::Resolver< Base >& resolver, unsigned id, const Message& message, const Parameter& parameter )
{
    Concrete* pElement = static_cast< Concrete* >( resolver.Find( id ) );
    if( !pElement )
    {
        pElement = new Concrete( *this, message, parameter );
        AddExtensions( *pElement );
        resolver.Register( pElement->GetId(), *pElement );
    }
    pElement->Update( message );
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
    else if( kernel::Inhabitant_ABC* inhab = inhabitants_.Find( id ) )
        inhab->Update( message );
    else throw std::runtime_error( __FUNCTION__ " : Unknown entity" );
}

// -----------------------------------------------------------------------------
// Name: Model::Destroy
// Created: SBO 2008-07-09
// -----------------------------------------------------------------------------
template< typename T, typename M >
void Model::Destroy( tools::Resolver< T >& resolver, unsigned id, const M& message )
{
    if( T* entity = resolver.Find( id ) )
    {
        entity->Update( message );
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
void Model::SendReplayInfo( ClientPublisher_ABC& publisher, unsigned totalTicks, sword::EnumSimulationState status, unsigned int factor ) const
{
    simulation_->SendReplayInfo( publisher, totalTicks, status, factor );
}

// -----------------------------------------------------------------------------
// Name: Model::Accept
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void Model::Accept( kernel::ModelVisitor_ABC& visitor ) const
{
    urbanBlocks_           .Apply( boost::bind( &dispatcher::Object_ABC::Accept, _1, boost::ref( visitor ) ) );
    sides_                 .Apply( boost::bind( &dispatcher::Team_ABC::Accept, _1, boost::ref( visitor ) ) );
    knowledgeGroups_       .Apply( boost::bind( &dispatcher::KnowledgeGroup_ABC::Accept, _1, boost::ref( visitor ) ) );
    agentKnowledges_       .Apply( boost::bind( &dispatcher::AgentKnowledge_ABC::Accept, _1, boost::ref( visitor ) ) );
    objectKnowledges_      .Apply( boost::bind( &dispatcher::ObjectKnowledge_ABC::Accept, _1, boost::ref( visitor ) ) );
    populationKnowledges_  .Apply( boost::bind( &PopulationKnowledge::Accept, _1, boost::ref( visitor ) ) );
    logConsignsMaintenance_.Apply( boost::bind( &LogConsignMaintenance::Accept, _1, boost::ref( visitor ) ) );
    logConsignsSupply_     .Apply( boost::bind( &LogConsignSupply::Accept, _1, boost::ref( visitor ) ) );
    logConsignsMedical_    .Apply( boost::bind( &LogConsignMedical::Accept, _1, boost::ref( visitor ) ) );
    fires_                 .Apply( boost::bind( &Fire::Accept, _1, boost::ref( visitor ) ) );
    populationFires_       .Apply( boost::bind( &PopulationFire::Accept, _1, boost::ref( visitor ) ) );
    fireEffects_           .Apply( boost::bind( &FireEffect::Accept, _1, boost::ref( visitor ) ) );
    reports_               .Apply( boost::bind( &Report::Accept, _1, boost::ref( visitor ) ) );
    urbanKnowledges_       .Apply( boost::bind( &dispatcher::UrbanKnowledge_ABC::Accept, _1, boost::ref( visitor ) ) );
    meteoModel_->Accept( visitor );
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

// -----------------------------------------------------------------------------
// Name: Model::SetToTasker
// Created: PHC 2010-07-07
// -----------------------------------------------------------------------------
void Model::SetToTasker( sword::Tasker& tasker, unsigned int id ) const
{
    if( automats_.Find( id ) )
        tasker.mutable_automat()->set_id( id );
    else if( formations_.Find( id ) )
        tasker.mutable_formation()->set_id( id );
    else if( populations_.Find( id ) )
        tasker.mutable_crowd()->set_id( id );
    else if( agents_.Find( id ) )
        tasker.mutable_unit()->set_id( id );
    else
        throw std::runtime_error( __FUNCTION__ " Misformed tasker in protocol message" );
}

// -----------------------------------------------------------------------------
// Name: Model::TaskerToId
// Created: RPD 2010-07-09
// -----------------------------------------------------------------------------
unsigned int Model::TaskerToId( const sword::Tasker& tasker ) const
{
    if( tasker.has_unit() )
        return tasker.unit().id();
    if( tasker.has_automat() )
        return tasker.automat().id();
    if( tasker.has_crowd() )
        return tasker.crowd().id();
    if( tasker.has_formation() )
        return tasker.formation().id();
    throw std::runtime_error( __FUNCTION__ " Misformed tasker in protocol message" );
}

// -----------------------------------------------------------------------------
// Name: Model::GetExtensionTypes
// Created: RPD 2010-12-28
// -----------------------------------------------------------------------------
const kernel::ExtensionTypes& Model::GetExtensionTypes() const
{
    return staticModel_.extensionTypes_;
}

// -----------------------------------------------------------------------------
// Name: Model::DeleteCheckpoint
// Created: LDC 2011-08-22
// -----------------------------------------------------------------------------
void Model::DeleteCheckpoint( const std::string& name )
{
    std::string oldName = config_.GetCheckpointDirectory( name );
    const boost::filesystem::path oldPath( oldName, boost::filesystem::native );
    boost::filesystem::remove_all( oldPath );
}
