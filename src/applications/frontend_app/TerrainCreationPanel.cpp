// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_app_pch.h"
#include "TerrainCreationPanel.h"
#include <qaction.h>

// -----------------------------------------------------------------------------
// Name: TerrainCreationPanel constructor
// Created: SBO 2007-10-04
// -----------------------------------------------------------------------------
TerrainCreationPanel::TerrainCreationPanel( QWidgetStack* widget, QAction& action )
    : QHBox( widget )
    , stack_( widget )
{
    setMargin( 10 );
    QGroupBox* box = new QGroupBox( 2, Qt::Horizontal, action.text(), this );

    connect( &action, SIGNAL( toggled( bool ) ), SLOT( setShown( bool ) ) );
}

// -----------------------------------------------------------------------------
// Name: TerrainCreationPanel destructor
// Created: SBO 2007-10-04
// -----------------------------------------------------------------------------
TerrainCreationPanel::~TerrainCreationPanel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TerrainCreationPanel::showEvent
// Created: SBO 2007-10-04
// -----------------------------------------------------------------------------
void TerrainCreationPanel::showEvent( QShowEvent* event )
{
    stack_->raiseWidget( this );
    QHBox::showEvent( event );
}
