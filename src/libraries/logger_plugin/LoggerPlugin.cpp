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
    : filename_   ( config.BuildSessionChildFile( "Messages.log" ).c_str() )
    , file_       ( 0 )
    , resolver_   ( model )
    , factory_    ( resolver_, objectTypes_, objectTypes_, 0 )
    , model_      ( model )
    , staticModel_( staticModel )
    , services_   ( services )
    , enabled_    ( true )
    , initialized_( false )
    , simulation_ ( new Simulation() )
    , actions_    ( new ActionsLogger( config, model, staticModel, *simulation_ ) )
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
    if( file_ )
        file_->close();
}

// -----------------------------------------------------------------------------
// Name: LoggerPlugin::NotifyClientAuthenticated
// Created: LDC 2010-03-17
// -----------------------------------------------------------------------------
void LoggerPlugin::NotifyClientAuthenticated( dispatcher::ClientPublisher_ABC& /*client*/, dispatcher::Profile_ABC& /*profile*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LoggerPlugin::NotifyClientLeft
// Created: LDC 2010-03-17
// -----------------------------------------------------------------------------
void LoggerPlugin::NotifyClientLeft( dispatcher::ClientPublisher_ABC& /*client*/ )
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
            if( !file_ )
            {
                file_ = new std::ofstream( filename_.c_str() );
                if( !file_ || !*file_ || !file_->is_open() )
                    enabled_ = false;
            }
        }
        else
            enabled_ = false;
        initialized_ = true;
    }
    return enabled_;
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
        std::string messageText = factory_.FormatReport( message.message().report() );
        *file_ << factory_.GetTime( message.message().report().time() ).toString( "hh:mm:ss" ).ascii()
               << " Report - " << ( entity ? entity->GetName() : "Unknown entity" ) << "[" << id << "] : "
               << messageText << std::endl;
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
        *file_ << date_
               << " Trace - " << ( entity ? entity->GetName() : "Unknown entity" ) << "[" << id << "] : "
               << message.message().trace().message() << std::endl;
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
            *file_ << date_ << " Fire - " << agent->GetName() << "[" << message.message().start_unit_fire().firing_unit().id()
                   << "] : Fire on ";
            if( target )
                *file_ << target->GetName() << "[" << target->GetId() << "]";
            else
                *file_ << "position";
            //*file_ << ", ammo = " << message.message().start_unit_fire().ammunition();
            *file_ << std::endl;
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
            *file_ << date_ << " Fire - " << agent->GetName() << "[" << message.message().start_crowd_fire().firing_crowd().id() << "] " << std::endl;
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
        *file_ << date_ << " Mission - " << name << "[" << id << "] : "
               << staticModel_.types_.tools::Resolver< kernel::MissionType >::Get( mission ).GetName() << std::endl;
    else
        *file_ << date_ << " Mission - " << name << "[" << id << "] : Mission cancelled." << std::endl;
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
        if( damages.has_target() && damages.has_dead_nbr() && damages.dead_nbr() > 0 )
        {
            kernel::Entity_ABC* target = model_.Populations().Find( damages.target().id() );
            if( target )
                *file_ << date_ << " Fire - " << target->GetName() << "[" << target->GetId() << "] damaged : " << damages.dead_nbr() << " killed" << std::endl;
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
            if( humanDamage.has_dead_nbr() )
                dead += humanDamage.dead_nbr();
            if( humanDamage.has_wounded_u1_nbr() )
                wounded += humanDamage.wounded_u1_nbr();
            if( humanDamage.has_wounded_u2_nbr() )
                wounded += humanDamage.wounded_u2_nbr();
            if( humanDamage.has_wounded_u3_nbr() )
                wounded += humanDamage.wounded_u3_nbr();
            if( humanDamage.has_wounded_ue_nbr() )
                wounded += humanDamage.wounded_ue_nbr();
        }
        if( dead > 0 || wounded > 0 )
        {
            *file_ << date_ << " Fire - " << target.GetName() << "[" << target.GetId() << "] damaged : ";
            if( dead > 0 )
                *file_ << dead << " killed";
            if( wounded > 0 )
            {
                if( dead > 0 )
                    *file_ << ", ";
                *file_ << wounded << " wounded";
            }
            *file_ << std::endl;
        }
    }

    if( unitDamages.has_equipments() )
    {
        int repairable = 0;
        int unavailable = 0;
        for( int i = 0; i < unitDamages.equipments().elem_size(); ++i )
        {
            const sword::UnitEquipmentFireDamage& equipmentDamage = unitDamages.equipments().elem( i );
            if( equipmentDamage.has_unavailable_nbr() )
                unavailable += equipmentDamage.unavailable_nbr();
            if( equipmentDamage.has_repairable_nbr() )
                repairable += equipmentDamage.repairable_nbr();
        }
        if( repairable > 0 || unavailable > 0)
        {
            *file_ << date_ << " Fire - " << target.GetName() << "[" << target.GetId() << "] damaged : ";
            if( unavailable > 0 )
                *file_ << unavailable << " unavailable";
            if( repairable > 0 )
            {
                if( unavailable > 0 )
                    *file_ << ", ";
                *file_ << repairable << " repairable";
            }
            *file_ << std::endl;
        }
    }
}
