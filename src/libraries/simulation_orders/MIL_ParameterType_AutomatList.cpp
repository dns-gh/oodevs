// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_orders_pch.h"

#include "MIL_ParameterType_AutomatList.h"
#include "MIL_MissionParameter_ABC.h"
#include "protocol/protocol.h"

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_AutomatList constructor
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
MIL_ParameterType_AutomatList::MIL_ParameterType_AutomatList( const std::string& name )
    : MIL_ParameterType_ABC( name )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_AutomatList destructor
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
MIL_ParameterType_AutomatList::~MIL_ParameterType_AutomatList()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_AutomatList::Copy
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
bool MIL_ParameterType_AutomatList::Copy( const MIL_MissionParameter_ABC& from, Common::MsgMissionParameter& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool /*bIsOptional*/ ) const
{
    // Check source
    if( !from.IsOfType( *this ) )
        return false;

    to.set_null_value( false );
    return from.ToAutomatList( *to.mutable_value()->mutable_automatlist() );
}
