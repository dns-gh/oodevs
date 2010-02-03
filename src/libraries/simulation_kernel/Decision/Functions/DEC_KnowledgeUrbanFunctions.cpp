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
#include "Knowledge/DEC_Knowledge_Urban.h"


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

