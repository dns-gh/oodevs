// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "UrbanHierarchies.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: UrbanHierarchies constructor
// Created: JSR 2012-05-14
// -----------------------------------------------------------------------------
UrbanHierarchies::UrbanHierarchies( Controller& controller, Entity_ABC& entity, Entity_ABC* superior )
    : EntityHierarchies< Hierarchies >( controller, entity, superior )
{
    if( superior )
    {
        const kernel::Hierarchies* superiorHierarchies = RetrieveHierarchies( *superior );
        level_ = ( superiorHierarchies && superiorHierarchies->GetSuperior() ) ? eUrbanLevelBlock : eUrbanLevelDistrict;
    }
    else
        level_ = eUrbanLevelCity;
}

// -----------------------------------------------------------------------------
// Name: UrbanHierarchies destructor
// Created: JSR 2012-05-14
// -----------------------------------------------------------------------------
UrbanHierarchies::~UrbanHierarchies()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanHierarchies::GetLevel
// Created: JSR 2012-05-15
// -----------------------------------------------------------------------------
EUrbanLevel UrbanHierarchies::GetLevel() const
{
    return level_;
}

// -----------------------------------------------------------------------------
// Name: UrbanHierarchies::ChangeSuperior
// Created: JSR 2012-06-08
// -----------------------------------------------------------------------------
void UrbanHierarchies::ChangeSuperior( kernel::Entity_ABC& superior )
{
    kernel::EntityHierarchies< kernel::Hierarchies >::ChangeSuperior( &superior );
}

// -----------------------------------------------------------------------------
// Name: UrbanHierarchies::SerializeAttributes
// Created: ABR 2012-05-24
// -----------------------------------------------------------------------------
void UrbanHierarchies::SerializeAttributes( xml::xostream& xos ) const
{
    if( level_ != eUrbanLevelBlock && Count() )
    {
        xos << xml::start( "urban-objects" );
        tools::Iterator< const Entity_ABC& > it = CreateSubordinateIterator();
        while( it.HasMoreElements() )
        {
            const Entity_ABC& entity = it.NextElement();
            xos << xml::start( "urban-object" );
            entity.GetInterfaces().Apply( &kernel::Serializable_ABC::SerializeAttributes, xos );
            xos << xml::end; // urban-object
        }
        xos << xml::end; // urban-objects
    }
}
