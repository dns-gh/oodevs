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

#include "GlTools_ABC.h"
#include "ParametersLayer.h"
#include "ShapeHandler_ABC.h"

#include "clients_kernel/Drawing_ABC.h"
#include "clients_kernel/LocationVisitor_ABC.h"
#include "clients_kernel/Point.h"
#include "clients_kernel/Tools.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: LocationCreator constructor
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
LocationCreator::LocationCreator( QObject* parent, const QString& menu, const std::shared_ptr< ParametersLayer >& layer, ShapeHandler_ABC& handler  )
    : QObject           ( parent )
    , layer_            ( layer )
    , handler_          ( handler )
    , menu_             ( menu )
    , drawing_          ( 0 )
    , pointAllowed_     ( true )
    , lineAllowed_      ( true )
    , polygonAllowed_   ( true )
    , circleAllowed_    ( true )
    , rectangleAllowed_ ( true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LocationCreator constructor
// Created: SBO 2006-06-19
// -----------------------------------------------------------------------------
LocationCreator::LocationCreator( QObject* parent, const std::shared_ptr< ParametersLayer >& layer, ShapeHandler_ABC& handler )
    : QObject           ( parent )
    , layer_            ( layer )
    , handler_          ( handler )
    , drawing_          ( 0 )
    , pointAllowed_     ( false )
    , lineAllowed_      ( false )
    , polygonAllowed_   ( false )
    , circleAllowed_    ( false )
    , rectangleAllowed_ ( false )
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

namespace
{
    struct LocationValidator : public LocationVisitor_ABC
    {
        LocationValidator( bool point, bool line, bool polygon, bool circle, bool rectangle )
            : pointAllowed_( point ), lineAllowed_( line ), polygonAllowed_( polygon ),
              circleAllowed_( circle ), rectangleAllowed_( rectangle )
            , valid_( false ) {}
        virtual void VisitLines( const T_PointVector& )
        {
            valid_ = lineAllowed_;
        }
        virtual void VisitRectangle( const T_PointVector& )
        {
            valid_ = rectangleAllowed_;
        }
        virtual void VisitPolygon( const T_PointVector& )
        {
            valid_ = polygonAllowed_;
        }
        virtual void VisitCircle( const geometry::Point2f&, float )
        {
            valid_ = circleAllowed_;
        }
        virtual void VisitPath( const geometry::Point2f& , const T_PointVector& )
        {
            valid_ = false;
        }
        virtual void VisitPoint( const geometry::Point2f& )
        {
            valid_ = pointAllowed_;
        }
        virtual void VisitCurve( const T_PointVector& )
        {
            valid_ = false;
        }
        virtual void VisitText( const QString&, const QFont&, const geometry::Point2f& )
        {
            valid_ = false;
        }

        bool pointAllowed_, lineAllowed_, polygonAllowed_, circleAllowed_, rectangleAllowed_;
        bool valid_;
    };
}

// -----------------------------------------------------------------------------
// Name: LocationCreator::Allows
// Created: SBO 2008-04-17
// -----------------------------------------------------------------------------
bool LocationCreator::Allows( const Location_ABC& location ) const
{
    LocationValidator validator( pointAllowed_, lineAllowed_, polygonAllowed_, circleAllowed_, rectangleAllowed_ );
    location.Accept( validator );
    return validator.valid_;
}

// -----------------------------------------------------------------------------
// Name: LocationCreator::Allow
// Created: SBO 2007-03-06
// -----------------------------------------------------------------------------
void LocationCreator::Allow( bool point, bool line, bool polygon, bool circle, bool rectangle )
{
    layer_->Reset();
    pointAllowed_ = point;
    lineAllowed_ = line;
    polygonAllowed_ = polygon;
    circleAllowed_ = circle;
    rectangleAllowed_ = rectangle;
}

// -----------------------------------------------------------------------------
// Name: LocationCreator::NotifyContextMenu
// Created: SBO 2007-03-06
// -----------------------------------------------------------------------------
void LocationCreator::NotifyContextMenu( const Nothing&, ContextMenu& menu )
{
    if( lineAllowed_ || polygonAllowed_ || circleAllowed_ || rectangleAllowed_ )
    {
        ContextMenu* subMenu = menu.SubMenu( "Creation", menu_ );
        if( lineAllowed_ )
            subMenu->InsertItem( "", tools::translate( "gui::LocationCreator", "Line" ), this, SLOT( StartLine() ) );
        if( polygonAllowed_ )
            subMenu->InsertItem( "", tools::translate( "gui::LocationCreator", "Polygon" ), this, SLOT( StartPolygon() ) );
        if( circleAllowed_ )
            subMenu->InsertItem( "", tools::translate( "gui::LocationCreator", "Circle" ), this, SLOT( StartCircle() ) );
        if( rectangleAllowed_ )
            subMenu->InsertItem( "", tools::translate( "gui::LocationCreator", "Rectangle" ), this, SLOT( StartRectangle() ) );
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
        ContextMenu* subMenu = menu.SubMenu( "Creation", menu_ );
        subMenu->InsertItem( "", tools::translate( "gui::LocationCreator", "Point" ), this, SLOT( StartPoint() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: LocationCreator::NotifyContextMenu
// Created: AGE 2008-08-19
// -----------------------------------------------------------------------------
void LocationCreator::NotifyContextMenu( const kernel::Drawing_ABC& drawing, ContextMenu& menu )
{
    if( Allows( drawing.GetLocation() ) )
    {
        drawing_ = static_cast< const Drawing_ABC* >( &drawing );
        menu.InsertItem( "Creation", tr( "Add drawing as '%1'" ).arg( menu_ ), this, SLOT( AddDrawing() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: LocationCreator::NotifyDeleted
// Created: AGE 2008-08-19
// -----------------------------------------------------------------------------
void LocationCreator::NotifyDeleted( const Drawing_ABC& drawing )
{
    if( drawing_ == &drawing )
        drawing_ = 0;
}

// -----------------------------------------------------------------------------
// Name: LocationCreator::StartPoint
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
void LocationCreator::StartPoint()
{
    if( layer_->IsInsideWorld( popupPoint_ ) && ( pointAllowed_ || lineAllowed_ || polygonAllowed_ || circleAllowed_ || rectangleAllowed_ ) )
    {
        Location_ABC* location = new Point();
        location->AddPoint( popupPoint_ );
        handler_.Handle( *location );
    }
    else
        layer_->StartPoint( handler_ );
}

// -----------------------------------------------------------------------------
// Name: LocationCreator::StartLine
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
void LocationCreator::StartLine()
{
    layer_->StartLine( handler_ );
}

// -----------------------------------------------------------------------------
// Name: LocationCreator::StartRectangle
// Created: SLG 2010-03-25
// -----------------------------------------------------------------------------
void LocationCreator::StartRectangle()
{
    layer_->StartRectangle( handler_ );
}

// -----------------------------------------------------------------------------
// Name: LocationCreator::StartPolygon
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
void LocationCreator::StartPolygon()
{
    layer_->StartPolygon( handler_ );
}

// -----------------------------------------------------------------------------
// Name: LocationCreator::StartCircle
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
void LocationCreator::StartCircle()
{
    layer_->StartCircle( handler_ );
}

// -----------------------------------------------------------------------------
// Name: LocationCreator::AddDrawing
// Created: AGE 2008-08-19
// -----------------------------------------------------------------------------
void LocationCreator::AddDrawing()
{
    if( drawing_ )
    {
        layer_->Reset();
        handler_.Handle( drawing_->GetLocation().Clone() );
    }
}
