// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "AttributeView.h"
#include "moc_AttributeView.cpp"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/Agent_ABC.h"
#include "AttributeViewCell.h"
#include "AttributeViewCellEditor.h"
#include "clients_kernel/Controllers.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: AttributeView constructor
// Created: AGE 2006-06-22
// -----------------------------------------------------------------------------
AttributeView::AttributeView( QWidget* parent, Controllers& controllers, const Entity_ABC& agent )
    : QTable( 1, 2, parent )
    , controllers_( controllers )
    , dictionary_( const_cast< Entity_ABC& >( agent ).Get< PropertiesDictionary >() ) // $$$$ SBO 2006-10-19: 
    , currentRow_( 0 )
{
    setSelectionMode( QTable::Single );
    verticalHeader()->hide();
    setLeftMargin( 0 );
    horizontalHeader()->setLabel( 0, tr( "Attribute" ) );
    horizontalHeader()->setLabel( 1, tr( "Value" ) );
    horizontalHeader()->setClickEnabled( false );
    setTopMargin( 16 );
    setColumnReadOnly( 0, false );
    setColumnReadOnly( 1, true );
    setItem( 0, 0, new AttributeViewCell( this, dictionary_ ) );
    setRowHeight( 0, 16 );

    connect( this, SIGNAL( valueChanged( int, int ) ), this, SLOT( OnValueChanged( int, int ) ) );

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: AttributeView destructor
// Created: AGE 2006-06-22
// -----------------------------------------------------------------------------
AttributeView::~AttributeView()
{
    controllers_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: AttributeView::OnValueChanged
// Created: AGE 2006-06-22
// -----------------------------------------------------------------------------
void AttributeView::OnValueChanged( int row, int )
{
    const QString attribute = text( row, 0 );
    if( attribute.isEmpty() && row != numRows() - 1 )
        removeRow( row );
    else if( ! attribute.isEmpty() )
    {
        currentRow_ = row;
        dictionary_.Display( attribute, *this );
        adjustColumn( 0 );
        adjustColumn( 1 );
        if( row == numRows() - 1 )
        {
            setNumRows( numRows() + 1 );
            setItem( numRows() - 1, 0, new AttributeViewCell( this, dictionary_ ) );
            setRowHeight( numRows() - 1, 16 );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: AttributeView::StartDisplay
// Created: AGE 2006-06-22
// -----------------------------------------------------------------------------
void AttributeView::StartDisplay()
{
    message_ = "";
}

// -----------------------------------------------------------------------------
// Name: AttributeView::DisplayFormatted
// Created: AGE 2006-06-22
// -----------------------------------------------------------------------------
void AttributeView::DisplayFormatted( const QString& formatted )
{
    message_ += formatted;
}

// -----------------------------------------------------------------------------
// Name: AttributeView::EndDisplay
// Created: AGE 2006-06-22
// -----------------------------------------------------------------------------
void AttributeView::EndDisplay()
{
    setText( currentRow_, 1, message_ );
}

// -----------------------------------------------------------------------------
// Name: AttributeView::Hide
// Created: AGE 2006-06-22
// -----------------------------------------------------------------------------
void AttributeView::Hide()
{
    // NOTHING 
}

// -----------------------------------------------------------------------------
// Name: AttributeView::SubItem
// Created: AGE 2006-06-22
// -----------------------------------------------------------------------------
Displayer_ABC& AttributeView::SubItem( const QString&  )
{
    return NotToBeCalled( __FUNCTION__ );
}

// -----------------------------------------------------------------------------
// Name: AttributeView::NotifyUpdated
// Created: AGE 2006-06-22
// -----------------------------------------------------------------------------
void AttributeView::NotifyUpdated( const Simulation::sEndTick& )
{
    for( int row = 0; row < numRows(); ++row )
    {
        currentRow_ = row;
        dictionary_.Display( text( row, 0 ), *this );
    }
}

// -----------------------------------------------------------------------------
// Name: AttributeView::keyPressEvent
// Created: AGE 2006-06-23
// -----------------------------------------------------------------------------
void AttributeView::keyPressEvent( QKeyEvent* e )
{
    if( e && e->key() == Key_Down && isEditing() )
    {
        QWidget* maybeEditor = cellWidget( currEditRow(), currEditCol() );
        if( maybeEditor && maybeEditor->inherits( "AttributeViewCellEditor" ) )
            (::qt_cast< AttributeViewCellEditor* >( maybeEditor ))->FlecheDuBas();
    }
    else
        QTable::keyPressEvent( e );
}