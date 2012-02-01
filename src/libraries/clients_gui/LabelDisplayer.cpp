// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "LabelDisplayer.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/DotationType.h"
#include "RichLabel.h"
#include "ItemFactory_ABC.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: LabelDisplayer constructor
// Created: AGE 2006-02-09
// -----------------------------------------------------------------------------
LabelDisplayer::LabelDisplayer( QBoxLayout* parent, const QString& name, bool bold, ItemFactory_ABC& factory )
{
    QHBoxLayout* layout = new QHBoxLayout();
    parent->addLayout( layout );
    layout->addWidget( new QLabel( name ) );
    valueLabel_ = factory.CreateLabel();
    if( bold )
    {
        QFont boldFont = valueLabel_->font();
        boldFont.setBold( true );
        valueLabel_->setFont( boldFont );
    }
    layout->addWidget( valueLabel_ );
}

// -----------------------------------------------------------------------------
// Name: LabelDisplayer destructor
// Created: AGE 2006-02-09
// -----------------------------------------------------------------------------
LabelDisplayer::~LabelDisplayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LabelDisplayer::Call
// Created: AGE 2006-10-16
// -----------------------------------------------------------------------------
void LabelDisplayer::Call( const Styles::Style& style )
{
    QFont boldFont = valueLabel_->font();
    boldFont.setBold( &style == &Styles::bold );
    valueLabel_->setFont( boldFont );
}

// -----------------------------------------------------------------------------
// Name: LabelDisplayer::SubItem
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
Displayer_ABC& LabelDisplayer::SubItem( const QString& )
{
    return NotToBeCalled( __FUNCTION__ );
}

// -----------------------------------------------------------------------------
// Name: LabelDisplayer::StartDisplay
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
void LabelDisplayer::StartDisplay()
{
    message_ = "";
}

// -----------------------------------------------------------------------------
// Name: LabelDisplayer::DisplayFormatted
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
void LabelDisplayer::DisplayFormatted( const QString& formatted )
{
    message_ += formatted;
}

// -----------------------------------------------------------------------------
// Name: LabelDisplayer::EndDisplay
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
void LabelDisplayer::EndDisplay()
{
    valueLabel_->setText( message_ );
}

// -----------------------------------------------------------------------------
// Name: LabelDisplayer::Hide
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void LabelDisplayer::Hide()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LabelDisplayer::Clear
// Created: LDC 2009-03-18
// -----------------------------------------------------------------------------
void LabelDisplayer::Clear()
{
    valueLabel_->setText( QString() );
}
