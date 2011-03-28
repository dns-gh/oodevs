// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "mission_tester/Exercise.h"
#include "mission_tester/Client.h"
#include "mission_tester/Model.h"
#include "clients_kernel/StaticModel.h"
#include "tools/ExerciseConfig.h"
#include "tools/NullFileLoaderObserver.h"
#pragma warning( push )
#pragma warning( disable: 4512 )
#include <boost/program_options.hpp>
#pragma warning( pop )
#pragma warning( push, 0 )
#include <boost/date_time/posix_time/posix_time.hpp>
#pragma warning( pop )

namespace
{
    struct Timeout
    {
        explicit Timeout( unsigned int duration ) : duration_( duration ) { Start(); }
        void Start()
        {
            start_ = boost::posix_time::microsec_clock::universal_time();
        }
        bool Expired() const
        {
            return ( boost::posix_time::microsec_clock::universal_time() - start_ ).total_milliseconds() > duration_;
        }
        unsigned int duration_;
        boost::posix_time::ptime start_;
    };

    const boost::program_options::variables_map ParseCommandLine( int argc, char* argv[] )
    {
        boost::program_options::options_description desc( "Allowed options" );
        desc.add_options()
            ( "help"                                                    , "produce help message" )
            ( "exercise", boost::program_options::value< std::string >(), "set exercise" )
            ( "root-dir", boost::program_options::value< std::string >(), "set root directory" );
        boost::program_options::variables_map vm;
        boost::program_options::store( boost::program_options::command_line_parser( argc, argv ).options( desc ).run(), vm );
        boost::program_options::notify( vm );
        if( vm.count( "help" ) )
            std::cout << desc << std::endl;
        else if( ! vm.count( "exercise" ) )
            throw std::invalid_argument( "Invalid application option argument: missing exercise definition" );
        else if( ! vm.count( "root-dir" ) )
            throw std::invalid_argument( "Invalid application option argument: missing root directory" );
        return vm;
    }
}

int main( int argc, char* argv[] )
{
    try
    {
        tools::NullFileLoaderObserver observer;
        tools::ExerciseConfig config( observer );
        config.Parse( argc, argv );

        kernel::StaticModel staticModel;
        staticModel.Load( config );

        mission_tester::Model model( staticModel );
        mission_tester::Exercise exercise( model, staticModel );
        
        mission_tester::Client client( model, "localhost", 10001, "Supervisor", "" );
        Timeout timeout( 1000 );
        while( !timeout.Expired() )
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