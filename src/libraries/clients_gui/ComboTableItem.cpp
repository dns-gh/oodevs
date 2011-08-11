// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "ComboTableItem.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: ComboTableItem constructor
// Created: ABR 2011-07-20
// -----------------------------------------------------------------------------
ComboTableItem::ComboTableItem( Q3Table* parent, const QStringList& content, Q3TableItem::EditType editType, int currentIndex /*= 0*/ )
    : Q3TableItem( parent, editType )
    , contents_    ( content )
    , currentIndex_( currentIndex )
    , currentCombo_( 0 )
{
    setText( CurrentText() );
}

// -----------------------------------------------------------------------------
// Name: ComboTableItem destructor
// Created: ABR 2011-07-20
// -----------------------------------------------------------------------------
ComboTableItem::~ComboTableItem()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ComboTableItem::createEditor
// Created: ABR 2011-07-20
// -----------------------------------------------------------------------------
QWidget* ComboTableItem::createEditor() const
{
    QComboBox* combo = new QComboBox( table()->viewport() );
    QObject::connect( combo, SIGNAL( activated( int ) ), table(), SLOT( doValueChanged() ) );
    combo->insertStringList( contents_ );
    combo->setCurrentItem( currentIndex_ );
    currentCombo_ = combo;
    return combo;
}

// -----------------------------------------------------------------------------
// Name: ComboTableItem::setContentFromEditor
// Created: ABR 2011-07-20
// -----------------------------------------------------------------------------
void ComboTableItem::setContentFromEditor( QWidget* widget )
{
    currentCombo_ = 0;
    if( widget->inherits( "QComboBox" ) )
    {
        QComboBox* combo = static_cast< QComboBox* >( widget );
        currentIndex_ = combo->currentItem();
        if( currentIndex_ == -1 )
            currentIndex_ = 0;
        setText( CurrentText() );
    }
    else
        Q3TableItem::setContentFromEditor( widget );
}

// -----------------------------------------------------------------------------
// Name: ComboTableItem::CurrentItem
// Created: ABR 2011-07-20
// -----------------------------------------------------------------------------
int ComboTableItem::CurrentItem() const
{
    if( currentCombo_ )
        return currentCombo_->currentItem();
    return currentIndex_;
}

// -----------------------------------------------------------------------------
// Name: ComboTableItem::SetCurrentItem
// Created: ABR 2011-07-20
// -----------------------------------------------------------------------------
void ComboTableItem::SetCurrentItem( int index )
{
    assert( index >= 0 && index < contents_.size() );
    currentIndex_ = index;
    setText( CurrentText() );
}

// -----------------------------------------------------------------------------
// Name: ComboTableItem::CurrentText
// Created: ABR 2011-07-20
// -----------------------------------------------------------------------------
QString ComboTableItem::CurrentText() const
{
    assert( currentIndex_ >= 0 && currentIndex_ < contents_.size() );
    if( currentCombo_ )
        return currentCombo_->currentText();
    return contents_[ currentIndex_ ];
}

// -----------------------------------------------------------------------------
// Name: ComboTableItem::SetCurrentText
// Created: ABR 2011-07-20
// -----------------------------------------------------------------------------
void ComboTableItem::SetCurrentText( const QString& text )
{
    int index = contents_.findIndex( text );
    assert( index >= 0 && index < contents_.size() );
    currentIndex_ = index;
    setText( CurrentText() );
}

// -----------------------------------------------------------------------------
// Name: ComboTableItem::GetTexts
// Created: ABR 2011-07-25
// -----------------------------------------------------------------------------
const QStringList& ComboTableItem::GetTexts() const
{
    return contents_;
}
