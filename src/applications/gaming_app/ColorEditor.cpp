// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ColorEditor.h"
#include "moc_ColorEditor.cpp"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/tools.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Ghost_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_gui/Color.h"
#include "clients_gui/ColorStrategy_ABC.h"
#include "clients_gui/ColorModifier_ABC.h"
#include "clients_gui/ColorController_ABC.h"
#include "clients_gui/RichRadioButton.h"
#include "clients_gui/RichCheckBox.h"
#include "clients_gui/RichPushButton.h"
#include "clients_gui/ColorButton.h"
#include "clients_gui/SignalAdapter.h"

// -----------------------------------------------------------------------------
// Name: ColorEditor constructor
// Created: LGY 2011-06-23
// -----------------------------------------------------------------------------
ColorEditor::ColorEditor( QWidget* parent, kernel::Controllers& controllers,
                          gui::ColorStrategy_ABC& colorStrategy, gui::ColorController_ABC& colorController )
    : QDialog( parent, "changeColorDialog" )
    , controllers_( controllers )
    , colorStrategy_( colorStrategy )
    , colorController_( colorController )
    , selected_( controllers )
{
    setWindowTitle( tr( "Change color" ) );
    setModal( true );
    QVBoxLayout *mainLayout = new QVBoxLayout();
    // default color
    defaultButton_ = new gui::RichRadioButton( "defaultColorRadioButton", tr( "Default color" ), this );
    defaultButton_->setChecked( true );
    mainLayout->addWidget( defaultButton_ );
    // side color
    sideButton_ = new gui::RichRadioButton( "sideColorRadioButton", tr( "Side color" ), this );
    mainLayout->addWidget( sideButton_ );
    // custom color
    QHBoxLayout* customLayout = new QHBoxLayout();
    customButton_ = new gui::RichRadioButton( "customColorRadioButton", tr( "Custom color" ), this );
    customLayout->addWidget( customButton_ );
    colorButton_ = new gui::ColorButton( "customEntityColorButton", this );
    customLayout->addWidget( colorButton_ );
    gui::connect( colorButton_, SIGNAL( clicked() ), [&]{ customButton_->setChecked( true ); } );
    mainLayout->addLayout( customLayout );
    // apply to subordinates
    subordinatesCheckBox_ = new gui::RichCheckBox( "applyToSubordinatesCheckBox", tr( "Apply to subordinates" ), this );
    mainLayout->addWidget( subordinatesCheckBox_ );
    // buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    okButton_ = new gui::RichPushButton( "okChangeColorPushButton", tr( "OK" ), this );
    QPushButton* cancelButton = new gui::RichPushButton( "cancelChangeColorPushButton", tr( "Cancel" ), this );
    connect( okButton_, SIGNAL( clicked() ), this, SLOT( Accept() ) );
    connect( cancelButton, SIGNAL( clicked() ), this, SLOT( reject() ) );
    buttonLayout->addWidget( okButton_ );
    buttonLayout->addWidget( cancelButton );
    mainLayout->addLayout( buttonLayout );
    // main layout
    mainLayout->addStretch( 1 );
    mainLayout->setSizeConstraint( QLayout::SetFixedSize );
    setLayout( mainLayout );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ColorEditor destructor
// Created: LGY 2011-06-23
// -----------------------------------------------------------------------------
ColorEditor::~ColorEditor()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: ColorEditor::Show
// Created: LGY 2011-06-23
// -----------------------------------------------------------------------------
void ColorEditor::Show()
{
    if( !selected_ )
        return;
    const QColor baseColor = colorStrategy_.FindBaseColor( *selected_ );
    const QColor sideColor = colorStrategy_.FindTeamColor( selected_->Get< kernel::TacticalHierarchies >().GetTop() );
    const QColor currentColor = colorStrategy_.FindColor( *selected_ );
    if( currentColor == baseColor )
        defaultButton_->setChecked( true );
    else if( currentColor == sideColor )
        sideButton_->setChecked( true );
    else
        customButton_->setChecked( true );
    subordinatesCheckBox_->setChecked( true );
    colorButton_->SetColor( currentColor );
    okButton_->setFocus();
    show();
}

// -----------------------------------------------------------------------------
// Name: ColorEditor::Accept
// Created: SLI 2014-10-01
// -----------------------------------------------------------------------------
void ColorEditor::Accept()
{
    if( !selected_ )
        return;
    const bool applyToSubordinates = subordinatesCheckBox_->isChecked();
    if( defaultButton_->isChecked() )
        colorController_.ApplyDefaultColor( *selected_, colorStrategy_, applyToSubordinates );
    else if( sideButton_->isChecked() )
    {
        const QColor sideColor = colorStrategy_.FindTeamColor( selected_->Get< kernel::TacticalHierarchies >().GetTop() );
        colorController_.Add( *selected_, sideColor, applyToSubordinates, true );
    }
    else if( customButton_->isChecked() )
        colorController_.Add( *selected_, colorButton_->GetColor(), applyToSubordinates, true );
    accept();
}

// -----------------------------------------------------------------------------
// Name: ColorEditor::NotifyContextMenu
// Created: LGY 2011-06-28
// -----------------------------------------------------------------------------
void ColorEditor::NotifyContextMenu( const kernel::Team_ABC& entity, kernel::ContextMenu& menu )
{
    Update( entity, menu );
}

// -----------------------------------------------------------------------------
// Name: ColorEditor::NotifyContextMenu
// Created: LGY 2011-06-28
// -----------------------------------------------------------------------------
void ColorEditor::NotifyContextMenu( const kernel::Formation_ABC& entity, kernel::ContextMenu& menu )
{
    Update( entity, menu );
}

// -----------------------------------------------------------------------------
// Name: ColorEditor::NotifyContextMenu
// Created: LGY 2011-06-28
// -----------------------------------------------------------------------------
void ColorEditor::NotifyContextMenu( const kernel::Automat_ABC& entity, kernel::ContextMenu& menu )
{
    Update( entity, menu );
}

// -----------------------------------------------------------------------------
// Name: ColorEditor::NotifyContextMenu
// Created: LGY 2011-06-28
// -----------------------------------------------------------------------------
void ColorEditor::NotifyContextMenu( const kernel::Agent_ABC& entity, kernel::ContextMenu& menu )
{
    Update( entity, menu );
}

// -----------------------------------------------------------------------------
// Name: ColorEditor::NotifyContextMenu
// Created: ABR 2011-10-19
// -----------------------------------------------------------------------------
void ColorEditor::NotifyContextMenu( const kernel::Ghost_ABC& entity, kernel::ContextMenu& menu )
{
    Update( entity, menu );
}

// -----------------------------------------------------------------------------
// Name: ColorEditor::NotifyContextMenu
// Created: LGY 2013-03-27
// -----------------------------------------------------------------------------
void ColorEditor::NotifyContextMenu( const kernel::Object_ABC& entity, kernel::ContextMenu& menu )
{
    Update( entity, menu );
}

// -----------------------------------------------------------------------------
// Name: ColorEditor::NotifyContextMenu
// Created: JSR 2014-02-13
// -----------------------------------------------------------------------------
void ColorEditor::NotifyContextMenu( const kernel::Population_ABC& entity, kernel::ContextMenu& menu )
{
    Update( entity, menu );
}

// -----------------------------------------------------------------------------
// Name: ColorEditor::NotifyUpdated
// Created: MMC 2012-01-10
// -----------------------------------------------------------------------------
void ColorEditor::NotifyUpdated( const kernel::Team_ABC& team )
{
    if( const kernel::Color_ABC* pTeamColor = team.Retrieve< kernel::Color_ABC >() ) // $$$$ MCO what's this ?
    {
        const QColor baseColor = colorStrategy_.FindBaseColor( team );
        if( pTeamColor->IsOverriden() && static_cast< QColor >( *pTeamColor ) != baseColor )
            colorController_.Reset( team, baseColor );
    }
}

// -----------------------------------------------------------------------------
// Name: ColorEditor::Update
// Created: LGY 2011-06-23
// -----------------------------------------------------------------------------
void ColorEditor::Update( const kernel::Entity_ABC& entity, kernel::ContextMenu& menu )
{
    selected_ = const_cast< kernel::Entity_ABC* >( &entity );
    menu.InsertItem( "Helpers", tools::translate( "ColorEditor", "Change color" ), this, SLOT( Show() ), false, 3 );
}
