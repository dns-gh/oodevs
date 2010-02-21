// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "KnowledgeGroupsModel.h"
#include "KnowledgeGroupHierarchies.h" // LTO
#include "TeamsModel.h"
#include "clients_kernel/CommunicationHierarchies.h" // LTO
#include "clients_kernel/Controllers.h" // LTO
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "protocol/simulationSenders.h"

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupsModel constructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
KnowledgeGroupsModel::KnowledgeGroupsModel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupsModel destructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
KnowledgeGroupsModel::~KnowledgeGroupsModel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupsModel::Purge
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
void KnowledgeGroupsModel::Purge()
{
    DeleteAll();
}
