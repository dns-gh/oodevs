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
#include "gaming/Color.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/tools.h"
#include "clients_kernel/Color_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Ghost_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_gui/ColorStrategy_ABC.h"
#include "clients_gui/ColorModifier_ABC.h"
#include "clients_gui/ColorEditor_ABC.h"
#include "clients_gui/RichRadioButton.h"
#include "clients_gui/RichCheckBox.h"
#include "clients_gui/RichPushButton.h"
#include "clients_gui/ColorButton.h"
#include "clients_gui/SignalAdapter.h"
#include <boost/optional.hpp>

// -----------------------------------------------------------------------------
// Name: ColorEditor constructor
// Created: LGY 2011-06-23
// -----------------------------------------------------------------------------
ColorEditor::ColorEditor( QWidget* parent, kernel::Controllers& controllers,
                          gui::ColorStrategy_ABC& colorStrategy, gui::ColorEditor_ABC& colorEditor )
    : QDialog( parent, "changeColorDialog" )
    , controllers_  ( controllers )
    , colorStrategy_( colorStrategy )
    , colorEditor_  ( colorEditor )
    , selected_     ( controllers )
{
    setWindowTitle( tr( "Change color" ) );
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
    gui::connect( customButton_, SIGNAL( toggled(bool) ), [=]{ colorButton_->setEnabled( customButton_->isChecked() ); } );
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

namespace
{
    QColor ToColor( const kernel::Color_ABC::T_Color& color )
    {
        return QColor( color.get< 0 >(), color.get< 1 >(), color.get< 2 >() );
    }

    QColor FindBaseColor( const kernel::Entity_ABC& entity, gui::ColorStrategy_ABC& strategy )
    {
        if( const kernel::Color_ABC* color = entity.Retrieve< kernel::Color_ABC >() )
            return ToColor( static_cast< const Color& >( *color ).GetBaseColor() );
        return strategy.FindBaseColor( entity );
    }

    void ApplyDefaultColor( const kernel::Entity_ABC& entity, gui::ColorStrategy_ABC& strategy,
                            gui::ColorEditor_ABC& colorEditor, bool applyToSubordinates )
    {
        QColor baseColor = FindBaseColor( entity, strategy );
        if( kernel::Color_ABC* color = const_cast< kernel::Color_ABC* >( entity.Retrieve< kernel::Color_ABC >() ) )
            color->ChangeColor( baseColor );
        colorEditor.Add( entity, baseColor, false, true );
        if( !applyToSubordinates )
            return;
        if( const kernel::TacticalHierarchies* pTacticalHierarchies =  entity.Retrieve< kernel::TacticalHierarchies >() )
            for( auto it = pTacticalHierarchies->CreateSubordinateIterator(); it.HasMoreElements(); )
                ApplyDefaultColor( it.NextElement(), strategy, colorEditor, applyToSubordinates );
    }
}

// -----------------------------------------------------------------------------
// Name: ColorEditor::Show
// Created: LGY 2011-06-23
// -----------------------------------------------------------------------------
void ColorEditor::Show()
{
    if( !selected_ )
        return;
    const QColor baseColor = FindBaseColor( *selected_, colorStrategy_ );
    const QColor sideColor = colorStrategy_.FindTeamColor( selected_->Get< kernel::TacticalHierarchies >().GetTop() );
    const QColor currentColor = colorStrategy_.FindColor( *selected_ );
    if( currentColor == baseColor )
        defaultButton_->setChecked( true );
    else if( currentColor == sideColor )
        sideButton_->setChecked( true );
    else
        customButton_->setChecked( true );
    subordinatesCheckBox_->setChecked( false );
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
    QColor sideColor = colorStrategy_.FindTeamColor( selected_->Get< kernel::TacticalHierarchies >().GetTop() );
    QColor currentColor = colorStrategy_.FindColor( *selected_ );
    boost::optional< QColor > newColor;
    if( defaultButton_->isChecked() )
        ApplyDefaultColor( *selected_, colorStrategy_, colorEditor_, applyToSubordinates );
    else if( sideButton_->isChecked() )
        newColor = sideColor;
    else if( customButton_->isChecked() )
        newColor = colorButton_->GetColor();
    if( newColor )
        colorEditor_.Add( *selected_, *newColor, applyToSubordinates, true );
    accept();
}

// -----------------------------------------------------------------------------
// Name: ColorEditor::Reset
// Created: LGY 2011-06-24
// -----------------------------------------------------------------------------
void ColorEditor::Reset()
{
    subordinatesCheckBox_->setChecked( false );
    defaultButton_->setChecked( true );
    colorEditor_.Remove( *selected_ );
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
// Name: ColorEditor::NotifyContextMenu
// Created: MMC 2012-01-10
// -----------------------------------------------------------------------------
void ColorEditor::NotifyUpdated( const kernel::Team_ABC& team )
{
    if( const kernel::Color_ABC* pTeamColor = team.Retrieve< kernel::Color_ABC >() )
    {
        const QColor baseColor = colorStrategy_.FindBaseColor( team );
        if( pTeamColor->IsOverriden() && static_cast< QColor >( *pTeamColor ) != baseColor )
            colorEditor_.Reset( team, baseColor );
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
