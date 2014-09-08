// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "LoggerPlugin.h"
#include "ActionsLogger.h"
#include "Simulation.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/MissionType.h"
#include "clients_kernel/StaticModel.h"
#include "dispatcher/Agent.h"
#include "dispatcher/Automat.h"
#include "dispatcher/Model_ABC.h"
#include "dispatcher/Population.h"
#include "dispatcher/Services.h"
#include "protocol/Dispatcher.h"
#include "protocol/Simulation.h"
#include "tools/SessionConfig.h"
#include "reports/ReportFactory.h"
#include "MT_Tools/MT_FileLogger.h"
#include <boost/lexical_cast.hpp>

namespace replay
{
    class Service;
}

using namespace plugins::logger;

// -----------------------------------------------------------------------------
// Name: LoggerPlugin constructor
// Created: LDC 2010-03-17
// -----------------------------------------------------------------------------
LoggerPlugin::LoggerPlugin( const dispatcher::Model_ABC& model, const kernel::StaticModel& staticModel, const tools::SessionConfig& config, const dispatcher::Services& services )
    : sessionConfig_( config )
    , filename_    ( config.BuildSessionChildFile( "Messages.log" ) )
    , resolver_    ( model )
    , factory_     ( resolver_, objectTypes_, objectTypes_, 0 )
    , model_       ( model )
    , staticModel_ ( staticModel )
    , services_    ( services )
    , enabled_     ( true )
    , initialized_ ( false )
    , simulation_  ( new Simulation() )
    , actions_     ( new ActionsLogger( config, model, *simulation_ ) )
    , nCurrentTick_( 0 )
{
    objectTypes_.Load( config );
    factory_.Load( config );
}

// -----------------------------------------------------------------------------
// Name: LoggerPlugin destructor
// Created: LDC 2010-03-17
// -----------------------------------------------------------------------------
LoggerPlugin::~LoggerPlugin()
{
    // NOTHING
}

namespace
{
    kernel::Entity_ABC* Find( const dispatcher::Model_ABC& model, int id )
    {
        kernel::Entity_ABC* entity = model.Agents().Find( id );
        if( !entity )
            entity = model.Automats().Find( id );
        if( !entity )
            entity = model.Populations().Find( id );
        return entity;
    }
    std::string Format( const char* date )
    {
        std::stringstream stream;
        stream << date[9] << date[10] << ":" << date[11] << date[12] << ":" << date[13] << date[14];
        return stream.str();
    }
}

// -----------------------------------------------------------------------------
// Name: LoggerPlugin::Initialize
// Created: LDC 2010-03-19
// -----------------------------------------------------------------------------
bool LoggerPlugin::Initialize()
{
    if( !initialized_ )
    {
        if( !services_.HasService( sword::service_replay ) )
        {
            if( !pLogger_.get() )
                pLogger_.reset( new MT_FileLogger( filename_,
                                                   sessionConfig_.GetLoggerPluginLogFiles(),
                                                   sessionConfig_.GetLoggerPluginLogSize(),
                                                   sessionConfig_.GetLoggerPluginLogLevel(),
                                                   !sessionConfig_.HasCheckpoint(), MT_Logger_ABC::eLoggerPlugin, sessionConfig_.IsLoggerPluginLogInBytes() ) );
        }
        else
            enabled_ = false;
        initialized_ = true;
    }
    return enabled_;
}

// -----------------------------------------------------------------------------
// Name: LoggerPlugin::FormatMessage
// Created: LGY 2011-09-07
// -----------------------------------------------------------------------------
void LoggerPlugin::FormatMessage( const std::string& message, const std::string& level, const std::string& entity,
                                  unsigned int id, const std::string& date )
{
    if( pLogger_.get() )
    {
        std::string information = "**** Time tick " + boost::lexical_cast< std::string >( nCurrentTick_ ) + " - [" + date + "] - "
                                + level + " - " + entity +  "[" + boost::lexical_cast< std::string >( id ) + "] : " + message;
        pLogger_->Log( MT_Logger_ABC::eLogLevel_Info, information.c_str() );
    }
}

