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
#include "TerrainPreference.h"

using namespace gui;

namespace
{
    QPushButton* CreateButton( const QIcon& icon, bool enable )
    {
        QPushButton* button = new QPushButton( icon, "" );
        button->setIconSize( QSize( 8, 8 ) );
        button->setEnabled( enable );
        return button;
    }
}

// -----------------------------------------------------------------------------
// Name: TerrainPreferenceWidget constructor
// Created: LGY 2014-06-30
// -----------------------------------------------------------------------------
TerrainPreferenceWidget::TerrainPreferenceWidget( const QString& category, bool first, bool last, unsigned int index,
                                                  const std::vector< std::shared_ptr< TerrainPreference > >& terrains,
                                                  QSignalMapper* upSignalMapper, QSignalMapper* downSignalMapper )
{
    QVBoxLayout* mainLayout = new QVBoxLayout();
    QHBoxLayout* layout = new QHBoxLayout();
    QCheckBox* box = new QCheckBox( category );
    QFont font = box->font();
    font.setBold( true );
    box->setFont( font );
    box->blockSignals( true );
    box->setChecked( false );
    QPushButton* up = CreateButton( QIcon( "resources/images/gui/up.png" ), !first );
    upSignalMapper->setMapping( up, index );
    connect( up, SIGNAL( clicked() ), upSignalMapper, SLOT( map() ) );
    QPushButton* down = CreateButton( QIcon( "resources/images/gui/down.png" ), !last );
    downSignalMapper->setMapping( down, index );
    connect( down, SIGNAL( clicked() ), downSignalMapper, SLOT( map() ) );
    layout->addWidget( box, 0, Qt::AlignLeft );
    layout->addStretch( 1 );
    layout->addWidget( up, 0, Qt::AlignRight );
    layout->addWidget( down, 0, Qt::AlignRight );
    mainLayout->addLayout( layout );
    for( auto it = terrains.begin(); it != terrains.end(); ++it )
        if( auto& preference = *it )
            preference->Display( mainLayout, box );
    if( !box->isChecked() )
        for( auto it = terrains.begin(); it != terrains.end(); ++it )
            if( auto& preference = *it )
                preference->SetEnabled( false );
    box->blockSignals( false );
    setLayout( mainLayout );
}

// -----------------------------------------------------------------------------
// Name: TerrainPreferenceWidget destructor
// Created: LGY 2014-06-30
// -----------------------------------------------------------------------------
TerrainPreferenceWidget::~TerrainPreferenceWidget()
{
    // NOTHING
}
