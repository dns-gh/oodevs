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
#include "Knowledge/DEC_KnowledgeResolver_ABC.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "protocol/Protocol.h"
#include "Checkpoints/SerializationTools.h"

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_ObjectKnowledgeParameter )

// -----------------------------------------------------------------------------
// Name: MIL_ObjectKnowledgeParameter constructor
// Created: LGY 2011-06-06
// -----------------------------------------------------------------------------
MIL_ObjectKnowledgeParameter::MIL_ObjectKnowledgeParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectKnowledgeParameter constructor
// Created: LDC 2009-06-16
// -----------------------------------------------------------------------------
MIL_ObjectKnowledgeParameter::MIL_ObjectKnowledgeParameter( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
    : pKnowledgeObject_( pKnowledge )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectKnowledgeParameter constructor
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
MIL_ObjectKnowledgeParameter::MIL_ObjectKnowledgeParameter( const sword::ObjectKnowledgeId& asn, const DEC_KnowledgeResolver_ABC& resolver )
    : pKnowledgeObject_( resolver.ResolveKnowledgeObject( asn ) )
{
    if( !pKnowledgeObject_ )
        throw std::runtime_error( "Object Knowledge does not exist" );
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
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
bool MIL_ObjectKnowledgeParameter::IsOfType( MIL_ParameterType_ABC::E_Type type ) const
{
    return type == MIL_ParameterType_ABC::eObjectKnowledge
        || type == MIL_ParameterType_ABC::eLocationComposite;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectKnowledgeParameter::ToObjectKnowledge
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
bool MIL_ObjectKnowledgeParameter::ToObjectKnowledge( boost::shared_ptr< DEC_Knowledge_Object >& value ) const
{
    value = pKnowledgeObject_;
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectKnowledgeParameter::ToElement
// Created: MGD 2010-11-12
// -----------------------------------------------------------------------------
bool MIL_ObjectKnowledgeParameter::ToElement( sword::MissionParameter_Value& elem ) const
{
    elem.mutable_objectknowledge()->set_id( pKnowledgeObject_->GetID() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectKnowledgeParameter::serialize
// Created: LGY 2011-06-06
// -----------------------------------------------------------------------------
template< typename Archive >
void MIL_ObjectKnowledgeParameter::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< MIL_BaseParameter >( *this )
         & pKnowledgeObject_;
}
