// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_MissionObjectiveListParameter.h"
#include "simulation_orders/MIL_ParameterType_ObjectiveList.h"
#include "Decision/DEC_Objective.h"
#include "Network/NET_AsnException.h"

// -----------------------------------------------------------------------------
// Name: MIL_MissionObjectiveListParameter constructor
// Created: LDC 2009-06-05
// -----------------------------------------------------------------------------
MIL_MissionObjectiveListParameter::MIL_MissionObjectiveListParameter( const ASN1T_MissionObjectiveList& asn )
{
    unsigned int size = asn.n;
    objectives_.reserve( size );
    for( uint i = 0; i < size; ++i )
    {
        boost::shared_ptr< DEC_Objective > pObjective( new DEC_Objective( asn.elem[i] ) );
        objectives_.push_back( pObjective );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionObjectiveListParameter destructor
// Created: LDC 2009-06-05
// -----------------------------------------------------------------------------
MIL_MissionObjectiveListParameter::~MIL_MissionObjectiveListParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionObjectiveListParameter::IsOfType
// Created: LDC 2009-06-05
// -----------------------------------------------------------------------------
bool MIL_MissionObjectiveListParameter::IsOfType( const MIL_ParameterType_ABC& type ) const
{
    return( dynamic_cast<const MIL_ParameterType_ObjectiveList*>( &type ) != 0 );
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionObjectiveListParameter::ToObjectiveList
// Created: LDC 2009-06-05
// -----------------------------------------------------------------------------
bool MIL_MissionObjectiveListParameter::ToObjectiveList( ASN1T_MissionObjectiveList& asn ) const
{
    unsigned int size = objectives_.size();
    asn.n = size;
    if( size )
    {
        asn.elem = new ASN1T_MissionObjective[ size ];
        for( unsigned int i = 0; i < size; ++i )
            objectives_[ i ]->Serialize( asn.elem[ i ] );
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionObjectiveListParameter::ToObjectiveList
// Created: LDC 2009-06-05
// -----------------------------------------------------------------------------
bool MIL_MissionObjectiveListParameter::ToObjectiveList( std::vector< boost::shared_ptr< DEC_Objective > >& value ) const
{
    value = objectives_;
    return true;
}
