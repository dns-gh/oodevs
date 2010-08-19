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
#include "protocol/protocol.h"

// -----------------------------------------------------------------------------
// Name: MIL_MissionObjectiveListParameter constructor
// Created: LDC 2009-06-05
// -----------------------------------------------------------------------------
MIL_MissionObjectiveListParameter::MIL_MissionObjectiveListParameter( const Common::MsgMissionObjectiveList& asn )
{
    unsigned int size = asn.elem_size();
    objectives_.reserve( size );
    for( unsigned int i = 0; i < size; ++i )
    {
        boost::shared_ptr< DEC_Objective > pObjective( new DEC_Objective( asn.elem(i) ) );
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
    return dynamic_cast<const MIL_ParameterType_ObjectiveList*>( &type ) != 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_MissionObjectiveListParameter::ToObjectiveList
// Created: LDC 2009-06-05
// -----------------------------------------------------------------------------
bool MIL_MissionObjectiveListParameter::ToObjectiveList( Common::MsgMissionObjectiveList& asn ) const
{
    for( unsigned int i = 0; i < objectives_.size(); ++i )
        objectives_[ i ]->Serialize( *asn.add_elem( ) );
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
