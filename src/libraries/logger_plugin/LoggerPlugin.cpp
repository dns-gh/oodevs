// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "LoggerPlugin.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/MissionType.h"
#include "clients_kernel/StaticModel.h"
#include "dispatcher/Agent.h"
#include "dispatcher/Automat.h"
#include "dispatcher/Model.h"
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
LoggerPlugin::LoggerPlugin( const dispatcher::Model& model, const kernel::StaticModel& staticModel, const tools::SessionConfig& config, const dispatcher::Services& services )
    : filename_   ( config.BuildSessionChildFile( "Messages.log" ).c_str() )
    , file_       ( 0 )
    , resolver_   ( model )
    , factory_    ( resolver_, objectTypes_, objectTypes_, 0 )
    , model_      ( model )
    , staticModel_( staticModel )
    , services_   ( services )
    , enabled_    ( true )
    , initialized_( false )
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
    kernel::Entity_ABC* Find( const dispatcher::Model& model, int id )
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
        stream << date[9] << date[10] << ":" << date[11] << date[12];
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
                if ( !file_ || !*file_ || !file_->is_open() )
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
void LoggerPlugin::Receive( const MsgsSimToClient::MsgSimToClient& message )
{
    if( !Initialize() )
        return;
    if( message.message().has_report() )
    {
        int id = message.message().report().oid();
        kernel::Entity_ABC* entity = Find( model_, id );
        std::string messageText = factory_.FormatReport( message.message().report() );
        *file_ << factory_.GetTime( message.message().report().time() ).toString( "hh:mm" ).ascii()
               << " Report - " << ( entity ? entity->GetName() : "Unknown entity" ) << "[" << id << "] : "
               << messageText << std::endl;
    }
    else if( message.message().has_trace() )
    {
        int id = message.message().trace().oid();
        kernel::Entity_ABC* entity = Find( model_, id );
        *file_ << date_
               << " Trace - " << ( entity ? entity->GetName() : "Unknown entity" ) << "[" << id << "] : "
               << message.message().trace().message() << std::endl;
    }
    else if( message.message().has_control_information() )
        date_ = Format( message.message().control_information().date_time().data().c_str() );
    else if( message.message().has_control_begin_tick() )
        date_ = Format( message.message().control_begin_tick().date_time().data().c_str() );
    else if( message.message().has_unit_order() )
    {
        kernel::Entity_ABC* agent = model_.Agents().Find( message.message().unit_order().oid() );
        if( agent )
            FormatMission( agent->GetName().ascii(), message.message().unit_order().oid(), message.message().unit_order().mission() );
    }
    else if( message.message().has_automat_order() )
    {
        kernel::Entity_ABC* automat = model_.Automats().Find( message.message().automat_order().oid() );
        if( automat )
            FormatMission( automat->GetName().ascii(), message.message().automat_order().oid(), message.message().automat_order().mission() );
    }
    else if( message.message().has_population_order() )
    {
        kernel::Entity_ABC* population = model_.Populations().Find( message.message().population_order().oid() );
        if( population )
            FormatMission( population->GetName().ascii(), message.message().population_order().oid(), message.message().population_order().mission() );
    }
    else if( message.message().has_start_unit_fire() )
    {
        kernel::Entity_ABC* agent = model_.Agents().Find( message.message().start_unit_fire().firer_oid() );
        if( agent )
        {
            kernel::Entity_ABC* target = 0;
            if( message.message().start_unit_fire().target().has_unit() )
                target = model_.Agents().Find( message.message().start_unit_fire().target().unit() );
            else if( message.message().start_unit_fire().target().has_population() )
                target = model_.Populations().Find( message.message().start_unit_fire().target().population() );
            *file_ << date_ << " Fire - " << agent->GetName() << "[" << message.message().start_unit_fire().firer_oid()
                   << "] : Fire on ";
            if( target )
                *file_ << target->GetName() << "[" << target->GetId() << "]" << std::endl;
            else
                *file_ << "position" << std::endl;
        }
    }
    else if( message.message().has_start_population_fire() )
    {
        kernel::Entity_ABC* agent = model_.Populations().Find( message.message().start_population_fire().firer_oid() );
        if( agent )
            *file_ << date_ << " Fire - " << agent->GetName() << "[" << message.message().start_population_fire().firer_oid() << "] " << std::endl;
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
