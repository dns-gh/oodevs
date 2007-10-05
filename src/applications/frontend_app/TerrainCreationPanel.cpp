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
    : Panel_ABC( widget, action )
{
    QHBox* box = new QHBox( this );
    box->setMargin( 10 );
    QGroupBox* group = new QGroupBox( 2, Qt::Horizontal, action.text(), box );
    new QLabel( "--> Click here to add text <--", group );
}

// -----------------------------------------------------------------------------
// Name: TerrainCreationPanel destructor
// Created: SBO 2007-10-04
// -----------------------------------------------------------------------------
TerrainCreationPanel::~TerrainCreationPanel()
{
    // NOTHING
}
