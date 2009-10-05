// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_ObjectKnowledgeParameter.h"

#include "simulation_orders/MIL_ParameterType_ObjectKnowledge.h"
#include "Knowledge/DEC_KnowledgeResolver_ABC.h"
#include "Knowledge/DEC_Knowledge_Object.h"

// -----------------------------------------------------------------------------
// Name: MIL_ObjectKnowledgeParameter constructor
// Created: LDC 2009-06-16
// -----------------------------------------------------------------------------
MIL_ObjectKnowledgeParameter::MIL_ObjectKnowledgeParameter( DEC_Knowledge_Object* pObjectKnowledge )
: pKnowledgeObject_( pObjectKnowledge )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectKnowledgeParameter constructor
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
MIL_ObjectKnowledgeParameter::MIL_ObjectKnowledgeParameter( const ASN1T_ObjectKnowledge& asn, const DEC_KnowledgeResolver_ABC& resolver )
: pKnowledgeObject_( resolver.ResolveKnowledgeObject( asn ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectKnowledgeParameter destructor
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
MIL_ObjectKnowledgeParameter::~MIL_ObjectKnowledgeParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectKnowledgeParameter::IsOfType
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
bool MIL_ObjectKnowledgeParameter::IsOfType( const MIL_ParameterType_ABC& type ) const
{
    return( dynamic_cast<const MIL_ParameterType_ObjectKnowledge*>( &type ) != 0 );
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectKnowledgeParameter::ToObjectKnowledge
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
bool MIL_ObjectKnowledgeParameter::ToObjectKnowledge( ASN1T_ObjectKnowledge& asn ) const
{
    asn = pKnowledgeObject_->GetID();
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectKnowledgeParameter::ToObjectKnowledge
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
bool MIL_ObjectKnowledgeParameter::ToObjectKnowledge( DEC_Knowledge_Object*& value ) const
{
    value = pKnowledgeObject_;
    return true;
}
