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
#include "ClientPublisher_ABC.h"
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
#include "Population.h"
#include "PopulationFlow.h"
#include "PopulationConcentration.h"
#include "PopulationFire.h"
#include "PopulationKnowledge.h"
#include "Report.h"
#include "Side.h"
#include "SimulationModel.h"
#include "Synchroniser.h"
#include "UrbanKnowledge.h"
#include "UrbanObject.h"
#include "Visitors.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/FormationLevels.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/CoordinateConverter.h"
#include "MT/MT_Logger/MT_Logger_lib.h"
#include "protocol/clientsenders.h"
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
    , levels_( new kernel::FormationLevels())
    , meteoModel_ (new MeteoModel( *(new kernel::CoordinateConverter( config )) ))
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
void Model::Receive( const MsgsSimToClient::MsgSimToClient& wrapper )
{
    if( wrapper.message().has_control_send_current_state_begin() )
    {
        Reset();
        MT_LOG_INFO_MSG( "Dispatcher - Initializing model" );
    }
    else if( wrapper.message().has_control_send_current_state_end() )
        MT_LOG_INFO_MSG( "Dispatcher - Model initialized" );
    Update( wrapper );
}

// -----------------------------------------------------------------------------
// Name: Model::Update
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
void Model::Update( const MsgsSimToClient::MsgSimToClient& wrapper )
{
    if( wrapper.message().has_unit_order_ack() ||
        wrapper.message().has_automat_order_ack() ||
        wrapper.message().has_population_order_ack() ||
        wrapper.message().has_frag_order_ack() ||
        wrapper.message().has_set_automat_mode_ack() ||
        wrapper.message().has_unit_magic_action_ack() ||
        wrapper.message().has_object_magic_action_ack() ||
        wrapper.message().has_population_magic_action_ack() )
        { // NOTHING 
        }
    if( wrapper.message().has_change_diplomacy_ack() )
        sides_.Get( wrapper.message().change_diplomacy_ack().oid_camp1() ).Update( wrapper.message().change_diplomacy_ack() ); 
    if( wrapper.message().has_change_diplomacy() )
        sides_.Get( wrapper.message().change_diplomacy().oid_camp1() ).Update( wrapper.message().change_diplomacy() ); 
    if( wrapper.message().has_automat_change_logistic_links() )
        automats_.Get( wrapper.message().automat_change_logistic_links().oid() ).Update( wrapper.message().automat_change_logistic_links() ); 
    if( wrapper.message().has_automat_change_knowledge_group() )
        automats_.Get( wrapper.message().automat_change_knowledge_group().oid() ).Update( wrapper.message().automat_change_knowledge_group() ); 
    if( wrapper.message().has_automat_change_superior() )
        automats_.Get( wrapper.message().automat_change_superior().oid() ).Update( wrapper.message().automat_change_superior() ); 
    if( wrapper.message().has_automat_change_logistic_links_ack() ||
        wrapper.message().has_automat_change_knowledge_group_ack() ||
        wrapper.message().has_automat_change_superior_ack() ||
        wrapper.message().has_unit_change_superior_ack() )
        { // NOTHING 
        }
    if( wrapper.message().has_unit_change_superior() )
        agents_.Get( wrapper.message().unit_change_superior().oid() ).Update( wrapper.message().unit_change_superior() ); 
    if( wrapper.message().has_log_supply_push_flow_ack() ||
        wrapper.message().has_log_supply_change_quotas_ack() )
        { // NOTHING 
        }
    if( wrapper.message().has_control_information() )
        simulation_->Update( wrapper.message().control_information() ); 
    if( wrapper.message().has_control_begin_tick() )
        simulation_->Update( wrapper.message().control_begin_tick() ); 
    if( wrapper.message().has_control_end_tick() )
        simulation_->Update( wrapper.message().control_end_tick() ); 
    if( wrapper.message().has_control_stop_ack() )
        simulation_->Update_Stop( wrapper.message().control_stop_ack() ); 
    if( wrapper.message().has_control_pause_ack() )
        simulation_->Update_Pause( wrapper.message().control_pause_ack() ); 
    if( wrapper.message().has_control_resume_ack() )
        simulation_->Update_Resume( wrapper.message().control_resume_ack() ); 
    if( wrapper.message().has_control_change_time_factor_ack() )
        simulation_->Update( wrapper.message().control_change_time_factor_ack() ); 
    if( wrapper.message().has_control_date_time_change_ack() ||
        wrapper.message().has_control_global_meteo_ack() ||
        wrapper.message().has_control_local_meteo_ack() ||
        wrapper.message().has_control_checkpoint_save_begin() ||
        wrapper.message().has_control_checkpoint_save_end() ||
        wrapper.message().has_control_checkpoint_set_frequency_ack() ||
        wrapper.message().has_control_checkpoint_save_now_ack() ||
        wrapper.message().has_control_send_current_state_begin() ||
        wrapper.message().has_control_send_current_state_end() )
        { // NOTHING // $$$$ AGE 2007-04-18: messages vides...
        }
        
    if( wrapper.message().has_unit_knowledge_creation() )
        CreateUpdate< AgentKnowledge >( agentKnowledges_, wrapper.message().unit_knowledge_creation() ); 
    if( wrapper.message().has_unit_knowledge_update() )
        agentKnowledges_.Get( wrapper.message().unit_knowledge_update().oid() ).Update( wrapper.message().unit_knowledge_update() ); 
    if( wrapper.message().has_unit_knowledge_destruction() )
        Destroy( agentKnowledges_, wrapper.message().unit_knowledge_destruction().oid() ); 
    if( wrapper.message().has_side_creation() )
        CreateUpdate< Side >( sides_, wrapper.message().side_creation() ); 
    if( wrapper.message().has_knowledge_group_creation() )
        CreateUpdate< KnowledgeGroup >( knowledgeGroups_, wrapper.message().knowledge_group_creation() ); 
    if( wrapper.message().has_knowledge_group_destruction() )
        Destroy( knowledgeGroups_, wrapper.message().knowledge_group_destruction().oid() ); 
    if( wrapper.message().has_knowledge_group_update() )
        knowledgeGroups_.Get( wrapper.message().knowledge_group_update().oid() ).Update( wrapper.message().knowledge_group_update() );
    if( wrapper.message().has_formation_creation() )
        CreateUpdate< Formation >( formations_, wrapper.message().formation_creation().oid(), wrapper.message().formation_creation(), *levels_ ); 
    if( wrapper.message().has_unit_creation() )
        CreateUpdate< Agent >( agents_, wrapper.message().unit_creation() ); 
    if( wrapper.message().has_unit_environment_type() )
        agents_.Get( wrapper.message().unit_environment_type().oid() ).Update( wrapper.message().unit_environment_type() ); 
    if( wrapper.message().has_unit_destruction() )
        Destroy( agents_, wrapper.message().unit_destruction().oid() ); 
    if( wrapper.message().has_automat_creation() )
        CreateUpdate< Automat >( automats_, wrapper.message().automat_creation() ); 
    if( wrapper.message().has_unit_attributes() )
        agents_.Get( wrapper.message().unit_attributes().oid() ).Update( wrapper.message().unit_attributes() ); 
    if( wrapper.message().has_automat_attributes() )
        automats_.Get( wrapper.message().automat_attributes().oid() ).Update( wrapper.message().automat_attributes()  ); 
    if( wrapper.message().has_unit_pathfind() )
        agents_.Get( wrapper.message().unit_pathfind().oid() ).Update( wrapper.message().unit_pathfind() ); 
    if( wrapper.message().has_start_unit_fire() )
    {
        CreateUpdate< Fire >( fires_, wrapper.message().start_unit_fire().fire_oid(), wrapper.message().start_unit_fire() );
        agents_.Get( wrapper.message().start_unit_fire().firer_oid() ).Update( wrapper.message().start_unit_fire() ); 
    }
    if( wrapper.message().has_stop_unit_fire() )
        Destroy( fires_, wrapper.message().stop_unit_fire().fire_oid() ); 
    if( wrapper.message().has_start_population_fire() )
        CreateUpdate< PopulationFire >( populationFires_, wrapper.message().start_population_fire().fire_oid(), wrapper.message().start_population_fire() ); 
    if( wrapper.message().has_stop_population_fire() )
        Destroy( populationFires_, wrapper.message().stop_population_fire().fire_oid() ); 
    if( wrapper.message().has_explosion() )
    {} // $$$$ merge

        
    if( wrapper.message().has_report() )
        CreateUpdate< Report >( reports_, wrapper.message().report().cr_oid(), wrapper.message().report() ); 
    if( wrapper.message().has_invalidate_report() )
        Destroy( reports_, wrapper.message().invalidate_report().cr_oid() ); 
    if( wrapper.message().has_trace() )

          // $$$$ AGE 2007-04-18: Evenements, modèle client => rien, ou remanier
    if( wrapper.message().has_unit_detection() )
        agents_.Get( wrapper.message().unit_detection().oid() ).Update( wrapper.message().unit_detection() ); 
    if( wrapper.message().has_object_detection() )
        agents_.Get( wrapper.message().object_detection().oid() ).Update( wrapper.message().object_detection() ); 
    if( wrapper.message().has_decisional_state() )
        UpdateAnyAgent( wrapper.message().decisional_state().oid(), wrapper.message().decisional_state() ); 
    if( wrapper.message().has_start_fire_effect() )
        CreateUpdate< FireEffect >( fireEffects_, wrapper.message().start_fire_effect().effect_oid(), wrapper.message().start_fire_effect() ); 
    if( wrapper.message().has_stop_fire_effect() )
        Destroy( fireEffects_, wrapper.message().stop_fire_effect().oid() ); 
    if( wrapper.message().has_unit_order() )
        agents_     .Get( wrapper.message().unit_order().oid() ).Update( wrapper.message().unit_order() ); 
    if( wrapper.message().has_automat_order() )
        automats_   .Get( wrapper.message().automat_order().oid() ).Update( wrapper.message().automat_order() ); 
    if( wrapper.message().has_population_order() )
        populations_.Get( wrapper.message().population_order().oid() ).Update( wrapper.message().population_order() ); 

    if( wrapper.message().has_object_creation() )
        CreateUpdate< Object >( objects_, wrapper.message().object_creation().oid(), wrapper.message().object_creation() ); 
    if( wrapper.message().has_object_update() )
        objects_.Get( wrapper.message().object_update().oid() ).Update( wrapper.message().object_update() ); 
    if( wrapper.message().has_object_destruction() )
        Destroy( objects_, wrapper.message().object_destruction().oid() ); 
    if( wrapper.message().has_object_knowledge_creation() )
        CreateUpdate< ObjectKnowledge >( objectKnowledges_, wrapper.message().object_knowledge_creation().oid(), wrapper.message().object_knowledge_creation() ); 
    if( wrapper.message().has_object_knowledge_update() )
        objectKnowledges_.Get( wrapper.message().object_knowledge_update().oid() ).Update( wrapper.message().object_knowledge_update() ); 
    if( wrapper.message().has_object_knowledge_destruction() )
        Destroy( objectKnowledges_, wrapper.message().object_knowledge_destruction().oid() ); 

    if( wrapper.message().has_log_maintenance_handling_creation() )
        CreateUpdate< LogConsignMaintenance >( logConsignsMaintenance_, wrapper.message().log_maintenance_handling_creation().oid_consigne(), wrapper.message().log_maintenance_handling_creation() ); 
    if( wrapper.message().has_log_maintenance_handling_destruction() )
        Destroy( logConsignsMaintenance_, wrapper.message().log_maintenance_handling_destruction().oid_consigne() ); 
    if( wrapper.message().has_log_maintenance_handling_update() )
        logConsignsMaintenance_.Get( wrapper.message().log_maintenance_handling_update().oid_consigne() ).Update( wrapper.message().log_maintenance_handling_update() ); 
    if( wrapper.message().has_log_maintenance_state() )
        agents_.Get( wrapper.message().log_maintenance_state().oid_pion() ).Update( wrapper.message().log_maintenance_state() ); 

    if( wrapper.message().has_log_supply_handling_creation() )
        CreateUpdate< LogConsignSupply >( logConsignsSupply_, wrapper.message().log_supply_handling_creation().oid_consigne(), wrapper.message().log_supply_handling_creation() ); 
    if( wrapper.message().has_log_supply_handling_destruction() )
        Destroy( logConsignsSupply_, wrapper.message().log_supply_handling_destruction().oid_consigne() ); 
    if( wrapper.message().has_log_supply_handling_update() )
        logConsignsSupply_.Get( wrapper.message().log_supply_handling_update().oid_consigne() ).Update( wrapper.message().log_supply_handling_update() ); 
    if( wrapper.message().has_log_medical_state() )
        agents_.Get( wrapper.message().log_medical_state().oid_pion() ).Update( wrapper.message().log_medical_state()); 
    if( wrapper.message().has_log_supply_quotas() )
        automats_.Get( wrapper.message().log_supply_quotas().oid_automate() ).Update( wrapper.message().log_supply_quotas() ); 

    if( wrapper.message().has_log_medical_handling_creation() )
        CreateUpdate< LogConsignMedical >( logConsignsMedical_, wrapper.message().log_medical_handling_creation().oid_consigne(),  wrapper.message().log_medical_handling_creation() ); 
    if( wrapper.message().has_log_medical_handling_destruction() )
        Destroy( logConsignsMedical_, wrapper.message().log_medical_handling_destruction().oid_consigne() ); 
    if( wrapper.message().has_log_medical_handling_update() )
        logConsignsMedical_.Get( wrapper.message().log_medical_handling_update().oid_consigne() ).Update( wrapper.message().log_medical_handling_update() ); 
    if( wrapper.message().has_log_medical_state() )
        agents_.Get( wrapper.message().log_medical_state().oid_pion() ).Update( wrapper.message().log_medical_state() ); 

    if( wrapper.message().has_population_creation() )
        CreateUpdate< Population >( populations_, wrapper.message().population_creation() ); 
    if( wrapper.message().has_population_update() )
        populations_.Get( wrapper.message().population_update().oid() ).Update( wrapper.message().population_update() ); 
    if( wrapper.message().has_population_concentration_creation() )
        populations_.Get( wrapper.message().population_concentration_creation().oid_population() ).Update( wrapper.message().population_concentration_creation() ); 
    if( wrapper.message().has_population_concentration_destruction() )
        populations_.Get( wrapper.message().population_concentration_destruction().oid_population() ).Update( wrapper.message().population_concentration_destruction() ); 
    if( wrapper.message().has_population_concentration_update() )
        populations_.Get( wrapper.message().population_concentration_update().oid_population() ).Update( wrapper.message().population_concentration_update() ); 
    if( wrapper.message().has_population_flow_creation() )
         populations_.Get( wrapper.message().population_flow_creation().oid_population() ).Update( wrapper.message().population_flow_creation() ); 
    if( wrapper.message().has_population_flow_destruction() )
         populations_.Get( wrapper.message().population_flow_destruction().oid_population() ).Update( wrapper.message().population_flow_destruction() ); 
    if( wrapper.message().has_population_flow_update() )
         populations_.Get( wrapper.message().population_flow_update().oid_population() ).Update( wrapper.message().population_flow_update() ); 

    if( wrapper.message().has_population_knowledge_creation() )
         CreateUpdate< PopulationKnowledge >( populationKnowledges_, wrapper.message().population_knowledge_creation().oid_connaissance(), wrapper.message().population_knowledge_creation() ); 
    if( wrapper.message().has_population_knowledge_update() )
         populationKnowledges_.Get( wrapper.message().population_knowledge_update().oid_connaissance() ).Update( wrapper.message().population_knowledge_update() ); 
    if( wrapper.message().has_population_knowledge_destruction() )
         Destroy( populationKnowledges_, wrapper.message().population_knowledge_destruction().oid_connaissance() ); 
    if( wrapper.message().has_population_concentration_knowledge_creation() )
         populationKnowledges_.Get( wrapper.message().population_concentration_knowledge_creation().oid_connaissance_population() ).Update( wrapper.message().population_concentration_knowledge_creation() ); 
    if( wrapper.message().has_population_concentration_knowledge_update() )
         populationKnowledges_.Get( wrapper.message().population_concentration_knowledge_update().oid_connaissance_population() ).Update( wrapper.message().population_concentration_knowledge_update() ); 
    if( wrapper.message().has_population_concentration_knowledge_destruction() )
         populationKnowledges_.Get( wrapper.message().population_concentration_knowledge_destruction().oid_connaissance_population() ).Update( wrapper.message().population_concentration_knowledge_destruction() ); 
    if( wrapper.message().has_population_flow_knowledge_creation() )
         populationKnowledges_.Get( wrapper.message().population_flow_knowledge_creation().oid_connaissance_population() ).Update( wrapper.message().population_flow_knowledge_creation() ); 
    if( wrapper.message().has_population_flow_knowledge_update() )
         populationKnowledges_.Get( wrapper.message().population_flow_knowledge_update().oid_connaissance_population() ).Update( wrapper.message().population_flow_knowledge_update() ); 
    if( wrapper.message().has_population_flow_knowledge_destruction() )
         populationKnowledges_.Get( wrapper.message().population_flow_knowledge_destruction().oid_connaissance_population() ).Update( wrapper.message().population_flow_knowledge_destruction() ); 
    if( wrapper.message().has_folk_creation() )
         folk_->Update( wrapper.message().folk_creation() ); 

    if( wrapper.message().has_urban_creation() )
        CreateUpdate< UrbanObject >( urbanBlocks_, wrapper.message().urban_creation() );


    if( wrapper.message().has_urban_knowledge_creation() )
        CreateUpdate< UrbanKnowledge >( urbanKnowledges_, wrapper.message().urban_knowledge_creation() );
    if( wrapper.message().has_urban_knowledge_update() )
        urbanKnowledges_.Get( wrapper.message().urban_knowledge_update().oid() ).Update( wrapper.message().urban_knowledge_update() ); 
    if( wrapper.message().has_urban_knowledge_destruction() )
        Destroy( urbanKnowledges_, wrapper.message().urban_knowledge_destruction().oid() ); 
    if( wrapper.message().has_control_global_meteo() )
        meteoModel_->OnReceiveMsgGlobalMeteo( wrapper.message().control_global_meteo() );
    if( wrapper.message().has_control_local_meteo() )
        meteoModel_->OnReceiveMsgLocalMeteo( wrapper.message().control_local_meteo() );
    if( wrapper.message().has_urban_creation() )
        CreateUpdate< UrbanObject >( urbanBlocks_, wrapper.message().urban_creation() );

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
    CreateUpdate< Concrete, Base, Message >( resolver, message.oid(), message );
}

