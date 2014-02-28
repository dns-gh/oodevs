// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "PositionEditor.h"
#include "moc_PositionEditor.cpp"
#include "clients_gui/LocationEditorBox.h"
#include "clients_gui/RichPushButton.h"
#include "clients_kernel/Moveable_ABC.h"
#include "clients_kernel/CoordinateConverter_ABC.h"

// -----------------------------------------------------------------------------
// Name: PositionEditor constructor
// Created: AME 2010-03-08
// -----------------------------------------------------------------------------
PositionEditor::PositionEditor( QWidget* parent, kernel::Controllers& controllers, const kernel::CoordinateConverter_ABC& converter )
    : gui::PropertyDialog( parent )
    , converter_( converter )
    , value_( 0 )
{
    gui::SubObjectName subObject( "PositionEditor" );
    setCaption( tr( "Position Editor" ) );
    QVBoxLayout* pMainLayout = new QVBoxLayout( this );
    pMainLayout->setMargin( 10 );

    locBox_ = new gui::LocationEditorBox( controllers, converter );
    locBox_->SelectParser( converter.GetDefaultCoordinateSystem() );
    pMainLayout->addWidget( locBox_ );

    QHBoxLayout* pbuttonBox = new QHBoxLayout();
    gui::RichPushButton* okBtn = new gui::RichPushButton( "ok", tr( "Ok" ) );
    gui::RichPushButton* cancelBtn = new gui::RichPushButton( "cancel", tr( "Cancel" ) );
    okBtn->setDefault( true );
    okBtn->setMaximumWidth( 100 );
    cancelBtn->setMaximumWidth( 100 );
    pbuttonBox->addWidget( okBtn );
    pbuttonBox->addWidget( cancelBtn );

    pMainLayout->addLayout( pbuttonBox );

    connect( okBtn    , SIGNAL( clicked() ), SLOT( OnAccept() ) );
    connect( cancelBtn, SIGNAL( clicked() ), SLOT( OnReject() ) );
}

// -----------------------------------------------------------------------------
// Name: PositionEditor destructor
// Created: AME 2010-03-08
// -----------------------------------------------------------------------------
PositionEditor::~PositionEditor()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PositionEditor::SetValue
// Created: AME 2010-03-08
// -----------------------------------------------------------------------------
void PositionEditor::SetValue( kernel::Moveable_ABC*& value )
{
    if( value )
    {
        value_ = &value;
        locBox_->UpdateField( value->GetPosition() );
    }
}

// -----------------------------------------------------------------------------
// Name: PositionEditor::GetValue
// Created: AME 2010-03-08
// -----------------------------------------------------------------------------
kernel::Moveable_ABC* PositionEditor::GetValue()
{
    return value_ ? *value_ : 0;
}

// -----------------------------------------------------------------------------
// Name: PositionEditor::OnAccept
// Created: AME 2010-03-08
// -----------------------------------------------------------------------------
void PositionEditor::OnAccept()
{
    try
    {
        geometry::Point2f point;
        if( locBox_->GetPosition( point ) )
        {
            if( converter_.IsInBoundaries( point ) )
            {
                if( value_ && *value_ )
                    ( *value_ )->Move( point );
                accept();
            }
            else
                QMessageBox::warning( this, tr( "Invalid coordinates" ), tr( " Location is outside terrain bounding box " ), QMessageBox::Ok, Qt::NoButton );
        }
    }
    catch( ... )
    {
        // NOTHING
    }
}

// -----------------------------------------------------------------------------
// Name: PositionEditor::OnReject
// Created: AME 2010-03-08
// -----------------------------------------------------------------------------
void PositionEditor::OnReject()
{
    reject();
}

// -----------------------------------------------------------------------------
// Name: PositionEditor::sizeHint
// Created: AME 2010-03-10
// -----------------------------------------------------------------------------
QSize PositionEditor::sizeHint() const
{
    return QSize( 300, 85 );
}
