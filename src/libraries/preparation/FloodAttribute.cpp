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
#include "clients_gui/GLView_ABC.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/AltitudeModified.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/DetectionMap.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/Positions.h"
#include "clients_gui/PropertiesDictionary.h"
#include "clients_gui/FloodDrawer.h"
#include "propagation/FloodModel.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: FloodAttribute constructor
// Created: JSR 2010-12-07
// -----------------------------------------------------------------------------
FloodAttribute::FloodAttribute( gui::PropertiesDictionary& dictionary, const kernel::DetectionMap& detection,
                                const kernel::Positions& positions, kernel::Controllers& controllers, const kernel::Entity_ABC& entity )
    : detection_  ( detection )
    , positions_  ( positions )
    , controllers_( controllers )
    , depth_      ( 0, Units::meters )
    , refDist_    ( 0, Units::meters )
    , floodModel_ ( new propagation::FloodModel( *this ) )
    , floodDrawer_( new gui::FloodDrawer() )
{
    CreateDictionary( dictionary, entity );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: FloodAttribute constructor
// Created: JSR 2010-12-07
// -----------------------------------------------------------------------------
FloodAttribute::FloodAttribute( xml::xistream& xis, const kernel::DetectionMap& detection,
                                const kernel::Positions& positions, gui::PropertiesDictionary& dictionary, kernel::Controllers& controllers, const kernel::Entity_ABC& entity )
    : detection_  ( detection )
    , positions_  ( positions )
    , controllers_( controllers )
    , depth_      ( 0, Units::meters )
    , refDist_    ( 0, Units::meters )
    , floodModel_ ( new propagation::FloodModel( *this ) )
{
    xis >> xml::attribute( "depth", depth_.value_ )
        >> xml::attribute( "reference-distance", refDist_.value_ );
    CreateDictionary( dictionary, entity );
    floodDrawer_.reset( new gui::FloodDrawer( *floodModel_, positions.GetPosition(), depth_.value_, refDist_.value_ ) );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: FloodAttribute destructor
// Created: JSR 2010-12-07
// -----------------------------------------------------------------------------
FloodAttribute::~FloodAttribute()
{
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
// Name: FloodAttribute::SerializeObjectAttributes
// Created: JSR 2010-12-07
// -----------------------------------------------------------------------------
void FloodAttribute::SerializeObjectAttributes( xml::xostream& xos ) const
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
void FloodAttribute::Draw( const geometry::Point2f& /*where*/, const gui::Viewport_ABC& /*viewport*/, gui::GLView_ABC& /*tools*/ ) const
{
    floodDrawer_->Draw();
}

// -----------------------------------------------------------------------------
// Name: FloodAttribute::NotifyUpdated
// Created: JSR 2010-12-08
// -----------------------------------------------------------------------------
void FloodAttribute::NotifyUpdated( const FloodAttribute& attribute )
{
    if( &attribute == this )
        floodDrawer_->Reset( *floodModel_, positions_.GetPosition(), depth_.value_, refDist_.value_ );
}

// -----------------------------------------------------------------------------
// Name: FloodAttribute::NotifyUpdated
// Created: JSR 2011-05-18
// -----------------------------------------------------------------------------
void FloodAttribute::NotifyUpdated( const AltitudeModified& attribute )
{
    const geometry::Point2f& center = positions_.GetPosition();
    if( attribute.polygon_.Intersect( center, static_cast< float >( refDist_.value_ ) ) )
        floodDrawer_->Reset( *floodModel_, center, depth_.value_, refDist_.value_ );
}

// -----------------------------------------------------------------------------
// Name: FloodAttribute::GetElevationAt
// Created: JSR 2010-12-08
// -----------------------------------------------------------------------------
double FloodAttribute::GetElevationAt( const geometry::Point2f& point ) const
{
    if( detection_.Extent().IsOutside( point ) )
        return std::numeric_limits< double >::max();
    return detection_.ElevationAt( point, true );
}

// -----------------------------------------------------------------------------
// Name: FloodAttribute::GetCellSize
// Created: JSR 2014-08-11
// -----------------------------------------------------------------------------
float FloodAttribute::GetCellSize() const
{
    return detection_.GetCellSize();
}

// -----------------------------------------------------------------------------
// Name: FloodAttribute::SetValues
// Created: JSR 2010-12-07
// -----------------------------------------------------------------------------
void FloodAttribute::SetValues( int depth, int refDist )
{
    if( depth_.value_ != depth || refDist_.value_ != refDist )
    {
        depth_.value_ = depth;
        refDist_.value_ = refDist;
        floodDrawer_->Reset( *floodModel_, positions_.GetPosition(), depth_.value_, refDist_.value_ );
    }
}

// -----------------------------------------------------------------------------
// Name: FloodAttribute::EndDrag
// Created: JSR 2012-05-10
// -----------------------------------------------------------------------------
void FloodAttribute::EndDrag() const
{
    controllers_.controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: FloodAttribute::CreateDictionary
// Created: JSR 2010-12-07
// -----------------------------------------------------------------------------
void FloodAttribute::CreateDictionary( gui::PropertiesDictionary& dictionary, const kernel::Entity_ABC& entity )
{
    dictionary.RegisterExtension( entity, this, tools::translate( "FloodAttribute", "Info/Flood parameters/Depth" ), depth_ );
    dictionary.RegisterExtension( entity, this, tools::translate( "FloodAttribute", "Info/Flood parameters/Reference distance" ), refDist_ );
}
