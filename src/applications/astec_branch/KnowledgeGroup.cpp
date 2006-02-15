// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: NLD 2004-03-18 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/KnowledgeGroup.cpp $
// $Author: Ape $
// $Modtime: 24/11/04 10:03 $
// $Revision: 7 $
// $Workfile: KnowledgeGroup.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "astec_pch.h"
#include "KnowledgeGroup.h"

#include "Team.h"
#include "App.h"
#include "Agent.h"
#include "AgentKnowledge.h"
#include "Population.h"
#include "PopulationKnowledge.h"
#include "AgentManager.h"

IDManager KnowledgeGroup::idManager_( 0 );

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup constructor
// Created: AGE 2005-09-21
// -----------------------------------------------------------------------------
KnowledgeGroup::KnowledgeGroup( uint32 nId )
    : nID_      ( nId )
{
    idManager_.LockIdentifier( nID_ );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup destructor
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
KnowledgeGroup::~KnowledgeGroup()
{
    idManager_.ReleaseIdentifier( nID_ );
}