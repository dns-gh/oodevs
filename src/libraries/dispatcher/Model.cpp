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
#include "DetectionRangeEffect.h"
#include "Fire.h"
#include "FireEffect.h"
#include "FolkModel.h"
#include "Formation.h"
#include "KnowledgeGroup.h"
#include "LogConsignMaintenance.h"
#include "LogConsignSupply.h"
#include "LogConsignMedical.h"
#include "LogConsignFuneral.h"
#include "MagicOrder.h"
#include "MeteoModel.h"
#include "Object.h"
#include "ObjectKnowledge.h"
#include "Inhabitant.h"
#include "Population.h"
#include "PopulationFlow.h"
#include "PopulationConcentration.h"
#include "PopulationFire.h"
#include "PopulationKnowledge.h"
#include "ReplaySynchronisations.h"
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
#include "protocol/MessageParameters.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/StaticModel.h"
#include "clients_kernel/PopulationType.h"
#include "clients_kernel/DecisionalModel.h"
#include "MT_Tools/MT_Logger.h"
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
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
    urbanKnowledges_       .DeleteAll();
    detectionRangeEffects_. DeleteAll();
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
    magicOrders_           .DeleteAll();
}

namespace
{
    class NoSide : public Team_ABC
    {
    public:
        NoSide() : Team_ABC( 0 ) {}
        virtual ~NoSide() {}

    public:
        virtual const kernel::Karma& GetKarma() const
        {
            return kernel::Karma::neutral_;
        }
        virtual void Register( dispatcher::Object_ABC& object )
        {
            objects_.Register( object.GetId(), object );
        }
        virtual void Remove( dispatcher::Object_ABC& object )
        {
            objects_.Remove( object.GetId() );
        }
        virtual void Accept( kernel::ModelVisitor_ABC& visitor ) const
        {
            objects_.Apply( boost::bind( &dispatcher::Object_ABC::Accept, _1, boost::ref( visitor ) ) );
        }
        virtual void Register       ( dispatcher::Formation_ABC& )      {}
        virtual void Remove         ( dispatcher::Formation_ABC& )      {}
        virtual void Register       ( dispatcher::Population_ABC& )     {}
        virtual void Remove         ( dispatcher::Population_ABC& )     {}
        virtual void Register       ( dispatcher::Inhabitant_ABC& )     {}
        virtual void Remove         ( dispatcher::Inhabitant_ABC& )     {}
        virtual void Register       ( dispatcher::KnowledgeGroup_ABC& ) {}
        virtual void Remove         ( dispatcher::KnowledgeGroup_ABC& ) {}
        virtual void SendCreation   ( ClientPublisher_ABC& ) const      {}
        virtual void SendFullUpdate ( ClientPublisher_ABC& ) const      {}
        virtual void SendDestruction( ClientPublisher_ABC& ) const      {}
        virtual bool GetExtension( const std::string&, std::string& ) const { return false; }

    private:
        tools::Resolver< dispatcher::Object_ABC > objects_;
    };
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
        sides_.Register( 0, *new NoSide() );
        MT_LOG_INFO_MSG( "Dispatcher - Initializing model" );
    }
    else if( wrapper.message().has_control_send_current_state_end() )
        MT_LOG_INFO_MSG( "Dispatcher - Model initialized" );
    try
    {
        Update( wrapper );
    }
    catch( const std::exception& e )
    {
        MT_LOG_ERROR_MSG( tools::GetExceptionMsg( e ) );
    }
}

