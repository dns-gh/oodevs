// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "InhabitantPositions.h"
#include "Inhabitant.h"
#include "UrbanModel.h"
#include "clients_kernel/UrbanObject_ABC.h"
#include "clients_kernel/UrbanPositions_ABC.h"
#include "protocol/SimulationSenders.h"
#include <boost/foreach.hpp>

using namespace geometry;

// -----------------------------------------------------------------------------
// Name: InhabitantPositions constructor
// Created: SLG 2010-11-30
// -----------------------------------------------------------------------------
InhabitantPositions::InhabitantPositions( const sword::PopulationCreation& message,
                                          const UrbanModel& model,
                                          const Inhabitant& inhabitant )
    : inhabitant_( inhabitant )
{
    Polygon2f polygon;
    for( int i = 0; i < message.objects_size(); ++i )
    {
        int id = message.objects( i ).id();
        kernel::UrbanObject_ABC& object = model.GetObject( id );
        if( const auto positions = object.Retrieve< kernel::UrbanPositions_ABC >() )
        {
            polygon.Add( positions->Barycenter() );
            const auto& vertices = positions->Vertices();
            for( auto it = vertices.begin(); it != vertices.end(); ++it )
                boundingBox_.Incorporate( *it );
        }
    }
    position_ = polygon.Barycenter();
}

// -----------------------------------------------------------------------------
// Name: InhabitantPositions destructor
// Created: SLG 2010-11-30
// -----------------------------------------------------------------------------
InhabitantPositions::~InhabitantPositions()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InhabitantPositions::GetPosition
// Created: SLG 2010-11-30
// -----------------------------------------------------------------------------
geometry::Point2f InhabitantPositions::GetPosition( bool ) const
{
    return position_;
}

// -----------------------------------------------------------------------------
// Name: InhabitantPositions::IsIn
// Created: SLG 2010-11-30
// -----------------------------------------------------------------------------
bool InhabitantPositions::IsIn( const geometry::Rectangle2f& ) const
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: InhabitantPositions::GetHeight
// Created: SLG 2010-11-30
// -----------------------------------------------------------------------------
float InhabitantPositions::GetHeight( bool ) const
{
    return 0;
}

// -----------------------------------------------------------------------------
// Name: InhabitantPositions::GetBoundingBox
// Created: SLG 2010-11-30
// -----------------------------------------------------------------------------
geometry::Rectangle2f InhabitantPositions::GetBoundingBox() const
{
    return boundingBox_;
}

// -----------------------------------------------------------------------------
// Name: InhabitantPositions::Accept
// Created: SLG 2010-11-30
// -----------------------------------------------------------------------------
void InhabitantPositions::Accept( kernel::LocationVisitor_ABC& /*visitor*/ ) const
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: InhabitantPositions::CanAggregate
// Created: SLG 2010-11-30
// -----------------------------------------------------------------------------
bool InhabitantPositions::CanAggregate() const
{
    return false;
}
