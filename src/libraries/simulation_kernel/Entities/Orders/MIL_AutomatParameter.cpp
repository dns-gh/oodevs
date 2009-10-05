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

#include "simulation_orders/MIL_ParameterType_Automat.h"
#include "Entities/Automates/DEC_AutomateDecision.h"
#include "Entities/MIL_EntityManager_ABC.h"
#include "Network/NET_ASN_Tools.h"

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
MIL_AutomatParameter::MIL_AutomatParameter( const ASN1T_Automat& asn, MIL_EntityManager_ABC& entityManager )
: pDecision_( 0 )
{
    MIL_Automate* pAutomate = entityManager.FindAutomate( asn );
    if( pAutomate )
        pDecision_ = &pAutomate->GetDecision();
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
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
bool MIL_AutomatParameter::IsOfType( const MIL_ParameterType_ABC& type ) const
{
    return( dynamic_cast<const MIL_ParameterType_Automat*>( &type ) != 0 );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomatParameter::ToAutomat
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
bool MIL_AutomatParameter::ToAutomat( ASN1T_Automat& asn ) const
{
    if( !pDecision_ )
        return false;
    NET_ASN_Tools::WriteAutomate( *pDecision_, asn );
    return true;
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
