// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "LimitsLayer.h"
#include "ModelBuilder.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: LimitsLayer constructor
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
LimitsLayer::LimitsLayer( Controllers& controllers, const GlTools_ABC& tools, ColorStrategy_ABC& strategy, ParametersLayer& parameters, ModelBuilder& modelBuilder, gui::View_ABC& view, const kernel::Profile_ABC& profile )
    : TacticalLinesLayer( controllers, tools, strategy, parameters, view, profile )
    , modelBuilder_( modelBuilder )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LimitsLayer destructor
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
LimitsLayer::~LimitsLayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LimitsLayer::CanCreateLine
// Created: AGE 2006-11-21
// -----------------------------------------------------------------------------
bool LimitsLayer::CanCreateLine()
{
    return modelBuilder_.CanCreateLine();
}

// -----------------------------------------------------------------------------
// Name: LimitsLayer::Delete
// Created: AGE 2006-11-21
// -----------------------------------------------------------------------------
void LimitsLayer::Delete( const kernel::TacticalLine_ABC& line )
{
    modelBuilder_.DeleteEntity( line );
}

// -----------------------------------------------------------------------------
// Name: LimitsLayer::ShouldDisplay
// Created: AGE 2006-11-21
// -----------------------------------------------------------------------------
bool LimitsLayer::ShouldDisplay( const kernel::Entity_ABC& )
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: LimitsLayer::CreateLimit
// Created: AGE 2006-11-21
// -----------------------------------------------------------------------------
void LimitsLayer::CreateLimit( const T_PointVector& points )
{
    modelBuilder_.CreateLimit( points );
}

// -----------------------------------------------------------------------------
// Name: LimitsLayer::CreateLima
// Created: AGE 2006-11-21
// -----------------------------------------------------------------------------
void LimitsLayer::CreateLima( const T_PointVector& points )
{
    modelBuilder_.CreateLima( points );
}
