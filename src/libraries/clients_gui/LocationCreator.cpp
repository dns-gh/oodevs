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
LocationCreator::LocationCreator( QWidget* parent, const std::string& menu, ParametersLayer& layer, ShapeHandler_ABC& handler  )
    : QObject   ( parent )
    , layer_    ( layer )
    , handler_  ( handler )
    , menu_     ( menu )
{
    pPopupMenu_ = new QPopupMenu( parent );
    Allow( true, true, true, true );
}

// -----------------------------------------------------------------------------
// Name: LocationCreator constructor
// Created: SBO 2006-06-19
// -----------------------------------------------------------------------------
LocationCreator::LocationCreator( QWidget* parent, ParametersLayer& layer, ShapeHandler_ABC& handler )
    : QObject    ( parent )
    , layer_     ( layer )
    , handler_   ( handler )
    , pPopupMenu_( 0 )
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
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
void LocationCreator::Allow( bool point, bool line, bool polygon, bool circle )
{   
    if( pPopupMenu_ )
    {
        pPopupMenu_->clear();
        // $$$$ AGE 2006-08-22: $$tr$$
        if( point )
            pPopupMenu_->insertItem( "point", this, SLOT( StartPoint() ) );
        if( line )
            pPopupMenu_->insertItem( "ligne", this, SLOT( StartLine() ) );
        if( polygon )
            pPopupMenu_->insertItem( "polygone", this, SLOT( StartPolygon() ) );
        if( circle )
            pPopupMenu_->insertItem( "cercle", this, SLOT( StartCircle() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: LocationCreator::NotifyContextMenu
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void LocationCreator::NotifyContextMenu( const geometry::Point2f& point, ContextMenu& menu )
{
    if( !pPopupMenu_ )
        return;
    popupPoint_ = point;
    // $$$$ AGE 2006-08-22: $$tr$$
    menu.InsertItem( "Parametre", menu_.c_str(), pPopupMenu_ );
}

// -----------------------------------------------------------------------------
// Name: LocationCreator::StartPoint
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
void LocationCreator::StartPoint()
{
    if( !pPopupMenu_ )
        layer_.StartPoint( handler_ );
    else
    {
        Location_ABC* location = new Point();
        location->AddPoint( popupPoint_ );
        handler_.Handle( *location );
    }
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
