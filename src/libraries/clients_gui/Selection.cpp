// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "Selection.h"
#include "clients_kernel/Entity_ABC.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: Selection constructor
// Created: LGY 2013-04-05
// -----------------------------------------------------------------------------
Selection::Selection( kernel::Controllers& controller )
    : controller_( controller )
    , selectedEntity_( controller )
{
    controller_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: Selection destructor
// Created: LGY 2013-04-05
// -----------------------------------------------------------------------------
Selection::~Selection()
{
    controller_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: Selection::NotifySelected
// Created: LGY 2013-04-05
// -----------------------------------------------------------------------------
void Selection::NotifySelected( const kernel::Entity_ABC* element )
{
    selectedEntity_ = element;
}

// -----------------------------------------------------------------------------
// Name: Selection::IsSelected
// Created: LGY 2013-04-05
// -----------------------------------------------------------------------------
bool Selection::IsSelected( const kernel::GraphicalEntity_ABC::T_GraphicalEntities& entities ) const
{
    if( !selectedEntity_ )
        return false;

    for( auto it = entities.begin(); it != entities.end(); ++it )
    {
        const kernel::Entity_ABC& entity = static_cast< const kernel::Entity_ABC& >( **it );
        if( entity.GetId() == selectedEntity_->GetId() )
            return true;
    }

    return false;
}
