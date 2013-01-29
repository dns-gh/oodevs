// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "UrbanColorEditor.h"
#include "moc_UrbanColorEditor.cpp"

// -----------------------------------------------------------------------------
// Name: UrbanColorEditor constructor
// Created: LGY 2012-08-17
// -----------------------------------------------------------------------------
UrbanColorEditor::UrbanColorEditor( QWidget* parent )
    : QColorDialog( parent )
{
    connect( this, SIGNAL( currentColorChanged( const QColor&  )), this, SLOT( OnAccept( const QColor& ) ) );
    setModal( true );
}

// -----------------------------------------------------------------------------
// Name: UrbanColorEditor destructor
// Created: LGY 2012-08-17
// -----------------------------------------------------------------------------
UrbanColorEditor::~UrbanColorEditor()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanColorEditor::GetValue
// Created: LGY 2012-08-17
// -----------------------------------------------------------------------------
kernel::UrbanBlockColor UrbanColorEditor::GetValue()
{
    return value_;
}

// -----------------------------------------------------------------------------
// Name: UrbanColorEditor::SetValue
// Created: LGY 2012-08-17
// -----------------------------------------------------------------------------
void UrbanColorEditor::SetValue( kernel::UrbanBlockColor& value )
{
    value_ = value;
    initial_ = QColor( value.red_, value.green_, value.blue_ );
    initial_.setAlpha( value.alpha_ );
    setCurrentColor( initial_ );
}

// -----------------------------------------------------------------------------
// Name: UrbanColorEditor::OnAccept
// Created: LGY 2012-08-17
// -----------------------------------------------------------------------------
void UrbanColorEditor::OnAccept( const QColor& color )
{
    value_.red_ = color.red();
    value_.green_ = color.green();
    value_.blue_ = color.blue();
    value_.alpha_ = color.alpha();
}

// -----------------------------------------------------------------------------
// Name: UrbanColorEditor::showEvent
// Created: LGY 2012-08-17
// -----------------------------------------------------------------------------
void UrbanColorEditor::showEvent( QShowEvent* event )
{
    QDialog::showEvent(  event );
    QDesktopWidget* desktop = QApplication::desktop();
    QRect scr = desktop->screenGeometry( desktop->screenNumber( qApp->activeWindow() ) );
    move( scr.center() - rect().center() );
}

// -----------------------------------------------------------------------------
// Name: UrbanColorEditor::done
// Created: LGY 2012-08-17
// -----------------------------------------------------------------------------
void UrbanColorEditor::done( int result )
{
    if( result == 0 )
        OnAccept( initial_ );
    QColorDialog::done( result );
}
