// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Color.h"
#include "protocol/Protocol.h"

// -----------------------------------------------------------------------------
// Name: Color constructor
// Created: LGY 2011-06-27
// -----------------------------------------------------------------------------
Color::Color( const sword::RgbColor& color )
{
    color_ = boost::tuples::make_tuple( color.red(), color.green(), color.blue() );
}

// -----------------------------------------------------------------------------
// Name: Color destructor
// Created: LGY 2011-06-27
// -----------------------------------------------------------------------------
Color::~Color()
{
    // NOTHING
}
