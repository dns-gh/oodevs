// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "ObjectPositions.h"
#include "LocationSerializer.h"
#include "clients_kernel/Location_ABC.h"
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: ObjectPositions constructor
// Created: AGE 2006-03-22
// -----------------------------------------------------------------------------
ObjectPositions::ObjectPositions( const CoordinateConverter_ABC& converter, const kernel::Location_ABC& location )
    : converter_( converter )
    , location_( &location.Clone() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectPositions destructor
// Created: AGE 2006-03-22
// -----------------------------------------------------------------------------
ObjectPositions::~ObjectPositions()
{
    delete location_;
}

// -----------------------------------------------------------------------------
// Name: ObjectPositions::GetPosition
// Created: SBO 2006-09-11
// -----------------------------------------------------------------------------
geometry::Point2f ObjectPositions::GetPosition() const
{
    return center_;
}
    
// -----------------------------------------------------------------------------
// Name: ObjectPositions::GetHeight
// Created: SBO 2006-09-11
// -----------------------------------------------------------------------------
float ObjectPositions::GetHeight() const
{
    return 0;
}
    
// -----------------------------------------------------------------------------
// Name: ObjectPositions::IsAt
// Created: SBO 2006-09-11
// -----------------------------------------------------------------------------
bool ObjectPositions::IsAt( const geometry::Point2f& pos, float precision /*= 100.f*/ ) const
{
    return false; // $$$$ SBO 2006-09-11: todo
}
    
// -----------------------------------------------------------------------------
// Name: ObjectPositions::IsIn
// Created: SBO 2006-09-11
// -----------------------------------------------------------------------------
bool ObjectPositions::IsIn( const geometry::Rectangle2f& rectangle ) const
{
    return false; // $$$$ SBO 2006-09-11: todo
}
    
// -----------------------------------------------------------------------------
// Name: ObjectPositions::GetBoundingBox
// Created: SBO 2006-09-11
// -----------------------------------------------------------------------------
geometry::Rectangle2f ObjectPositions::GetBoundingBox() const
{
    return boundingBox_;
}

// -----------------------------------------------------------------------------
// Name: ObjectPositions::DoSerialize
// Created: SBO 2006-09-12
// -----------------------------------------------------------------------------
void ObjectPositions::DoSerialize( xml::xostream& xos ) const
{
    LocationSerializer serializer( converter_ );
    xos << start( "shape" );
    serializer.Serialize( *location_, xos );
    xos << end();
}
