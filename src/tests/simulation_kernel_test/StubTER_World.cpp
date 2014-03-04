// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_test_pch.h"
#include "StubTER_World.h"
#include "simulation_terrain/TER_World.h"
#include "tools/ExerciseConfig.h"
#include "tools/NullFileLoaderObserver.h"

namespace
{

std::shared_ptr< tools::ExerciseConfig > CreateConfig( const std::string& exercise )
{
    auto config = std::make_shared< tools::ExerciseConfig >(
            tools::CreateNullFileLoaderObserver() );
    char* params[4];
    params[0] = "simulation_kernel_test.exe";
    params[1] = "--root-dir=../../data";
    const std::string option = "--exercise=" + exercise;
    params[2] = const_cast< char* >( option .c_str() );
    params[3] = "--session=default";
    config->Parse( 4, params );
    return config;
}

}  // namespace

FakeWorld::FakeWorld( const std::string& exercise )
{
    const auto config = CreateConfig( exercise );
    TER_World::Initialize( *config );
}

FakeWorld::~FakeWorld()
{
    TER_World::DestroyWorld();
}

boost::shared_ptr< TER_World > CreateWorld( const std::string& exercise )
{
    const auto config = CreateConfig( exercise );
    return boost::make_shared< TER_World >( *config );
}
