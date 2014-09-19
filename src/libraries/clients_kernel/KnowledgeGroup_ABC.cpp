// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "KnowledgeGroup_ABC.h"
#include "ActionController.h"

using namespace kernel;

const std::string KnowledgeGroup_ABC::typeName_ = "knowledgeGroup";

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup_ABC constructor
// Created: SBO 2006-08-09
// -----------------------------------------------------------------------------
KnowledgeGroup_ABC::KnowledgeGroup_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup_ABC destructor
// Created: SBO 2006-08-09
// -----------------------------------------------------------------------------
KnowledgeGroup_ABC::~KnowledgeGroup_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup_ABC::GetTypeName
// Created: AGE 2008-07-16
// -----------------------------------------------------------------------------
const std::string& KnowledgeGroup_ABC::GetTypeName() const
{
    return typeName_;
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup_ABC::Select
// Created: AGE 2006-09-19
// -----------------------------------------------------------------------------
void KnowledgeGroup_ABC::Select( ActionController& controller ) const
{
    controller.Select( *this, *(const Entity_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup_ABC::MultipleSelect
// Created: JSR 2012-05-30
// -----------------------------------------------------------------------------
void KnowledgeGroup_ABC::MultipleSelect( ActionController& controller, const std::vector< const kernel::GraphicalEntity_ABC* >& elements ) const
{
    std::vector< const KnowledgeGroup_ABC* > first;
    std::vector< const Entity_ABC* > second;
    for( std::size_t i = 0; i < elements.size(); ++i )
    {
        first.push_back( static_cast< const KnowledgeGroup_ABC* >( elements[ i ] ) );
        second.push_back( static_cast< const Entity_ABC* >( elements[ i ] ) );
    }
    controller.MultipleSelect( first, second );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup_ABC::ContextMenu
// Created: AGE 2006-09-19
// -----------------------------------------------------------------------------
void KnowledgeGroup_ABC::ContextMenu( ActionController& controller, const QPoint& where, QObject* emitter ) const
{
    controller.ContextMenu( emitter, *this, *(const Entity_ABC*)this, where );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup_ABC::Activate
// Created: AGE 2006-09-19
// -----------------------------------------------------------------------------
void KnowledgeGroup_ABC::Activate( ActionController& controller ) const
{
    controller.Activate( *this, *(const Entity_ABC*)this );
}
