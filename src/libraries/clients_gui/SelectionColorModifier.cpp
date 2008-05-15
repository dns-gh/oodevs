// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "SelectionColorModifier.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"

using namespace gui;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: SelectionColorModifier constructor
// Created: AGE 2008-05-14
// -----------------------------------------------------------------------------
SelectionColorModifier::SelectionColorModifier( kernel::Controllers& controllers, kernel::GlTools_ABC& tools )
    : controllers_   ( controllers )
    , tools_         ( tools )
    , selectedEntity_( controllers )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: SelectionColorModifier destructor
// Created: AGE 2008-05-14
// -----------------------------------------------------------------------------
SelectionColorModifier::~SelectionColorModifier()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: SelectionColorModifier::Apply
// Created: AGE 2008-05-14
// -----------------------------------------------------------------------------
QColor SelectionColorModifier::Apply( const kernel::Entity_ABC& entity, const QColor& base )
{
    bool selected         = selectedEntity_ == &entity;
    bool superiorSelected = selectedEntity_ 
        && entity.Retrieve< TacticalHierarchies >()
        && entity.Retrieve< TacticalHierarchies >()->IsSubordinateOf( *selectedEntity_ );
    tools_.Select( selected, superiorSelected );
    if( selected )
        return SelectedColor( base );
    if( superiorSelected )
        return SuperiorSelectedColor( base );
    return base;
}

// -----------------------------------------------------------------------------
// Name: SelectionColorModifier::Select
// Created: AGE 2008-05-14
// -----------------------------------------------------------------------------
void SelectionColorModifier::NotifySelected( const kernel::Entity_ABC* element )
{
    selectedEntity_ = element;
}

// -----------------------------------------------------------------------------
// Name: SelectionColorModifier::SelectedColor
// Created: AGE 2008-05-14
// -----------------------------------------------------------------------------
QColor SelectionColorModifier::SelectedColor( const QColor& base ) const
{
    return base.light( 150 );
}

// -----------------------------------------------------------------------------
// Name: SelectionColorModifier::SuperiorSelectedColor
// Created: AGE 2008-05-14
// -----------------------------------------------------------------------------
QColor SelectionColorModifier::SuperiorSelectedColor( const QColor& base ) const
{
    return base.light( 120 );
}
