// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "AttributeViewCellEditor.h"
#include "moc_AttributeViewCellEditor.cpp"
#include "DataDictionary.h"
#include <qtable.h>

// -----------------------------------------------------------------------------
// Name: AttributeViewCellEditor constructor
// Created: AGE 2006-06-23
// -----------------------------------------------------------------------------
AttributeViewCellEditor::AttributeViewCellEditor( QWidget* parent, const DataDictionary& dictionary, QTableItem* myItem )
    : QLineEdit( parent )
    , dictionary_( dictionary )
    , myItem_( myItem )
{
    setFrame( false );
    menu_ = new QPopupMenu();
    menu_->setSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding );
    list_ = new QListBox( menu_ );
    list_->setColumnMode( QListBox::Variable );
    list_->setRowMode( QListBox::Variable );
    menu_->insertItem( list_ );
    menu_->hide();

    connect( list_, SIGNAL( clicked( QListBoxItem* ) ), this, SLOT( OnListClicked( QListBoxItem* ) ) );
    connect( this, SIGNAL( textChanged( const QString& ) ), this, SLOT( OnTextChanged( const QString& ) ) );
}

// -----------------------------------------------------------------------------
// Name: AttributeViewCellEditor destructor
// Created: AGE 2006-06-23
// -----------------------------------------------------------------------------
AttributeViewCellEditor::~AttributeViewCellEditor()
{
    delete menu_;
}

// -----------------------------------------------------------------------------
// Name: AttributeViewCellEditor::OnTextChanged
// Created: AGE 2006-06-23
// -----------------------------------------------------------------------------
void AttributeViewCellEditor::OnTextChanged( const QString& text )
{
    if( ! text.isEmpty() )
    {
        const QRect geometry = frameGeometry();
        list_->clear();
        list_->insertStringList( dictionary_.FindKey( text ) );
        list_->setMinimumSize( geometry.width(), 42 );
        if( list_->count() )
        {
            menu_->popup( mapToGlobal( QPoint( 0, height() ) ) );
            parentWidget()->setFocus();
        }
    }
    else 
        menu_->hide();
}

// -----------------------------------------------------------------------------
// Name: AttributeViewCellEditor::OnListClicked
// Created: AGE 2006-06-23
// -----------------------------------------------------------------------------
void AttributeViewCellEditor::OnListClicked( QListBoxItem* item )
{
    menu_->hide();
    disconnect( this, SIGNAL( textChanged( const QString& ) ), this, SLOT( OnTextChanged( const QString& ) ) );
    setText( item->text() );
    connect( this, SIGNAL( textChanged( const QString& ) ), this, SLOT( OnTextChanged( const QString& ) ) );
}

    

    
    
