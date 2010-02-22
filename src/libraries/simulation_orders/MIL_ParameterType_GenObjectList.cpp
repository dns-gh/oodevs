// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_orders_pch.h"
#include "MIL_ParameterType_GenObjectList.h"
#include "MIL_MissionParameter_ABC.h"
#include "protocol/protocol.h"

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_GenObjectList constructor
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
MIL_ParameterType_GenObjectList::MIL_ParameterType_GenObjectList( const std::string& name )
    : MIL_ParameterType_ABC( name )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_GenObjectList destructor
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
MIL_ParameterType_GenObjectList::~MIL_ParameterType_GenObjectList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_GenObjectList::Copy
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
bool MIL_ParameterType_GenObjectList::Copy( const MIL_MissionParameter_ABC& from, Common::MsgMissionParameter& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool /*bIsOptional*/ ) const
{
    // Check source
    if( !from.IsOfType( *this ) )
        return false;

    to.set_null_value( false);
    return from.ToGenObjectList( *to.mutable_value()->mutable_plannedworklist() );
}