// -----------------------------------------------------------------------------
// Name: Model::Update
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
void Model::Update( const sword::SimToClient& wrapper )
{
    const ::sword::SimToClient_Content& message = wrapper.message();
    if( message.has_order_ack() ||
        message.has_frag_order_ack() ||
        message.has_set_automat_mode_ack() ||
        message.has_unit_magic_action_ack() ||
        message.has_object_magic_action_ack() ||
        message.has_crowd_magic_action_ack() )
        { // NOTHING
        }
    else if( message.has_change_diplomacy_ack() )
        sides_.Get( message.change_diplomacy_ack().party1().id() ).Update( message.change_diplomacy_ack() );
    else if( message.has_change_diplomacy() )
        sides_.Get( message.change_diplomacy().party1().id() ).Update( message.change_diplomacy() );
    else if( message.has_automat_change_logistic_links() )
    {
        if( message.automat_change_logistic_links().requester().has_automat() )
            automats_.Get( message.automat_change_logistic_links().requester().automat().id() ).Update( message.automat_change_logistic_links() );
        else
            formations_.Get( message.automat_change_logistic_links().requester().formation().id() ).Update( message.automat_change_logistic_links() );
    }
    else if( message.has_automat_change_knowledge_group() )
        automats_.Get( message.automat_change_knowledge_group().automat().id() ).Update( message.automat_change_knowledge_group() );
    else if( message.has_automat_change_superior() )
        automats_.Get( message.automat_change_superior().automat().id() ).Update( message.automat_change_superior() );
    else if( message.has_automat_change_logistic_links_ack() ||
        message.has_automat_change_knowledge_group_ack() ||
        message.has_automat_change_superior_ack() ||
        message.has_unit_change_superior_ack() )
        { // NOTHING
        }
    else if( message.has_unit_change_superior() )
        agents_.Get( message.unit_change_superior().unit().id() ).Update( message.unit_change_superior() );
    else if( message.has_log_supply_push_flow_ack() ||
        message.has_log_supply_pull_flow_ack() ||
        message.has_log_supply_change_quotas_ack() )
        { // NOTHING
        }
    else if( message.has_control_information() )
        simulation_->Update( message.control_information() );
    else if( message.has_control_begin_tick() )
    {
        const sword::ControlBeginTick& beginTick = message.control_begin_tick();
        simulation_->Update( beginTick );
        ClearOldReports( beginTick.current_tick() );
    }
    else if( message.has_control_end_tick() )
    {
        const sword::ControlEndTick& endTick = message.control_end_tick();
        logger_.Update( endTick );
        simulation_->Update( endTick );
    }
    else if( message.has_control_stop_ack() )
        simulation_->Update( message.control_stop_ack() );
    else if( message.has_control_pause_ack() )
        simulation_->Update( message.control_pause_ack() );
    else if( message.has_control_resume_ack() )
        simulation_->Update( message.control_resume_ack() );
    else if( message.has_control_change_time_factor_ack() )
        simulation_->Update( message.control_change_time_factor_ack() );
    else if( message.has_control_date_time_change_ack() ||
        message.has_control_global_weather_ack() ||
        message.has_control_local_weather_ack() ||
        message.has_control_checkpoint_save_begin() ||
        message.has_control_checkpoint_set_frequency_ack() ||
        message.has_control_checkpoint_save_now_ack() ||
        message.has_control_send_current_state_begin() ||
        message.has_control_send_current_state_end() ||
        message.has_control_checkpoint_save_end() )
        { // NOTHING // $$$$ AGE 2007-04-18: messages vides...
        }
    else if( message.has_control_checkpoint_save_delete() )
        DeleteCheckpoint( message.control_checkpoint_save_delete().name() );
    else if( message.has_unit_knowledge_creation() )
        CreateUpdate< AgentKnowledge >( agentKnowledges_, message.unit_knowledge_creation().knowledge().id(), message.unit_knowledge_creation() );
    else if( message.has_unit_knowledge_update() )
        agentKnowledges_.Get( message.unit_knowledge_update().knowledge().id() ).Update( message.unit_knowledge_update() );
    else if( message.has_unit_knowledge_destruction() )
        Destroy( agentKnowledges_, message.unit_knowledge_destruction().knowledge().id(), message.unit_knowledge_destruction() );
    else if( message.has_party_creation() )
        CreateUpdate< Side >( sides_, message.party_creation().party().id(), message.party_creation() );
    else if( message.has_knowledge_group_creation() )
        CreateUpdate< KnowledgeGroup >( knowledgeGroups_, message.knowledge_group_creation().knowledge_group().id(), message.knowledge_group_creation() );
    else if( message.has_knowledge_group_destruction() )
        Destroy( knowledgeGroups_, message.knowledge_group_destruction().knowledge_group().id(), message.knowledge_group_destruction() );
    else if( message.has_knowledge_group_update() )
        knowledgeGroups_.Get( message.knowledge_group_update().knowledge_group().id() ).Update( message.knowledge_group_update() );
    else if( message.has_formation_creation() )
        CreateUpdate< Formation >( formations_, message.formation_creation().formation().id(), message.formation_creation() );
    else if( message.has_formation_update() )
        formations_.Get( message.formation_update().formation().id() ).Update( message.formation_update() );
    else if( message.has_formation_destruction() )
        Destroy( formations_, message.formation_destruction().formation().id(), message.formation_destruction() );
    else if( message.has_unit_creation() )
        CreateUpdate< Agent >( agents_, message.unit_creation().unit().id(), message.unit_creation(), staticModel_.types_ );
    else if( message.has_unit_environment_type() )
        agents_.Get( message.unit_environment_type().unit().id() ).Update( message.unit_environment_type() );
    else if( message.has_unit_destruction() )
    {
        for( auto it = agents_.CreateIterator(); it.HasMoreElements(); )
        {
            const dispatcher::Agent_ABC& agent = it.NextElement();
            const_cast< dispatcher::Agent_ABC& >( agent ).Update( message.unit_destruction() );
        }
        ClearLogisticRequests( message.unit_destruction().unit().id() );
        Destroy( agents_, message.unit_destruction().unit().id(), message.unit_destruction() );
    }
    else if( message.has_automat_creation() )
        CreateUpdate< Automat >( automats_, message.automat_creation().automat().id(), message.automat_creation(), staticModel_.types_ );
    else if( message.has_automat_destruction() )
        Destroy( automats_, message.automat_destruction().automat().id(), message.automat_destruction() );
    else if( message.has_unit_attributes() )
        agents_.Get( message.unit_attributes().unit().id() ).Update( message.unit_attributes() );
    else if( message.has_automat_attributes() )
        automats_.Get( message.automat_attributes().automat().id() ).Update( message.automat_attributes() );
    else if( message.has_unit_pathfind() )
        agents_.Get( message.unit_pathfind().unit().id() ).Update( message.unit_pathfind() );
    else if( message.has_start_unit_fire() )
    {
        CreateUpdate< Fire >( fires_, message.start_unit_fire().fire().id(), message.start_unit_fire() );
        agents_.Get( message.start_unit_fire().firing_unit().id() ).Update( message.start_unit_fire() );
    }
    else if( message.has_stop_unit_fire() )
        Destroy( fires_, message.stop_unit_fire().fire().id(), message.stop_unit_fire() );
    else if( message.has_start_crowd_fire() )
        CreateUpdate< PopulationFire >( populationFires_, message.start_crowd_fire().fire().id(), message.start_crowd_fire() );
    else if( message.has_stop_crowd_fire() )
        Destroy( populationFires_, message.stop_crowd_fire().fire().id(), message.stop_crowd_fire() );
    else if( message.has_explosion() )
    {} // $$$$ merge

    else if( message.has_report() )
        CreateUpdate< Report >( reports_, message.report().report().id(), message.report() );
    else if( message.has_invalidate_report() )
        Destroy( reports_, message.invalidate_report().report().id(), message.invalidate_report() );
    else if( message.has_trace() )
    {
        // $$$$ AGE 2007-04-18: Evenements, modèle client => rien, ou remanier
    }
    else if( message.has_unit_detection() )
        agents_.Get( message.unit_detection().observer().id() ).Update( message.unit_detection() );
    else if( message.has_object_detection() )
        agents_.Get( message.object_detection().observer().id() ).Update( message.object_detection() );
    else if( message.has_crowd_concentration_detection() )
        agents_.Get( message.crowd_concentration_detection().observer().id() ).Update( message.crowd_concentration_detection() );
    else if( message.has_crowd_flow_detection() )
        agents_.Get( message.crowd_flow_detection().observer().id() ).Update( message.crowd_flow_detection() );
    else if( message.has_decisional_state() )
    {
        const auto id = protocol::TryGetTasker( message.decisional_state().source() );
        if( !id )
            throw MASA_EXCEPTION( "tasker is empty" );
        UpdateAnyAgent( *id, message.decisional_state() );
    }
    else if( message.has_start_fire_effect() )
        CreateUpdate< FireEffect >( fireEffects_, message.start_fire_effect().fire_effect().id(), message.start_fire_effect() );
    else if( message.has_stop_fire_effect() )
    {
        Destroy( fireEffects_, message.stop_fire_effect().fire_effect().id(), message.stop_fire_effect() );
        Destroy( detectionRangeEffects_, message.stop_fire_effect().fire_effect().id(), message.stop_fire_effect() );
    }
    else if( message.has_indirect_fire_perception() )
        CreateUpdate< DetectionRangeEffect >( detectionRangeEffects_, message.indirect_fire_perception().fire_effects( 0 ).id(), message.indirect_fire_perception() );
    else if( message.has_unit_order() )
        agents_.Get( message.unit_order().tasker().id() ).Update( message.unit_order() );
    else if( message.has_automat_order() )
        automats_.Get( message.automat_order().tasker().id() ).Update( message.automat_order() );
    else if( message.has_crowd_order() )
        populations_.Get( message.crowd_order().tasker().id() ).Update( message.crowd_order() );

    else if( message.has_object_creation() )
        CreateUpdate< Object >( objects_, message.object_creation().object().id(), message.object_creation(), staticModel_.objectTypes_ );
    else if( message.has_object_update() )
    {
        if( objects_.Find( message.object_update().object().id() ) )
            objects_.Get( message.object_update().object().id() ).Update( message.object_update() );
        else if( urbanBlocks_.Find( message.object_update().object().id() ) )
            urbanBlocks_.Get( message.object_update().object().id() ).Update( message.object_update() );
    }
    else if( message.has_object_destruction() )
        Destroy( objects_, message.object_destruction().object().id(), message.object_destruction() );
    else if( message.has_object_knowledge_creation() )
        CreateUpdate< ObjectKnowledge >( objectKnowledges_, message.object_knowledge_creation().knowledge().id(), message.object_knowledge_creation() );
    else if( message.has_object_knowledge_update() )
        objectKnowledges_.Get( message.object_knowledge_update().knowledge().id() ).Update( message.object_knowledge_update() );
    else if( message.has_object_knowledge_destruction() )
        Destroy( objectKnowledges_, message.object_knowledge_destruction().knowledge().id(), message.object_knowledge_destruction() );

    else if( message.has_log_maintenance_handling_creation() )
        CreateUpdate< LogConsignMaintenance >( logConsignsMaintenance_, message.log_maintenance_handling_creation().request().id(), message.log_maintenance_handling_creation() );
    else if( message.has_log_maintenance_handling_destruction() )
        Destroy( logConsignsMaintenance_, message.log_maintenance_handling_destruction().request().id(), message.log_maintenance_handling_destruction() );
    else if( message.has_log_maintenance_handling_update() )
    {
        if( LogConsignMaintenance* consign = logConsignsMaintenance_.Find( message.log_maintenance_handling_update().request().id() ) )
            consign->Update( message.log_maintenance_handling_update() );
    }
    else if( message.has_log_maintenance_state() )
        agents_.Get( message.log_maintenance_state().unit().id() ).Update( message.log_maintenance_state() );

    else if( message.has_log_supply_handling_creation() )
        CreateUpdate< LogConsignSupply >( logConsignsSupply_, message.log_supply_handling_creation().request().id(), message.log_supply_handling_creation() );
    else if( message.has_log_supply_handling_destruction() )
        Destroy( logConsignsSupply_, message.log_supply_handling_destruction().request().id(), message.log_supply_handling_destruction() );
    else if( message.has_log_supply_handling_update() )
    {
        if( LogConsignSupply* consign = logConsignsSupply_.Find( message.log_supply_handling_update().request().id() ) )
            consign->Update( message.log_supply_handling_update() );
    }
    else if( message.has_log_supply_state() )
        agents_.Get( message.log_supply_state().unit().id() ).Update( message.log_supply_state() );
    else if( message.has_log_supply_quotas() )
    {
        if( message.log_supply_quotas().supplied().has_automat() )
            automats_.Get( message.log_supply_quotas().supplied().automat().id() ).Update( message.log_supply_quotas() );
        else
            formations_.Get( message.log_supply_quotas().supplied().formation().id() ).Update( message.log_supply_quotas() );
    }
    else if( message.has_log_medical_handling_creation() )
        CreateUpdate< LogConsignMedical >( logConsignsMedical_, message.log_medical_handling_creation().request().id(),  message.log_medical_handling_creation() );
    else if( message.has_log_medical_handling_destruction() )
        Destroy( logConsignsMedical_, message.log_medical_handling_destruction().request().id(), message.log_medical_handling_destruction() );
    else if( message.has_log_medical_handling_update() )
        logConsignsMedical_.Get( message.log_medical_handling_update().request().id() ).Update( message.log_medical_handling_update() );
    else if( message.has_log_medical_state() )
        agents_.Get( message.log_medical_state().unit().id() ).Update( message.log_medical_state() );
    else if( message.has_log_funeral_handling_creation() )
        CreateUpdate< LogConsignFuneral >( logConsignsFuneral_, message.log_funeral_handling_creation().request().id(),  message.log_funeral_handling_creation() );
    else if( message.has_log_funeral_handling_destruction() )
        Destroy( logConsignsFuneral_, message.log_funeral_handling_destruction().request().id(), message.log_funeral_handling_destruction() );
    else if( message.has_log_funeral_handling_update() )
    {
        if( LogConsignFuneral* consign = logConsignsFuneral_.Find( message.log_funeral_handling_update().request().id() ) )
            consign->Update( message.log_funeral_handling_update() );
    }

    else if( message.has_population_creation() )
        CreateUpdate< Inhabitant >( inhabitants_, message.population_creation().id().id(), message.population_creation(), staticModel_.types_ );
    else if( message.has_population_update() )
        inhabitants_.Get( message.population_update().id().id() ).Update( message.population_update() );

    else if( message.has_crowd_creation() )
        CreateUpdate< Population >( populations_, message.crowd_creation().crowd().id(), message.crowd_creation(),
                static_cast<const tools::Resolver< kernel::PopulationType >& >(staticModel_.types_).Get( message.crowd_creation().type().id() ).GetDecisionalModel().GetName());
    else if( message.has_crowd_destruction() )
        Destroy( populations_, message.crowd_destruction().crowd().id(), message.crowd_destruction() );
    else if( message.has_crowd_update() )
        populations_.Get( message.crowd_update().crowd().id() ).Update( message.crowd_update() );
    else if( message.has_crowd_concentration_creation() )
        populations_.Get( message.crowd_concentration_creation().crowd().id() ).Update( message.crowd_concentration_creation() );
    else if( message.has_crowd_concentration_destruction() )
        populations_.Get( message.crowd_concentration_destruction().crowd().id() ).Update( message.crowd_concentration_destruction() );
    else if( message.has_crowd_concentration_update() )
        populations_.Get( message.crowd_concentration_update().crowd().id() ).Update( message.crowd_concentration_update() );
    else if( message.has_crowd_flow_creation() )
        populations_.Get( message.crowd_flow_creation().crowd().id() ).Update( message.crowd_flow_creation() );
    else if( message.has_crowd_flow_destruction() )
        populations_.Get( message.crowd_flow_destruction().crowd().id() ).Update( message.crowd_flow_destruction() );
    else if( message.has_crowd_flow_update() )
        populations_.Get( message.crowd_flow_update().crowd().id() ).Update( message.crowd_flow_update() );

    else if( message.has_crowd_knowledge_creation() )
        CreateUpdate< PopulationKnowledge >( populationKnowledges_, message.crowd_knowledge_creation().knowledge().id(), message.crowd_knowledge_creation() );
    else if( message.has_crowd_knowledge_update() )
        populationKnowledges_.Get( message.crowd_knowledge_update().knowledge().id() ).Update( message.crowd_knowledge_update() );
    else if( message.has_crowd_knowledge_destruction() )
        Destroy( populationKnowledges_, message.crowd_knowledge_destruction().knowledge().id(), message.crowd_knowledge_destruction() );
    else if( message.has_crowd_concentration_knowledge_creation() )
        populationKnowledges_.Get( message.crowd_concentration_knowledge_creation().crowd().id() ).Update( message.crowd_concentration_knowledge_creation() );
    else if( message.has_crowd_concentration_knowledge_update() )
        populationKnowledges_.Get( message.crowd_concentration_knowledge_update().crowd().id() ).Update( message.crowd_concentration_knowledge_update() );
    else if( message.has_crowd_concentration_knowledge_destruction() )
        populationKnowledges_.Get( message.crowd_concentration_knowledge_destruction().crowd().id() ).Update( message.crowd_concentration_knowledge_destruction() );
    else if( message.has_crowd_flow_knowledge_creation() )
        populationKnowledges_.Get( message.crowd_flow_knowledge_creation().crowd().id() ).Update( message.crowd_flow_knowledge_creation() );
    else if( message.has_crowd_flow_knowledge_update() )
        populationKnowledges_.Get( message.crowd_flow_knowledge_update().crowd().id() ).Update( message.crowd_flow_knowledge_update() );
    else if( message.has_crowd_flow_knowledge_destruction() )
        populationKnowledges_.Get( message.crowd_flow_knowledge_destruction().crowd().id() ).Update( message.crowd_flow_knowledge_destruction() );
    else if( message.has_folk_creation() )
        folk_->Update( message.folk_creation() );

    else if( message.has_urban_creation() )
        CreateUpdate< UrbanObject >( urbanBlocks_, message.urban_creation().object().id(), message.urban_creation() );
    else if( message.has_urban_update() )
        urbanBlocks_.Get( message.urban_update().object().id() ).Update( message.urban_update() );

    else if( message.has_urban_knowledge_creation() )
        CreateUpdate< UrbanKnowledge >( urbanKnowledges_, message.urban_knowledge_creation().knowledge().id(), message.urban_knowledge_creation() );
    else if( message.has_urban_knowledge_update() )
        urbanKnowledges_.Get( message.urban_knowledge_update().knowledge().id() ).Update( message.urban_knowledge_update() );
    else if( message.has_urban_knowledge_destruction() )
        Destroy( urbanKnowledges_, message.urban_knowledge_destruction().knowledge().id(), message.urban_knowledge_destruction() );
    else if( message.has_control_global_weather() )
        meteoModel_->OnReceiveMsgGlobalMeteo( message.control_global_weather() );
    else if( message.has_control_local_weather_creation() )
        meteoModel_->OnReceiveMsgLocalMeteoCreation( message.control_local_weather_creation() );
    else if( message.has_control_local_weather_destruction() )
        meteoModel_->OnReceiveMsgLocalMeteoDestruction( message.control_local_weather_destruction() );
    else if( message.has_formation_change_superior() )
        formations_.Get( message.formation_change_superior().formation().id() ).Update( message.formation_change_superior() );
    else if( message.has_magic_order() )
        CreateUpdate< MagicOrder >( magicOrders_, message.magic_order().id(), message.magic_order() );
    else if( message.has_magic_order_destruction() )
        Destroy( magicOrders_, message.magic_order_destruction().id(), nullptr );
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
    else throw MASA_EXCEPTION( "unknown entity: " + boost::lexical_cast< std::string >( id ) );
}

