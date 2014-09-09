// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_AutomatParameter.h"
#include "Decision/DEC_Decision_ABC.h"
#include "Entities/MIL_EntityManager_ABC.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Network/NET_ASN_Tools.h"
#include "Tools/NET_AsnException.h"
#include "protocol/Protocol.h"

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_AutomatParameter )

// -----------------------------------------------------------------------------
// Name: MIL_AutomatParameter constructor
// Created: LGY 2011-06-06
// -----------------------------------------------------------------------------
MIL_AutomatParameter::MIL_AutomatParameter()
    : pDecision_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomatParameter constructor
// Created: LDC 2009-06-16
// -----------------------------------------------------------------------------
MIL_AutomatParameter::MIL_AutomatParameter( DEC_Decision_ABC* pDecision )
    : pDecision_( pDecision )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomatParameter constructor
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
MIL_AutomatParameter::MIL_AutomatParameter( const sword::Id& asn, const MIL_EntityManager_ABC& entityManager )
    : pDecision_( 0 )
{
    MIL_Automate* pAutomate = entityManager.FindAutomate( asn.id() );
    if( pAutomate )
        pDecision_ = &pAutomate->GetDecision();
    if( !pDecision_ )
        throw MASA_EXCEPTION_ASN( sword::OrderAck_ErrorCode, sword::OrderAck::error_invalid_parameter );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomatParameter destructor
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
MIL_AutomatParameter::~MIL_AutomatParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomatParameter::IsOfType
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
bool MIL_AutomatParameter::IsOfType( MIL_ParameterType_ABC::E_Type type ) const
{
    return type == MIL_ParameterType_ABC::eAutomat
        || type == MIL_ParameterType_ABC::eLocationComposite;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomatParameter::ToAutomat
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
bool MIL_AutomatParameter::ToAutomat( DEC_Decision_ABC*& value ) const
{
    value = pDecision_;
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomatParameter::ToElement
// Created: MGD 2010-11-12
// -----------------------------------------------------------------------------
bool MIL_AutomatParameter::ToElement( sword::MissionParameter_Value& elem ) const
{
    if( !pDecision_ )
        return false;
    NET_ASN_Tools::WriteAutomate( *pDecision_, *elem.mutable_automat() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomatParameter::load
// Created: LGY 2011-08-22
// -----------------------------------------------------------------------------
void MIL_AutomatParameter::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< MIL_BaseParameter >( *this );
    file >> pDecision_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomatParameter::save
// Created: LGY 2011-08-22
// -----------------------------------------------------------------------------
void  MIL_AutomatParameter::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< MIL_BaseParameter >( *this );
    file << pDecision_;
}
