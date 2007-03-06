// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "LocationEditorToolbar.h"
#include "moc_LocationEditorToolbar.cpp"
#include "ParametersLayer.h"
#include "View_ABC.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "resources.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: LocationEditorToolbar constructor
// Created: SBO 2007-03-06
// -----------------------------------------------------------------------------
LocationEditorToolbar::LocationEditorToolbar( QMainWindow* parent, const kernel::CoordinateConverter_ABC& converter, View_ABC& view )
    : QToolBar( parent, "location editor" )
    , converter_( converter )
    , view_( view )
    , parameters_( 0 )
{
    setLabel( tr( "Location editor" ) );
    new QLabel( tr( "Location: " ), this );
    utm_ = new QLineEdit( this );
    QToolTip::add( utm_, tr( "Enter UTM coordinate" ) );
    utm_->setMaxLength( 15 );
    utm_->setFixedWidth( 110 );
    gotoButton_ = new QToolButton( this );
    gotoButton_->setIconSet( MAKE_PIXMAP( goto ) );
    QToolTip::add( gotoButton_, tr( "Center on location" ) );
    okButton_ = new QToolButton( this );
    okButton_->setIconSet( MAKE_PIXMAP( add_point ) );
    QToolTip::add( okButton_, tr( "Add point to current location" ) );
    okButton_->hide();
    connect( utm_, SIGNAL( returnPressed() ), SLOT( Goto() ) );
    connect( gotoButton_, SIGNAL( clicked() ), SLOT( Goto() ) );
    connect( okButton_, SIGNAL( clicked() ), SLOT( AddPoint() ) );
}

// -----------------------------------------------------------------------------
// Name: LocationEditorToolbar destructor
// Created: SBO 2007-03-06
// -----------------------------------------------------------------------------
LocationEditorToolbar::~LocationEditorToolbar()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LocationEditorToolbar::StartEdit
// Created: SBO 2007-03-06
// -----------------------------------------------------------------------------
void LocationEditorToolbar::StartEdit( ParametersLayer& parameters )
{
    show();
    parameters_ = &parameters;
    okButton_->show();
}

// -----------------------------------------------------------------------------
// Name: LocationEditorToolbar::EndEdit
// Created: SBO 2007-03-06
// -----------------------------------------------------------------------------
void LocationEditorToolbar::EndEdit()
{
    parameters_ = 0;
    okButton_->hide();
}

// -----------------------------------------------------------------------------
// Name: LocationEditorToolbar::Goto
// Created: SBO 2007-03-06
// -----------------------------------------------------------------------------
void LocationEditorToolbar::Goto()
{
    try
    {
        view_.CenterOn( GetPosition() );
    }
    catch( ... )
    {
    }
}

// -----------------------------------------------------------------------------
// Name: LocationEditorToolbar::AddPoint
// Created: SBO 2007-03-06
// -----------------------------------------------------------------------------
void LocationEditorToolbar::AddPoint()
{
    if( !parameters_ )
        return;
    try
    {
        parameters_->AddPoint( GetPosition() );
    }
    catch( ... )
    {
    }
}

// -----------------------------------------------------------------------------
// Name: LocationEditorToolbar::GetPosition
// Created: SBO 2007-03-06
// -----------------------------------------------------------------------------
geometry::Point2f LocationEditorToolbar::GetPosition() const
{
    try
    {
        return converter_.ConvertToXY( std::string( utm_->text().ascii() ) );
    }
    catch( ... )
    {
        QMessageBox::critical( topLevelWidget(), tr( "Error" ), tr( "Location is not valid UTM coordinates." ) );
        throw;
    }
}
