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
    : QTableView( parent )
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
