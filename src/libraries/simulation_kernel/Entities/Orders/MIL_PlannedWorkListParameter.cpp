// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_PlannedWorkListParameter.h"
#include "MIL_ParameterType_GenObjectList.h"
#include "Decision/DEC_Gen_Object.h"
#include "Network/NET_AsnException.h"

// -----------------------------------------------------------------------------
// Name: MIL_PlannedWorkListParameter constructor
// Created: LDC 2009-06-05
// -----------------------------------------------------------------------------
MIL_PlannedWorkListParameter::MIL_PlannedWorkListParameter( const ASN1T_PlannedWorkList& asn, const MIL_EntityManager_ABC& entityManager )
{
    unsigned int size = asn.n;
    plannedWorkList_.reserve( size );
    for( unsigned int i = 0; i < size; ++i )
    {
        boost::shared_ptr< DEC_Gen_Object > pGenObject( new DEC_Gen_Object( asn.elem[i], entityManager ) );
        plannedWorkList_.push_back( pGenObject );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_PlannedWorkListParameter constructor
// Created: LDC 2009-09-25
// -----------------------------------------------------------------------------
MIL_PlannedWorkListParameter::MIL_PlannedWorkListParameter( const std::vector< boost::shared_ptr< DEC_Gen_Object > >& plannedWorkList )
: plannedWorkList_( plannedWorkList )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_PlannedWorkListParameter destructor
// Created: LDC 2009-06-05
// -----------------------------------------------------------------------------
MIL_PlannedWorkListParameter::~MIL_PlannedWorkListParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_PlannedWorkListParameter::IsOfType
// Created: LDC 2009-06-05
// -----------------------------------------------------------------------------
bool MIL_PlannedWorkListParameter::IsOfType( const MIL_ParameterType_ABC& type ) const
{
    return( dynamic_cast<const MIL_ParameterType_GenObjectList*>( &type ) != 0 );
}

// -----------------------------------------------------------------------------
// Name: MIL_PlannedWorkListParameter::ToGenObjectList
// Created: LDC 2009-06-05
// -----------------------------------------------------------------------------
bool MIL_PlannedWorkListParameter::ToGenObjectList( ASN1T_PlannedWorkList& asn ) const
{
    unsigned int size = plannedWorkList_.size();
    asn.n = size;
    if( size )
    {
        asn.elem = new ASN1T_PlannedWork[ size ];
        for( unsigned int i = 0; i < size; ++i )
            plannedWorkList_[i]->Serialize( asn.elem[ i ] );
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_PlannedWorkListParameter::ToGenObjectList
// Created: LDC 2009-06-05
// -----------------------------------------------------------------------------
bool MIL_PlannedWorkListParameter::ToGenObjectList( std::vector< boost::shared_ptr< DEC_Gen_Object > >& value ) const
{
    value = plannedWorkList_;
    return true;
}
