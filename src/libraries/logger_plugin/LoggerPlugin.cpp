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
#include "tools/SessionConfig.h"
#include "reports/ReportFactory.h"
#include "MT_Tools/MT_FileLogger.h"
#include <boost/lexical_cast.hpp>
#pragma warning( disable: 4996 )

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
    : filename_    ( config.BuildSessionChildFile( "Messages.log" ).c_str() )
    , pLogger_     ( 0 )
    , resolver_    ( model )
    , factory_     ( resolver_, objectTypes_, objectTypes_, 0 )
    , model_       ( model )
    , staticModel_ ( staticModel )
    , services_    ( services )
    , enabled_     ( true )
    , initialized_ ( false )
    , simulation_  ( new Simulation() )
    , actions_     ( new ActionsLogger( config, model, staticModel, *simulation_ ) )
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
        if( !services_.HasService< replay::Service >() )
        {
            if( !pLogger_.get() )
                pLogger_.reset( new MT_FileLogger( filename_.c_str(), MT_Logger_ABC::eLogLevel_All, true, MT_Logger_ABC::eLoggerPlugin ) );
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
    if( message.message().has_report() )
    {
        int id = 0;
        if( message.message().report().source().has_unit() )
            id = message.message().report().source().unit().id();
        else if( message.message().report().source().has_automat() )
            id = message.message().report().source().automat().id();
        else if( message.message().report().source().has_crowd() )
            id = message.message().report().source().crowd().id();
        kernel::Entity_ABC* entity = Find( model_, id );
        FormatMessage( factory_.FormatReport( message.message().report() ), "Report",
                       entity ? entity->GetName().ascii() : "Unknown entity", id,
                       factory_.GetTime( message.message().report().time() ).toString( "hh:mm:ss" ).ascii() );
    }
    else if( message.message().has_trace() )
    {
        int id = 0;
        if( message.message().trace().source().has_unit() )
            id = message.message().trace().source().unit().id();
        else if( message.message().trace().source().has_automat() )
            id = message.message().trace().source().automat().id();
        else if( message.message().trace().source().has_crowd() )
            id = message.message().trace().source().crowd().id();
        kernel::Entity_ABC* entity = Find( model_, id );
        FormatMessage( message.message().trace().message(), "Trace",
                       entity ? entity->GetName().ascii() : "Unknown entity", id, date_ );
    }
    else if( message.message().has_control_information() )
    {
        date_ = Format( message.message().control_information().date_time().data().c_str() );
        simulation_->Update( message.message().control_information() );
    }
    else if( message.message().has_control_begin_tick() )
    {
        date_ = Format( message.message().control_begin_tick().date_time().data().c_str() );
        simulation_->Update( message.message().control_begin_tick() );
    }
    else if( message.message().has_control_end_tick() )
        nCurrentTick_ = message.message().control_end_tick().current_tick();
    else if( message.message().has_unit_order() )
    {
        kernel::Entity_ABC* agent = model_.Agents().Find( message.message().unit_order().tasker().id() );
        if( agent )
            FormatMission( agent->GetName().ascii(), message.message().unit_order().tasker().id(), message.message().unit_order().type().id() );
        actions_->Log( message.message().unit_order() );
    }
    else if( message.message().has_automat_order() )
    {
        kernel::Entity_ABC* automat = model_.Automats().Find( message.message().automat_order().tasker().id() );
        if( automat )
            FormatMission( automat->GetName().ascii(), message.message().automat_order().tasker().id(), message.message().automat_order().type().id() );
        actions_->Log( message.message().automat_order() );
    }
    else if( message.message().has_crowd_order() )
    {
        kernel::Entity_ABC* population = model_.Populations().Find( message.message().crowd_order().tasker().id() );
        if( population )
            FormatMission( population->GetName().ascii(), message.message().crowd_order().tasker().id(), message.message().crowd_order().type().id() );
        actions_->Log( message.message().crowd_order() );
    }
    else if( message.message().has_start_unit_fire() )
    {
        kernel::Entity_ABC* agent = model_.Agents().Find( message.message().start_unit_fire().firing_unit().id() );
        if( agent )
        {
            kernel::Entity_ABC* target = 0;
            if( message.message().start_unit_fire().target().has_unit() )
                target = model_.Agents().Find( message.message().start_unit_fire().target().unit().id() );
            else if( message.message().start_unit_fire().target().has_crowd() )
                target = model_.Populations().Find( message.message().start_unit_fire().target().crowd().id() );
            std::string text = "Fire on ";
            if( target )
                text += boost::lexical_cast< std::string >( target->GetName().ascii() ) + "[" +
                        boost::lexical_cast< std::string >( target->GetId() ) + "]";
            else
                text += "position";
            //*file_ << ", ammo = " << message.message().start_unit_fire().ammunition();
            FormatMessage( text, "Fire", agent->GetName().ascii(), message.message().start_unit_fire().firing_unit().id(), date_ );
        }
    }
    else if( message.message().has_stop_unit_fire() )
    {
        if( message.message().stop_unit_fire().has_units_damages() )
            LogUnitsFireDamages( message.message().stop_unit_fire().units_damages() );
        if( message.message().stop_unit_fire().has_crowds_damages() )
            LogPopulationsFireDamages( message.message().stop_unit_fire().crowds_damages() );
    }
    else if( message.message().has_start_crowd_fire() )
    {
        kernel::Entity_ABC* agent = model_.Populations().Find( message.message().start_crowd_fire().firing_crowd().id() );
        if( agent )
            FormatMessage( "", "Fire", agent->GetName().ascii(), message.message().start_crowd_fire().firing_crowd().id(), date_ );
    }
    else if( message.message().has_stop_crowd_fire() )
    {
        if( message.message().stop_crowd_fire().has_units_damages() )
            LogUnitsFireDamages( message.message().stop_crowd_fire().units_damages() );
    }
}

// -----------------------------------------------------------------------------
// Name: LoggerPlugin::FormatMission
// Created: LDC 2010-03-18
// -----------------------------------------------------------------------------
void LoggerPlugin::FormatMission( const char* name, int id, int mission )
{
    if( mission )
        FormatMessage( staticModel_.types_.tools::Resolver< kernel::MissionType >::Get( mission ).GetName(),
                       "Mission", name, id, date_ );
    else
        FormatMessage( "Mission cancelled.", "Mission", name, id, date_ );
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
                FormatMessage( text, "Fire", target->GetName().ascii(), target->GetId(), date_ );
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
            FormatMessage( text, "Fire", target.GetName().ascii(), target.GetId(), date_ );
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
            FormatMessage( text, "Fire", target.GetName().ascii(), target.GetId(), date_ );
        }
    }
}
