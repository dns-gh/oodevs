// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "DefaultLayer.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/ActionController.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: DefaultLayer constructor
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
DefaultLayer::DefaultLayer( Controllers& controllers )
    : controllers_( controllers )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DefaultLayer destructor
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
DefaultLayer::~DefaultLayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DefaultLayer::Paint
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
void DefaultLayer::Paint( const geometry::Rectangle2f& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DefaultLayer::HandleMousePress
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
bool DefaultLayer::HandleMousePress( QMouseEvent* mouse, const geometry::Point2f& point )
{
    if( mouse && mouse->state() != Qt::NoButton )
    {
        point_ = point;
        if( mouse->button() == Qt::LeftButton )
            controllers_.actions_.Select( point_ );
        else if( mouse->button() == Qt::RightButton )
            controllers_.actions_.ContextMenu( point_, mouse->globalPos() );
    }
    return false;
}