// -----------------------------------------------------------------------------
// Name: LoggerPlugin::Receive
// Created: LDC 2010-03-17
// -----------------------------------------------------------------------------
void LoggerPlugin::Receive( const sword::SimToClient& message )
{
    if( !Initialize() )
        return;
    const auto& msg = message.message();
    if( msg.has_report() )
    {
        int id = 0;
        if( msg.report().source().has_unit() )
            id = msg.report().source().unit().id();
        else if( msg.report().source().has_automat() )
            id = msg.report().source().automat().id();
        else if( msg.report().source().has_crowd() )
            id = msg.report().source().crowd().id();
        kernel::Entity_ABC* entity = Find( model_, id );
        FormatMessage( factory_.FormatReport( entity, msg.report() ), "Report",
                       entity ? entity->GetName().toStdString() : "Unknown entity", id,
                       factory_.GetTime( msg.report().time() ).toString( "hh:mm:ss" ).toStdString() );
    }
    else if( msg.has_trace() )
    {
        int id = 0;
        if( msg.trace().source().has_unit() )
            id = msg.trace().source().unit().id();
        else if( msg.trace().source().has_automat() )
            id = msg.trace().source().automat().id();
        else if( msg.trace().source().has_crowd() )
            id = msg.trace().source().crowd().id();
        kernel::Entity_ABC* entity = Find( model_, id );
        FormatMessage( msg.trace().message(), "Trace",
                       entity ? entity->GetName().toStdString() : "Unknown entity", id, date_ );
    }
    else if( msg.has_control_information() )
    {
        date_ = Format( msg.control_information().date_time().data().c_str() );
        simulation_->Update( msg.control_information() );
    }
    else if( msg.has_control_begin_tick() )
    {
        date_ = Format( msg.control_begin_tick().date_time().data().c_str() );
        simulation_->Update( msg.control_begin_tick() );
    }
    else if( msg.has_control_end_tick() )
    {
        nCurrentTick_ = msg.control_end_tick().current_tick();
        if( pLogger_.get() )
        {
            std::string information = "**** Time tick " + boost::lexical_cast< std::string >( nCurrentTick_ ) + " - [" + date_ + "] - "
                                    + boost::lexical_cast< std::string >( missions_.size() ) + " Missions running";
            pLogger_->Log( MT_Logger_ABC::eLogLevel_Info, information.c_str() );
        }
    }
    else if( msg.has_unit_order() )
    {
        kernel::Entity_ABC* agent = model_.Agents().Find( msg.unit_order().tasker().id() );
        if( agent )
            FormatMission( agent->GetName().toStdString().c_str(), msg.unit_order().tasker().id(), msg.unit_order().type().id() );
        actions_->Log( msg.unit_order() );
    }
    else if( msg.has_automat_order() )
    {
        kernel::Entity_ABC* automat = model_.Automats().Find( msg.automat_order().tasker().id() );
        if( automat )
            FormatMission( automat->GetName().toStdString().c_str(), msg.automat_order().tasker().id(), msg.automat_order().type().id() );
        actions_->Log( msg.automat_order() );
    }
    else if( msg.has_crowd_order() )
    {
        kernel::Entity_ABC* population = model_.Populations().Find( msg.crowd_order().tasker().id() );
        if( population )
            FormatMission( population->GetName().toStdString().c_str(), msg.crowd_order().tasker().id(), msg.crowd_order().type().id() );
        actions_->Log( msg.crowd_order() );
    }
    else if( msg.has_start_unit_fire() )
    {
        kernel::Entity_ABC* agent = model_.Agents().Find( msg.start_unit_fire().firing_unit().id() );
        if( agent )
        {
            kernel::Entity_ABC* target = 0;
            if( msg.start_unit_fire().target().has_unit() )
                target = model_.Agents().Find( msg.start_unit_fire().target().unit().id() );
            else if( msg.start_unit_fire().target().has_crowd() )
                target = model_.Populations().Find( msg.start_unit_fire().target().crowd().id() );
            std::string text = "Fire on ";
            if( target )
                text += boost::lexical_cast< std::string >( target->GetName().toStdString() ) + "[" +
                        boost::lexical_cast< std::string >( target->GetId() ) + "]";
            else
                text += "position";
            //*file_ << ", ammo = " << msg.start_unit_fire().ammunition();
            FormatMessage( text, "Fire", agent->GetName().toStdString(), msg.start_unit_fire().firing_unit().id(), date_ );
        }
    }
    else if( msg.has_stop_unit_fire() )
    {
        if( msg.stop_unit_fire().has_units_damages() )
            LogUnitsFireDamages( msg.stop_unit_fire().units_damages() );
        if( msg.stop_unit_fire().has_crowds_damages() )
            LogPopulationsFireDamages( msg.stop_unit_fire().crowds_damages() );
    }
    else if( msg.has_start_crowd_fire() )
    {
        kernel::Entity_ABC* agent = model_.Populations().Find( msg.start_crowd_fire().firing_crowd().id() );
        if( agent )
            FormatMessage( "", "Fire", agent->GetName().toStdString(), msg.start_crowd_fire().firing_crowd().id(), date_ );
    }
    else if( msg.has_stop_crowd_fire() )
    {
        if( msg.stop_crowd_fire().has_units_damages() )
            LogUnitsFireDamages( msg.stop_crowd_fire().units_damages() );
    }
    else if( msg.has_control_checkpoint_save_end() )
    {
        actions_->SaveCheckpointActiveMissions( msg.control_checkpoint_save_end().name() );
    }
    else if( msg.has_unit_magic_action_ack() )
    {
        actions_->Log( msg.unit_magic_action_ack() );
    }
}

