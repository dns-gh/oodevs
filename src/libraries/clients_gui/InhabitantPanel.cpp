// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "InhabitantPanel.h"
#include "moc_InhabitantPanel.cpp"
#include "DensityWidget.h"
#include "GlProxy.h"
#include "GLOptions.h"
#include "RichGroupBox.h"
#include "SubObjectName.h"
#include "UrbanDisplayOptions.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: InhabitantPanel constructor
// Created: LGY 2010-12-31
// -----------------------------------------------------------------------------
InhabitantPanel::InhabitantPanel( QWidget* parent, kernel::OptionsController& options )
    : PreferencePanel_ABC( parent, "InhabitantPanel" )
{
    SubObjectName subObject( "InhabitantPanel" );
    RichGroupBox* densityGroup = new RichGroupBox( "densityGroup", tr( "Density Gradient map" ) );
    QHBoxLayout* densityLayout = new QHBoxLayout( densityGroup );
    density_ = new DensityWidget( options, "densityGroup", "Density" );
    densityLayout->addWidget( density_ );

    RichGroupBox* occupationGroup = new RichGroupBox( "occupationGroup", tr( "Occupation Gradient map" ) );
    QHBoxLayout* occupationLayout = new QHBoxLayout( occupationGroup );
    accommodation_ = new DensityWidget( options, "occupationWidget", "Accommodation" );
    occupationLayout->addWidget( accommodation_ );

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget( densityGroup );
    layout->addWidget( occupationGroup );
    layout->addStretch( 1 );
    setLayout( layout );
}

// -----------------------------------------------------------------------------
// Name: InhabitantPanel destructor
// Created: LGY 2010-12-31
// -----------------------------------------------------------------------------
InhabitantPanel::~InhabitantPanel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InhabitantPanel::Load
// Created: ABR 2014-08-06
// -----------------------------------------------------------------------------
void InhabitantPanel::Load( const GlProxy& view )
{
    const auto& glOptions = view.GetOptions();
    const auto& urbanOptions = glOptions.GetUrbanDisplayOptions();
    const auto& options = glOptions.GetOptions();
    if( !urbanOptions || !options )
        throw MASA_EXCEPTION( "Invalid urban options" );
    density_->Load( *options, urbanOptions->GetDensityGradient() );
    accommodation_->Load( *options, urbanOptions->GetAccommodationGradient() );
}
