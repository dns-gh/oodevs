// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "dis_plugin_pch.h"
#include "EntityMarking.h"

using namespace plugins::dis;

// -----------------------------------------------------------------------------
// Name: EntityMarking constructor
// Created: AGE 2008-03-10
// -----------------------------------------------------------------------------
EntityMarking::EntityMarking( const std::string& name )
    : characterSet_( 1 )
{
    const unsigned length = std::min( sizeof( marking_ ), name.length() );
    ::memcpy( marking_, name.c_str(), length );
}

// -----------------------------------------------------------------------------
// Name: EntityMarking destructor
// Created: AGE 2008-03-10
// -----------------------------------------------------------------------------
EntityMarking::~EntityMarking()
{
    // NOTHING
}
