// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Team.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Team constructor
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
Team::Team( uint id, DIN::DIN_Input& input, Controller& controller )
    : EntityImplementation< Team_ABC >( controller, id, ReadName( input ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Team destructor
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
Team::~Team()
{
    Destroy();
}

// -----------------------------------------------------------------------------
// Name: Team::ReadName
// Created: AGE 2006-10-12
// -----------------------------------------------------------------------------
QString Team::ReadName( DIN::DIN_Input& input )
{
    std::string name;
    input >> name;
    return name.c_str();
}