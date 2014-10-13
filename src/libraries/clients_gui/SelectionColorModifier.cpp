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

#include "GLOptions.h"
#include "GLView_ABC.h"

#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/UrbanObject_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: SelectionColorModifier constructor
// Created: AGE 2008-05-14
// -----------------------------------------------------------------------------
SelectionColorModifier::SelectionColorModifier( kernel::Controllers& controllers, gui::GLView_ABC& tools, const kernel::Profile_ABC& profile )
    : controllers_( controllers )
    , tools_( tools )
    , profile_( profile )
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

namespace
{
    template< typename H >
    bool IsSubordinate( const kernel::Entity_ABC& entity, const kernel::Entity_ABC& candidate )
    {
        if( const H* hierarchy = entity.Retrieve< H >() )
            return hierarchy->IsSubordinateOf( candidate );
        return false;
    }
}

// -----------------------------------------------------------------------------
// Name: SelectionColorModifier::Apply
// Created: AGE 2008-05-14
// -----------------------------------------------------------------------------
QColor SelectionColorModifier::Apply( const kernel::Entity_ABC& entity, const QColor& base ) const
{
    const bool selected = selectedEntity_ == &entity;
    const bool superiorSelected = selectedEntity_ && ( IsSubordinate< kernel::TacticalHierarchies >( entity, *selectedEntity_ )
                                                    || IsSubordinate< kernel::CommunicationHierarchies >( entity, *selectedEntity_ ) );
    tools_.GetOptions().Select( selected, superiorSelected, profile_.CanBeOrdered( entity ) );
    if( selected )
        return SelectedColor( base );
    if( superiorSelected )
        return SuperiorSelectedColor( base );
    return base;
}

// -----------------------------------------------------------------------------
// Name: SelectionColorModifier::Apply
// Created: LGY 2013-03-06
// -----------------------------------------------------------------------------
float SelectionColorModifier::Apply( const kernel::Entity_ABC& entity, float alpha ) const
{
    if( entity.GetTypeName() == kernel::UrbanObject_ABC::typeName_ )
        if( selectedEntity_ == &entity ||
            std::find( selectedElements_.begin(), selectedElements_.end(), &entity ) != selectedElements_.end() )
            return alpha * 0.6f;
    return alpha;
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
QColor SelectionColorModifier::SelectedColor( const QColor& base )
{
    return base.light( 150 );
}

// -----------------------------------------------------------------------------
// Name: SelectionColorModifier::SuperiorSelectedColor
// Created: AGE 2008-05-14
// -----------------------------------------------------------------------------
QColor SelectionColorModifier::SuperiorSelectedColor( const QColor& base )
{
    return base.light( 120 );
}

// -----------------------------------------------------------------------------
// Name: SelectionColorModifier::MultipleSelect
// Created: LGY 2013-03-06
// -----------------------------------------------------------------------------
void SelectionColorModifier::NotifySelectionChanged( const T_Elements& elements )
{
    selectedElements_ = elements;
}
