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
#include "KnowledgeGroupHierarchies.h"

#include "TeamsModel.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/CommunicationHierarchies.h"

#include "clients_kernel/KnowledgeGroup_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupsModel constructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
KnowledgeGroupsModel::KnowledgeGroupsModel( kernel::Controller& controller )
: controller_( controller )
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
    tools::Resolver< KnowledgeGroup_ABC >::DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupsModel::KnowledgeGroupsModel::Delete
// Created: FHD 2009-12-21
// -----------------------------------------------------------------------------
void KnowledgeGroupsModel::Delete( const ASN1T_MsgKnowledgeGroupDelete& asn )
{
//    KnowledgeGroup_ABC* pCurrentKnowledgegroup = Find( asn.oid );
    Remove( asn.oid );
}
