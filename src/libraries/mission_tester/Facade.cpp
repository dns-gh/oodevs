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
Facade::Facade( const Config& config )
    : staticModel_( new kernel::StaticModel() )
    , factory_    ( new SchedulerFactory() ) 
{
    staticModel_->Load( config );
    config.ConfigureLogging( *this );
}

// -----------------------------------------------------------------------------
// Name: Facade destructor
// Created: PHC 2011-04-05
// -----------------------------------------------------------------------------
Facade::~Facade()
{
    // NOTHING
}

namespace
{
    void StartMission( mission_tester::Exercise& exercise, mission_tester::Client& client )
    {
        const std::string mission = "<action id='131' name='Faire Mouvement' target='33' time='2009-05-05T12:49:34' type='mission'>"
                                    "  <parameter name='direction dangereuse' type='heading' value='360'/>"
                                    "  <parameter name='Limas' type='phaseline'/>"
                                    "  <parameter name='Limit 1' type='limit'/>"
                                    "  <parameter name='Limit 2' type='limit'/>"
                                    "  <parameter name='Itineraire' type='path'>"
                                    "    <parameter name='Destination' type='pathpoint'>"
                                    "      <location type='point'>"
                                    "        <point coordinates='31UEQ0337626262'/>"
                                    "      </location>"
                                    "    </parameter>"
                                    "  </parameter>"
                                    "</action>";
        exercise.SendOrder( mission , client );
    }
}

// -----------------------------------------------------------------------------
// Name: Facade::Run
// Created: PHC 2011-04-05
// -----------------------------------------------------------------------------
void Facade::Run()
{
    boost::shared_ptr< Scheduler_ABC > scheduler( factory_->CreateAgentScheduler() );
    Model model( *staticModel_, *scheduler );
    Exercise exercise( model, *staticModel_ );
    Client client( model, "localhost", 10001, "Supervisor", "" );
    exercise.Register( *this );
    client.Register( *this );    
    while( !client.IsConnected() )
        client.Update();
    while( !client.IsAuthentified() )
        client.Update();
    while( client.IsConnected() )
    {
        if( _kbhit() )
        {
            const int key = _getch();
            if( key == 'q' )
                break;
            else if( key == 's' )
                StartMission( exercise, client );
        }
        client.Update();
        scheduler->Step( 100, exercise );
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
