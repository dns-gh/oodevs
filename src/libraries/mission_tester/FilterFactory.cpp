// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "mission_tester_pch.h"
#include "FilterFactory.h"
#include "Filter.h"

using namespace mission_tester;

// -----------------------------------------------------------------------------
// Name: FilterFactory constructor
// Created: PHC 2011-04-06
// -----------------------------------------------------------------------------
FilterFactory::FilterFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FilterFactory destructor
// Created: PHC 2011-04-06
// -----------------------------------------------------------------------------
FilterFactory::~FilterFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FilterFactory::Create
// Created: PHC 2011-04-06
// -----------------------------------------------------------------------------
std::auto_ptr< Filter_ABC > FilterFactory::Create( const std::string& criteria ) const
{
    return std::auto_ptr< Filter_ABC >( new Filter( criteria ) );
}
