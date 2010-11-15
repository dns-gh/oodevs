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
#include "simulation_orders/MIL_ParameterType_Agent.h"
#include "simulation_orders/MIL_ParameterType_LocationComposite.h"
#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"
#include "Entities/MIL_EntityManager_ABC.h"
#include "Network/NET_ASN_Tools.h"
#include "protocol/protocol.h"

// -----------------------------------------------------------------------------
// Name: MIL_AgentParameter constructor
// Created: LDC 2009-06-16
// -----------------------------------------------------------------------------
MIL_AgentParameter::MIL_AgentParameter( DEC_RolePion_Decision* pAgent )
    : pDecision_( pAgent )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentParameter constructor
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
MIL_AgentParameter::MIL_AgentParameter( const Common::UnitId& asn, MIL_EntityManager_ABC& entityManager )
    : pDecision_( 0 )
{
    MIL_AgentPion* pPion = entityManager.FindAgentPion( asn.id() );
    if( pPion )
        pDecision_ = dynamic_cast< DEC_RolePion_Decision* >( &pPion->GetDecision() );
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
bool MIL_AgentParameter::IsOfType( const MIL_ParameterType_ABC& type ) const
{
    return dynamic_cast<const MIL_ParameterType_Agent*>( &type ) != 0
        || dynamic_cast< const MIL_ParameterType_LocationComposite* >( &type ) != 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentParameter::ToAgent
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
bool MIL_AgentParameter::ToAgent( Common::UnitId& asn ) const
{
    if( !pDecision_ )
        return false;
    NET_ASN_Tools::WriteAgent( *pDecision_, asn );
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentParameter::ToAgent
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
bool MIL_AgentParameter::ToAgent( DEC_Decision_ABC*& value ) const
{
    value = pDecision_;
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentParameter::ToElement
// Created: MGD 2010-11-12
// -----------------------------------------------------------------------------
bool MIL_AgentParameter::ToElement( Common::MsgMissionParameter_Value& elem ) const
{
    ToAgent( *elem.mutable_agent() );
    return true;
}
