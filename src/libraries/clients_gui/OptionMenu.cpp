// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "OptionMenu.h"
#include "moc_OptionMenu.cpp"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: OptionMenuBase constructor
// Created: SBO 2006-04-28
// -----------------------------------------------------------------------------
OptionMenuBase::OptionMenuBase( QWidget* parent )
    : kernel::ContextMenu( parent )
    , selected_( -1 )
{
    connect( this, SIGNAL( activated( int ) ), this, SLOT( OnItemSelected( int ) ) );
}

// -----------------------------------------------------------------------------
// Name: OptionMenuBase destructor
// Created: SBO 2006-04-28
// -----------------------------------------------------------------------------
OptionMenuBase::~OptionMenuBase()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: OptionMenuBase::AddItem
// Created: SBO 2006-04-28
// -----------------------------------------------------------------------------
void OptionMenuBase::AddItem( const QString& label, int i )
{
    insertItem( label, i );
    if( selected_ == -1 )
        OnItemSelected( i );
}

// -----------------------------------------------------------------------------
// Name: OptionMenuBase::OnItemSelected
// Created: AGE 2006-03-27
// -----------------------------------------------------------------------------
void OptionMenuBase::OnItemSelected( int id )
{
    Select( id );
    OnSelected( id );
}

// -----------------------------------------------------------------------------
// Name: OptionMenuBase::Select
// Created: AGE 2006-03-27
// -----------------------------------------------------------------------------
void OptionMenuBase::Select( int id )
{
    if( id == selected_ )
    {
        if ( !isItemChecked( id ) )
            setItemChecked( selected_, true );
    }
    else
    {
        setItemChecked( selected_, false );
        selected_ = id;
        setItemChecked( selected_, true );
    }
}
