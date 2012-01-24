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
    searchButton_ = new ADN_SearchButton( this );
    AddWidget( searchButton_, ADN_BaseEditLine::LeftSide );

    // clear button on the right
    clearButton_ = new ADN_ClearButton( this );
    connect( clearButton_, SIGNAL( clicked() ), this, SLOT( clear() ) );
    connect( this, SIGNAL( textChanged( const QString& ) ), clearButton_, SLOT( TextChanged( const QString& ) ) );
    AddWidget( clearButton_, ADN_BaseEditLine::RightSide );
    clearButton_->hide();

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
