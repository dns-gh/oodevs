// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dis_plugin_pch.h"
#include "BurstDescriptor.h"

using namespace plugins::dis;

// -----------------------------------------------------------------------------
// Name: BurstDescriptor constructor
// Created: AGE 2008-05-05
// -----------------------------------------------------------------------------
BurstDescriptor::BurstDescriptor()
    : warhead_ ( (unsigned short)high_explosive )
    , fuse_    ( 1000 ) // Contact
    , quantity_( 1 )
    , rate_    ( 1 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BurstDescriptor constructor
// Created: AGE 2008-04-30
// -----------------------------------------------------------------------------
BurstDescriptor::BurstDescriptor( unsigned short quantity, unsigned seconds, warhead w /*= high_explosive*/ )
    : warhead_ ( (unsigned short)w )
    , fuse_    ( 1000 ) // Contact
    , quantity_( quantity )
    , rate_    ( (unsigned short)( 60.f * float( quantity ) / float( seconds ) ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BurstDescriptor destructor
// Created: AGE 2008-04-30
// -----------------------------------------------------------------------------
BurstDescriptor::~BurstDescriptor()
{
    // NOTHING
}
