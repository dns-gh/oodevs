// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_KnowledgeUrbanFunctions.h"

#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h"
#include "Knowledge/DEC_Knowledge_Urban.h"
#include "urban/MaterialCompositionType.h"
#include "urban/TerrainObject_ABC.h"
#include "urban/Architecture.h"


// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeUrbanFunctions::GetCurrentPerceptionLevel
// Created: SLG 2010-02-01
// -----------------------------------------------------------------------------
int DEC_KnowledgeUrbanFunctions::GetCurrentPerceptionLevel( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Urban > pKnowledge )
{
    if( pKnowledge.get() && pKnowledge->IsValid() )
        return (int)pKnowledge->GetCurrentPerceptionLevel( callerAgent ).GetID();
    else
        return 0;
}


// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeUrbanFunctions::GetCurrentBarycenter
// Created: MGD 2010-02-19
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_KnowledgeUrbanFunctions::GetCurrentBarycenter( boost::shared_ptr< DEC_Knowledge_Urban > pKnowledge )
{
    boost::shared_ptr< MT_Vector2D > pos;
    if( pKnowledge.get() && pKnowledge->IsValid() )
    {
        geometry::Point2f temp = pKnowledge->GetBarycenter();
        pos.reset( new MT_Vector2D( temp.X(), temp.Y() ) );
    }
    return pos;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeUrbanFunctions::GetPathfindCost
// Created: MGD 2010-03-18
// -----------------------------------------------------------------------------
float DEC_KnowledgeUrbanFunctions::GetPathfindCost( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Urban > pKnowledge )
{  
     if( pKnowledge.get() && pKnowledge->IsValid() )
        return pKnowledge->GetPathfindCost( callerAgent.GetRole< PHY_RoleInterface_Composantes >().GetMajorComponentWeight() );
     else
        return -1;
}
