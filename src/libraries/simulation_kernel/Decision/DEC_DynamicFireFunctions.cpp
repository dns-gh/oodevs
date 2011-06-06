// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_DynamicFireFunctions.h"

// -----------------------------------------------------------------------------
// Name: DEC_FireFunctions::GetRangeToExtinguish
// Created: RFT 2008
// -----------------------------------------------------------------------------
float DEC_DynamicFireFunctions::GetRangeToExtinguish( const MIL_AgentPion& /*callerAgent*/, boost::shared_ptr< DEC_Knowledge_Object > /*knowledge*/ )
{
    return -1.0f;
}
