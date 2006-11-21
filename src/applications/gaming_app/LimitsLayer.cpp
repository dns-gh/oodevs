// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "LimitsLayer.h"

#include "gaming/TacticalLine_ABC.h"
#include "gaming/TacticalLineFactory.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Profile_ABC.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: LimitsLayer constructor
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
LimitsLayer::LimitsLayer( Controllers& controllers, const GlTools_ABC& tools, ColorStrategy_ABC& strategy, ParametersLayer& parameters, TacticalLineFactory& factory, gui::View_ABC& view, const kernel::Profile_ABC& profile )
    : TacticalLinesLayer( controllers, tools, strategy, parameters, view, profile )
    , tools_            ( tools )
    , factory_          ( factory )
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
    return factory_.IsReady();
}

// -----------------------------------------------------------------------------
// Name: LimitsLayer::Delete
// Created: AGE 2006-11-21
// -----------------------------------------------------------------------------
void LimitsLayer::Delete( const kernel::TacticalLine_ABC& l )
{
    const kernel::Entity_ABC* superior = l.Get< kernel::TacticalHierarchies >().GetSuperior();
    if( superior && profile_.CanBeOrdered( *superior ) )
    {
        kernel::TacticalLine_ABC& line = const_cast< kernel::TacticalLine_ABC& >( l );
        static_cast< ::TacticalLine_ABC& >( line ).Delete();// $$$$ SBO 2006-11-07: 
    }
}

// -----------------------------------------------------------------------------
// Name: LimitsLayer::ShouldDisplay
// Created: SBO 2006-11-06
// -----------------------------------------------------------------------------
bool LimitsLayer::ShouldDisplay( const kernel::Entity_ABC& entity )
{
    if( ! TacticalLinesLayer::ShouldDisplay( entity ) )
        return false;
    if( const kernel::Entity_ABC* superior = entity.Get< kernel::TacticalHierarchies >().GetSuperior() )
        return gui::EntityLayer< kernel::TacticalLine_ABC >::ShouldDisplay( *superior );
    return false;
}

// -----------------------------------------------------------------------------
// Name: LimitsLayer::CreateLimit
// Created: AGE 2006-11-21
// -----------------------------------------------------------------------------
void LimitsLayer::CreateLimit( const T_PointVector& points )
{
    factory_.CreateLimit( points );
}

// -----------------------------------------------------------------------------
// Name: LimitsLayer::CreateLima
// Created: AGE 2006-11-21
// -----------------------------------------------------------------------------
void LimitsLayer::CreateLima( const T_PointVector& points )
{
    factory_.CreateLima( points );
}
