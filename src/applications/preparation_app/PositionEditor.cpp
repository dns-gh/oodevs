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

// -----------------------------------------------------------------------------
// Name: PositionEditor constructor
// Created: AME 2010-03-08
// -----------------------------------------------------------------------------
PositionEditor::PositionEditor( QDialog*& self, QWidget* parent, kernel::Controllers& controllers, const kernel::CoordinateConverter_ABC& converter )
    : QDialog( parent, 0, true )
    , converter_( converter )
    , value_( 0 )
    , self_( self )
{
    setCaption( tr( "Position Editor" ) );
    Q3VBoxLayout* pMainLayout = new Q3VBoxLayout( this );
    pMainLayout->setMargin( 10 );

    locBox_ =  new gui::LocationEditorBox( this, controllers, converter );
    locBox_->SelectDefaultParser( converter.GetCoordSystem().defaultCoordinateSystem_ );
    pMainLayout->addWidget( locBox_ );

    Q3HBox* pbuttonBox = new Q3HBox( this );
    QPushButton* okBtn     = new QPushButton( tr( "Ok" ), pbuttonBox );
    QPushButton* cancelBtn = new QPushButton( tr( "Cancel" ), pbuttonBox );
    okBtn->setDefault( true );
    okBtn->setMaximumWidth( 60 );
    cancelBtn->setMaximumWidth( 60 );

    pMainLayout->addWidget( pbuttonBox, 0, Qt::AlignRight );

    connect( okBtn    , SIGNAL( clicked() ), SLOT( OnAccept() ) );
    connect( cancelBtn, SIGNAL( clicked() ), SLOT( OnReject() ) );

    show();
}
// -----------------------------------------------------------------------------
// Name: PositionEditor destructor
// Created: AME 2010-03-08
// -----------------------------------------------------------------------------
PositionEditor::~PositionEditor()
{
    self_ = 0;
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
