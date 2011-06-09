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
#include "Entities/MIL_EntityManager_ABC.h"
#include "Entities/Objects/UrbanObjectWrapper.h"
#include "Network/NET_AsnException.h"
#include "protocol/Protocol.h"

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_UrbanBlockParameter )

// -----------------------------------------------------------------------------
// Name: MIL_UrbanBlockParameter constructor
// Created: LGY 2011-06-06
// -----------------------------------------------------------------------------
MIL_UrbanBlockParameter::MIL_UrbanBlockParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanBlockParameter constructor
// Created: MGD 2009-11-02
// -----------------------------------------------------------------------------
MIL_UrbanBlockParameter::MIL_UrbanBlockParameter( const sword::UrbanObjectKnowledgeId& asn, MIL_EntityManager_ABC& entityManager )
    : pUrbanBlock_( 0 )
{
    MIL_Object_ABC* pObject = entityManager.FindObject( asn.id() );
    if( pObject )
        pUrbanBlock_ = dynamic_cast< UrbanObjectWrapper* >( pObject );
    if( !pUrbanBlock_ )
        throw NET_AsnException< sword::OrderAck_ErrorCode >( sword::OrderAck::error_invalid_parameter );
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanBlockParameter constructor
// Created: MGD 2010-01-15
// -----------------------------------------------------------------------------
MIL_UrbanBlockParameter::MIL_UrbanBlockParameter( UrbanObjectWrapper* pUrbanBlock )
    : pUrbanBlock_( pUrbanBlock )
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
bool MIL_UrbanBlockParameter::ToUrbanBlock( UrbanObjectWrapper*& value ) const
{
    value = pUrbanBlock_;
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanBlockParameter::ToElement
// Created: MGD 2010-11-12
// -----------------------------------------------------------------------------
bool MIL_UrbanBlockParameter::ToElement( sword::MissionParameter_Value& elem ) const
{
    elem.mutable_urbanknowledge()->set_id( pUrbanBlock_->GetID() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanBlockParameter::serialize
// Created: LGY 2011-06-06
// -----------------------------------------------------------------------------
template< typename Archive >
void MIL_UrbanBlockParameter::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< MIL_BaseParameter >( *this )
         & pUrbanBlock_;
}
