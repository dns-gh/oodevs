#ifndef __StubTER_World_h_
#define __StubTER_World_h_

#include "simulation_terrain/TER_World.h"
#include "tools/ExerciseConfig.h"
#include "tools/WorldParameters.h"


static void WorldInitialize( const std::string& exercise )
{
    tools::ExerciseConfig config;
    char* params[4];
    params[0] = "simulation_kernel_test.exe";
    params[1] = "--root-dir=../../data";
    std::string exerciseOption = "--exercise="+exercise;
    params[2] = const_cast< char* >((exerciseOption).c_str());
    params[3] = "--session=default";
    config.Parse(4, params );

    TER_World::Initialize( tools::WorldParameters( config ) );
}

#endif
