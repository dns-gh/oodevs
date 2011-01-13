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
#include "Entities/Automates/DEC_AutomateDecision.h"
#include "Entities/MIL_EntityManager_ABC.h"
#include "Network/NET_ASN_Tools.h"
#include "Network/NET_AsnException.h"
#include "protocol/Protocol.h"

// -----------------------------------------------------------------------------
// Name: MIL_AutomatParameter constructor
// Created: LDC 2009-06-16
// -----------------------------------------------------------------------------
MIL_AutomatParameter::MIL_AutomatParameter( DEC_AutomateDecision* pDecision )
    : pDecision_( pDecision )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomatParameter constructor
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
MIL_AutomatParameter::MIL_AutomatParameter( const sword::AutomatId& asn, MIL_EntityManager_ABC& entityManager )
    : pDecision_( 0 )
{
    MIL_Automate* pAutomate = entityManager.FindAutomate( asn.id() );
    if( pAutomate )
        pDecision_ = &pAutomate->GetDecision();
    if( !pDecision_ )
        throw NET_AsnException< sword::OrderAck_ErrorCode >( sword::OrderAck::error_invalid_parameter );
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
