// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "UniqueId.h"

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: UniqueIdSerializer constructor
// Created: AHC 2012-09-03
// -----------------------------------------------------------------------------
UniqueIdSerializer::UniqueIdSerializer( int netnVersion )
    : netnVersion_( netnVersion )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UniqueIdSerializer destructor
// Created: AHC 2012-09-03
// -----------------------------------------------------------------------------
UniqueIdSerializer::~UniqueIdSerializer()
{
    // NOTHING
}
