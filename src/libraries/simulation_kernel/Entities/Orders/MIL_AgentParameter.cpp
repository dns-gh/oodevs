// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_AgentParameter.h"
#include "Decision/DEC_Decision_ABC.h"
#include "Entities/MIL_EntityManager_ABC.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Network/NET_ASN_Tools.h"
#include "Tools/NET_AsnException.h"
#include "protocol/Protocol.h"

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_AgentParameter )

// -----------------------------------------------------------------------------
// Name: MIL_AgentParameter constructor
// Created: LGY 2011-06-06
// -----------------------------------------------------------------------------
MIL_AgentParameter::MIL_AgentParameter()
    : pDecision_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentParameter constructor
// Created: LDC 2009-06-16
// -----------------------------------------------------------------------------
MIL_AgentParameter::MIL_AgentParameter( const DEC_Decision_ABC* pAgent )
    : pDecision_( pAgent )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentParameter constructor
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
MIL_AgentParameter::MIL_AgentParameter( const sword::Id& asn, const MIL_EntityManager_ABC& entityManager )
    : pDecision_( 0 )
{
    MIL_AgentPion* pPion = entityManager.FindAgentPion( asn.id() );
    if( pPion )
        pDecision_ = &pPion->GetDecision();
    if( !pDecision_ )
        throw MASA_EXCEPTION_ASN( sword::OrderAck_ErrorCode, sword::OrderAck::error_invalid_parameter );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentParameter destructor
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
MIL_AgentParameter::~MIL_AgentParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentParameter::IsOfType
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
bool MIL_AgentParameter::IsOfType( MIL_ParameterType_ABC::E_Type type ) const
{
    return type == MIL_ParameterType_ABC::eAgent
        || type == MIL_ParameterType_ABC::eAgentKnowledge
        || type == MIL_ParameterType_ABC::eLocationComposite;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentParameter::ToAgent
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
bool MIL_AgentParameter::ToAgent( const DEC_Decision_ABC*& value ) const
{
    value = pDecision_;
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentParameter::ToElement
// Created: MGD 2010-11-12
// -----------------------------------------------------------------------------
bool MIL_AgentParameter::ToElement( sword::MissionParameter_Value& elem ) const
{
    if( !pDecision_ )
        return false;
    NET_ASN_Tools::WriteAgent( *pDecision_, *elem.mutable_agent() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentParameter::load
// Created: LGY 2011-06-14
// -----------------------------------------------------------------------------
void MIL_AgentParameter::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< MIL_BaseParameter >( *this );
    file >> pDecision_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentParameter::save
// Created: LGY 2011-06-14
// -----------------------------------------------------------------------------
void MIL_AgentParameter::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< MIL_BaseParameter >( *this );
    file << pDecision_;
}
