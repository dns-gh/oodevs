// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "TerrainPreference.h"
#include "moc_TerrainPreference.cpp"
#include "SizeButton.h"
#include "TerrainSetting.h"
#include "OptionWidgets.h"
#include "SignalAdapter.h"
#include "ENT/Ent_Tr.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: TerrainPreference constructor
// Created: SBO 2006-04-04
// -----------------------------------------------------------------------------
TerrainPreference::TerrainPreference( kernel::OptionsController& options,
                                      const std::shared_ptr< TerrainSetting >& setting,
                                      QWidget* parent /* = 0 */ )
    : QWidget( parent )
    , options_( options )
    , colorButton_( 0 )
    , showCheckbox_( 0 )
    , sizeButton_( 0 )
{
    const auto name = QString::fromStdString( ENT_Tr::ConvertFromLocation( ENT_Tr::ConvertToLocation( setting->name_.toStdString() ), ENT_Tr::eToTr ) );
    const auto type = QString::fromStdString( setting->type_ );

    showCheckbox_ = new OptionCheckBox(   options_, type + "-shown", "Terrains/" + setting->type_ + "/Shown", name );
    showCheckbox_->setChecked( setting->shown_ );
    sizeButton_  = new SizeButton(        options_, type + "-size",  "Terrains/" + setting->type_ + "/Width", 0, 10, setting->lineWidth_ );
    auto label = new QLabel( locale().toString( setting->lineWidth_ ) +  tr( " px" ) );
    label->setFixedWidth( 35 );
    colorButton_ = new OptionColorButton( options_, type + "-color", "Terrains/" + setting->type_ + "/Color", setting->color_ );

    QHBoxLayout* layout = new QHBoxLayout( this );
    layout->setContentsMargins( 15, 0, 0, 0 );
    layout->addWidget( showCheckbox_, 2 );
    layout->addWidget( sizeButton_ );
    layout->addWidget( label );
    layout->addWidget( colorButton_ );

    gui::connect( showCheckbox_, SIGNAL( stateChanged( int ) ), [=](){
        setting->shown_ = showCheckbox_->isChecked();
    } );
    gui::connect( sizeButton_, SIGNAL( valueChanged( int ) ), [=](){
        const auto size = sizeButton_->GetSize();
        setting->lineWidth_ = size;
        label->setText( locale().toString( size ) + tr( " px" ) );
    } );
    gui::connect( colorButton_, SIGNAL( ColorChanged( const QColor& ) ), [=](){
        setting->color_ = colorButton_->GetColor();
    } );
}

// -----------------------------------------------------------------------------
// Name: TerrainPreference destructor
// Created: SBO 2006-04-04
// -----------------------------------------------------------------------------
TerrainPreference::~TerrainPreference()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TerrainPreference::SetEnabled
// Created: LGY 2014-06-20
// -----------------------------------------------------------------------------
void TerrainPreference::SetEnabled( bool value )
{
    if( !showCheckbox_ || !colorButton_ || !sizeButton_ )
        return;
    showCheckbox_->setChecked( value );
    showCheckbox_->setEnabled( value );
    colorButton_->setEnabled( value );
    sizeButton_->setEnabled( value );
}

// -----------------------------------------------------------------------------
// Name: TerrainPreference::IsChecked
// Created: ABR 2014-10-08
// -----------------------------------------------------------------------------
bool TerrainPreference::IsChecked() const
{
    return showCheckbox_->isChecked();
}
