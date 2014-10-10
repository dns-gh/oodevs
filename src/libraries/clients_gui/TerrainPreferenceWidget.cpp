// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "TerrainPreferenceWidget.h"
#include "moc_TerrainPreferenceWidget.cpp"
#include "TerrainPreference.h"
#include <boost/bind.hpp>

using namespace gui;

namespace
{
    QPushButton* CreateButton( const QIcon& icon )
    {
        QPushButton* button = new QPushButton( icon, "" );
        button->setIconSize( QSize( 8, 8 ) );
        return button;
    }
}

// -----------------------------------------------------------------------------
// Name: TerrainPreferenceWidget constructor
// Created: LGY 2014-06-30
// -----------------------------------------------------------------------------
TerrainPreferenceWidget::TerrainPreferenceWidget( const QString& category,
                                                  const std::vector< TerrainPreference* >& terrains,
                                                  QSignalMapper* upSignalMapper,
                                                  QSignalMapper* downSignalMapper )
{
    QCheckBox* box = new QCheckBox( category );
    QFont font = box->font();
    font.setBold( true );
    box->setFont( font );
    box->setChecked( true );

    up_ = CreateButton( QIcon( "resources/images/gui/up.png" ) );
    down_ = CreateButton( QIcon( "resources/images/gui/down.png" ) );
    upSignalMapper->setMapping( up_, this );
    downSignalMapper->setMapping( down_, this );
    connect( up_, SIGNAL( clicked() ), upSignalMapper, SLOT( map() ) );
    connect( down_, SIGNAL( clicked() ), downSignalMapper, SLOT( map() ) );

    QHBoxLayout* headerLayer = new QHBoxLayout();
    headerLayer->setSpacing( 5 );
    headerLayer->addWidget( box );
    headerLayer->addStretch( 1 );
    headerLayer->addWidget( up_ );
    headerLayer->addWidget( down_ );

    QVBoxLayout* layout = new QVBoxLayout();
    layout->setMargin( 0 );
    layout->setSpacing( 3 );
    layout->addLayout( headerLayer );
    bool hasChildChecked = false;
    for( auto it = terrains.begin(); it != terrains.end(); ++it )
    {
        auto widget = *it;
        layout->addWidget( widget );
        connect( box, SIGNAL( toggled( bool ) ), widget, SLOT( SetEnabled( bool ) ) );
        if( widget->IsChecked() )
            hasChildChecked = true;
    }
    box->setChecked( hasChildChecked );
    setLayout( layout );
}

// -----------------------------------------------------------------------------
// Name: TerrainPreferenceWidget destructor
// Created: LGY 2014-06-30
// -----------------------------------------------------------------------------
TerrainPreferenceWidget::~TerrainPreferenceWidget()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TerrainPreferenceWidget::UpdateUpDownButton
// Created: ABR 2014-10-08
// -----------------------------------------------------------------------------
void TerrainPreferenceWidget::UpdateUpDownButton( bool enableUp, bool enableDown )
{
    up_->setEnabled( enableUp );
    down_->setEnabled( enableDown );
}
