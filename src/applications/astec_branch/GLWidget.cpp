// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "GlWidget.h"
#include "TerrainLayer.h"
#include "ElevationLayer.h"
#include "AgentsLayer.h"

// -----------------------------------------------------------------------------
// Name: GlWidget::GlWidget
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
GlWidget::GlWidget( QWidget* pParent, const std::string& scipioXml, Controller& controller, ActionController& actions, const CoordinateConverter& converter )
    : WorldParameters( scipioXml )
    , MapWidget( pParent, width_, height_ )
{
    Register( *new ElevationLayer( detection_ ) );
    Register( *new TerrainLayer( graphicsDirectory_ ) );
    Register( *new AgentsLayer( controller, actions, converter ) );
}

// -----------------------------------------------------------------------------
// Name: GlWidget::~GlWidget
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
GlWidget::~GlWidget()
{
    // NOTHING
}
