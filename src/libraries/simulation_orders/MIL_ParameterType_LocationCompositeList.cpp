// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "simulation_orders_pch.h"
#include "MIL_ParameterType_LocationCompositeList.h"
#include "MIL_MissionParameter_ABC.h"
#include "protocol/protocol.h"

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_LocationCompositeList constructor
// Created: LDC 2010-09-21
// -----------------------------------------------------------------------------
MIL_ParameterType_LocationCompositeList::MIL_ParameterType_LocationCompositeList( const std::string& name )
    : MIL_ParameterType_ABC( name )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_LocationCompositeList destructor
// Created: LDC 2010-09-21
// -----------------------------------------------------------------------------
MIL_ParameterType_LocationCompositeList::~MIL_ParameterType_LocationCompositeList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ParameterType_LocationCompositeList::Copy
// Created: LDC 2010-09-21
// -----------------------------------------------------------------------------
bool MIL_ParameterType_LocationCompositeList::Copy( const MIL_MissionParameter_ABC& from, Common::MsgMissionParameter& to, const DEC_KnowledgeResolver_ABC& /*knowledgeResolver*/, bool bIsOptional ) const
{
    std::vector< Common::MsgMissionParameter_Value > list;
    if( from.ToList( list ) )
    {
        ::google::protobuf::RepeatedPtrField< Common::MsgMissionParameter_Value >& mutableList = *to.mutable_value()->mutable_list();
        for( std::vector< Common::MsgMissionParameter_Value >::const_iterator it = list.begin(); it != list.end(); ++it )
            *(mutableList.Add()) = *it;
        return true;
    }
    to.clear_value();
    return bIsOptional;
}    
