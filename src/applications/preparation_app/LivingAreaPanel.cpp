// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "LivingAreaPanel.h"
#include "moc_LivingAreaPanel.cpp"
#include "preparation/InhabitantPositions.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/ModeController_ABC.h"
#include "clients_kernel/Options.h"
#include "clients_kernel/SimpleLocationDrawer.h"
#include "clients_kernel/Location_ABC.h"
#include "clients_kernel/Inhabitant_ABC.h"
#include "clients_kernel/tools.h"
#include "clients_gui/ParametersLayer.h"

namespace
{
    QString GetContent( const kernel::Inhabitant_ABC& inhabitant )
    {
        return QString( "<font size='5'>"+ inhabitant.GetName() +"</font><p><font size='3'><ul><li>" +
            tools::translate( "LivingAreaPanel", "Click on a urban block to add or remove it from the living area." ) +
            "</li><li>"+ tools::translate( "LivingAreaPanel", "Use the context menu to add or remove several urban blocks at once." ) +
            "</li></ul></font>" );
    }
}

// -----------------------------------------------------------------------------
// Name: LivingAreaPanel constructor
// Created: LGY 2012-01-05
// -----------------------------------------------------------------------------
LivingAreaPanel::LivingAreaPanel( QMainWindow* parent, kernel::Controllers& controllers, gui::ParametersLayer& paramLayer,
                                  const kernel::GlTools_ABC& tools )
    : gui::RichDockWidget( controllers, parent, "livingArea", tools::translate( "LivingAreaPanel", "Living Area" ) )
    , controllers_  ( controllers )
    , selected_     ( controllers )
    , paramLayer_   ( paramLayer )
    , tools_        ( tools )
    , mode_         ( LivingAreaPanel::add )
    , location_     ( 0 )
{
    QWidget* main = new QWidget( this );
    QHBoxLayout* mainLayout = new QHBoxLayout( main );
    QGroupBox* instruction = new QGroupBox();
    QVBoxLayout* instructionLayout = new QVBoxLayout( instruction );
    QHBoxLayout* iconLayout = new QHBoxLayout();
    QPixmap pixmap( "resources/images/preparation/livingArea.png" );
    QLabel* icon = new QLabel();
    icon->setPixmap( pixmap );
    iconLayout->addWidget( icon );
    QHBoxLayout* textLayout = new QHBoxLayout();
    text_ = new QLabel();
    iconLayout->addWidget( text_, Qt::AlignLeft );
    instructionLayout->addLayout( iconLayout );
    instructionLayout->addLayout( textLayout );
    QGroupBox* buttons = new QGroupBox();
    buttons->setTitle( tools::translate( "LivingAreaPanel", "Validation" ) );
    QHBoxLayout* buttonsLayout = new QHBoxLayout( buttons );
    QDialogButtonBox* okButton = new QDialogButtonBox( QDialogButtonBox::Ok );
    connect( okButton, SIGNAL( accepted() ), this, SLOT( Accept() ) );
    okButton->setFocus();
    QDialogButtonBox* cancelButton = new QDialogButtonBox( QDialogButtonBox::Cancel );
    connect( cancelButton, SIGNAL( rejected() ), this, SLOT( Reject() ) );
    buttonsLayout->addWidget( okButton );
    buttonsLayout->addWidget( cancelButton );
    mainLayout->addWidget( instruction, 1 );
    mainLayout->addWidget( buttons );
    setWidget( main );
    controllers_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: LivingAreaPanel destructor
// Created: LGY 2012-01-05
// -----------------------------------------------------------------------------
LivingAreaPanel::~LivingAreaPanel()
{
    controllers_.Unregister( *this );

}

// -----------------------------------------------------------------------------
// Name: LivingAreaPanel::closeEvent
// Created: LGY 2012-01-06
// -----------------------------------------------------------------------------
void LivingAreaPanel::closeEvent( QCloseEvent* /*pEvent*/ )
{
    Reset();
    controllers_.ChangeMode( ePreparationMode_Exercise );
}

// -----------------------------------------------------------------------------
// Name: LivingAreaPanel::NotifyContextMenu
// Created: LGY 2012-01-10
// -----------------------------------------------------------------------------
void LivingAreaPanel::NotifyContextMenu( const kernel::Inhabitant_ABC& entity, kernel::ContextMenu& menu )
{
    selected_ = &entity;
    menu.InsertItem( "Helpers", tools::translate( "LivingAreaEditor", "Change living Area" ), this, SLOT( Update() ), false, 2 );
    text_->setText( GetContent( entity ) );
}

// -----------------------------------------------------------------------------
// Name: LivingAreaPanel::NotifyContextMenu
// Created: LGY 2012-01-11
// -----------------------------------------------------------------------------
void LivingAreaPanel::NotifyContextMenu( const geometry::Point2f& /*point*/, kernel::ContextMenu& menu )
{
    if( controllers_.modes_ && controllers_.modes_->GetCurrentMode() == ePreparationMode_LivingArea )
    {
        menu.InsertItem( "Update", tools::translate( "LivingAreaPanel", "Add urban blocks" ), this, SLOT( Add() ) );
        menu.InsertItem( "Update", tools::translate( "LivingAreaPanel", "Remove urban blocks" ), this, SLOT( Remove() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: LivingAreaPanel::Handle
// Created: LGY 2012-01-11
// -----------------------------------------------------------------------------
void LivingAreaPanel::Handle( kernel::Location_ABC& location )
{
    delete location_;
    location_ = 0;
    location_ = &location;
    if( location_ && location_->IsValid() )
        Valid();
}

// -----------------------------------------------------------------------------
// Name: LivingAreaPanel::Draw
// Created: LGY 2012-01-11
// -----------------------------------------------------------------------------
void LivingAreaPanel::Draw( kernel::Viewport_ABC& /*viewport*/ )
{
    kernel::SimpleLocationDrawer visitor( tools_ );
    if( location_ )
        location_->Accept( visitor );
}

// -----------------------------------------------------------------------------
// Name: LivingAreaPanel::Valid
// Created: LGY 2012-01-11
// -----------------------------------------------------------------------------
void LivingAreaPanel::Valid()
{
    if( selected_ )
    {
        if( InhabitantPositions* positions = static_cast< InhabitantPositions* >( selected_.ConstCast()->Retrieve< kernel::Positions >() ) )
        {
            if( mode_ == LivingAreaPanel::add )
                positions->Add( *location_ );
            else
                positions->Remove( *location_ );
        }
        Reset();
    }
}

// -----------------------------------------------------------------------------
// Name: LivingAreaPanel::Reset
// Created: LGY 2012-01-11
// -----------------------------------------------------------------------------
void LivingAreaPanel::Reset()
{
    delete location_;
    location_ = 0;
}

// -----------------------------------------------------------------------------
// Name: LivingAreaPanel::Add
// Created: LGY 2012-01-11
// -----------------------------------------------------------------------------
void LivingAreaPanel::Add()
{
    mode_ = add;
    paramLayer_.StartPolygon( *this );
}

// -----------------------------------------------------------------------------
// Name: LivingAreaPanel::Remove
// Created: LGY 2012-01-11
// -----------------------------------------------------------------------------
void LivingAreaPanel::Remove()
{
    mode_ = remove;
    paramLayer_.StartPolygon( *this );
}

// -----------------------------------------------------------------------------
// Name: LivingAreaPanel::Update
// Created: LGY 2012-01-11
// -----------------------------------------------------------------------------
void LivingAreaPanel::Update()
{
    assert( controllers_.modes_ );
    controllers_.ChangeMode( ePreparationMode_LivingArea );
    if( selected_ )
        if( InhabitantPositions* positions = static_cast< InhabitantPositions* >( selected_.ConstCast()->Retrieve< kernel::Positions >() ) )
            positions->StartEdition();
}

// -----------------------------------------------------------------------------
// Name: LivingAreaPanel::Accept
// Created: LGY 2012-01-11
// -----------------------------------------------------------------------------
void LivingAreaPanel::Accept()
{
    if( selected_ )
        if( InhabitantPositions* positions = static_cast< InhabitantPositions* >( selected_.ConstCast()->Retrieve< kernel::Positions >() ) )
            positions->Accept();
    controllers_.ChangeMode( ePreparationMode_Exercise );
    Reset();
}

// -----------------------------------------------------------------------------
// Name: LivingAreaPanel::Reject
// Created: LGY 2012-01-11
// -----------------------------------------------------------------------------
void LivingAreaPanel::Reject()
{
    if( selected_ )
        if( InhabitantPositions* positions = static_cast< InhabitantPositions* >( selected_.ConstCast()->Retrieve< kernel::Positions >() ) )
            positions->Reject();
    controllers_.ChangeMode( ePreparationMode_Exercise );
    Reset();
}

// -----------------------------------------------------------------------------
// Name: LivingAreaPanel::Select
// Created: LGY 2012-01-12
// -----------------------------------------------------------------------------
void LivingAreaPanel::Select( const geometry::Point2f& point )
{
    if( selected_ )
        if( InhabitantPositions* positions = static_cast< InhabitantPositions* >( selected_.ConstCast()->Retrieve< kernel::Positions >() ) )
            positions->Update( point );
}