// -----------------------------------------------------------------------------
// Name: Model::Destroy
// Created: BAX 2014-02-24
// -----------------------------------------------------------------------------
template< typename T, typename M >
void Model::Destroy( tools::Resolver< T >& resolver, unsigned id, const M* message )
{
    if( T* entity = resolver.Find( id ) )
    {
        if( message )
            entity->Update( *message );
        if( auto sync = entity->Retrieve< ReplaySynchronisations >() )
        {
            if( sync->MustBeDestroyedLater() )
                return;
        }
        resolver.Remove( id );
        delete entity;
    }
}

// -----------------------------------------------------------------------------
// Name: Model::Destroy
// Created: SBO 2008-07-09
// -----------------------------------------------------------------------------
template< typename T, typename M >
void Model::Destroy( tools::Resolver< T >& resolver, unsigned id, const M& message )
{
    Destroy( resolver, id, &message );
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
void Model::SendReplayInfo( ClientPublisher_ABC& publisher, unsigned int totalTicks, const std::string& endDateTime, sword::EnumSimulationState status, unsigned int factor, unsigned int firstTick /*= 1*/ ) const
{
    simulation_->SendReplayInfo( publisher, totalTicks, endDateTime, status, factor, firstTick );
}

// -----------------------------------------------------------------------------
// Name: Model::Accept
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void Model::Accept( kernel::ModelVisitor_ABC& visitor ) const
{
    urbanBlocks_           .Apply( boost::bind( &dispatcher::UrbanObject_ABC::Accept, _1, boost::ref( visitor ) ) );
    sides_                 .Apply( boost::bind( &dispatcher::Team_ABC::Accept, _1, boost::ref( visitor ) ) );
    agentKnowledges_       .Apply( boost::bind( &dispatcher::AgentKnowledge_ABC::Accept, _1, boost::ref( visitor ) ) );
    objectKnowledges_      .Apply( boost::bind( &dispatcher::ObjectKnowledge_ABC::Accept, _1, boost::ref( visitor ) ) );
    populationKnowledges_  .Apply( boost::bind( &PopulationKnowledge::Accept, _1, boost::ref( visitor ) ) );
    logConsignsMaintenance_.Apply( boost::bind( &LogConsignMaintenance::Accept, _1, boost::ref( visitor ) ) );
    logConsignsSupply_     .Apply( boost::bind( &LogConsignSupply::Accept, _1, boost::ref( visitor ) ) );
    logConsignsMedical_    .Apply( boost::bind( &LogConsignMedical::Accept, _1, boost::ref( visitor ) ) );
    logConsignsFuneral_    .Apply( boost::bind( &LogConsignFuneral::Accept, _1, boost::ref( visitor ) ) );
    fires_                 .Apply( boost::bind( &Fire::Accept, _1, boost::ref( visitor ) ) );
    populationFires_       .Apply( boost::bind( &PopulationFire::Accept, _1, boost::ref( visitor ) ) );
    fireEffects_           .Apply( boost::bind( &FireEffect::Accept, _1, boost::ref( visitor ) ) );
    detectionRangeEffects_ .Apply( boost::bind( &DetectionRangeEffect::Accept, _1, boost::ref( visitor ) ) );
    reports_               .Apply( boost::bind( &Report::Accept, _1, boost::ref( visitor ) ) );
    urbanKnowledges_       .Apply( boost::bind( &dispatcher::UrbanKnowledge_ABC::Accept, _1, boost::ref( visitor ) ) );
    magicOrders_           .Apply( boost::bind( &dispatcher::MagicOrder::Accept, _1, boost::ref( visitor ) ) );
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
        throw MASA_EXCEPTION( "Misformed tasker in protocol message." );
}

// -----------------------------------------------------------------------------
// Name: Model::GetExtensionTypes
// Created: RPD 2010-12-28
// -----------------------------------------------------------------------------
const kernel::ExtensionTypes& Model::GetExtensionTypes() const
{
    return staticModel_.extensions_;
}

// -----------------------------------------------------------------------------
// Name: Model::DeleteCheckpoint
// Created: LDC 2011-08-22
// -----------------------------------------------------------------------------
void Model::DeleteCheckpoint( const std::string& name )
{
    const tools::Path oldPath = config_.GetCheckpointDirectory( tools::Path::FromUTF8( name ) );
    oldPath.RemoveAll();
}

// -----------------------------------------------------------------------------
// Name: Model::ClearOldReports
// Created: LDC 2011-09-13
// -----------------------------------------------------------------------------
void Model::ClearOldReports( unsigned int tick )
{
    const unsigned hz = config_.GetReportsClearFrequency();
    if( hz && !( tick % hz ) )
        reports_.DeleteAll();
}

namespace
{
    template< typename T >
    struct FillObsoleteConsigns : boost::noncopyable
    {
        FillObsoleteConsigns( unsigned int unitId ) : unitId_( unitId ) {}
        void operator()( const T& consign ) const
        {
            if( consign.IsObsoleteForUnit( unitId_ ) )
                obsoleteIds_.push_back( consign.GetId() );
        }
        unsigned int unitId_;
        mutable std::vector< unsigned long > obsoleteIds_;
    };

    template< typename T >
    void ClearConsigns( tools::Resolver< T >& resolver, unsigned int unitId )
    {
        FillObsoleteConsigns< T > functor( unitId );
        resolver.Apply( functor );
        for( auto it = functor.obsoleteIds_.begin(); it != functor.obsoleteIds_.end(); ++it )
            resolver.Delete( *it );
    }
}

// -----------------------------------------------------------------------------
// Name: Model::ClearLogisticRequests
// Created: JSR 2013-03-14
// -----------------------------------------------------------------------------
void Model::ClearLogisticRequests( unsigned int unitId )
{
    // Just to avoid a random crash if a supply consign has not been properly deleted by the simulation when an unit is destroyed
    ClearConsigns( logConsignsMaintenance_, unitId );
    ClearConsigns( logConsignsSupply_, unitId );
    ClearConsigns( logConsignsMedical_, unitId );
    ClearConsigns( logConsignsFuneral_, unitId );
}
