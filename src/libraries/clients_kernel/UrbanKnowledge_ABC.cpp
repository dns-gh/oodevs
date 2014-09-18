// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "UrbanKnowledge_ABC.h"
#include "ActionController.h"

using namespace kernel;

const std::string UrbanKnowledge_ABC::typeName_ = "TerrainObjectKnowledge";

// -----------------------------------------------------------------------------
// Name: UrbanKnowledge_ABC constructor
// Created: MGD 2009-12-09
// -----------------------------------------------------------------------------
UrbanKnowledge_ABC::UrbanKnowledge_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledge_ABC destructor
// Created: MGD 2009-12-09
// -----------------------------------------------------------------------------
UrbanKnowledge_ABC::~UrbanKnowledge_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledge_ABC::GetTypeName
// Created: MGD 2009-12-09
// -----------------------------------------------------------------------------
const std::string& UrbanKnowledge_ABC::GetTypeName() const
{
    return typeName_;
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledge_ABC::Select
// Created: MGD 2009-12-09
// -----------------------------------------------------------------------------
void UrbanKnowledge_ABC::Select( ActionController& controller ) const
{
    controller.Select( *this, *(const Knowledge_ABC*)this, *(const Entity_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledge_ABC::MultipleSelect
// Created: JSR 2012-05-30
// -----------------------------------------------------------------------------
void UrbanKnowledge_ABC::MultipleSelect( ActionController& controller, const std::vector< const kernel::GraphicalEntity_ABC* >& elements ) const
{
    std::vector< const UrbanKnowledge_ABC* > first;
    std::vector< const Knowledge_ABC* > second;
    std::vector< const Entity_ABC* > third;
    for( std::size_t i = 0; i < elements.size(); ++i )
    {
        first.push_back( static_cast< const UrbanKnowledge_ABC* >( elements[ i ] ) );
        second.push_back( static_cast< const Knowledge_ABC* >( elements[ i ] ) );
        third.push_back( static_cast< const Entity_ABC* >( elements[ i ] ) );
    }
    controller.MultipleSelect( first, second, third );
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledge_ABC::ContextMenu
// Created: MGD 2009-12-09
// -----------------------------------------------------------------------------
void UrbanKnowledge_ABC::ContextMenu( ActionController& controller, const QPoint& where, QObject* emitter ) const
{
    controller.ContextMenu( emitter, *this, *(const Knowledge_ABC*)this, *(const Entity_ABC*)this, where );
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledge_ABC::Activate
// Created: MGD 2009-12-09
// -----------------------------------------------------------------------------
void UrbanKnowledge_ABC::Activate( ActionController& controller ) const
{
    controller.Activate( *this, *(const Knowledge_ABC*)this );
}
