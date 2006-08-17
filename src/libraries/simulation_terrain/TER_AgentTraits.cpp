// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2005-01-31 $
// $Archive: /MVW_v10/Build/SDK/ter/src/TER_AgentTraits.cpp $
// $Author: Age $
// $Modtime: 31/01/05 16:33 $
// $Revision: 1 $
// $Workfile: TER_AgentTraits.cpp $
//
// *****************************************************************************

#include "simulation_terrain_pch.h"
#include "TER_AgentTraits.h"
#include "TER_Agent_ABC.h"

// -----------------------------------------------------------------------------
// Name: TER_AgentTraits::CompareOnX
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
int TER_AgentTraits::CompareOnX( MT_Float rValue, const T_Value& pAgent ) const
{
    return Compare( pAgent->GetPosition().rX_ - rValue );
}

// -----------------------------------------------------------------------------
// Name: TER_AgentTraits::CompareOnY
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
int TER_AgentTraits::CompareOnY( MT_Float rValue, const T_Value& pAgent ) const
{
    return Compare( pAgent->GetPosition().rY_ - rValue );
}

// -----------------------------------------------------------------------------
// Name: TER_AgentTraits::Compare
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
int TER_AgentTraits::Compare( MT_Float rDelta ) const
{
    static const MT_Float rPrecision = 0.;
    if( rDelta < -rPrecision )
        return -1;
    else if( rDelta > rPrecision )
        return 1;
    return 0;
}