// -----------------------------------------------------------------------------
// Name: Model::CreateUpdate
// Created: MGD 2009-12-22
// -----------------------------------------------------------------------------
template< typename Concrete, typename Base, typename Message >
void Model::CreateUpdate( tools::Resolver< Base >& resolver, unsigned id, const Message& message )
{
    Base* pElement = resolver.Find( id );
    if( !pElement )
    {
        pElement = new Concrete( *this, message );
        AddExtensions( *pElement );
        resolver.Register( pElement->GetId(), *pElement );
    }
    static_cast< Concrete* >( pElement )->Update( message );
}

// -----------------------------------------------------------------------------
// Name: Model::CreateUpdate
// Created: MGD 2009-12-22
// -----------------------------------------------------------------------------
template< typename Concrete, typename Base, typename Message, typename Parameter >
void Model::CreateUpdate( tools::Resolver< Base >& resolver, unsigned id, const Message& message, const Parameter& parameter )
{
    Base* pElement = resolver.Find( id );
    if( !pElement )
    {
        pElement = new Concrete( *this, message, parameter );
        AddExtensions( *pElement );
        resolver.Register( pElement->GetId(), *pElement );
    }
    static_cast< Concrete* >( pElement )->Update( message );
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
void Model::SendReplayInfo( ClientPublisher_ABC& publisher, unsigned totalTicks, Common::EnumSimulationState status, unsigned factor ) const
{
    simulation_->SendReplayInfo( publisher, totalTicks, status, factor );
}

// -----------------------------------------------------------------------------
// Name: Model::Accept
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void Model::Accept( kernel::ModelVisitor_ABC& visitor ) const
{
    sides_                 .Apply( boost::bind( &Sendable< kernel::Team_ABC >::Accept, _1, boost::ref( visitor ) ) );
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
    urbanKnowledges_       .Apply( boost::bind( &UrbanKnowledge::Accept, _1, boost::ref( visitor ) ) );
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
