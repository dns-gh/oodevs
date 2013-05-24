// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "TacticalLinePositions.h"
#include "TacticalLine_ABC.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/LocationVisitor_ABC.h"
#include "protocol/Protocol.h"

// -----------------------------------------------------------------------------
// Name: TacticalLinePositions constructor
// Created: SBO 2006-11-06
// -----------------------------------------------------------------------------
TacticalLinePositions::TacticalLinePositions( kernel::Controller& controller, const T_PointVector& pointList, const kernel::CoordinateConverter_ABC& converter,
                                              const kernel::TacticalLine_ABC& owner )
    : TacticalLinePositions_ABC( controller, pointList, converter, owner )
{
     // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TacticalLinePositions constructor
// Created: SBO 2006-11-06
// -----------------------------------------------------------------------------
TacticalLinePositions::TacticalLinePositions( kernel::Controller& controller, const sword::Location& message, const kernel::CoordinateConverter_ABC& converter,
                                              const kernel::TacticalLine_ABC& owner )
    : TacticalLinePositions_ABC( controller, converter, owner )
{
    Update( message.coordinates() );
}

// -----------------------------------------------------------------------------
// Name: TacticalLinePositions destructor
// Created: SBO 2006-11-06
// -----------------------------------------------------------------------------
TacticalLinePositions::~TacticalLinePositions()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TacticalLinePositions::DoUpdate
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
void TacticalLinePositions::DoUpdate( const sword::PhaseLineUpdate& message )
{
    Update( message.tactical_line().geometry().coordinates() );
}

// -----------------------------------------------------------------------------
// Name: TacticalLinePositions::DoUpdate
// Created: SBO 2006-11-14
// -----------------------------------------------------------------------------
void TacticalLinePositions::DoUpdate( const sword::LimitUpdate& message )
{
    Update( message.tactical_line().geometry().coordinates() );
}

// -----------------------------------------------------------------------------
// Name: TacticalLinePositions::Update
// Created: MMC 2012-05-11
// -----------------------------------------------------------------------------
void TacticalLinePositions::Update( const sword::CoordLatLongList& coordinates )
{
    pointList_.clear();
    pointList_.reserve( coordinates.elem_size() );
    for( int i = 0; i < coordinates.elem_size(); ++i )
        pointList_.push_back( converter_.ConvertToXY( coordinates.elem(i) ) );
    ComputeBoundingBox();
}
