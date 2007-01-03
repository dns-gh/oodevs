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

using namespace gui;

// -----------------------------------------------------------------------------
// Name: GraphicsPanel constructor
// Created: SBO 2006-04-04
// -----------------------------------------------------------------------------
GraphicsPanel::GraphicsPanel( QWidget* parent )
    : PreferencePanel_ABC( parent )
    , preferences_( *new GraphicPreferences() )
    , parent_( parent )
{
    preferences_.Display( this );
}
    
// -----------------------------------------------------------------------------
// Name: GraphicsPanel destructor
// Created: SBO 2006-04-04
// -----------------------------------------------------------------------------
GraphicsPanel::~GraphicsPanel()
{
    delete &preferences_;
}

// -----------------------------------------------------------------------------
// Name: GraphicsPanel::GetPreferences
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
GraphicPreferences& GraphicsPanel::GetPreferences()
{
    return preferences_;
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
