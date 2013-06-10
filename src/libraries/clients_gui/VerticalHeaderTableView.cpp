// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "VerticalHeaderTableView.h"

using namespace gui;

// -----------------------------------------------------------------------------
// VerticalHeaderStyle
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Name: VerticalHeaderStyle constructor
// Created: ABR 2012-02-24
// -----------------------------------------------------------------------------
VerticalHeaderStyle::VerticalHeaderStyle( QStyle* style )
    : QProxyStyle( style )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: VerticalHeaderStyle destructor
// Created: ABR 2012-02-24
// -----------------------------------------------------------------------------
VerticalHeaderStyle::~VerticalHeaderStyle()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: VerticalHeaderStyle::drawControl
// Created: ABR 2012-02-24
// -----------------------------------------------------------------------------
void VerticalHeaderStyle::drawControl( ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget  ) const
{
    if( element == QStyle::CE_HeaderLabel )
    {
        const QHeaderView *hv = qobject_cast<const QHeaderView *>( widget );
        if( hv && hv->orientation() == Qt::Horizontal )
        {
            QWidget* parent = hv->parentWidget();
            if( dynamic_cast< VerticalHeaderTableView* >( parent ) )
            {
                const QStyleOptionHeader* header = qstyleoption_cast< const QStyleOptionHeader* >( option );
                painter->save();
                painter->translate( header->rect.bottomRight() );
                painter->translate( -2.5, 0 );
                painter->rotate( 270 );
                painter->drawText( 0, 0, header->text );
                painter->restore();
                return;
            }
        }
    }
    else if( element == QStyle::CE_Header && widget && widget->inherits( "QTableCornerButton" ) )
            return;
    return QProxyStyle::drawControl( element, option, painter, widget );
}

// -----------------------------------------------------------------------------
// VerticalHeaderTableView
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Name: VerticalHeaderTableView constructor
// Created: ABR 2012-02-24
// -----------------------------------------------------------------------------
VerticalHeaderTableView::VerticalHeaderTableView( QWidget* parent )
    : RichWidget< QTableView >( "VerticalHeaderTableView", parent )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: VerticalHeaderTableView destructor
// Created: ABR 2012-02-24
// -----------------------------------------------------------------------------
VerticalHeaderTableView::~VerticalHeaderTableView()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: VerticalHeaderTableView::event
// Created: ABR 2013-01-03
// -----------------------------------------------------------------------------
bool VerticalHeaderTableView::event( QEvent *event )
{
    if( event->type() == QEvent::ToolTip )
    {
        QHelpEvent *helpEvent = static_cast< QHelpEvent* >( event );
        if( !helpEvent )
            return RichWidget< QTableView >::event( event );

        const int yPos = helpEvent->pos().y();
        const int xPos = helpEvent->pos().x();
        bool mouseOnHHeader = false;
        bool mouseOnVHeader = false;
        int row = 0;
        int col = 0;

        if( horizontalHeader()->isVisible() )
        {
            if( yPos <= horizontalHeader()->height() )
                mouseOnHHeader = true;
            else
                row = rowAt( yPos - horizontalHeader()->height() + verticalScrollBar()->sliderPosition() );
        }
        else
            row = rowAt( yPos + verticalScrollBar()->sliderPosition() );

        if( verticalHeader()->isVisible() )
        {
            if( xPos <= verticalHeader()->width() )
                mouseOnVHeader = true;
            else
                col = columnAt( xPos - verticalHeader()->width() + horizontalScrollBar()->sliderPosition() );
        }
        else
            col = columnAt( xPos + horizontalScrollBar()->sliderPosition() );

        // Outside, or on both header.
        if( row == -1 || col == -1 || ( mouseOnHHeader && mouseOnVHeader ) )
        {
            QToolTip::hideText();
            event->ignore();
        }
        else
        {
            QStandardItemModel* dataModel = static_cast< QStandardItemModel* >( model() );
            if( !dataModel )
                return false;
            QString toolTips;
            if( mouseOnHHeader )
            {
                QStandardItem* item = dataModel->horizontalHeaderItem( col );
                if( item )
                    toolTips = item->text();
            }
            else if( mouseOnVHeader )
            {
                QStandardItem* item = dataModel->verticalHeaderItem( row );
                if( item )
                    toolTips = item->text();
            }
            else
            {
                assert( row >= 0 && row < dataModel->rowCount() && col >= 0 && col < dataModel->columnCount() );
                if( QStandardItem* item = dataModel->item( row, col ) )
                    toolTips = item->text();
            }

            if( !toolTips.isEmpty() )
                QToolTip::showText( helpEvent->globalPos(), toolTips );
        }
        return true;
    }
    return RichWidget< QTableView >::event( event );
}
