// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_SearchLineEdit.h"

#include "ADN_ClearButton.h"
#include "ADN_SearchButton.h"

// -----------------------------------------------------------------------------
// Name: ADN_SearchLineEdit constructor
// Created: ABR 2012-01-19
// -----------------------------------------------------------------------------
ADN_SearchLineEdit::ADN_SearchLineEdit( QWidget* parent /*= 0*/ )
    : ADN_BaseEditLine( parent )
{
    // search icon on the left
    AddWidget( new ADN_SearchButton( this ), ADN_BaseEditLine::LeftSide );

    // clear button on the right
    ADN_ClearButton* clearButton = new ADN_ClearButton( this );
    AddWidget( clearButton, ADN_BaseEditLine::RightSide );
    clearButton->hide();

    // Connection
    connect( clearButton, SIGNAL( clicked() ), this, SLOT( clear() ) );
    connect( this, SIGNAL( textChanged( const QString& ) ), clearButton, SLOT( TextChanged( const QString& ) ) );

    // Initialization
    UpdateTextMargins();
    SetInactiveText( tr("Search") );
}

// -----------------------------------------------------------------------------
// Name: ADN_SearchLineEdit destructor
// Created: ABR 2012-01-19
// -----------------------------------------------------------------------------
ADN_SearchLineEdit::~ADN_SearchLineEdit()
{
    // NOTHING
}
