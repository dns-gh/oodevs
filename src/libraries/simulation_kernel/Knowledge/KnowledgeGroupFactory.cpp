// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "KnowledgeGroupFactory.h"
#include "MIL_KnowledgeGroup.h"
#include "simulation_kernel/AutomateFactory_ABC.h"

#include <xeumeuleu/xml.h>

BOOST_CLASS_EXPORT_GUID( KnowledgeGroupFactory, "KnowledgeGroupFactory" )

template< typename Archive >
void save_construct_data( Archive& archive, const KnowledgeGroupFactory* factory, const unsigned int /*version*/ )
{
    //TODO
}
template< typename Archive >
void load_construct_data( Archive& archive, KnowledgeGroupFactory* factory, const unsigned int /*version*/ )
{
    //TODO
}

// -----------------------------------------------------------------------------
// Name: FormationFactory constructor
// Created: MGD 2009-10-22
// -----------------------------------------------------------------------------
KnowledgeGroupFactory::KnowledgeGroupFactory()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: FormationFactory destructor
// Created: MGD 2009-10-22
// -----------------------------------------------------------------------------
KnowledgeGroupFactory::~KnowledgeGroupFactory()
{

}

// -----------------------------------------------------------------------------
// Name: FormationFactory::Create
// Created: MGD 2009-10-22
// -----------------------------------------------------------------------------
MIL_KnowledgeGroup& KnowledgeGroupFactory::Create( xml::xistream& xis, MIL_Army_ABC& army, MIL_KnowledgeGroup* parent )
{
    MIL_KnowledgeGroup& knowledgeGroup = *new MIL_KnowledgeGroup( xis, army, parent, *this );
    Register( knowledgeGroup.GetID(), knowledgeGroup );
    return knowledgeGroup;
}