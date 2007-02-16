// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "InfoDialog.h"
#include "moc_InfoDialog.cpp"

// -----------------------------------------------------------------------------
// Name: InfoDialog constructor
// Created: SBO 2007-02-15
// -----------------------------------------------------------------------------
InfoDialog::InfoDialog( QWidget* parent, const QString& title )
    : QDialog( parent )
{
    setCaption( title );
    QLayout* layout = new QVBoxLayout( this );
    setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Preferred );
    box_ = new QVBox( this );
    layout->add( box_ );
}

// -----------------------------------------------------------------------------
// Name: InfoDialog destructor
// Created: SBO 2007-02-15
// -----------------------------------------------------------------------------
InfoDialog::~InfoDialog()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InfoDialog::Add
// Created: SBO 2007-02-15
// -----------------------------------------------------------------------------
void InfoDialog::Add( QWidget* )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InfoDialog::Remove
// Created: SBO 2007-02-15
// -----------------------------------------------------------------------------
void InfoDialog::Remove( QWidget* )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InfoDialog::sizeHint
// Created: SBO 2007-02-15
// -----------------------------------------------------------------------------
QSize InfoDialog::sizeHint() const
{
    return QSize( 300, 400 );
}

// -----------------------------------------------------------------------------
// Name: InfoDialog::RootWidget
// Created: SBO 2007-02-15
// -----------------------------------------------------------------------------
QWidget* InfoDialog::RootWidget()
{
    return box_;
}

// -----------------------------------------------------------------------------
// Name: InfoDialog::hideEvent
// Created: SBO 2007-02-15
// -----------------------------------------------------------------------------
void InfoDialog::hideEvent( QHideEvent* event )
{
    emit Closed();
    QDialog::hideEvent( event );
}
