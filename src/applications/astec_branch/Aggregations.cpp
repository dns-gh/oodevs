// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "Aggregations.h"
#include "Life.h"
#include "Agent.h"

// -----------------------------------------------------------------------------
// Name: Aggregations constructor
// Created: AGE 2006-04-11
// -----------------------------------------------------------------------------
Aggregations::Aggregations( const Agent& automat )
    : automat_( automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Aggregations destructor
// Created: AGE 2006-04-11
// -----------------------------------------------------------------------------
Aggregations::~Aggregations()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Aggregations::Draw
// Created: AGE 2006-04-11
// -----------------------------------------------------------------------------
void Aggregations::Draw( const geometry::Point2f& where, const GlTools_ABC& tools ) const
{
    automat_.Get< Life >().DrawAggregated( where, tools );
    automat_.DrawAggregated( where, tools );
}
