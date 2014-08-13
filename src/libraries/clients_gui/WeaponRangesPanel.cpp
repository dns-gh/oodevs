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
#include "SignalAdapter.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/Options.h"
#include "clients_kernel/StaticModel.h"
#include "clients_kernel/VolumeType.h"
#include <tools/Resolver.h>

using namespace gui;

WeaponRangesPanel::WeaponRangesPanel( QWidget* parent, kernel::Controllers& controllers, const kernel::StaticModel& model )
    : PreferencePanel_ABC( parent, "WeaponRangesPanel" )
    , controllers_( controllers )
    , model_( model )
    , ph_( 0 )
    , volume_( 0 )
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

    QVBoxLayout* boxLayout = new QVBoxLayout( box );
    boxLayout->addWidget( directFire );
    boxLayout->addStretch( 1 );
    setWidget( box );

    connect( phSpinbox_, SIGNAL( valueChanged( int ) ), SLOT( OnPhChanged( int ) ) );
    connect( volumeCombo_, SIGNAL( currentIndexChanged( int ) ), SLOT( OnVolumeChanged( int ) ) );

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
    auto it = model_.objectTypes_.tools::Resolver< kernel::VolumeType >::CreateIterator();
    while( it.HasMoreElements() )
        volumeCombo_->insertItem( it.NextElement().GetName().c_str() );
    volumeCombo_->setCurrentItem( volume_ < volumeCombo_->count() ? volume_ : 0 );
}

void WeaponRangesPanel::Commit()
{
    ph_ = controllers_.options_.GetOption( "EfficientRangePh", 50 ).To< int >();
    volume_ = controllers_.options_.GetOption( "EfficientRangeVolume", 0 ).To< int >();
}

void WeaponRangesPanel::Reset()
{
    OnPhChanged( ph_ );
    OnVolumeChanged( volume_ );
}

void WeaponRangesPanel::OnPhChanged( int value )
{
    controllers_.options_.Change( "EfficientRangePh", value );
}

void WeaponRangesPanel::OnVolumeChanged( int index )
{
    controllers_.options_.Change( "EfficientRangeVolume", index );
}

void WeaponRangesPanel::OptionChanged( const std::string& name, const kernel::OptionVariant& value )
{
    if( name == "EfficientRangeVolume" )
        volumeCombo_->setCurrentItem( value.To< int >() );
    else if( name == "EfficientRangePh" )
        phSpinbox_->setValue( value.To< int >() );
}
