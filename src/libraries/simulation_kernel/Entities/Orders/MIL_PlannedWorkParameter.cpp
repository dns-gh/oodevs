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
#include "simulation_orders/MIL_ParameterType_GenObject.h"
#include "Decision/DEC_Gen_Object.h"
#include "protocol/protocol.h"

// -----------------------------------------------------------------------------
// Name: MIL_PlannedWorkParameter constructor
// Created: LDC 2009-06-04
// -----------------------------------------------------------------------------
MIL_PlannedWorkParameter::MIL_PlannedWorkParameter( const Common::MsgPlannedWork& asn, const MIL_EntityManager_ABC& entityManager )
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
// Created: LDC 2009-06-04
// -----------------------------------------------------------------------------
bool MIL_PlannedWorkParameter::IsOfType( const MIL_ParameterType_ABC& type ) const
{
    return( dynamic_cast<const MIL_ParameterType_GenObject*>( &type ) != 0 );
}

// -----------------------------------------------------------------------------
// Name: MIL_PlannedWorkParameter::ToGenObject
// Created: LDC 2009-06-04
// -----------------------------------------------------------------------------
bool MIL_PlannedWorkParameter::ToGenObject( Common::MsgPlannedWork& asn ) const
{
    pGenObject_->Serialize( asn );
    return true;
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
bool MIL_PlannedWorkParameter::ToElement( Common::MsgMissionParameter_Value& elem ) const
{
    ToGenObject( *elem.mutable_plannedwork() );
    return true;
}
