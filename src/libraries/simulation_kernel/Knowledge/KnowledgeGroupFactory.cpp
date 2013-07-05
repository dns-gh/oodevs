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
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Entities/MIL_Army_ABC.h"

BOOST_CLASS_EXPORT_IMPLEMENT( KnowledgeGroupFactory )

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
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupFactory::Create
// Created: MGD 2009-10-22
// -----------------------------------------------------------------------------
boost::shared_ptr< MIL_KnowledgeGroup > KnowledgeGroupFactory::Create( xml::xistream& xis, MIL_Army_ABC& army, MIL_KnowledgeGroup* parent )
{
    boost::shared_ptr< MIL_KnowledgeGroup > group( new MIL_KnowledgeGroup( xis, army, parent ) );
    xis >> xml::list( "knowledge-group", boost::bind( &KnowledgeGroupFactory::Create, this, _1, boost::ref( army ), group.get() ) );
    if( parent )
        parent->RegisterKnowledgeGroup( group );
    else
        army.RegisterKnowledgeGroup( group );
    return group;
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
