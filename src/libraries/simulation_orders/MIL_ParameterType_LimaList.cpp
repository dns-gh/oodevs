// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_orders_pch.h"

#include "MIL_ParameterType_LimaList.h"
#include "MIL_MissionParameter_ABC.h"
#include "protocol/protocol.h"

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_LimaList constructor
// Created: MGD 2010-10-27
//-----------------------------------------------------------------------------
MIL_ParameterType_LimaList::MIL_ParameterType_LimaList()
    : MIL_ParameterType_ABC( "PhaseLineList" )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_LimaList destructor
// Created: MGD 2010-10-27
//-----------------------------------------------------------------------------
MIL_ParameterType_LimaList::~MIL_ParameterType_LimaList()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: MIL_ParameterType_LimaList::Copy
// Created: MGD 2010-10-27
//-----------------------------------------------------------------------------
bool MIL_ParameterType_LimaList::Copy( const MIL_MissionParameter_ABC& from, Common::MsgMissionParameter& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool bIsOptional ) const
{
    // Check source
    if( !from.IsOfType( *this ) )
        return false;
    to.set_null_value( !from.ToLimaList( *to.mutable_value()->mutable_limasorder() ) );
    if( to.null_value() )
        to.clear_value();
    return !to.null_value() || bIsOptional;
}
