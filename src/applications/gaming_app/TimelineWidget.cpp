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
#include "TimelineEditor.h"
#include <qcanvas.h>
#include <qpainter.h>

const unsigned int rulerHeight_ = 15; // $$$$ SBO 2007-07-06: constants to put somewhere...
const unsigned int lineHeight_  = 30; // $$$$ SBO 2007-07-06: 

namespace
{
    class TimelineCanvas : public QCanvas
    {
    public:
        TimelineCanvas()
            : QCanvas( 10000, 200 )
            , linePen_( QColor( 150, 150, 150 ), 1, QPen::DotLine )
        {
//            setUpdatePeriod( 50 );
            setDoubleBuffering( true );
            setBackgroundColor( Qt::white );
        }
        virtual ~TimelineCanvas() {}

    private:
        virtual void drawBackground( QPainter& painter, const QRect& area )
        {
            QCanvas::drawBackground( painter, area );
            const QPen oldPen = painter.pen();
            painter.setPen( linePen_ );
            for( int i = rulerHeight_; i < height(); i += lineHeight_ ) // $$$$ SBO 2007-07-06: optimize to draw only "area"
                painter.drawLine( 0, i, width(), i );
            painter.setPen( oldPen );
        }

        QPen linePen_;
    };
}

// -----------------------------------------------------------------------------
// Name: TimelineWidget constructor
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
TimelineWidget::TimelineWidget( QWidget* parent, kernel::Controllers& controllers, ActionsScheduler& scheduler )
    : QHBox( parent )
{
    new TimelineEditor( parent, new TimelineCanvas(), controllers, scheduler );
}

// -----------------------------------------------------------------------------
// Name: TimelineWidget destructor
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
TimelineWidget::~TimelineWidget()
{
    // NOTHING
}
