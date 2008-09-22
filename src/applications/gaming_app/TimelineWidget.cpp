// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "TimelineWidget.h"
#include "TimelineListView.h"
#include "TimelineView.h"
#include "TimelineRuler.h"
#include <qcanvas.h>
#include <qpainter.h>
#include <qsplitter.h>

namespace
{
    class TimelineCanvas : public QCanvas
    {
    public:
        TimelineCanvas( QObject* parent, unsigned int lineHeight )
            : QCanvas( parent )
            , linePen_( QColor( 225, 225, 225 ), 1, QPen::SolidLine )
            , lineHeight_( lineHeight )
        {
            setUpdatePeriod( 50 );
            setDoubleBuffering( true );
            setBackgroundColor( Qt::white );
            resize( 10000, 0 );
        }
        virtual ~TimelineCanvas() {}

    private:
        virtual void drawBackground( QPainter& painter, const QRect& area )
        {
            QCanvas::drawBackground( painter, area );
            const QPen oldPen = painter.pen();
            painter.setPen( linePen_ );
            for( int i = lineHeight_ - 1; i < height(); i += lineHeight_ ) // $$$$ SBO 2007-07-06: optimize to draw only "area"
                painter.drawLine( 0, i, width(), i );
            painter.setPen( oldPen );
        }

        QPen linePen_;
        unsigned int lineHeight_;
    };
}

using namespace actions;

// -----------------------------------------------------------------------------
// Name: TimelineWidget constructor
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
TimelineWidget::TimelineWidget( QWidget* parent, kernel::Controllers& controllers, ActionsModel& model, ActionsScheduler& scheduler )
    : QHBox( parent, "TimelineWidget" )
{
    QSplitter* splitter = new QSplitter( this );
    QListView* list = new TimelineListView( splitter, controllers );
    QVBox* box = new QVBox( splitter );
    TimelineRuler* ruler = new TimelineRuler( box, controllers, list->header()->height() );
    QCanvasView* editor = new TimelineView( box, new TimelineCanvas( this, 25 ), controllers, model, scheduler, *ruler );

    connect( editor, SIGNAL( contentsMoving( int, int ) ), list  , SLOT( setContentsPos( int, int ) ) );
    connect( list  , SIGNAL( contentsMoving( int, int ) ), editor, SLOT( setContentsPos( int, int ) ) );
    connect( editor, SIGNAL( contentsMoving( int, int ) ), ruler , SLOT( SetContentsPos( int, int ) ) );
}

// -----------------------------------------------------------------------------
// Name: TimelineWidget destructor
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
TimelineWidget::~TimelineWidget()
{
    // NOTHING
}
