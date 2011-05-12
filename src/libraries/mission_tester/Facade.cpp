// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "mission_tester_pch.h"
#include "Facade.h"
#include "Logger.h"
#include "Timeout.h"
#include "mission_tester/Config.h"
#include "mission_tester/Logger.h"
#include "mission_tester/Exercise.h"
#include "mission_tester/SchedulerFactory.h"
#include "mission_tester/Scheduler.h"
#include "mission_tester/Client.h"
#include "mission_tester/Model.h"
#include "clients_kernel/StaticModel.h"
#include <boost/shared_ptr.hpp>
#include <boost/foreach.hpp>
#include <iostream>
#include <conio.h>

using namespace mission_tester;

// -----------------------------------------------------------------------------
// Name: Facade constructor
// Created: PHC 2011-04-05
// -----------------------------------------------------------------------------
Facade::Facade( const tools::ExerciseConfig& config, const MainFactory_ABC& mainFactory )
    : mainFactory_( mainFactory ) 
    , staticModel_( new kernel::StaticModel() )
    , factory_    ( mainFactory_.CreateSchedulerFactory() )
{
    staticModel_->Load( config );
    mainFactory_.ConfigureLogging( *this );
}

// -----------------------------------------------------------------------------
// Name: Facade destructor
// Created: PHC 2011-04-05
// -----------------------------------------------------------------------------
Facade::~Facade()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Facade::Run
// Created: PHC 2011-04-05
// -----------------------------------------------------------------------------
void Facade::Run()
{
    boost::shared_ptr< Scheduler_ABC > scheduler( mainFactory_.CreateScheduler( *factory_ ) );
    Model model( *staticModel_, *scheduler );
    std::auto_ptr< Client > client = mainFactory_.CreateClient( model );
    std::auto_ptr< Exercise > exercise = mainFactory_.CreateExercise( model, *staticModel_, *client );
    exercise->Register( *this );
    client->Register( *this );
    model.Register( *this );
    client->Connect();
    std::auto_ptr< Timeout > timeout =  mainFactory_.CreateTimeout();
    try
    {
        while( !timeout->Expired() )
        {
            client->Update();
            if( client->IsAuthentified() )
                break;
        }
        if( timeout->Expired() )
            throw std::runtime_error( "Timeout exceeded." );
    }
    catch( std::exception& e )
    {
        std::cerr << __FUNCTION__ << ": Error Connecting or Authentifying to simulation, '" << e.what() << "'." << std::endl;
    }
    while( client->IsConnected() )
    {
        if( _kbhit() )
        {
            const int key = _getch();
            if( key == 'q' )
                break;
        }
        client->Update();
        scheduler->Step( *exercise );
    }
}

// -----------------------------------------------------------------------------
// Name: Facade::AddListener
// Created: PHC 2011-04-07
// -----------------------------------------------------------------------------
void Facade::AddListener( boost::shared_ptr< Listener_ABC > listener )
{
    listeners_.push_back( listener );
}

// -----------------------------------------------------------------------------
// Name: Facade::MissionCreated
// Created: PHC 2011-04-07
// -----------------------------------------------------------------------------
void Facade::MissionCreated( const kernel::Entity_ABC& target, const kernel::MissionType& mission ) const
{
    BOOST_FOREACH( const T_Listeners::value_type& listener, listeners_ )
        listener->MissionCreated( target, mission );
}

// -----------------------------------------------------------------------------
// Name: Facade::MissionAknowledged
// Created: PHC 2011-04-08
// -----------------------------------------------------------------------------
void Facade::MissionAcknowledged( const sword::Tasker& tasker ) const
{
    BOOST_FOREACH( const T_Listeners::value_type& listener, listeners_ )
        listener->MissionAcknowledged( tasker );
}

// -----------------------------------------------------------------------------
// Name: Facade::MissionErrorAck
// Created: PHC 2011-04-08
// -----------------------------------------------------------------------------
void Facade::MissionErrorAck( const sword::Tasker& tasker ) const
{
    BOOST_FOREACH( const T_Listeners::value_type& listener, listeners_ )
        listener->MissionErrorAck( tasker );
}

// -----------------------------------------------------------------------------
// Name: Facade::ConnectionSucceeded
// Created: PHC 2011-04-07
// -----------------------------------------------------------------------------
void Facade::ConnectionSucceeded( const std::string& endpoint ) const
{
    BOOST_FOREACH( const T_Listeners::value_type& listener, listeners_ )
        listener->ConnectionSucceeded( endpoint );
}

// -----------------------------------------------------------------------------
// Name: Facade::AuthenticationSucceeded
// Created: PHC 2011-04-07
// -----------------------------------------------------------------------------
void Facade::AuthenticationSucceeded( const std::string& profile ) const
{
    BOOST_FOREACH( const T_Listeners::value_type& listener, listeners_ )
        listener->AuthenticationSucceeded( profile );
}

// -----------------------------------------------------------------------------
// Name: Facade::ParameterCreationFailed
// Created: PHC 2011-04-07
// -----------------------------------------------------------------------------
void Facade::ParameterCreationFailed( const kernel::Entity_ABC& target, const kernel::MissionType& mission, const kernel::OrderParameter& parameter ) const
{
    BOOST_FOREACH( const T_Listeners::value_type& listener, listeners_ )
        listener->ParameterCreationFailed( target, mission, parameter );
}
