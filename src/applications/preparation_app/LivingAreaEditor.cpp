// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "LivingAreaEditor.h"
#include "moc_LivingAreaEditor.cpp"
#include "preparation/InhabitantPositions.h"
#include "clients_kernel/tools.h"
#include "clients_kernel/Location_ABC.h"
#include "clients_kernel/SimpleLocationDrawer.h"
#include "clients_kernel/ActionController.h"
#include "clients_gui/RichLabel.h"
#include "clients_gui/LocationCreator.h"

// -----------------------------------------------------------------------------
// Name: LivingAreaEditor constructor
// Created: LGY 2011-09-19
// -----------------------------------------------------------------------------
LivingAreaEditor::LivingAreaEditor( QWidget* parent, kernel::Controllers& controllers,
                                    gui::ParametersLayer& layer, const kernel::GlTools_ABC& tools )
    : QDialog( parent, "LivingAreaEditor" )
    , controllers_    ( controllers )
    , selected_       ( controllers )
    , tools_          ( tools )
    , mode_           ( LivingAreaEditor::add )
    , position_       ( 0 )
    , locationLabel_  ( 0 )
    , locationCreator_( 0 )
    , location_       ( 0 )
{
    setFixedSize( 250, 100 );
    Q3VBox* box = new Q3VBox( this );
    Q3GroupBox* groupBox = new Q3GroupBox( 2, Qt::Horizontal, tr( "Information" ), box );
    position_ = new gui::RichLabel( tr( "Location:" ), groupBox );
    locationLabel_ = new QLabel( tr( "---" ), groupBox );
    locationLabel_->setMinimumWidth( 150 );
    locationLabel_->setAlignment( Qt::AlignCenter );
    locationLabel_->setFrameStyle( QFrame::Box | QFrame::Sunken );
    locationCreator_ = new gui::LocationCreator( position_, tools::translate( "LivingAreaEditor", "Urban blocks" ), layer, *this );
    Q3HBox* buttonLayout = new Q3HBox( box );
    QPushButton* okButton = new QPushButton( tr( "Ok" ), buttonLayout );
    QPushButton* cancelButton = new QPushButton( tr( "Cancel" ), buttonLayout );
    okButton->setDefault( true );
    connect( okButton, SIGNAL( clicked() ), SLOT( Accept() ) );
    connect( cancelButton, SIGNAL( clicked() ), SLOT( Reject() ) );
    hide();
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: LivingAreaEditor destructor
// Created: LGY 2011-09-19
// -----------------------------------------------------------------------------
LivingAreaEditor::~LivingAreaEditor()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: LivingAreaEditor::NotifyContextMenu
// Created: LGY 2011-09-19
// -----------------------------------------------------------------------------
void LivingAreaEditor::NotifyContextMenu( const kernel::Inhabitant_ABC& entity, kernel::ContextMenu& menu )
{
    selected_ = &entity;
    menu.InsertItem( "Update", tools::translate( "LivingAreaEditor", "Add urban blocks" ), this, SLOT( Add() ) );
    menu.InsertItem( "Update", tools::translate( "LivingAreaEditor", "Remove urban blocks" ), this, SLOT( Remove() ) );
}

// -----------------------------------------------------------------------------
// Name: LivingAreaEditor::Handle
// Created: LGY 2011-09-19
// -----------------------------------------------------------------------------
void LivingAreaEditor::Handle( kernel::Location_ABC& location )
{
    delete location_;
    location_ = 0;
    location_ = &location;
    if( location.IsValid() )
        locationLabel_->setText( location.GetName() );
}

// -----------------------------------------------------------------------------
// Name: LivingAreaEditor::Draw
// Created: LGY 2011-09-19
// -----------------------------------------------------------------------------
void LivingAreaEditor::Draw( kernel::Viewport_ABC& /*viewport*/ )
{
    kernel::SimpleLocationDrawer visitor( tools_ );
    if( location_ )
        location_->Accept( visitor );
}

// -----------------------------------------------------------------------------
// Name: LivingAreaEditor::Add
// Created: LGY 2011-09-19
// -----------------------------------------------------------------------------
void LivingAreaEditor::Add()
{
    mode_ = add;
    setCaption( tools::translate( "LivingAreaEditor", "Add urban blocks" ) );
    locationCreator_->Allow( false, false, true, false, false );
    if( location_ && !location_->IsValid() )
        ResetLocation();
    controllers_.Register( *locationCreator_ );
    show();
}

// -----------------------------------------------------------------------------
// Name: LivingAreaEditor::Remove
// Created: LGY 2011-09-19
// -----------------------------------------------------------------------------
void LivingAreaEditor::Remove()
{
    mode_ = remove;
    setCaption( tools::translate( "LivingAreaEditor", "Remove urban blocks" ) );
    locationCreator_->Allow( false, false, true, false, false );
    if( location_ && !location_->IsValid() )
        ResetLocation();
    controllers_.Register( *locationCreator_ );
    show();
}

// -----------------------------------------------------------------------------
// Name: LivingAreaEditor::closeEvent
// Created: LGY 2011-09-19
// -----------------------------------------------------------------------------
void LivingAreaEditor::closeEvent( QCloseEvent* /*pEvent*/ )
{
    if( locationCreator_ )
        controllers_.Unregister( *locationCreator_ );
    ResetLocation();
}

// -----------------------------------------------------------------------------
// Name: LivingAreaEditor::ResetLocation
// Created: LGY 2011-09-19
// -----------------------------------------------------------------------------
void LivingAreaEditor::ResetLocation()
{
    delete location_;
    location_ = 0;
    locationLabel_->setText( tr( "---" ) );
}

// -----------------------------------------------------------------------------
// Name: LivingAreaEditor::Accept
// Created: LGY 2011-09-19
// -----------------------------------------------------------------------------
void LivingAreaEditor::Accept()
{
    if( CheckValidity() && selected_ )
    {
        if( InhabitantPositions* positions = static_cast< InhabitantPositions* >( selected_.ConstCast()->Retrieve< kernel::Positions >() ) )
        {
            if( mode_ == LivingAreaEditor::add )
                positions->Add( *location_ );
            else
                positions->Remove( *location_ );
        }
        if( locationCreator_ )
            controllers_.Unregister( *locationCreator_ );
        ResetLocation();
        accept();
    }
}

// -----------------------------------------------------------------------------
// Name: LivingAreaEditor::Reject
// Created: LGY 2011-09-19
// -----------------------------------------------------------------------------
void LivingAreaEditor::Reject()
{
    if( locationCreator_ )
        controllers_.Unregister( *locationCreator_ );
    ResetLocation();
    reject();
}

// -----------------------------------------------------------------------------
// Name: LivingAreaEditor::CheckValidity
// Created: LGY 2011-09-19
// -----------------------------------------------------------------------------
bool LivingAreaEditor::CheckValidity() const
{
    if( ! location_ )
    {
        position_->Warn( 3000 );
        return false;
    }
    return true;
}
