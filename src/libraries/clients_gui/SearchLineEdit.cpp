// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

/* TRANSLATOR gui::SearchLineEdit */

#include "clients_gui_pch.h"
#include "SearchLineEdit.h"
#include "SearchButton.h"
#include "ClearButton.h"
#include "Tools.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: SearchLineEdit constructor
// Created: ABR 2012-03-27
// -----------------------------------------------------------------------------
SearchLineEdit::SearchLineEdit( QWidget* parent /*= 0*/ )
    : RichLineEdit( parent )
{
    // search icon on the left
    AddWidget( new SearchButton( this ), RichLineEdit::LeftSide );

    // clear button on the right
    ClearButton* clearButton = new ClearButton( this );
    AddWidget( clearButton, RichLineEdit::RightSide );
    clearButton->hide();

    // Connection
    connect( clearButton, SIGNAL( clicked() ), this, SLOT( clear() ) );
    connect( this, SIGNAL( textChanged( const QString& ) ), clearButton, SLOT( TextChanged( const QString& ) ) );

    // Initialization
    UpdateTextMargins();
    SetInactiveText( tools::translate( "gui::SearchLineEdit", "Search") );
}

// -----------------------------------------------------------------------------
// Name: SearchLineEdit destructor
// Created: ABR 2012-03-27
// -----------------------------------------------------------------------------
SearchLineEdit::~SearchLineEdit()
{
    // NOTHING
}
