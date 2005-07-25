// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-05-12 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/KnowledgeGroup.cpp $
// $Author: Sbo $
// $Modtime: 21/07/05 17:04 $
// $Revision: 6 $
// $Workfile: KnowledgeGroup.cpp $
//
// *****************************************************************************

#include "TIC_Pch.h"
#include "Entities/KnowledgeGroup.h"

using namespace TIC;

KnowledgeGroup::T_KnowledgeGroupMap KnowledgeGroup::knowledgeGroups_;
KnowledgeGroup::T_EntityIdSet       KnowledgeGroup::disaggregKnowledgeGroups_;

//-----------------------------------------------------------------------------
// Name: KnowledgeGroup::KnowledgeGroup
// Created: SBO 2005-05-12
//-----------------------------------------------------------------------------
KnowledgeGroup::KnowledgeGroup( T_EntityId nSimId, DIN::DIN_Input& /*input*/ )
    : nId_              ( nSimId )
    , bConfigDisaggreg_ ( disaggregKnowledgeGroups_.find( nId_ ) != disaggregKnowledgeGroups_.end() )
{
}

//-----------------------------------------------------------------------------
// Name: KnowledgeGroup::~KnowledgeGroup
// Created: SBO 2005-05-12
//-----------------------------------------------------------------------------
KnowledgeGroup::~KnowledgeGroup()
{
}

//-----------------------------------------------------------------------------
// Name: KnowledgeGroup::Initialize
// Created: SBO 2005-05-19
//-----------------------------------------------------------------------------
void KnowledgeGroup::Initialize()
{
}

//-----------------------------------------------------------------------------
// Name: KnowledgeGroup::Terminate
// Created: SBO 2005-05-19
//-----------------------------------------------------------------------------
void KnowledgeGroup::Terminate()
{
    for( CIT_KnowledgeGroupMap it = knowledgeGroups_.begin(); it != knowledgeGroups_.end(); ++it )
        delete it->second;
    knowledgeGroups_.clear();
    disaggregKnowledgeGroups_.clear();
}
