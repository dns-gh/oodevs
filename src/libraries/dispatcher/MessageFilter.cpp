// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MessageFilter.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MessageFilter constructor
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
MessageFilter::MessageFilter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MessageFilter destructor
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
MessageFilter::~MessageFilter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MessageFilter::IsRelevant
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
bool MessageFilter::IsRelevant( const ASN1T_MsgsOutSim& message )
{
    return IsRelevant( message.msg.t );
}

// -----------------------------------------------------------------------------
// Name: MessageFilter::IsRelevant
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
bool MessageFilter::IsRelevant( const ASN1T_MsgsInClient& message )
{
    return IsRelevant( message.msg.t );
}

// -----------------------------------------------------------------------------
// Name: MessageFilter::IsRelevant
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
bool MessageFilter::IsRelevant( unsigned type )
{
    switch( type )
    {
    case T_MsgsOutSim_msg_msg_side_creation:
    case T_MsgsOutSim_msg_msg_knowledge_group_creation:
    case T_MsgsOutSim_msg_msg_formation_creation:
    case T_MsgsOutSim_msg_msg_pion_creation:
    case T_MsgsOutSim_msg_msg_automate_creation:
    case T_MsgsOutSim_msg_msg_object_creation:
    case T_MsgsOutSim_msg_msg_unit_attributes:
    case T_MsgsOutSim_msg_msg_automate_attributes:
    case T_MsgsOutSim_msg_msg_object_update:
    case T_MsgsOutSim_msg_msg_object_destruction:
        return true;
    default:
        return false;
    }
}
