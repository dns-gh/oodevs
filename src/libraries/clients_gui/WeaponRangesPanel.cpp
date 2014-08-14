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
#include "SignalAdapter.h"
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
    , ph_( 0 )
    , volume_( 0 )
    , filter_( false )
{
    RichGroupBox* box = new RichGroupBox( "weaponRanges", tr( "Weapon ranges" ), this );
    RichGroupBox* directFire = new RichGroupBox( "directFireRange", tr( "Direct fire efficient range" ), box );
    {
        QHBoxLayout* phLayout = new QHBoxLayout();
        {
            phLayout->addWidget( new QLabel( tr( "Percentage (PH):" ) ) );
            phSpinbox_ = new gui::RichSpinBox( "phSpinbox", 0, 0, 100 );
            phLayout->addWidget( phSpinbox_ );
        }
        QHBoxLayout* volumeLayout = new QHBoxLayout();
        {
            volumeLayout->addWidget( new QLabel( tr( "Size:" ) ) );
            volumeCombo_ = new QComboBox( directFire );
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
        QVBoxLayout* indirectFireLayout = new QVBoxLayout( indirectFire );
        indirectFireLayout->addWidget( filterCheckBox_ );
        indirectFireLayout->addWidget( indirectWeaponCombo_ );
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
    filterCheckBox_->setState( filter_ ? QCheckBox::On : QCheckBox::Off );
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
}

void WeaponRangesPanel::Commit()
{
    ph_ = controllers_.options_.GetOption( "EfficientRangePh", 50 ).To< int >();
    volume_ = controllers_.options_.GetOption( "EfficientRangeVolume", 0 ).To< int >();
    filter_ = controllers_.options_.GetOption( "EfficientRangeFilterIndirectWeapon", false ).To< bool >();
    indirectWeapon_ = controllers_.options_.GetOption( "EfficientRangeIndirectWeapon", QString() ).To< QString >();
}

void WeaponRangesPanel::Reset()
{
    OnPhChanged( ph_ );
    OnVolumeChanged( volume_ );
    OnFilterToggled( filter_ );
    OnIndirectWeaponChanged( indirectWeapon_ );
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

void WeaponRangesPanel::OptionChanged( const std::string& name, const kernel::OptionVariant& value )
{
    if( name == "EfficientRangeVolume" )
        volumeCombo_->setCurrentIndex( value.To< int >() );
    else if( name == "EfficientRangePh" )
        phSpinbox_->setValue( value.To< int >() );
    else if( name == "EfficientRangeFilterIndirectWeapon" )
        filterCheckBox_->setState( value.To< bool >() ? QCheckBox::On : QCheckBox::Off );
    else if( name == "EfficientRangeIndirectWeapon" )
        indirectWeaponCombo_->setCurrentIndex( indirectWeaponCombo_->findText( value.To< QString >() ) );
}
