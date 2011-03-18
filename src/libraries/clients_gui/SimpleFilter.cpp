// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "SimpleFilter.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: SimpleFilter constructor
// Created: ABR 2011-03-01
// -----------------------------------------------------------------------------
SimpleFilter::SimpleFilter()
    : LayerFilter_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SimpleFilter destructor
// Created: ABR 2011-03-01
// -----------------------------------------------------------------------------
SimpleFilter::~SimpleFilter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SimpleFilter::IsAllowed
// Created: ABR 2011-03-01
// -----------------------------------------------------------------------------
bool SimpleFilter::IsAllowed( const QString& /*typeName*/ ) const
{
    return true;
}
