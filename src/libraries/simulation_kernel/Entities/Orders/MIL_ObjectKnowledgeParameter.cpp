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
#include "Entities/MIL_EntityManager_ABC.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Knowledge/DEC_KnowledgeResolver_ABC.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Tools/NET_AsnException.h"
#include "protocol/Protocol.h"

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
MIL_ObjectKnowledgeParameter::MIL_ObjectKnowledgeParameter( const sword::Id& asn, const DEC_KnowledgeResolver_ABC& resolver,
                                                            const MIL_EntityManager_ABC& entityManager )
{
    MIL_Object_ABC* pObject = entityManager.FindObject( asn.id() );
    if( pObject )
        pKnowledgeObject_ = resolver.ResolveKnowledgeObject( *pObject );
    else
        pKnowledgeObject_ = resolver.ResolveKnowledgeObjectByObjectID( asn.id() );
    if( !pKnowledgeObject_ )
       throw MASA_EXCEPTION_ASN( sword::OrderAck_ErrorCode, sword::OrderAck::error_invalid_parameter );
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
    elem.mutable_objectknowledge()->set_id( pKnowledgeObject_->GetObjectId() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectKnowledgeParameter::load
// Created: LGY 2011-08-22
// -----------------------------------------------------------------------------
void MIL_ObjectKnowledgeParameter::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< MIL_BaseParameter >( *this );
    file >> pKnowledgeObject_;
}

// -----------------------------------------------------------------------------
// Name: MIL_ObjectKnowledgeParameter::save
// Created: LGY 2011-08-22
// -----------------------------------------------------------------------------
void MIL_ObjectKnowledgeParameter::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< MIL_BaseParameter >( *this );
    file << pKnowledgeObject_;
}
