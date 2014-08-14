// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "WeaponRangesPanel.h"
#include "moc_WeaponRangesPanel.cpp"
#include "RichGroupBox.h"
#include "RichSpinBox.h"
#include "RichCheckBox.h"
#include "ColorButton.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/Options.h"
#include "clients_kernel/StaticModel.h"
#include "clients_kernel/VolumeType.h"
#include "clients_kernel/WeaponSystemType.h"
#include <tools/Resolver.h>

using namespace gui;

WeaponRangesPanel::WeaponRangesPanel( QWidget* parent, kernel::Controllers& controllers, const kernel::StaticModel& model )
    : PreferencePanel_ABC( parent, "WeaponRangesPanel" )
    , controllers_( controllers )
    , model_( model )
    , volumeCombo_( 0 )
    , phSpinbox_( 0 )
    , filterCheckBox_( 0 )
    , indirectWeaponCombo_( 0 )
    , colorCheckBox_( 0 )
    , colorButton_( 0 )
    , ph_( 0 )
    , volume_( 0 )
    , filter_( false )
    , useColor_( false )
    , color_( Qt::white )
{
    RichGroupBox* box = new RichGroupBox( "weaponRanges", tr( "Weapon ranges" ), this );
    RichGroupBox* directFire = new RichGroupBox( "directFireRange", tr( "Direct fire efficient range" ), box );
    {
        QHBoxLayout* phLayout = new QHBoxLayout();
        {
            phSpinbox_ = new gui::RichSpinBox( "phSpinbox", 0, 0, 100 );
            phLayout->addWidget( new QLabel( tr( "Percentage (PH):" ) ) );
            phLayout->addWidget( phSpinbox_ );
        }
        QHBoxLayout* volumeLayout = new QHBoxLayout();
        {
            volumeCombo_ = new QComboBox( directFire );
            volumeLayout->addWidget( new QLabel( tr( "Size:" ) ) );
            volumeLayout->addWidget( volumeCombo_ );
        }
        QVBoxLayout* directFireLayout = new QVBoxLayout( directFire );
        directFireLayout->addLayout( phLayout );
        directFireLayout->addLayout( volumeLayout );
        directFireLayout->addStretch();
    }
    RichGroupBox* indirectFire = new RichGroupBox( "indirectFireRange", tr( "Indirect fire range" ), box );
    {
        filterCheckBox_ = new gui::RichCheckBox( "toggleDisplayOnlyRangeOf", tr( "Display only the range of:" ) );
        indirectWeaponCombo_ = new QComboBox( indirectFire );
        QHBoxLayout* colorLayout = new QHBoxLayout();
        {
            colorCheckBox_ = new gui::RichCheckBox( "toggleUseCustomColor", tr( "Use custom color:" ) );
            colorButton_ = new gui::ColorButton( "weaponRangesColor", indirectFire );
            colorLayout->addWidget( colorCheckBox_ );
            colorLayout->addWidget( colorButton_ );
        }
        QVBoxLayout* indirectFireLayout = new QVBoxLayout( indirectFire );
        indirectFireLayout->addWidget( filterCheckBox_ );
        indirectFireLayout->addWidget( indirectWeaponCombo_ );
        indirectFireLayout->addLayout( colorLayout );
        indirectFireLayout->addStretch();
    }

    QVBoxLayout* boxLayout = new QVBoxLayout( box );
    boxLayout->addWidget( directFire );
    boxLayout->addWidget( indirectFire );
    boxLayout->addStretch( 1 );
    setWidget( box );

    connect( phSpinbox_, SIGNAL( valueChanged( int ) ), SLOT( OnPhChanged( int ) ) );
    connect( volumeCombo_, SIGNAL( currentIndexChanged( int ) ), SLOT( OnVolumeChanged( int ) ) );
    connect( filterCheckBox_, SIGNAL( toggled( bool ) ), SLOT( OnFilterToggled( bool ) ) );
    connect( indirectWeaponCombo_, SIGNAL( currentIndexChanged( const QString& ) ), SLOT( OnIndirectWeaponChanged( const QString& ) ) );
    connect( colorCheckBox_, SIGNAL( toggled( bool ) ), SLOT( OnColorToggled( bool ) ) );
    connect( colorButton_, SIGNAL( ColorChanged( const QColor& ) ), SLOT( OnColorChanged( const QColor& ) ) );

    controllers_.Register( *this );
}

