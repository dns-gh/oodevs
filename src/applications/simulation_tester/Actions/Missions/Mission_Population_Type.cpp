// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Mission_Population_Type.h"

#include "Population/Mission_Population_FaireMouvement.cpp"
#include "Population/Mission_Population_Manifester.cpp"
#include "Population/Mission_Population_CommettreExactions.cpp"


using namespace TEST;

Mission_Population_Type::T_MissionPopulationTypeMap Mission_Population_Type::missions_;

// -----------------------------------------------------------------------------
// Name: Mission_Population_Type constructor
// Created: SBO 2005-08-09
// -----------------------------------------------------------------------------
Mission_Population_Type::Mission_Population_Type( const std::string& strName, T_MissionAllocator missionAllocator )
    : strName_          ( strName )
    , missionAllocator_ ( missionAllocator )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Population_Type destructor
// Created: SBO 2005-08-09
// -----------------------------------------------------------------------------
Mission_Population_Type::~Mission_Population_Type()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Population_Type::Initialize
// Created: SBO 2005-08-09
// -----------------------------------------------------------------------------
void Mission_Population_Type::Initialize()
{
    Register< Mission_Population_FaireMouvement >( "Population FaireMouvement" );
    Register< Mission_Population_Manifester >( "Population Manifester" );
    Register< Mission_Population_CommettreExactions >( "Population CommettreExactions" );

}

// -----------------------------------------------------------------------------
// Name: Mission_Population_Type::Terminate
// Created: SBO 2005-08-09
// -----------------------------------------------------------------------------
void Mission_Population_Type::Terminate()
{
    for( CIT_MissionPopulationTypeMap it = missions_.begin(); it != missions_.end(); ++it )
        delete it->second;
    missions_.clear();
}