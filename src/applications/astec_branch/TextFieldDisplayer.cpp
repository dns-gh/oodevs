// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "TextFieldDisplayer.h"

// -----------------------------------------------------------------------------
// Name: TextFieldDisplayer constructor
// Created: SBO 2006-04-18
// -----------------------------------------------------------------------------
TextFieldDisplayer::TextFieldDisplayer( QWidget* parent, const char* name )
    : message_()
{
    new QLabel( qApp->tr( name ), parent );
    edit_ = new QLineEdit( parent );
}

// -----------------------------------------------------------------------------
// Name: TextFieldDisplayer destructor
// Created: SBO 2006-04-18
// -----------------------------------------------------------------------------
TextFieldDisplayer::~TextFieldDisplayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TextFieldDisplayer::Hide
// Created: SBO 2006-04-18
// -----------------------------------------------------------------------------
void TextFieldDisplayer::Hide()
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: TextFieldDisplayer::SubItem
// Created: SBO 2006-04-18
// -----------------------------------------------------------------------------
Displayer_ABC& TextFieldDisplayer::SubItem( const char* /*name*/ )
{
    throw std::runtime_error( __FUNCTION__ ); // $$$$ SBO 2006-04-18: 
}
    
// -----------------------------------------------------------------------------
// Name: TextFieldDisplayer::StartDisplay
// Created: SBO 2006-04-18
// -----------------------------------------------------------------------------
void TextFieldDisplayer::StartDisplay()
{
    message_ = "";
}
    
// -----------------------------------------------------------------------------
// Name: TextFieldDisplayer::DisplayFormatted
// Created: SBO 2006-04-18
// -----------------------------------------------------------------------------
void TextFieldDisplayer::DisplayFormatted( const QString& formatted )
{
    message_ += formatted;
}
    
// -----------------------------------------------------------------------------
// Name: TextFieldDisplayer::EndDisplay
// Created: SBO 2006-04-18
// -----------------------------------------------------------------------------
void TextFieldDisplayer::EndDisplay()
{
    edit_->setText( message_ );
}
