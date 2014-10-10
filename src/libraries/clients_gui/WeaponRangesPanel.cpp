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
#include "OptionWidgets.h"
#include "RichGroupBox.h"
#include "SignalAdapter.h"
#include "Tools.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/OptionsController.h"
#include "clients_kernel/OptionVariant.h"
#include "clients_kernel/StaticModel.h"
#include "clients_kernel/VolumeType.h"
#include "clients_kernel/WeaponSystemType.h"
#include <tools/Resolver.h>

using namespace gui;

WeaponRangesPanel::WeaponRangesPanel( QWidget* parent,
                                      kernel::OptionsController& options,
                                      const kernel::StaticModel& model )
    : PreferencePanel_ABC( parent, "WeaponRangesPanel" )
    , options_( options )
    , model_( model )
{
    volumeCombo_ = new QComboBox();
    indirectWeaponCombo_ = new QComboBox();

    RichGroupBox* directFire = new RichGroupBox( "directFireRange", tr( "Direct fire efficient range" ) );
    QVBoxLayout* directFireLayout = new QVBoxLayout( directFire );
    directFireLayout->addWidget( tools::AddLabeledWidget( tr( "Percentage (PH):" ),
        new OptionSpinBox( options, "phSpinbox", "EfficientRange/Ph", 0, 100 ) ) );
    directFireLayout->addWidget( tools::AddLabeledWidget( tr( "Size:" ), volumeCombo_ ) );

    QHBoxLayout* colorLayout = new QHBoxLayout();
    colorLayout->addWidget( new OptionCheckBox( options, "toggleUseCustomColor", "EfficientRange/UseCustomColor", tr( "Use custom color:" ) ) );
    colorLayout->addWidget( new OptionColorButton( options, "weaponRangesColor", "EfficientRange/CustomColor" ) );

    RichGroupBox* indirectFire = new RichGroupBox( "indirectFireRange", tr( "Indirect fire range" ) );
    QVBoxLayout* indirectFireLayout = new QVBoxLayout( indirectFire );
    indirectFireLayout->addWidget( new OptionCheckBox( options,
                                                       "toggleDisplayOnlyRangeOf",
                                                       "EfficientRange/FilterIndirectWeapon",
                                                       tr( "Display only the range of:" ) ) );
    indirectFireLayout->addWidget( indirectWeaponCombo_ );
    indirectFireLayout->addLayout( colorLayout );

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget( directFire );
    layout->addWidget( indirectFire );
    layout->addStretch( 1 );
    setLayout( layout );

    gui::connect( volumeCombo_, SIGNAL( currentIndexChanged( int ) ), [&](){
        options_.Change( "EfficientRange/Volume", volumeCombo_->currentIndex() );
    } );
    gui::connect( indirectWeaponCombo_, SIGNAL( currentIndexChanged( const QString& ) ), [&](){
        options_.Change( "EfficientRange/IndirectWeapon", indirectWeaponCombo_->currentText() );
    } );
}

WeaponRangesPanel::~WeaponRangesPanel()
{
    // NOTHING
}

void WeaponRangesPanel::Load( const GlProxy& )
{
    volumeCombo_->clear();
    indirectWeaponCombo_->clear();
    auto itVolume = model_.objectTypes_.tools::Resolver< kernel::VolumeType >::CreateIterator();
    while( itVolume.HasMoreElements() )
        volumeCombo_->insertItem( itVolume.NextElement().GetName().c_str() );
    auto it = model_.objectTypes_.tools::Resolver< kernel::WeaponSystemType, std::string >::CreateIterator();
    while( it.HasMoreElements() )
    {
        const auto& weapon = it.NextElement();
        if( weapon.IsIndirect() )
            indirectWeaponCombo_->insertItem( weapon.GetId().c_str() );
    }

    const auto volume = options_.GetOption( "EfficientRange/Volume" ).To< int >();
    volumeCombo_->setCurrentItem( volume < volumeCombo_->count() ? volume : 0 );
    const auto indirectWeapon = options_.GetOption( "EfficientRange/IndirectWeapon" ).To< QString >();
    indirectWeaponCombo_->setCurrentIndex( indirectWeaponCombo_->findText( indirectWeapon ) );
}
