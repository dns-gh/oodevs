// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_AutomatListParameter.h"

#include "MIL_ParameterType_AutomatList.h"
#include "Entities/Automates/DEC_AutomateDecision.h"
#include "Entities/MIL_EntityManager_ABC.h"
#include "Network/NET_ASN_Tools.h"

// -----------------------------------------------------------------------------
// Name: MIL_AutomatListParameter constructor
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
MIL_AutomatListParameter::MIL_AutomatListParameter( const ASN1T_AutomatList& asn, MIL_EntityManager_ABC& entityManager )
{
    automatList_.reserve( asn.n );
    for( unsigned int i = 0; i < asn.n; ++i )
    {
        MIL_Automate* pAutomate = entityManager.FindAutomate( asn.elem[i] );
        if( !pAutomate ) 
            throw std::runtime_error( "Automat does not exist" );
        automatList_.push_back( &pAutomate->GetDecision() );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomatListParameter constructor
// Created: LDC 2009-09-25
// -----------------------------------------------------------------------------
MIL_AutomatListParameter::MIL_AutomatListParameter( const std::vector< DEC_AutomateDecision* >& automatList )
: automatList_( automatList )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomatListParameter destructor
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
MIL_AutomatListParameter::~MIL_AutomatListParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomatListParameter::IsOfType
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
bool MIL_AutomatListParameter::IsOfType( const MIL_ParameterType_ABC& type ) const
{
    return( dynamic_cast<const MIL_ParameterType_AutomatList*>( &type ) != 0 );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomatListParameter::ToAutomatList
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
bool MIL_AutomatListParameter::ToAutomatList( ASN1T_AutomatList& asn ) const
{
    unsigned int size = automatList_.size();
    asn.n = size;
    if( size != 0 )
    {    
        ASN1T_OID* pOID = new ASN1T_OID[ size ]; //$$$ RAM
        asn.elem = pOID;

        for( unsigned int i = 0; i < size; ++i )
            pOID[i] = automatList_[i]->GetAutomate().GetID();
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomatListParameter::ToAutomatList
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
bool MIL_AutomatListParameter::ToAutomatList( std::vector< DEC_Decision_ABC* >& value ) const
{
    value.resize( automatList_.size() );
    for( unsigned int i = 0; i < automatList_.size(); ++i )
        value[i] = automatList_[i];
    return true;
}
