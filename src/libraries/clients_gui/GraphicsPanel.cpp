// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "GraphicsPanel.h"
#include "GraphicPreferences.h"
#include "SubObjectName.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: GraphicsPanel constructor
// Created: SBO 2006-04-04
// -----------------------------------------------------------------------------
GraphicsPanel::GraphicsPanel( QWidget* parent, GraphicPreferences& preferences )
    : PreferencePanel_ABC( parent, "GraphicsPanel" )
    , preferences_( preferences )
{
    SubObjectName subObject( "GraphicsPanel" );
    Q3VBox* container = new Q3VBox( this );
    preferences_.Display( container );
    setWidget( container );
}

// -----------------------------------------------------------------------------
// Name: GraphicsPanel destructor
// Created: SBO 2006-04-04
// -----------------------------------------------------------------------------
GraphicsPanel::~GraphicsPanel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GraphicsPanel::Commit
// Created: SBO 2007-01-03
// -----------------------------------------------------------------------------
void GraphicsPanel::Commit()
{
    preferences_.Commit();
}

// -----------------------------------------------------------------------------
// Name: GraphicsPanel::Reset
// Created: SBO 2007-01-03
// -----------------------------------------------------------------------------
void GraphicsPanel::Reset()
{
    preferences_.Revert();
}
