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
#include "clients_kernel/Moveable_ABC.h"
#include "clients_kernel/CoordinateSystems.h"
#include "clients_kernel/CoordinateConverter_ABC.h"

// -----------------------------------------------------------------------------
// Name: PositionEditor constructor
// Created: AME 2010-03-08
// -----------------------------------------------------------------------------
PositionEditor::PositionEditor( QWidget* parent, kernel::Controllers& controllers, const kernel::CoordinateConverter_ABC& converter )
    : gui::PropertyDialog( parent )
    , converter_( converter )
    , value_    ( 0 )
{
    setCaption( tr( "Position Editor" ) );
    QVBoxLayout* pMainLayout = new QVBoxLayout( this );
    pMainLayout->setMargin( 10 );

    locBox_ =  new gui::LocationEditorBox( this, controllers, converter );
    locBox_->SelectDefaultParser( converter.GetCoordSystem().default_ );
    pMainLayout->addWidget( locBox_ );

    QHBoxLayout* pbuttonBox = new QHBoxLayout();
    QPushButton* okBtn     = new QPushButton( tr( "Ok" ) );
    QPushButton* cancelBtn = new QPushButton( tr( "Cancel" ) );
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
    value_ = &value;
    locBox_->UpdateField( (*value_)->GetPosition() );
}

// -----------------------------------------------------------------------------
// Name: PositionEditor::GetValue
// Created: AME 2010-03-08
// -----------------------------------------------------------------------------
kernel::Moveable_ABC* PositionEditor::GetValue()
{
    return *value_;
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
            if( !converter_.IsInBoundaries( point ) )
            {
                QMessageBox::warning( this, tr( "Invalid coordinates" ), tr( " Location is outside terrain bounding box " ), QMessageBox::Ok, Qt::NoButton );
                return;
            }
            else
            {
                (*value_)->Move( point );
                accept();
            }
        }
    }
    catch( ... ){}

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
