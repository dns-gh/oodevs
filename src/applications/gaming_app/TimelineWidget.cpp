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
#include "ActionProperties.h"
#include "TimelineListView.h"
#include "TimelineView.h"
#include "TimelineRuler.h"

namespace
{
    class TimelineCanvas : public Q3Canvas
    {
    public:
        TimelineCanvas( QObject* parent, unsigned int lineHeight )
            : Q3Canvas( parent )
            , linePen_( QColor( 225, 225, 225 ), 1, Qt::SolidLine )
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
            Q3Canvas::drawBackground( painter, area );
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
TimelineWidget::TimelineWidget( QWidget* parent, kernel::Controllers& controllers, ActionsModel& model, ActionsScheduler& scheduler, gui::ItemFactory_ABC& factory )
    : Q3HBox( parent, "TimelineWidget" )
{
    QSplitter* splitter = new QSplitter( this );
    list_ = new TimelineListView( splitter, controllers );
    Q3VBox* box = new Q3VBox( splitter );
    TimelineRuler* ruler = new TimelineRuler( box, controllers, list_->header()->height() );
    view_ = new TimelineView( box, new TimelineCanvas( this, 25 ), controllers, model, scheduler, *ruler );
    new ActionProperties( splitter, controllers, factory );

    connect( view_, SIGNAL( contentsMoving( int, int ) ), list_, SLOT( setContentsPos( int, int ) ) );
    connect( list_, SIGNAL( contentsMoving( int, int ) ), view_, SLOT( setContentsPos( int, int ) ) );
    connect( view_, SIGNAL( contentsMoving( int, int ) ), ruler, SLOT( SetContentsPos( int, int ) ) );
}

// -----------------------------------------------------------------------------
// Name: TimelineWidget destructor
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
TimelineWidget::~TimelineWidget()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TimelineWidget::SetFilter
// Created: SBO 2010-05-06
// -----------------------------------------------------------------------------
void TimelineWidget::SetFilter( const actions::ActionsFilter_ABC& filter )
{
    list_->SetFilter( filter );
    view_->SetFilter( filter );
}