// -----------------------------------------------------------------------------
// Name: LoggerPlugin::Close
// Created: LDC 2013-02-21
// -----------------------------------------------------------------------------
void LoggerPlugin::Close()
{
    actions_->Close();
}

// -----------------------------------------------------------------------------
// Name: LoggerPlugin::FormatMission
// Created: LDC 2010-03-18
// -----------------------------------------------------------------------------
void LoggerPlugin::FormatMission( const char* name, int id, int mission )
{
    if( mission )
    {
        FormatMessage( staticModel_.types_.tools::Resolver< kernel::MissionType >::Get( mission ).GetName(),
                       "Mission", name, id, date_ );
        missions_.insert( id );
    }
    else
    {
        FormatMessage( "Mission cancelled.", "Mission", name, id, date_ );
        missions_.erase( id );
    }
}

// -----------------------------------------------------------------------------
// Name: LoggerPlugin::LogUnitsFireDamages
// Created: JSR 2010-08-04
// -----------------------------------------------------------------------------
void LoggerPlugin::LogUnitsFireDamages( const sword::UnitsFireDamages& unitsDamages )
{
    for( int i = 0; i < unitsDamages.elem_size(); ++i )
    {
        const sword::UnitFireDamages& damages = unitsDamages.elem( i );
        if( damages.has_target() )
        {
            kernel::Entity_ABC* target = model_.Agents().Find( damages.target().id() );
            if( target )
                LogDamagesOnTarget( damages, *target );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: LoggerPlugin::LogPopulationsFireDamages
// Created: JSR 2010-08-04
// -----------------------------------------------------------------------------
void LoggerPlugin::LogPopulationsFireDamages( const sword::CrowdsFireDamages& populationsDamages )
{
    for( int i = 0; i < populationsDamages.elem_size(); ++i )
    {
        const sword::CrowdFireDamages& damages = populationsDamages.elem( i );
        if( damages.has_target() && damages.has_dead() && damages.dead() > 0 )
        {
            kernel::Entity_ABC* target = model_.Populations().Find( damages.target().id() );
            if( target )
            {
                std::string text = "damaged : " + boost::lexical_cast< std::string >( damages.dead() ) + " killed";
                FormatMessage( text, "Fire", target->GetName().toStdString(), target->GetId(), date_ );
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: LoggerPlugin::LogDamagesOnTarget
// Created: JSR 2010-08-04
// -----------------------------------------------------------------------------
void LoggerPlugin::LogDamagesOnTarget( const sword::UnitFireDamages& unitDamages, const kernel::Entity_ABC& target )
{
    if( unitDamages.has_humans() )
    {
        int dead = 0;
        int wounded = 0;
        for( int i = 0; i < unitDamages.humans().elem_size(); ++i )
        {
            const sword::UnitHumanFireDamage& humanDamage = unitDamages.humans().elem( i );
            if( humanDamage.has_dead() )
                dead += humanDamage.dead();
            if( humanDamage.has_wounded_u1() )
                wounded += humanDamage.wounded_u1();
            if( humanDamage.has_wounded_u2() )
                wounded += humanDamage.wounded_u2();
            if( humanDamage.has_wounded_u3() )
                wounded += humanDamage.wounded_u3();
            if( humanDamage.has_wounded_ue() )
                wounded += humanDamage.wounded_ue();
        }
        if( dead > 0 || wounded > 0 )
        {
            std::string text = "damaged : ";
            if( dead > 0 )
                text += boost::lexical_cast< std::string >( dead ) + " killed";
            if( wounded > 0 )
            {
                if( dead > 0 )
                    text += ", ";
                text += boost::lexical_cast< std::string >( wounded ) + " wounded";
            }
            FormatMessage( text, "Fire", target.GetName().toStdString(), target.GetId(), date_ );
        }
    }

    if( unitDamages.has_equipments() )
    {
        int repairable = 0;
        int unavailable = 0;
        for( int i = 0; i < unitDamages.equipments().elem_size(); ++i )
        {
            const sword::UnitEquipmentFireDamage& equipmentDamage = unitDamages.equipments().elem( i );
            if( equipmentDamage.has_unavailable() )
                unavailable += equipmentDamage.unavailable();
            if( equipmentDamage.has_repairable() )
                repairable += equipmentDamage.repairable();
        }
        if( repairable > 0 || unavailable > 0)
        {
            std::string text = "damaged : ";
            if( unavailable > 0 )
                text += boost::lexical_cast< std::string >( unavailable ) + " unavailable";
            if( repairable > 0 )
            {
                if( unavailable > 0 )
                    text += ", ";
                text += boost::lexical_cast< std::string >( repairable ) + " repairable";
            }
            FormatMessage( text, "Fire", target.GetName().toStdString(), target.GetId(), date_ );
        }
    }
}
