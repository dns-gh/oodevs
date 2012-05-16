// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "UrbanFilter.h"
#include "clients_kernel/UrbanObject_ABC.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: UrbanFilter constructor
// Created: ABR 2011-03-01
// -----------------------------------------------------------------------------
UrbanFilter::UrbanFilter()
    : LayerFilter_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanFilter destructor
// Created: ABR 2011-03-01
// -----------------------------------------------------------------------------
UrbanFilter::~UrbanFilter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanFilter::IsAllowed
// Created: ABR 2011-03-01
// -----------------------------------------------------------------------------
bool UrbanFilter::IsAllowed( const QString& typeName ) const
{
    return typeName != kernel::UrbanObject_ABC::typeName_;
}
