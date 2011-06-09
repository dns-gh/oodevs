// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_PlannedWorkParameter.h"
#include "Decision/DEC_Gen_Object.h"
#include "protocol/Protocol.h"
#include "Checkpoints/SerializationTools.h"

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_PlannedWorkParameter )

// -----------------------------------------------------------------------------
// Name: MIL_PlannedWorkParameter constructor
// Created: LGY 2011-06-06
// -----------------------------------------------------------------------------
MIL_PlannedWorkParameter::MIL_PlannedWorkParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_PlannedWorkParameter constructor
// Created: LDC 2009-06-04
// -----------------------------------------------------------------------------
MIL_PlannedWorkParameter::MIL_PlannedWorkParameter( const sword::PlannedWork& asn, const MIL_EntityManager_ABC& entityManager )
    : pGenObject_( new DEC_Gen_Object( asn, entityManager ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_PlannedWorkParameter constructor
// Created: MGD 2010-11-12
// -----------------------------------------------------------------------------
MIL_PlannedWorkParameter::MIL_PlannedWorkParameter( boost::shared_ptr< DEC_Gen_Object > param )
    : pGenObject_( param )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_PlannedWorkParameter destructor
// Created: LDC 2009-06-04
// -----------------------------------------------------------------------------
MIL_PlannedWorkParameter::~MIL_PlannedWorkParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_PlannedWorkParameter::IsOfType
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
bool MIL_PlannedWorkParameter::IsOfType( MIL_ParameterType_ABC::E_Type type ) const
{
    return type == MIL_ParameterType_ABC::ePlannedWork;
}

// -----------------------------------------------------------------------------
// Name: MIL_PlannedWorkParameter::ToGenObject
// Created: LDC 2009-06-04
// -----------------------------------------------------------------------------
bool MIL_PlannedWorkParameter::ToGenObject( boost::shared_ptr< DEC_Gen_Object >& value ) const
{
    value = pGenObject_;
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_PlannedWorkParameter::ToElement
// Created: MGD 2010-11-12
// -----------------------------------------------------------------------------
bool MIL_PlannedWorkParameter::ToElement( sword::MissionParameter_Value& elem ) const
{
    pGenObject_->Serialize( *elem.mutable_plannedwork() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_PlannedWorkParameter::serialize
// Created: LGY 2011-06-06
// -----------------------------------------------------------------------------
template< typename Archive >
void MIL_PlannedWorkParameter::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< MIL_BaseParameter >( *this )
         & pGenObject_;
}