WeaponRangesPanel::~WeaponRangesPanel()
{
    controllers_.Unregister( *this );
}

void WeaponRangesPanel::NotifyUpdated( const kernel::ModelLoaded& /*model*/ )
{
    Commit();
    phSpinbox_->setValue( ph_ );
    {
        auto it = model_.objectTypes_.tools::Resolver< kernel::VolumeType >::CreateIterator();
        while( it.HasMoreElements() )
            volumeCombo_->insertItem( it.NextElement().GetName().c_str() );
        volumeCombo_->setCurrentItem( volume_ < volumeCombo_->count() ? volume_ : 0 );
    }
    filterCheckBox_->setChecked( filter_ );
    {
        auto it = model_.objectTypes_.tools::Resolver< kernel::WeaponSystemType, std::string >::CreateIterator();
        while( it.HasMoreElements() )
        {
            const auto& weapon = it.NextElement();
            if( weapon.IsIndirect() )
                indirectWeaponCombo_->insertItem( weapon.GetId().c_str() );
        }
        indirectWeaponCombo_->setCurrentItem( indirectWeaponCombo_->findText( indirectWeapon_ ) );
    }
    colorCheckBox_->setChecked( useColor_ );
    colorButton_->SetColor( color_ );
}

void WeaponRangesPanel::Commit()
{
    ph_ = controllers_.options_.GetOption( "EfficientRangePh", 50 ).To< int >();
    volume_ = controllers_.options_.GetOption( "EfficientRangeVolume", 0 ).To< int >();
    filter_ = controllers_.options_.GetOption( "EfficientRangeFilterIndirectWeapon", false ).To< bool >();
    indirectWeapon_ = controllers_.options_.GetOption( "EfficientRangeIndirectWeapon", QString() ).To< QString >();
    useColor_ = controllers_.options_.GetOption( "EfficientRangeUseCustomColor", false ).To< bool >();
    color_ = controllers_.options_.GetOption( "EfficientRangeCustomColor", color_.name() ).To< QString >();
    colorButton_->Commit();
}

void WeaponRangesPanel::Reset()
{
    OnPhChanged( ph_ );
    OnVolumeChanged( volume_ );
    OnFilterToggled( filter_ );
    OnIndirectWeaponChanged( indirectWeapon_ );
    OnColorToggled( useColor_ );
    OnColorChanged( color_ );
    colorButton_->Revert();
}

void WeaponRangesPanel::OnPhChanged( int value )
{
    controllers_.options_.Change( "EfficientRangePh", value );
}

void WeaponRangesPanel::OnVolumeChanged( int index )
{
    controllers_.options_.Change( "EfficientRangeVolume", index );
}

void WeaponRangesPanel::OnFilterToggled( bool state )
{
    controllers_.options_.Change( "EfficientRangeFilterIndirectWeapon", state );
}

void WeaponRangesPanel::OnIndirectWeaponChanged( const QString& value )
{
    controllers_.options_.Change( "EfficientRangeIndirectWeapon", value );
}

void WeaponRangesPanel::OnColorToggled( bool state )
{
    controllers_.options_.Change( "EfficientRangeUseCustomColor", state );
}

void WeaponRangesPanel::OnColorChanged( const QColor& color )
{
    controllers_.options_.Change( "EfficientRangeCustomColor", color.name() );
}

void WeaponRangesPanel::OptionChanged( const std::string& name, const kernel::OptionVariant& value )
{
    if( name == "EfficientRangeVolume" )
        volumeCombo_->setCurrentIndex( value.To< int >() );
    else if( name == "EfficientRangePh" )
        phSpinbox_->setValue( value.To< int >() );
    else if( name == "EfficientRangeFilterIndirectWeapon" )
        filterCheckBox_->setChecked( value.To< bool >() );
    else if( name == "EfficientRangeIndirectWeapon" )
        indirectWeaponCombo_->setCurrentIndex( indirectWeaponCombo_->findText( value.To< QString >() ) );
    else if( name == "EfficientRangeUseCustomColor" )
        colorCheckBox_->setChecked( value.To< bool >() );
    else if( name == "EfficientRangeCustomColor" )
    {
        const QColor color( value.To< QString >() );
        if( color != colorButton_->GetColor() )
            colorButton_->SetColor( color );
    }
}
