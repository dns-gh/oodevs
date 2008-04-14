// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "MiniView.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Positions.h"
#include "GlWidget.h"
#include "clients_kernel/Controllers.h"
#include <qpainter.h>

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: MiniView constructor
// Created: AGE 2006-06-23
// -----------------------------------------------------------------------------
MiniView::MiniView( QWidget* parent, Controllers& controllers, const Entity_ABC& agent )
    : QFrame( parent, "MiniView", WRepaintNoErase )
    , actions_( controllers.actions_ )
    , agent_( agent )
    , position_( agent.Get< Positions >() )
    , pixmap_( 128, 128, 32 )
{
    setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
    setFrameStyle( Box | Plain );
    show();
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
    pixmap_ = image;
    update();
}

// -----------------------------------------------------------------------------
// Name: MiniView::GetViewport
// Created: AGE 2006-06-23
// -----------------------------------------------------------------------------
geometry::Rectangle2f MiniView::GetViewport() const
{
    if( isVisible() )
    {
        geometry::Rectangle2f boundingBox = position_.GetBoundingBox();
        const float maxDimension = std::max( boundingBox.Height(), boundingBox.Width() ) + 1000; // $$$$ SBO 2006-07-05: 
        const geometry::Vector2f scale( maxDimension, maxDimension );
        const geometry::Point2f center( boundingBox.Center() );
        boundingBox.Incorporate( center + scale );
        boundingBox.Incorporate( center - scale );
        return boundingBox;
    }
    else
        return geometry::Rectangle2f();
}

// -----------------------------------------------------------------------------
// Name: MiniView::contextMenuEvent
// Created: AGE 2006-06-23
// -----------------------------------------------------------------------------
void MiniView::contextMenuEvent( QContextMenuEvent * e )
{
    agent_.ContextMenu( actions_, e->globalPos() );
}

// -----------------------------------------------------------------------------
// Name: MiniView::mouseReleaseEvent
// Created: AGE 2006-06-23
// -----------------------------------------------------------------------------
void MiniView::mouseReleaseEvent( QMouseEvent * e )
{
    if( e->button() == QMouseEvent::LeftButton )
        agent_.Select( actions_ );
}

// -----------------------------------------------------------------------------
// Name: MiniView::mouseDoubleClickEvent
// Created: AGE 2006-06-23
// -----------------------------------------------------------------------------
void MiniView::mouseDoubleClickEvent( QMouseEvent * )
{
    agent_.Activate( actions_ );
}

// -----------------------------------------------------------------------------
// Name: MiniView::drawContents
// Created: AGE 2006-06-23
// -----------------------------------------------------------------------------
void MiniView::drawContents( QPainter * p )
{
    p->drawPixmap( contentsRect(), pixmap_ );
    QFont base = p->font();
    base.setPointSize( 9 );
    p->setFont( base );
    p->drawText( contentsRect(), AlignHCenter | Qt::AlignBottom, agent_.GetName() );
}

// -----------------------------------------------------------------------------
// Name: MiniView::sizeHint
// Created: AGE 2006-06-23
// -----------------------------------------------------------------------------
QSize MiniView::sizeHint() const
{
    return QSize( 130, 130 );
}
