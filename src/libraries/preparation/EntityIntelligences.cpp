// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "EntityIntelligences.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Intelligence_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/HierarchyLevel_ABC.h"
#include "xeumeuleu/xml.h"

using namespace xml;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: EntityIntelligences constructor
// Created: SBO 2007-10-18
// -----------------------------------------------------------------------------
EntityIntelligences::EntityIntelligences( Controller& controller, Entity_ABC& holder, Entity_ABC* superior )
    : EntityHierarchies< IntelligenceHierarchies >( controller, holder, superior )
    , controller_( controller )
    , holder_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EntityIntelligences constructor
// Created: SBO 2007-10-18
// -----------------------------------------------------------------------------
EntityIntelligences::EntityIntelligences( Controller& controller, Intelligence_ABC& holder, Entity_ABC* superior )
    : EntityHierarchies< IntelligenceHierarchies >( controller, holder, superior )
    , controller_( controller )
    , holder_( &holder )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EntityIntelligences destructor
// Created: SBO 2007-10-18
// -----------------------------------------------------------------------------
EntityIntelligences::~EntityIntelligences()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EntityIntelligences::ChangeSuperior
// Created: SBO 2007-10-18
// -----------------------------------------------------------------------------
void EntityIntelligences::ChangeSuperior( Entity_ABC& superior )
{
    EntityHierarchies< IntelligenceHierarchies >::ChangeSuperior( &superior );
}

// -----------------------------------------------------------------------------
// Name: EntityIntelligences::SerializeIntelligences
// Created: SBO 2007-10-15
// -----------------------------------------------------------------------------
void EntityIntelligences::SerializeIntelligences( xml::xostream& xos ) const
{
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
    {
        xos << start( "intelligence" );
        it->second->Interface().Apply( &Serializable_ABC::SerializeIntelligences, xos );
        xos << end();
    }
}

// -----------------------------------------------------------------------------
// Name: EntityIntelligences::GetSymbol
// Created: SBO 2007-10-18
// -----------------------------------------------------------------------------
std::string EntityIntelligences::GetSymbol() const
{
    if( holder_ )
        return holder_->GetSymbol();
    return GetEntity().Get< kernel::TacticalHierarchies >().GetSymbol();
}

// -----------------------------------------------------------------------------
// Name: EntityIntelligences::GetLevel
// Created: SBO 2007-10-18
// -----------------------------------------------------------------------------
std::string EntityIntelligences::GetLevel() const
{
    if( holder_ )
        return holder_->GetLevel().GetSymbol();
    return GetEntity().Get< kernel::TacticalHierarchies >().GetLevel();
}

// -----------------------------------------------------------------------------
// Name: EntityIntelligences::UpdateSymbol
// Created: SBO 2007-11-02
// -----------------------------------------------------------------------------
void EntityIntelligences::UpdateSymbol( bool up /*= true*/ )
{
    controller_.Update( *this );
    controller_.Update( *(Symbol_ABC*)this );
    if( ! up )
    {
        kernel::Iterator< const kernel::Entity_ABC& > it = CreateSubordinateIterator();
        while( it.HasMoreElements() )
        {
            const kernel::IntelligenceHierarchies* child = it.NextElement().Retrieve< kernel::IntelligenceHierarchies >();
            if( child )
                const_cast< kernel::IntelligenceHierarchies* >( child )->UpdateSymbol( false );
        }
    }
}
