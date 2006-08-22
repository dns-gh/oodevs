// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "OptionsPanel.h"
#include "moc_OptionsPanel.cpp"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Options.h"
#include "clients_kernel/OptionVariant.h"
#include "Settings.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: OptionsPanel constructor
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
OptionsPanel::OptionsPanel( QWidget* pParent, Controllers& controllers )
    : QVBox       ( pParent )
    , controllers_( controllers )
    , options_    ( controllers_.options_ )
{
    setMargin( 5 );
    setSpacing( 5 );

    // Display panel
    QWidget* pDisplayPanel = new QWidget( this );
    QGridLayout* pSubLayout2 = new QGridLayout( pDisplayPanel, 9, 2, 5 );
    pSubLayout2->setMargin( 5 );

    QLabel* pL2 = new QLabel( tr( "Taille police" ), pDisplayPanel );
    pFontSpinbox_ = new QSpinBox( 1, 50, 1, pDisplayPanel );
    pSubLayout2->addWidget( pL2, 1, 0 );
    pSubLayout2->addWidget( pFontSpinbox_, 1, 1 );
    pSubLayout2->setRowStretch( 9, 10 );
    connect( pFontSpinbox_, SIGNAL( valueChanged( int ) ), this, SLOT( FontSizeChanged( int ) ) );

    pDrawObjectIcons_ = new QCheckBox( tr( "Afficher les icones des objets"), pDisplayPanel );
    pSubLayout2->addWidget( pDrawObjectIcons_, 2, 0 );
    connect( pDrawObjectIcons_, SIGNAL( toggled( bool ) ), this, SLOT( DrawObjectsChanged( bool ) ) );

    pDrawHoveredInfo_ = new QCheckBox( tr( "Afficher des informations au survol des pions" ), pDisplayPanel );
    pSubLayout2->addWidget( pDrawHoveredInfo_, 3, 0 );
    connect( pDrawHoveredInfo_, SIGNAL( toggled( bool ) ), this, SLOT( DrawHoveredInfoChanged( bool ) ) );

    pDisplayRCOnMap_ = new QCheckBox( tr("Afficher les RC sur la carte"), pDisplayPanel );
    pSubLayout2->addWidget( pDisplayRCOnMap_, 4, 0 );
    connect( pDisplayRCOnMap_, SIGNAL( toggled( bool ) ), this, SLOT( DrawRCsChanged( bool ) ) );

    pDisplayOnlySubscribedAgentsRC_ = new QCheckBox( tr("Afficher uniquement les RC des unités abonnées"), pDisplayPanel );
    pSubLayout2->addWidget( pDisplayOnlySubscribedAgentsRC_, 5, 0 );
    connect( pDisplayOnlySubscribedAgentsRC_, SIGNAL( toggled( bool ) ), this, SLOT( DrawSubscribedRCsOnlyChanged( bool ) ) );

    pDisplayMessagesOnMap_ = new QCheckBox( tr("Afficher aussi les messages"), pDisplayPanel );
    pSubLayout2->addWidget( pDisplayMessagesOnMap_, 6, 0 );
    connect( pDisplayMessagesOnMap_, SIGNAL( toggled( bool ) ), this, SLOT( DrawMessagesChanged( bool ) ) );

    pDisplayTracesOnMap_ = new QCheckBox( tr("Afficher aussi les traces"), pDisplayPanel );
    pSubLayout2->addWidget( pDisplayTracesOnMap_, 7, 0 );
    connect( pDisplayTracesOnMap_, SIGNAL( toggled( bool ) ), this, SLOT( DrawTracesChanged( bool ) ) );

    pDisplayIdentificationLevelOnMap_ = new QCheckBox( tr("Afficher aussi les RC d'identification"), pDisplayPanel );
    pSubLayout2->addWidget( pDisplayIdentificationLevelOnMap_, 8, 0 );
    connect( pDisplayIdentificationLevelOnMap_, SIGNAL( toggled( bool ) ), this, SLOT( DrawIdentificationsChanged( bool ) ) );

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: OptionsPanel destructor
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
OptionsPanel::~OptionsPanel()
{
    controllers_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: OptionsPanel::OptionChanged
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
void OptionsPanel::OptionChanged( const std::string& name, const OptionVariant& value )
{
    if( name == "FontSize" )
        pFontSpinbox_->setValue( value.To< int >() );
    else if( name == "DrawObjectsIcons" )
        pDrawObjectIcons_->setChecked( value.To< bool >() );
    else if( name == "DrawHoveredInfos" )
        pDrawHoveredInfo_->setChecked( value.To< bool >() );
    else if( name == "DrawRCs" )
        pDisplayRCOnMap_->setChecked( value.To< bool >() );
    else if( name == "DrawSubscribedRCsOnly" )
        pDisplayOnlySubscribedAgentsRC_->setChecked( value.To< bool >() );
    else if( name == "DrawMessages" )
        pDisplayMessagesOnMap_->setChecked( value.To< bool >() );
    else if( name == "DrawTraces" )
        pDisplayTracesOnMap_->setChecked( value.To< bool >() );
    else if( name == "DrawIdentifications" )
        pDisplayIdentificationLevelOnMap_->setChecked( value.To< bool >() );
}

// -----------------------------------------------------------------------------
// Name: OptionsPanel::FontSizeChanged
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
void OptionsPanel::FontSizeChanged( int value )
{
    options_.Change( "FontSize", value );
}

// -----------------------------------------------------------------------------
// Name: OptionsPanel::DrawObjectsChanged
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
void OptionsPanel::DrawObjectsChanged( bool value )
{
    options_.Change( "DrawObjectsIcons", value );
}

// -----------------------------------------------------------------------------
// Name: OptionsPanel::DrawHoveredInfoChanged
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
void OptionsPanel::DrawHoveredInfoChanged( bool value )
{
    options_.Change( "DrawHoveredInfos", value );
}

// -----------------------------------------------------------------------------
// Name: OptionsPanel::DrawRCsChanged
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
void OptionsPanel::DrawRCsChanged( bool value )
{
    options_.Change( "DrawRCs", value );
}

// -----------------------------------------------------------------------------
// Name: OptionsPanel::DrawSubscribedRCsOnlyChanged
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
void OptionsPanel::DrawSubscribedRCsOnlyChanged( bool value )
{
    options_.Change( "DrawSubscribedRCsOnly", value );
}

// -----------------------------------------------------------------------------
// Name: OptionsPanel::DrawMessagesChanged
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
void OptionsPanel::DrawMessagesChanged( bool value )
{
    options_.Change( "DrawMessages", value );
}

// -----------------------------------------------------------------------------
// Name: OptionsPanel::DrawTracesChanged
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
void OptionsPanel::DrawTracesChanged( bool value )
{
    options_.Change( "DrawTraces", value );
}

// -----------------------------------------------------------------------------
// Name: OptionsPanel::DrawIdentificationsChanged
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
void OptionsPanel::DrawIdentificationsChanged( bool value )
{
    options_.Change( "DrawIdentifications", value );
}   
