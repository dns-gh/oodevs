// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "LocationCreator.h"
#include "moc_LocationCreator.cpp"
#include "ParametersLayer.h"
#include "Tools.h"
#include "clients_kernel/GlTools_ABC.h"
#include "ShapeHandler_ABC.h"
#include "clients_kernel/Point.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: LocationCreator constructor
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
LocationCreator::LocationCreator( QObject* parent, const QString& menu, ParametersLayer& layer, ShapeHandler_ABC& handler  )
    : QObject        ( parent )
    , layer_         ( layer )
    , handler_       ( handler )
    , menu_          ( menu )
    , pointAllowed_  ( true )
    , lineAllowed_   ( true )
    , polygonAllowed_( true )
    , circleAllowed_ ( true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LocationCreator constructor
// Created: SBO 2006-06-19
// -----------------------------------------------------------------------------
LocationCreator::LocationCreator( QObject* parent, ParametersLayer& layer, ShapeHandler_ABC& handler )
    : QObject        ( parent )
    , layer_         ( layer )
    , handler_       ( handler )
    , pointAllowed_  ( false )
    , lineAllowed_   ( false )
    , polygonAllowed_( false )
    , circleAllowed_ ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LocationCreator destructor
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
LocationCreator::~LocationCreator()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LocationCreator::Allow
// Created: SBO 2007-03-06
// -----------------------------------------------------------------------------
void LocationCreator::Allow( bool point, bool line, bool polygon, bool circle )
{
    pointAllowed_ = point;
    lineAllowed_ = line;
    polygonAllowed_ = polygon;
    circleAllowed_ = circle;
}

// -----------------------------------------------------------------------------
// Name: LocationCreator::NotifyContextMenu
// Created: SBO 2007-03-06
// -----------------------------------------------------------------------------
void LocationCreator::NotifyContextMenu( const kernel::Nothing&, kernel::ContextMenu& menu )
{
    if( lineAllowed_ || polygonAllowed_ || circleAllowed_ )
    {
        QPopupMenu* subMenu = menu.SubMenu( "Creation", menu_ );
        if( lineAllowed_ )
            subMenu->insertItem( tools::translate( "Localisation", "Line" ), this, SLOT( StartLine() ) );
        if( polygonAllowed_ )
            subMenu->insertItem( tools::translate( "Localisation", "Polygon" ), this, SLOT( StartPolygon() ) );
        if( circleAllowed_ )
            subMenu->insertItem( tools::translate( "Localisation", "Circle" ), this, SLOT( StartCircle() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: LocationCreator::NotifyContextMenu
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void LocationCreator::NotifyContextMenu( const geometry::Point2f& point, ContextMenu& menu )
{
    if( pointAllowed_ )
    {
        popupPoint_ = point;
        QPopupMenu* subMenu = menu.SubMenu( "Creation", menu_ );
        if( pointAllowed_ )
            subMenu->insertItem( tools::translate( "Localisation", "Point" ), this, SLOT( StartPoint() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: LocationCreator::StartPoint
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
void LocationCreator::StartPoint()
{
    if( pointAllowed_ || lineAllowed_ || polygonAllowed_ || circleAllowed_ )
    {
        Location_ABC* location = new Point();
        location->AddPoint( popupPoint_ );
        handler_.Handle( *location );
    }
    else
        layer_.StartPoint( handler_ );
}

// -----------------------------------------------------------------------------
// Name: LocationCreator::StartLine
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
void LocationCreator::StartLine()
{
    layer_.StartLine( handler_ );
}

// -----------------------------------------------------------------------------
// Name: LocationCreator::StartPolygon
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
void LocationCreator::StartPolygon()
{
    layer_.StartPolygon( handler_ );
}   
    
// -----------------------------------------------------------------------------
// Name: LocationCreator::StartCircle
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
void LocationCreator::StartCircle()
{
    layer_.StartCircle( handler_ );
}
