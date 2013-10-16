// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "script_plugin_pch.h"
#include "PopulationManipulator.h"
#include "dispatcher/Population.h"
#include <directia/brain/Brain.h>

using namespace plugins::script;
using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: PopulationManipulator constructor
// Created: SBO 2008-11-18
// -----------------------------------------------------------------------------
PopulationManipulator::PopulationManipulator( const dispatcher::Population& population )
    : population_( population )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationManipulator destructor
// Created: SBO 2008-11-18
// -----------------------------------------------------------------------------
PopulationManipulator::~PopulationManipulator()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationManipulator::Registrar::RegisterIn
// Created: SBO 2008-11-18
// -----------------------------------------------------------------------------
void PopulationManipulator::Registrar::RegisterIn( directia::brain::Brain& brain )
{
    brain.Register( "GetIdentifier", &PopulationManipulator::GetIdentifier );
    brain.Register( "GetName",       &PopulationManipulator::GetName );
}

// -----------------------------------------------------------------------------
// Name: PopulationManipulator::GetIdentifier
// Created: SBO 2008-11-18
// -----------------------------------------------------------------------------
unsigned int PopulationManipulator::GetIdentifier() const
{
    return population_.GetId();
}

// -----------------------------------------------------------------------------
// Name: PopulationManipulator::GetName
// Created: SBO 2008-11-18
// -----------------------------------------------------------------------------
std::string PopulationManipulator::GetName() const
{
    const QString name = population_.GetName();
    return name.isEmpty() ? "" : name.toStdString();
}
