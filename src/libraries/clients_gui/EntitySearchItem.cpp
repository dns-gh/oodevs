// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "EntitySearchItem.h"
#include "clients_kernel/Entity_ABC.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: EntitySearchItem constructor
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
EntitySearchItem::EntitySearchItem( ActionController& actions, const Entity_ABC& entity )
    : actions_( actions )
    , entity_ ( entity )
{
    name_ = ( entity_.GetName() + QString::number( entity_.GetId() ) ).lower();
}

// -----------------------------------------------------------------------------
// Name: EntitySearchItem destructor
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
EntitySearchItem::~EntitySearchItem()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EntitySearchItem::Matches
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
bool EntitySearchItem::Matches( const QString& input ) const
{
    return name_.find( input ) != -1;
}

// -----------------------------------------------------------------------------
// Name: EntitySearchItem::Activate
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
void EntitySearchItem::Activate()
{
    entity_.Select( actions_ );
    entity_.Activate( actions_ );
}

// -----------------------------------------------------------------------------
// Name: EntitySearchItem::Matches
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
bool EntitySearchItem::Matches( const Entity_ABC& entity ) const
{
    return &entity == &entity_;
}
