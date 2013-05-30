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
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/OptionVariant.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: LimitsLayer constructor
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
LimitsLayer::LimitsLayer( Controllers& controllers, GlTools_ABC& tools, ColorStrategy_ABC& strategy, ParametersLayer& parameters,
                          TacticalLineFactory& factory, gui::View_ABC& view, const kernel::Profile_ABC& profile )
    : TacticalLinesLayer( controllers, tools, strategy, parameters, view, profile )
    , tools_         ( tools )
    , factory_       ( factory )
    , selectedEntity_( controllers )
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
    return selectedEntity_ != 0;
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
    if( ! drawLines_.IsSet( true, true, true ) )
        return false;
    return gui::EntityLayer< kernel::TacticalLine_ABC >::ShouldDisplay( entity );
}

// -----------------------------------------------------------------------------
// Name: LimitsLayer::CreateLimit
// Created: AGE 2006-11-21
// -----------------------------------------------------------------------------
void LimitsLayer::CreateLimit( const T_PointVector& points )
{
    if( selectedEntity_ )
        factory_.CreateLimit( points, *selectedEntity_ );
}

// -----------------------------------------------------------------------------
// Name: LimitsLayer::CreateLima
// Created: AGE 2006-11-21
// -----------------------------------------------------------------------------
void LimitsLayer::CreateLima( const T_PointVector& points )
{
    if( selectedEntity_ )
        factory_.CreateLima( points, *selectedEntity_ );
}

// -----------------------------------------------------------------------------
// Name: LimitsLayer::BeforeSelection
// Created: SBO 2009-08-06
// -----------------------------------------------------------------------------
void LimitsLayer::BeforeSelection()
{
    selectedEntity_ = 0;
}

// -----------------------------------------------------------------------------
// Name: LimitsLayer::Select
// Created: JSR 2013-05-23
// -----------------------------------------------------------------------------
void LimitsLayer::Select( const kernel::Agent_ABC& element )
{
    selectedEntity_ = &element;
}

// -----------------------------------------------------------------------------
// Name: LimitsLayer::Select
// Created: JSR 2012-05-31
// -----------------------------------------------------------------------------
void LimitsLayer::Select( const kernel::Automat_ABC& element )
{
    selectedEntity_ = &element;
}

// -----------------------------------------------------------------------------
// Name: LimitsLayer::Select
// Created: JSR 2012-05-31
// -----------------------------------------------------------------------------
void LimitsLayer::Select( const kernel::Formation_ABC& element )
{
    selectedEntity_ = &element;
}

// -----------------------------------------------------------------------------
// Name: LimitsLayer::AfterSelection
// Created: SBO 2009-08-06
// -----------------------------------------------------------------------------
void LimitsLayer::AfterSelection()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LimitsLayer::OptionChanged
// Created: LGY 2012-01-04
// -----------------------------------------------------------------------------
void LimitsLayer::OptionChanged( const std::string& name, const kernel::OptionVariant& value )
{
    if( name == "TacticalLines" )
        drawLines_ = value.To< FourStateOption >();
}
