// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "mission_tester/mission_tester_pch.h"
#include "mission_tester/Client.h"
#include "mission_tester/Exercise.h"
#include "mission_tester/Model.h"
#include "mission_tester/Scheduler.h"
#include "mission_tester/SchedulerFactory.h"
#include "clients_kernel/StaticModel.h"
#include "tools/ExerciseConfig.h"
#include "tools/NullFileLoaderObserver.h"

#include <iostream>

int main( int argc, char* argv[] )
{
    try
    {
        mission_tester::SchedulerFactory factory;
        mission_tester::Scheduler_ABC* scheduler = factory.CreateAgentScheduler();
        tools::NullFileLoaderObserver observer;
        tools::ExerciseConfig config( observer );
        config.Parse( argc, argv );
        kernel::StaticModel staticModel;
        staticModel.Load( config );
        mission_tester::Model model( staticModel, *scheduler );
        mission_tester::Exercise exercise( model, staticModel );
        mission_tester::Client client( model, "localhost", 10001, "Supervisor", "" );
        while( true )
            client.Update();
        std::cout << std::endl << "Launch mission" << std::endl;
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
        client.Disconnect();
    }
    catch( std::exception& e )
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}