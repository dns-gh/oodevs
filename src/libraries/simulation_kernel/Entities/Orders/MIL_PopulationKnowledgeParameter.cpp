// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_PopulationKnowledgeParameter.h"
#include "simulation_orders/MIL_ParameterType_PopulationKnowledge.h"
#include "Knowledge/DEC_KnowledgeResolver_ABC.h"
#include "Knowledge/DEC_Knowledge_Population.h"

// -----------------------------------------------------------------------------
// Name: MIL_PopulationKnowledgeParameter constructor
// Created: LDC 2009-06-16
// -----------------------------------------------------------------------------
MIL_PopulationKnowledgeParameter::MIL_PopulationKnowledgeParameter( DEC_Knowledge_Population* pKnowledge )
: pKnowledgePopulation_( pKnowledge )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationKnowledgeParameter constructor
// Created: LDC 2009-06-04
// -----------------------------------------------------------------------------
MIL_PopulationKnowledgeParameter::MIL_PopulationKnowledgeParameter( const ASN1T_PopulationKnowledge& asn, const DEC_KnowledgeResolver_ABC& resolver )
: pKnowledgePopulation_( resolver.ResolveKnowledgePopulation( asn ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationKnowledgeParameter destructor
// Created: LDC 2009-06-04
// -----------------------------------------------------------------------------
MIL_PopulationKnowledgeParameter::~MIL_PopulationKnowledgeParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationKnowledgeParameter::IsOfType
// Created: LDC 2009-06-04
// -----------------------------------------------------------------------------
bool MIL_PopulationKnowledgeParameter::IsOfType( const MIL_ParameterType_ABC& type ) const
{
    return( dynamic_cast<const MIL_ParameterType_PopulationKnowledge*>( &type ) != 0 );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationKnowledgeParameter::ToPopulationKnowledge
// Created: LDC 2009-06-04
// -----------------------------------------------------------------------------
bool MIL_PopulationKnowledgeParameter::ToPopulationKnowledge( ASN1T_PopulationKnowledge& asn ) const
{
    asn = pKnowledgePopulation_->GetID();
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationKnowledgeParameter::ToPopulationKnowledge
// Created: LDC 2009-06-04
// -----------------------------------------------------------------------------
bool MIL_PopulationKnowledgeParameter::ToPopulationKnowledge( DEC_Knowledge_Population* value ) const
{
    value = pKnowledgePopulation_;
    return true;
}
