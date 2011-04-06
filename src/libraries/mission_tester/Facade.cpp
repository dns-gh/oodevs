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
#include "mission_tester/Exercise.h"
#include "mission_tester/SchedulerFactory.h"
#include "mission_tester/Scheduler.h"
#include "mission_tester/Client.h"
#include "mission_tester/Model.h"
#include "clients_kernel/StaticModel.h"
#include "tools/ExerciseConfig.h"
#include "tools/NullFileLoaderObserver.h"
#include <iostream>
#include <conio.h>

using namespace mission_tester;

// -----------------------------------------------------------------------------
// Name: Facade constructor
// Created: PHC 2011-04-05
// -----------------------------------------------------------------------------
Facade::Facade()
{
    // NOTHING
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
    // $$$$ PHC 2011-04-05: While 
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
void Facade::Run( int argc, char* argv[] )
{
    SchedulerFactory factory;
    boost::shared_ptr< Scheduler_ABC > scheduler( factory.CreateAgentScheduler() );
    tools::NullFileLoaderObserver observer;
    tools::ExerciseConfig config( observer );
    config.Parse( argc, argv );
    kernel::StaticModel staticModel;
    staticModel.Load( config );
    Model model( staticModel, *scheduler );
    Exercise exercise( model, staticModel );
    Client client( model, "localhost", 10001, "Supervisor", "" );

    std::cout << "Connecting ..." << std::endl;
    while( !client.IsConnected() )
        client.Update();
    std::cout << "Authentifing ..." << std::endl;
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
