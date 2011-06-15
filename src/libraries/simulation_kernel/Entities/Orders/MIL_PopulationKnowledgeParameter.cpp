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
#include "Knowledge/DEC_KnowledgeResolver_ABC.h"
#include "Knowledge/DEC_Knowledge_Population.h"
#include "protocol/Protocol.h"
#include "MIL.h"

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_PopulationKnowledgeParameter )

// -----------------------------------------------------------------------------
// Name: MIL_PopulationKnowledgeParameter constructor
// Created: LGY 2011-06-06
// -----------------------------------------------------------------------------
MIL_PopulationKnowledgeParameter::MIL_PopulationKnowledgeParameter()
{
    // NOTHING
}

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
MIL_PopulationKnowledgeParameter::MIL_PopulationKnowledgeParameter( const sword::CrowdKnowledgeId& asn, const DEC_KnowledgeResolver_ABC& resolver )
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
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
bool MIL_PopulationKnowledgeParameter::IsOfType( MIL_ParameterType_ABC::E_Type type ) const
{
    return type == MIL_ParameterType_ABC::eCrowdKnowledge
        || type == MIL_ParameterType_ABC::eLocationComposite;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationKnowledgeParameter::ToPopulationKnowledge
// Created: LDC 2009-06-04
// -----------------------------------------------------------------------------
bool MIL_PopulationKnowledgeParameter::ToPopulationKnowledge( DEC_Knowledge_Population*& value ) const
{
    value = pKnowledgePopulation_;
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationKnowledgeParameter::ToElement
// Created: MGD 2010-11-19
// -----------------------------------------------------------------------------
bool MIL_PopulationKnowledgeParameter::ToElement( sword::MissionParameter_Value& elem ) const
{
    elem.mutable_crowdknowledge()->set_id( pKnowledgePopulation_->GetID() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationKnowledgeParameter::load
// Created: LGY 2011-06-15
// -----------------------------------------------------------------------------
void MIL_PopulationKnowledgeParameter::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< MIL_BaseParameter >( *this )
         >> pKnowledgePopulation_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationKnowledgeParameter::save
// Created: LGY 2011-06-15
// -----------------------------------------------------------------------------
void MIL_PopulationKnowledgeParameter::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< MIL_BaseParameter >( *this )
         << pKnowledgePopulation_;
}
