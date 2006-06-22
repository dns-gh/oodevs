// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "AttributeViewCell.h"
#include "moc_AttributeViewCell.cpp"
#include "DataDictionary.h"
#include <qlistbox.h>

// -----------------------------------------------------------------------------
// Name: AttributeViewCell constructor
// Created: AGE 2006-06-22
// -----------------------------------------------------------------------------
AttributeViewCell::AttributeViewCell( QTable* parent, const DataDictionary& dictionary )
    : QTableItem( parent, WhenCurrent )
    , dictionary_( dictionary )
    , bDummy_( false )
{
    menu_ = new QPopupMenu();
    menu_->setSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding );
    list_ = new QListBox( menu_ );
    list_->setColumnMode( QListBox::Variable );
    list_->setRowMode( QListBox::Variable );
    menu_->insertItem( list_ );
    menu_->hide();

    connect( list_, SIGNAL( clicked( QListBoxItem* ) ), this, SLOT( OnListClicked( QListBoxItem* ) ) );
}

// -----------------------------------------------------------------------------
// Name: AttributeViewCell destructor
// Created: AGE 2006-06-22
// -----------------------------------------------------------------------------
AttributeViewCell::~AttributeViewCell()
{
    delete menu_;
}

// -----------------------------------------------------------------------------
// Name: AttributeViewCell::createEditor
// Created: AGE 2006-06-22
// -----------------------------------------------------------------------------
QWidget* AttributeViewCell::createEditor() const
{
    QWidget* result = QTableItem::createEditor();
    connect( result, SIGNAL( textChanged( const QString& ) ), this, SLOT( OnTextChanged( const QString& ) ) );
    connect( this, SIGNAL( SetText( const QString& ) ), result, SLOT( setText( const QString& ) ) );
    return result;
}

// -----------------------------------------------------------------------------
// Name: AttributeViewCell::OnTextChanged
// Created: AGE 2006-06-22
// -----------------------------------------------------------------------------
void AttributeViewCell::OnTextChanged( const QString& text )
{
    if( ! bDummy_ && ! text.isEmpty() )
    {
        const QRect geometry = table()->cellGeometry( row(), col() );
        list_->clear();
        list_->insertStringList( dictionary_.FindKey( text ) );
        list_->setMinimumSize( geometry.width(), 42 );
        if( list_->count() )
        {
            QPoint where = geometry.bottomLeft();
            where.setY( where.y() + table()->horizontalHeader()->height() );
            menu_->popup( table()->mapToGlobal( where ) );
            table()->setFocus();
        }
    }
    else 
        menu_->hide();
}

// -----------------------------------------------------------------------------
// Name: AttributeViewCell::OnListClicked
// Created: AGE 2006-06-22
// -----------------------------------------------------------------------------
void AttributeViewCell::OnListClicked( QListBoxItem* item )
{
    menu_->hide();
    bDummy_ = true;
    emit SetText( item->text() );
    bDummy_ = false;
}
