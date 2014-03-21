// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "core/Model.h"
#include <tools/win32/ProcessMonitor.h>
#include <stdexcept>
#include <iostream>
#include <string>
#include <ctime>

namespace
{
    std::size_t identifier = 0;

    const int number_of_components = 20;
    const int number_of_agents = 1000;
    const int number_of_knowledges = 200;

    void UpdateKnowledge( core::Model& knowledge )
    {
        knowledge[ "data" ].SetUserData( 42 );
        knowledge[ "identifier" ] = ++identifier;
        knowledge[ "valid" ] = true;
        knowledge[ "dead" ] = false;
        core::Model& components = knowledge[ "components" ];
        components.Clear();
        for( int c = 0; c < number_of_components; ++c )
        {
            core::Model& component = components.AddElement();
            component[ "volume" ] = ++identifier;
            component[ "score" ] = 7;
        }
    }
    void UpdateAgent( core::Model& entity )
    {
        core::Model& knowledges = entity[ "knowledges" ];
        core::Model& enemies = entity[ "enemies" ];
        core::Model& friends = entity[ "friends" ];
        for( int k = 0; k < number_of_knowledges; ++k )
        {
            UpdateKnowledge( knowledges[ k ] );
            enemies.Clear();
            friends.Clear();
            if( k % 2 )
                UpdateKnowledge( enemies.AddElement() );
            else
                UpdateKnowledge( friends.AddElement() );
        }
    }

    double ToMb( unsigned long long bytes )
    {
        return bytes / 1048576.;
    }
}

int main( int /*argc*/, char** /*argv*/ )
{
    ProcessMonitor monitor;
    core::Model model;
#ifdef _DEBUG
    std::cout << "DEBUG" << std::endl;
#else
    std::cout << "RELEASE" << std::endl;
#endif
    std::cout << "model_app running with "
        << number_of_agents << " agents, "
        << number_of_knowledges << " knowledges and "
        << number_of_components << " components" << std::endl;
    std::cout << "sizeof( Model* ) : " << sizeof( core::Model* ) << std::endl;
    std::cout << "sizeof( Model::T_NamedChildren ) : " << sizeof( core::Model::T_NamedChildren ) << std::endl;
    std::cout << "sizeof( Model::T_IdentifiedChildren ) : " << sizeof( core::Model::T_IdentifiedChildren ) << std::endl;
    std::cout << "sizeof( Elements ) : " << sizeof( core::Elements ) << std::endl;
    std::cout << "sizeof( Model::T_Data ) : " << sizeof( core::Model::T_Data ) << std::endl;
    std::cout << "sizeof( boost::shared_ptr< UserData_ABC > ) : " << sizeof( boost::shared_ptr< core::UserData_ABC > ) << std::endl;
    std::cout << "sizeof( Model::T_Content ) : " << sizeof( core::Model::T_Content ) << std::endl;
    std::cout << "=> sizeof( Model ) : " << sizeof( core::Model ) << std::endl;
    int a = 0;
    std::clock_t start = std::clock();
    try
    {
        for( ; a < number_of_agents; ++a )
            UpdateAgent( model[ a ] );
    }
    catch( std::exception& e )
    {
        std::cerr << "**** ERROR at agent " << a << " : " << e.what() << std::endl;
    }
    catch( ... )
    {
        std::cerr << "**** ERROR at agent " << a << " : " << "unknown exception" << std::endl;
    }
    std::clock_t end = std::clock();
    monitor.MonitorProcess();
    std::cout << " => " << 1000.0 * (end - start) / CLOCKS_PER_SEC << " ms" << std::endl
              << " => " << ToMb( monitor.GetMemory() ) << " MB" << std::endl;
    return EXIT_SUCCESS;
}
