// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "LogisticPlugin.h"
#include "clients_kernel/StaticModel.h"
#include "dispatcher/Model_ABC.h"
#include "protocol/Protocol.h"
#include "protocol/ServerPublisher_ABC.h"
#include "tools/SessionConfig.h"
#include "FuneralResolver.h"
#include "MaintenanceResolver.h"
#include "MedicalResolver.h"
#include "SupplyResolver.h"

using namespace plugins::logistic;

// -----------------------------------------------------------------------------
// Name: LogisticPlugin constructor
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
LogisticPlugin::LogisticPlugin( const dispatcher::Model_ABC& model, const kernel::StaticModel& staticModel, const tools::SessionConfig& config )
    : sessionConfig_( config )
    , staticModel_ ( staticModel )
    , currentTick_( 0 )
    , maintenanceResolver_  ( new MaintenanceResolver( config.BuildSessionChildFile( "LogMaintenance" ), model ) )
    , supplyResolver_       ( new SupplyResolver( config.BuildSessionChildFile( "LogAppro" ), model ) )
    , funeralResolver_      ( new FuneralResolver( config.BuildSessionChildFile( "LogFuneraire" ), model ) )
    , medicalResolver_      ( new MedicalResolver( config.BuildSessionChildFile( "LogMedical" ), model ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticPlugin destructor
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
LogisticPlugin::~LogisticPlugin()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticPlugin::Receive
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
void LogisticPlugin::Receive( const sword::SimToClient& message )
{
    if( message.message().has_control_begin_tick() )
    {
        currentTick_= message.message().control_begin_tick().current_tick();
        simTime_    = message.message().control_begin_tick().date_time().data();
        
        maintenanceResolver_->SetTime( currentTick_, simTime_ );
        supplyResolver_->SetTime( currentTick_, simTime_ );
        funeralResolver_->SetTime( currentTick_, simTime_ );
        medicalResolver_->SetTime( currentTick_, simTime_ );
    }
    maintenanceResolver_->Receive( message );
    supplyResolver_->Receive( message );
    funeralResolver_->Receive( message );
    medicalResolver_->Receive( message );
}
