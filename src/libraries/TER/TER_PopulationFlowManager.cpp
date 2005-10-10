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
// $Archive: /MVW_v10/Build/SDK/TER/src/TER_PopulationFlowManager.cpp $
// $Author: Age $
// $Modtime: 4/03/05 14:46 $
// $Revision: 2 $
// $Workfile: TER_PopulationFlowManager.cpp $
//
// *****************************************************************************

#include "ter_pch.h"

#include "TER_PopulationFlowManager.h"
#include "TER_PopulationFlow_ABC.h"

// -----------------------------------------------------------------------------
// Name: TER_PopulationFlowManager constructor
// Created: NLD 2005-10-07
// -----------------------------------------------------------------------------
TER_PopulationFlowManager::TER_PopulationFlowManager( const MT_Rect& extent )
    : flows_( T_PopulationFlows::T_Rectangle( extent.GetLeft(), extent.GetBottom(), extent.GetRight(), extent.GetTop() ) )
{
    flows_.SetRefinementPolicy( 20 ); // $$$$ AGE 2005-02-02: profile something ?
}

// -----------------------------------------------------------------------------
// Name: TER_PopulationFlowManager destructor
// Created: NLD 2005-10-07
// -----------------------------------------------------------------------------
TER_PopulationFlowManager::~TER_PopulationFlowManager()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: TER_PopulationFlowManager::UpdatePopulationFlowPosition
// Created: NLD 2005-10-07
// -----------------------------------------------------------------------------
TER_PopulationFlow_ABC::T_Hint TER_PopulationFlowManager::UpdatePosition( TER_PopulationFlow_ABC& flow, const TER_PopulationFlow_ABC::T_Hint& hint )
{
    flows_.Erase( &flow, hint );
    return flows_.Insert( &flow );
}

// -----------------------------------------------------------------------------
// Name: TER_PopulationFlowManager::RemovePopulationFlow
// Created: NLD 2005-10-07
// -----------------------------------------------------------------------------
bool TER_PopulationFlowManager::Remove( TER_PopulationFlow_ABC& flow, const TER_PopulationFlow_ABC::T_Hint& hint )
{
    return flows_.Erase( &flow, hint );
}
