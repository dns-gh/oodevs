#ifndef __StubTER_World_h_
#define __StubTER_World_h_

#include "simulation_terrain/TER_World.h"
#include "tools/ExerciseConfig.h"
#include "tools/Loader.h"
#include "tools/NullFileLoaderObserver.h"

static void WorldInitialize( const std::string& exercise )
{
    tools::NullFileLoaderObserver observer;
    tools::ExerciseConfig config( observer );
    char* params[4];
    params[0] = "simulation_kernel_test.exe";
    params[1] = "--root-dir=../../data";
    params[2] = const_cast< char* >( ( "--exercise=" + exercise ).c_str() );
    params[3] = "--session=default";
    config.Parse( 4, params );
    TER_World::Initialize( config );
}

#endif
