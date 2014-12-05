// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Populations.h"
#include "Population.h"

Populations::Populations()
{
    // NOTHING
}

Populations::~Populations()
{
    // NOTHING
}

void Populations::AddPopulation( const Population& population )
{
    Register( population.GetId(), population );
}
