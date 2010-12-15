// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "FloodAttribute.h"
#include "Tools.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/DetectionMap.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "flood/FloodHelper.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: FloodAttribute constructor
// Created: JSR 2010-12-07
// -----------------------------------------------------------------------------
FloodAttribute::FloodAttribute( PropertiesDictionary& dico, const kernel::DetectionMap& detection, const kernel::Positions& positions, kernel::Controllers& controllers )
    : detection_  ( detection )
    , positions_  ( positions )
    , controllers_( controllers )
    , depth_      ( 0, Units::meters )
    , refDist_    ( 0, Units::meters )
    , floodHelper_( *new flood::FloodHelper( *this ) )
{
    controllers_.Register( *this );
    CreateDictionary( dico );
}

// -----------------------------------------------------------------------------
// Name: FloodAttribute constructor
// Created: JSR 2010-12-07
// -----------------------------------------------------------------------------
FloodAttribute::FloodAttribute( xml::xistream& xis, const kernel::DetectionMap& detection, const kernel::Positions& positions, PropertiesDictionary& dico, kernel::Controllers& controllers )
    : detection_  ( detection )
    , positions_  ( positions )
    , controllers_( controllers )
    , depth_      ( 0, Units::meters )
    , refDist_    ( 0, Units::meters )
    , floodHelper_( *new flood::FloodHelper( *this ) )
{
    controllers_.Register( *this );
    xis >> xml::attribute( "depth", depth_.value_ )
        >> xml::attribute( "reference-distance", refDist_.value_ );
    CreateDictionary( dico );
    floodHelper_.GenerateFlood( positions_.GetPosition(), depth_.value_, refDist_.value_ );
}

// -----------------------------------------------------------------------------
// Name: FloodAttribute destructor
// Created: JSR 2010-12-07
// -----------------------------------------------------------------------------
FloodAttribute::~FloodAttribute()
{
    delete &floodHelper_;
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: FloodAttribute::Display
// Created: JSR 2010-12-07
// -----------------------------------------------------------------------------
void FloodAttribute::Display( Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "Object", "Flood" ) )
             .Display( tools::translate( "Object", "Depth:" ), depth_ )
             .Display( tools::translate( "Object", "Reference distance:" ), refDist_ );
}

// -----------------------------------------------------------------------------
// Name: FloodAttribute::DisplayInTooltip
// Created: JSR 2010-12-07
// -----------------------------------------------------------------------------
void FloodAttribute::DisplayInTooltip( Displayer_ABC& ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FloodAttribute::SerializeAttributes
// Created: JSR 2010-12-07
// -----------------------------------------------------------------------------
void FloodAttribute::SerializeAttributes( xml::xostream& xos ) const
{
    xos << xml::start( "flood" )
            << xml::attribute( "depth", depth_.value_ )
            << xml::attribute( "reference-distance", refDist_.value_ )
        << xml::end;
}

// -----------------------------------------------------------------------------
// Name: FloodAttribute::Draw
// Created: JSR 2010-12-08
// -----------------------------------------------------------------------------
void FloodAttribute::Draw( const geometry::Point2f& /*where*/, const Viewport_ABC& /*viewport*/, const GlTools_ABC& /*tools*/ ) const
{
    floodHelper_.Draw();
}

// -----------------------------------------------------------------------------
// Name: FloodAttribute::NotifyUpdated
// Created: JSR 2010-12-08
// -----------------------------------------------------------------------------
void FloodAttribute::NotifyUpdated( const FloodAttribute& attribute )
{
    if( &attribute == this )
        floodHelper_.GenerateFlood( positions_.GetPosition(), depth_.value_, refDist_.value_ );
}

// -----------------------------------------------------------------------------
// Name: FloodAttribute::GetElevationAt
// Created: JSR 2010-12-08
// -----------------------------------------------------------------------------
short FloodAttribute::GetElevationAt( const geometry::Point2f& point ) const
{
    return detection_.ElevationAt( point );
}

// -----------------------------------------------------------------------------
// Name: FloodAttribute::SetValues
// Created: JSR 2010-12-07
// -----------------------------------------------------------------------------
void FloodAttribute::SetValues( int depth, int refDist )
{
    depth_.value_ = depth;
    refDist_.value_ = refDist;
    floodHelper_.GenerateFlood( positions_.GetPosition(), depth_.value_, refDist_.value_ );
}

// -----------------------------------------------------------------------------
// Name: FloodAttribute::CreateDictionary
// Created: JSR 2010-12-07
// -----------------------------------------------------------------------------
void FloodAttribute::CreateDictionary( PropertiesDictionary& dico )
{
    dico.Register( *this, tools::translate( "FloodAttribute", "Info/Flood parameters/Depth" ), depth_ );
    dico.Register( *this, tools::translate( "FloodAttribute", "Info/Flood parameters/Reference distance" ), refDist_ );
}
