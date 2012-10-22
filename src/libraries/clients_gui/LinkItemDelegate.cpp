// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "LinkItemDelegate.h"
#include "moc_LinkItemDelegate.cpp"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: LinkItemDelegate constructor
// Created: JSR 2012-10-19
// -----------------------------------------------------------------------------
LinkItemDelegate::LinkItemDelegate( QWidget* parent )
    : QStyledItemDelegate( parent )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LinkItemDelegate destructor
// Created: JSR 2012-10-19
// -----------------------------------------------------------------------------
LinkItemDelegate::~LinkItemDelegate()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LinkItemDelegate::paint
// Created: JSR 2012-10-19
// -----------------------------------------------------------------------------
void LinkItemDelegate::paint( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const
{
    QStyleOptionViewItemV4 optionV4 = option;
    initStyleOption( &optionV4, index );
    QStyle* style = optionV4.widget ? optionV4.widget->style() : QApplication::style();
    QTextDocument doc;
    doc.setHtml( optionV4.text );
    optionV4.text = QString();
    style->drawControl( QStyle::CE_ItemViewItem, &optionV4, painter );
    QAbstractTextDocumentLayout::PaintContext ctx;
    if( optionV4.state & QStyle::State_Selected )
        ctx.palette.setColor( QPalette::Text, optionV4.palette.color( QPalette::Active, QPalette::HighlightedText ) );
    QRect textRect = style->subElementRect( QStyle::SE_ItemViewItemText, &optionV4 );
    painter->save();
    QPoint offset;
    if( QApplication::isRightToLeft() )
        offset.setX( textRect.width() - static_cast< int >( doc.idealWidth() ) );
    painter->translate( textRect.topLeft() + offset );
    painter->setClipRect( textRect.translated( -textRect.topLeft() - offset ) );
    doc.documentLayout()->draw( painter, ctx );
    painter->restore();
}

// -----------------------------------------------------------------------------
// Name: LinkItemDelegate::sizeHint
// Created: JSR 2012-10-19
// -----------------------------------------------------------------------------
QSize LinkItemDelegate::sizeHint( const QStyleOptionViewItem& option, const QModelIndex& index ) const
{
    QStyleOptionViewItemV4 optionV4 = option;
    initStyleOption( &optionV4, index );
    QTextDocument doc;
    doc.setHtml( optionV4.text );
    doc.setTextWidth( optionV4.rect.width() );
    return QSize( static_cast< int >( doc.idealWidth() ), static_cast< int >( doc.size().height() ) );
}

// -----------------------------------------------------------------------------
// Name: LinkItemDelegate::editorEvent
// Created: JSR 2012-10-19
// -----------------------------------------------------------------------------
bool LinkItemDelegate::editorEvent( QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index )
{
    QEvent::Type type = event->type();
    if( type != QEvent::MouseButtonPress && type != QEvent::MouseMove )
        return QStyledItemDelegate::editorEvent( event, model, option, index );
    QPoint p = static_cast< QMouseEvent* >( event )->pos() - option.rect.topLeft();
    QStyleOptionViewItemV4 optionV4 = option;
    initStyleOption( &optionV4, index );
    QTextDocument doc;
    doc.setHtml( optionV4.text );
    QPoint offset;
    if( QApplication::isRightToLeft() )
    {
        QStyle* style = optionV4.widget ? optionV4.widget->style() : QApplication::style();
        QRect textRect = style->subElementRect( QStyle::SE_ItemViewItemText, &optionV4 );
        offset.setX( textRect.width() - static_cast< int >( doc.idealWidth() ) );
    }
    QString anchor = doc.documentLayout()->anchorAt( p - offset );
    if( anchor.isEmpty() )
        static_cast< QWidget* >( parent() )->unsetCursor();
    else
    {
        static_cast< QWidget* >( parent() )->setCursor( Qt::PointingHandCursor );
        if( type == QEvent::MouseButtonPress )
        {
            emit LinkClicked( anchor, index );
            return true;
        }
    }
    return false;
}
