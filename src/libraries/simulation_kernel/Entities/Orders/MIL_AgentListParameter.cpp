// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_AgentListParameter.h"

#include "MIL_AgentServer.h"
#include "simulation_orders/MIL_ParameterType_AgentList.h"
#include "Entities/MIL_EntityManager_ABC.h"
#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"

// -----------------------------------------------------------------------------
// Name: MIL_AgentListParameter constructor
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
MIL_AgentListParameter::MIL_AgentListParameter( const ASN1T_UnitList& asn, MIL_EntityManager_ABC& entityManager )
{
    unitList_.reserve( asn.n );
    for( unsigned int i = 0; i < asn.n; ++i )
    {
        MIL_AgentPion* pPion = entityManager.FindAgentPion( asn .elem[i]);
        if( !pPion )
            throw std::runtime_error( "Agent does not exist" );
        DEC_RolePion_Decision* pAgent = dynamic_cast< DEC_RolePion_Decision* >( &pPion->GetDecision() );
        unitList_.push_back( pAgent );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentListParameter constructor
// Created: LDC 2009-09-25
// -----------------------------------------------------------------------------
MIL_AgentListParameter::MIL_AgentListParameter( const std::vector< DEC_RolePion_Decision* >& unitList )
: unitList_( unitList )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentListParameter destructor
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
MIL_AgentListParameter::~MIL_AgentListParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentListParameter::IsOfType
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
bool MIL_AgentListParameter::IsOfType( const MIL_ParameterType_ABC& type ) const
{
    return( dynamic_cast<const MIL_ParameterType_AgentList*>( &type ) != 0 );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentListParameter::ToAgentList
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
bool MIL_AgentListParameter::ToAgentList( ASN1T_UnitList& asn ) const
{
    unsigned int size = unitList_.size();
    asn.n = size;
    if( size != 0 )
    {    
        ASN1T_OID* pOID = new ASN1T_OID[ size ]; //$$$ RAM
        asn.elem = pOID;

        for( unsigned int i = 0; i < size; ++i )
            pOID[i] = unitList_[i]->GetPion().GetID();
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentListParameter::ToAgentList
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
bool MIL_AgentListParameter::ToAgentList( std::vector< DEC_Decision_ABC* >& value ) const
{
    value.resize( unitList_.size() );
    for( unsigned int i = 0; i < unitList_.size(); ++i )
        value[i] = unitList_[i];
    return true;
}
