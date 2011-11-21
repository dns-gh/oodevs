// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "SummariesDisplayer.h"

// -----------------------------------------------------------------------------
// Name: SummariesDisplayer constructor
// Created: SBO 2007-03-01
// -----------------------------------------------------------------------------
SummariesDisplayer::SummariesDisplayer( QWidget* parent )
    : parent_( parent )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SummariesDisplayer destructor
// Created: SBO 2007-03-01
// -----------------------------------------------------------------------------
SummariesDisplayer::~SummariesDisplayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SummariesDisplayer::SubItem
// Created: SBO 2007-03-01
// -----------------------------------------------------------------------------
kernel::Displayer_ABC& SummariesDisplayer::SubItem( const QString& name )
{
    currentItem_ = name;
    return *this;
}

// -----------------------------------------------------------------------------
// Name: SummariesDisplayer::StartDisplay
// Created: SBO 2007-03-01
// -----------------------------------------------------------------------------
void SummariesDisplayer::StartDisplay()
{
    message_ = "";
}

// -----------------------------------------------------------------------------
// Name: SummariesDisplayer::DisplayFormatted
// Created: SBO 2007-03-01
// -----------------------------------------------------------------------------
void SummariesDisplayer::DisplayFormatted( const QString& formatted )
{
    message_ += formatted;
}

// -----------------------------------------------------------------------------
// Name: SummariesDisplayer::EndDisplay
// Created: SBO 2007-03-01
// -----------------------------------------------------------------------------
void SummariesDisplayer::EndDisplay()
{
    QLabel*& value = values_[currentItem_];
    if( !value )
    {
        QLabel* lbl = new QLabel( parent_ );
        parent_->layout()->addWidget( lbl );
        QFont font;
        font.setBold( true );
        lbl->setFont( font );
        lbl->setText( currentItem_ );
        names_[currentItem_] = lbl;
        value = new QLabel( parent_ );
        value->setWordWrap( true );
        parent_->layout()->addWidget( value );
    }
    value->setText( message_ );
    names_[currentItem_]->show();
    value->show();
    parent_->show();
}

// -----------------------------------------------------------------------------
// Name: SummariesDisplayer::Hide
// Created: SBO 2007-03-01
// -----------------------------------------------------------------------------
void SummariesDisplayer::Hide()
{
    for( CIT_Labels it = names_.begin(); it != names_.end(); ++it )
        it->second->hide();
    for( CIT_Labels it = values_.begin(); it != values_.end(); ++it )
        it->second->hide();
    parent_->hide();
}
