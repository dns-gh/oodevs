// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "geostore_pch.h"
#include "SpatialRequestStatus.h"

using namespace geostore;

// -----------------------------------------------------------------------------
// Name: SpatialRequestStatus constructor
// Created: AME 2010-08-02
// -----------------------------------------------------------------------------
SpatialRequestStatus::SpatialRequestStatus()
    : numberCreatedObjects_( 0 )
    , valid_               ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SpatialRequestStatus destructor
// Created: AME 2010-08-02
// -----------------------------------------------------------------------------
SpatialRequestStatus::~SpatialRequestStatus()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SpatialRequestStatus::UpdateSpatialStatus
// Created: AME 2010-08-02
// -----------------------------------------------------------------------------
void SpatialRequestStatus::UpdateSpatialStatus( int number, bool status )
{
    numberCreatedObjects_ = number;
    status = valid_;
}
