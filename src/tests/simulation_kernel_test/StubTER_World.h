#ifndef __StubTER_World_h_
#define __StubTER_World_h_

#include "simulation_terrain/TER_World.h"
#include "tools/ExerciseConfig.h"
#include "tools/NullFileLoaderObserver.h"

inline void WorldInitialize( const std::string& exercise )
{
    tools::NullFileLoaderObserver observer;
    tools::ExerciseConfig config( observer );
    char* params[4];
    params[0] = "simulation_kernel_test.exe";
    params[1] = "--root-dir=../../data";
    const std::string option = "--exercise=" + exercise;
    params[2] = const_cast< char* >( option .c_str() );
    params[3] = "--session=default";
    config.Parse( 4, params );
    TER_World::Initialize( config );
}

#endif
