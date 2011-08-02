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
// Name: InfoDialog_Base constructor
// Created: SBO 2007-02-15
// -----------------------------------------------------------------------------
InfoDialog_Base::InfoDialog_Base( QWidget* parent, const QString& title )
    : QDialog( parent )
    , disconnected_( false )
{
    setCaption( title );
    QLayout* layout = new Q3VBoxLayout( this );
    layout->setMargin( 5 );
    setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Preferred );
    box_ = new Q3VBox( this );
    layout->add( box_ );
}

// -----------------------------------------------------------------------------
// Name: InfoDialog_Base destructor
// Created: SBO 2007-02-15
// -----------------------------------------------------------------------------
InfoDialog_Base::~InfoDialog_Base()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InfoDialog_Base::sizeHint
// Created: SBO 2007-02-15
// -----------------------------------------------------------------------------
QSize InfoDialog_Base::sizeHint() const
{
    return QSize( 300, 400 );
}

// -----------------------------------------------------------------------------
// Name: InfoDialog_Base::RootWidget
// Created: SBO 2007-02-15
// -----------------------------------------------------------------------------
QWidget* InfoDialog_Base::RootWidget()
{
    return box_;
}

// -----------------------------------------------------------------------------
// Name: InfoDialog_Base::hideEvent
// Created: SBO 2007-02-15
// -----------------------------------------------------------------------------
void InfoDialog_Base::hideEvent( QHideEvent* event )
{
    if( !disconnected_ )
        emit Closed();
    QDialog::hideEvent( event );
}

// -----------------------------------------------------------------------------
// Name: InfoDialog_Base::OnToggle
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
void InfoDialog_Base::OnToggle( bool state )
{
    if( !disconnected_ )
    {
        disconnected_ = true;
        setShown( state );
    }
    disconnected_ = false;
}

// -----------------------------------------------------------------------------
// Name: InfoDialog_Base::SetEnabled
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
void InfoDialog_Base::SetEnabled( bool state )
{
    if( !state && isShown() )
        hide();
    emit Disabled( !state );
}
