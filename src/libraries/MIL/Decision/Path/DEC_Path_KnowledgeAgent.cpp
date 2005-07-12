// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: NLD 2004-04-06 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Path/DEC_Path_KnowledgeAgent.cpp $
// $Author: Age $
// $Modtime: 8/03/05 18:54 $
// $Revision: 8 $
// $Workfile: DEC_Path_KnowledgeAgent.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"
#include "DEC_Path_KnowledgeAgent.h"

#include "DEC_Path.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Tools/MIL_Tools.h"

// -----------------------------------------------------------------------------
// Name: DEC_Path_KnowledgeAgent constructor
// Created: NLD 2004-04-06
// -----------------------------------------------------------------------------
DEC_Path_KnowledgeAgent::DEC_Path_KnowledgeAgent( const DEC_Knowledge_Agent& knowledge, const DEC_Path& )
    : vPosition_( knowledge.GetPosition() )
{ 
    
}

// -----------------------------------------------------------------------------
// Name: DEC_Path_KnowledgeAgent destructor
// Created: NLD 2004-04-06
// -----------------------------------------------------------------------------
DEC_Path_KnowledgeAgent::~DEC_Path_KnowledgeAgent()
{
    
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Path_KnowledgeAgent::ComputeCost
// Created: AGE 2005-02-01
// -----------------------------------------------------------------------------
MT_Float DEC_Path_KnowledgeAgent::ComputeCost( const MT_Vector2D& from, const MT_Vector2D& to, const TerrainData&, const TerrainData& ) const
{
    static MT_Float rSecurityDistance       = MIL_Tools::ConvertMeterToSim( 1000. );
    static MT_Float rCostAtSecurityDistance = 100.;
    static MT_Float rCostOnEnemy            = 1000.;

    static MT_Float rB = rCostOnEnemy;
    static MT_Float rA = ( rCostAtSecurityDistance - rB ) / rSecurityDistance;

    const MT_Line lineLink( from, to );
    const MT_Vector2D vPositionProjection = lineLink.ClosestPointOnLine( vPosition_ );
    const MT_Float rDistBtwUnitAndEnemy = vPositionProjection.Distance( vPosition_  );

    // Plus la connaissance est l'ennemi est loin, moins le cout est important
    if( rDistBtwUnitAndEnemy > rSecurityDistance )
        return 0.;
    return rA * rDistBtwUnitAndEnemy + rB;    
}
