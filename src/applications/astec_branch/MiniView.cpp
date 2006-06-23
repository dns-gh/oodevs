// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "MiniView.h"
#include "Agent.h"
#include "Positions.h"

// -----------------------------------------------------------------------------
// Name: MiniView constructor
// Created: AGE 2006-06-23
// -----------------------------------------------------------------------------
MiniView::MiniView( QWidget* parent, const Agent& agent )
    : QLabel( parent )
    , position_( agent.Get< Positions >() )
{
    setPixmap( QPixmap( 128, 128, 32 ) );
}

// -----------------------------------------------------------------------------
// Name: MiniView destructor
// Created: AGE 2006-06-23
// -----------------------------------------------------------------------------
MiniView::~MiniView()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MiniView::SetImage
// Created: AGE 2006-06-23
// -----------------------------------------------------------------------------
void MiniView::SetImage( const QImage& image )
{
    *pixmap() = image;
    update();
}

// -----------------------------------------------------------------------------
// Name: MiniView::GetViewport
// Created: AGE 2006-06-23
// -----------------------------------------------------------------------------
geometry::Rectangle2f MiniView::GetViewport() const
{
    const geometry::Point2f center = position_.GetPosition();
    const geometry::Vector2f diag( 1000, 1000 );
    return geometry::Rectangle2f( center - diag, center + diag );

}
