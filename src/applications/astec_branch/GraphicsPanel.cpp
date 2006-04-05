// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "GraphicsPanel.h"
#include "GraphicPreferences.h"

// -----------------------------------------------------------------------------
// Name: GraphicsPanel constructor
// Created: SBO 2006-04-04
// -----------------------------------------------------------------------------
GraphicsPanel::GraphicsPanel( QWidget* parent )
    : QWidget     ( parent )
    , preferences_( *new GraphicPreferences() )
{
    QVBoxLayout* pLayout = new QVBoxLayout( this );
    pLayout->setMargin( 5 );
    pLayout->setSpacing( 5 );

    QTabWidget* pTabWidget = new QTabWidget( this );
    pLayout->addWidget( pTabWidget );

    preferences_.AddPanels( pTabWidget );
}
    
// -----------------------------------------------------------------------------
// Name: GraphicsPanel destructor
// Created: SBO 2006-04-04
// -----------------------------------------------------------------------------
GraphicsPanel::~GraphicsPanel()
{
    delete &preferences_;
}
