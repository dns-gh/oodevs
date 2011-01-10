// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_UrbanBlockParameter.h"
#include "protocol/Protocol.h"
#include "simulation_kernel/knowledge/DEC_KnowledgeResolver_ABC.h"
#include "simulation_kernel/knowledge/DEC_Knowledge_Urban.h"

// -----------------------------------------------------------------------------
// Name: MIL_UrbanBlockParameter constructor
// Created: MGD 2009-11-02
// -----------------------------------------------------------------------------
MIL_UrbanBlockParameter::MIL_UrbanBlockParameter( const sword::UrbanObjectKnowledgeId& asn, const DEC_KnowledgeResolver_ABC& resolver )
    : pKnowledgeUrbanBlock_( resolver.ResolveKnowledgeUrban( asn.id() ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanBlockParameter constructor
// Created: MGD 2010-01-15
// -----------------------------------------------------------------------------
MIL_UrbanBlockParameter::MIL_UrbanBlockParameter( boost::shared_ptr< DEC_Knowledge_Urban > urbanBlock )
    : pKnowledgeUrbanBlock_( urbanBlock )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanBlockParameter destructor
// Created: MGD 2009-11-02
// -----------------------------------------------------------------------------
MIL_UrbanBlockParameter::~MIL_UrbanBlockParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanBlockParameter::IsOfType
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
bool MIL_UrbanBlockParameter::IsOfType( MIL_ParameterType_ABC::E_Type type ) const
{
    return type == MIL_ParameterType_ABC::eUrbanKnowledge
        || type == MIL_ParameterType_ABC::eLocationComposite;
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanBlockParameter::ToUrbanBlock
// Created: MGD 2009-11-02
// -----------------------------------------------------------------------------
bool MIL_UrbanBlockParameter::ToUrbanBlock( boost::shared_ptr< DEC_Knowledge_Urban >& value ) const
{
    value = pKnowledgeUrbanBlock_;
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanBlockParameter::ToElement
// Created: MGD 2010-11-12
// -----------------------------------------------------------------------------
bool MIL_UrbanBlockParameter::ToElement( sword::MissionParameter_Value& elem ) const
{
    elem.mutable_urbanknowledge()->set_id( pKnowledgeUrbanBlock_->GetID() );
    return true;
}
