// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "CheckBoxDelegate.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: CommonDelegate constructor
// Created: LGY 2014-06-03
// -----------------------------------------------------------------------------
CheckBoxDelegate::CheckBoxDelegate( QObject* parent /*= 0*/ )
    : CommonDelegate( parent )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CheckBoxDelegate destructor
// Created: LGY 2014-06-03
// -----------------------------------------------------------------------------
CheckBoxDelegate::~CheckBoxDelegate()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CheckBoxDelegate::drawCheck
// Created: LGY 2014-06-03
// -----------------------------------------------------------------------------
void CheckBoxDelegate::drawCheck( QPainter* painter, const QStyleOptionViewItem& option, const QRect& rect, Qt::CheckState state ) const
{
    if( !rect.isValid() )
        return;
    const int textMargin = QApplication::style()->pixelMetric( QStyle::PM_FocusFrameHMargin ) + 1;
    QRect checkRect = QStyle::alignedRect( option.direction,
                                           Qt::AlignCenter,
                                           check( option, option.rect, Qt::Checked ).size(),
                                           QRect( option.rect.x() + textMargin,
                                           option.rect.y(),
                                           option.rect.width() - ( textMargin * 2 ),
                                           option.rect.height() ) );
    CommonDelegate::drawCheck( painter, option, checkRect, state );
}

// -----------------------------------------------------------------------------
// Name: CheckBoxDelegate::editorEvent
// Created: LGY 2014-06-03
// -----------------------------------------------------------------------------
bool CheckBoxDelegate::editorEvent( QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index )
{
    Qt::ItemFlags flags = model->flags( index );
    if( ( flags & Qt::ItemIsUserCheckable ) && ( flags & Qt::ItemIsEnabled ) )
    {
        QVariant value = index.data( Qt::CheckStateRole );
        if( !value.isValid() )
            return false;
        if( event->type() == QEvent::MouseButtonRelease )
        {
            const int textMargin = QApplication::style()->pixelMetric( QStyle::PM_FocusFrameHMargin ) + 1;
            QRect checkRect = QStyle::alignedRect( option.direction, Qt::AlignCenter,
                option.decorationSize,
                QRect( option.rect.x() + ( 2 * textMargin ), option.rect.y(),
                option.rect.width() - ( 2 * textMargin ),
                option.rect.height() ) );
            if( !checkRect.contains( static_cast< QMouseEvent* >( event )->pos() ) )
                return false;
        }
        else if( event->type() == QEvent::KeyPress )
        {
            if( static_cast< QKeyEvent* >( event )->key() != Qt::Key_Space &&
                static_cast< QKeyEvent* >( event )->key() != Qt::Key_Select )
                return false;
        }
        else
            return false;
        Qt::CheckState state =( static_cast< Qt::CheckState >( value.toInt() ) == Qt::Checked
            ? Qt::Unchecked : Qt::Checked );
        return model->setData( index, state, Qt::CheckStateRole );
    }
    return CommonDelegate::editorEvent( event, model, option, index );
}
