// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
// LTO
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "KnowledgeGroupFactory.h"

BOOST_CLASS_EXPORT_IMPLEMENT( KnowledgeGroupFactory )

template< typename Archive >
void save_construct_data( Archive& /*archive*/, const KnowledgeGroupFactory* /*factory*/, const unsigned int /*version*/ )
{
    // NOTHING
}

template< typename Archive >
void load_construct_data( Archive& /*archive*/, KnowledgeGroupFactory* factory, const unsigned int /*version*/ )
{
    ::new( factory )KnowledgeGroupFactory();
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupFactory constructor
// Created: MGD 2009-10-22
// -----------------------------------------------------------------------------
KnowledgeGroupFactory::KnowledgeGroupFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupFactory destructor
// Created: MGD 2009-10-22
// -----------------------------------------------------------------------------
KnowledgeGroupFactory::~KnowledgeGroupFactory()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupFactory::Create
// Created: MGD 2009-10-22
// -----------------------------------------------------------------------------
MIL_KnowledgeGroup& KnowledgeGroupFactory::Create( xml::xistream& xis, MIL_Army_ABC& army, MIL_KnowledgeGroup* parent )
{
    MIL_KnowledgeGroup& knowledgeGroup = *new MIL_KnowledgeGroup( xis, army, parent, *this );
    Register( knowledgeGroup.GetId(), knowledgeGroup );
    return knowledgeGroup;
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupFactory::load
// Created: SLG 2010-02-10
// -----------------------------------------------------------------------------
void KnowledgeGroupFactory::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object < KnowledgeGroupFactory_ABC >( *this );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupFactory::save
// Created: SLG 2010-02-10
// -----------------------------------------------------------------------------
void KnowledgeGroupFactory::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object < KnowledgeGroupFactory_ABC >( *this );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupFactory_ABC::load
// Created: LDC 2010-03-04
// -----------------------------------------------------------------------------
void KnowledgeGroupFactory_ABC::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    std::size_t nNbr;
    file >> nNbr;
    while ( nNbr-- )
    {
        unsigned long index;
        file >> index;
        file >> elements_[ index ];
    }
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupFactory_ABC::save
// Created: LDC 2010-03-04
// -----------------------------------------------------------------------------
void KnowledgeGroupFactory_ABC::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    std::size_t size = elements_.size();
    file << size;
    for ( std::map< unsigned long, MIL_KnowledgeGroup* >::const_iterator it = elements_.begin(); it != elements_.end(); ++it )
    {
        file << it->first
             << it->second;
    }
}
