// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "Extractor_ABC.h"

// -----------------------------------------------------------------------------
// Name: Extractor_ABC constructor
// Created: JSR 2014-04-15
// -----------------------------------------------------------------------------
Extractor_ABC::Extractor_ABC()
    : ncols_( 0 )
    , nrows_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Extractor_ABC destructor
// Created: JSR 2014-04-15
// -----------------------------------------------------------------------------
Extractor_ABC::~Extractor_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Extractor_ABC::GetCols
// Created: JSR 2014-04-15
// -----------------------------------------------------------------------------
int Extractor_ABC::GetCols() const
{
    return ncols_;
}

// -----------------------------------------------------------------------------
// Name: Extractor_ABC::GetRows
// Created: JSR 2014-04-15
// -----------------------------------------------------------------------------
int Extractor_ABC::GetRows() const
{
    return nrows_;
}

// -----------------------------------------------------------------------------
// Name: Extractor_ABC::GetExtent
// Created: JSR 2014-04-15
// -----------------------------------------------------------------------------
const geometry::Rectangle2d& Extractor_ABC::GetExtent() const
{
    return extent_;
}

// -----------------------------------------------------------------------------
// Name: Extractor_ABC::GetValues
// Created: JSR 2014-04-15
// -----------------------------------------------------------------------------
const Extractor_ABC::T_Values& Extractor_ABC::GetValues() const
{
    return values_;
}

// -----------------------------------------------------------------------------
// Name: Extractor_ABC::GetPixelSize
// Created: JSR 2014-04-16
// -----------------------------------------------------------------------------
const geometry::Point2d& Extractor_ABC::GetPixelSize() const
{
    return pixelSize_;
}

// -----------------------------------------------------------------------------
// Name: Extractor_ABC::GetValue
// Created: LGY 2012-11-12
// -----------------------------------------------------------------------------
float Extractor_ABC::GetValue( double longitude, double latitude ) const
{
    const double dlng = longitude - extent_.Left();
    const double dlat = latitude - extent_.Bottom();
    if( dlng < 0 || dlng > extent_.Width() || dlat < 0 || dlat > extent_.Height() )
        return 0;
    // We want the boundary points to belong to the shape
    const int x = std::min( static_cast< int >( ( dlng / extent_.Width() ) * ncols_ ), ncols_ - 1 );
    const int y = nrows_ - 1 - std::min( static_cast< int >( ( dlat / extent_.Height() ) * nrows_ ), nrows_ - 1);
    return values_[ y * ncols_ + x ];
}
