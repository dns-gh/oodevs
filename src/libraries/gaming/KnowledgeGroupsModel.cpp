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
#include "TeamsModel.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"

using namespace kernel;

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
    tools::Resolver< KnowledgeGroup_ABC >::DeleteAll();
}

void KnowledgeGroupsModel::Update( const ASN1T_MsgKnowledgeGroupChangeSuperior& asn )
{
    kernel::KnowledgeGroup_ABC* pCurrentKnowledgegroup = Find( asn.oid );
    kernel::KnowledgeGroup_ABC* pKnowledgegroupParent = Find( asn.oid_knowledgegroup_parent );
    if( pCurrentKnowledgegroup )
    {
//        kernel::KnowledgeGroup_ABC* pOldKnowledgegroup = Find( pCurrentKnowledgegroup->GetParent()->GetId() );
//        if( pOldKnowledgegroup && pKnowledgegroupParent && pCurrentKnowledgegroup )
//        {
//            pOldKnowledgegroup->UnregisterKnowledgeGroup( pCurrentKnowledgegroup );
//            pKnowledgegroupParent->Register( pCurrentKnowledgegroup->GetId(), pCurrentKnowledgegroup );
//        }
    }
}

void KnowledgeGroupsModel::Delete( const ASN1T_MsgKnowledgeGroupDelete& asn )
{
    KnowledgeGroup_ABC* pCurrentKnowledgegroup = Find( asn.oid );
//    kernel::KnowledgeGroup_ABC* pParentKnowledgegroup = Find( asn.oid_knowledgegroup_parente );
//    if( pParentKnowledgegroup )
//        pParentKnowledgegroup->UnregisterKnowledgeGroup( pCurrentKnowledgegroup );
}

void KnowledgeGroupsModel::SetType( const ASN1T_MsgKnowledgeGroupSetType& asn )
{
    kernel::KnowledgeGroup_ABC* pCurrentKnowledgegroup = Find( asn.oid );
    //kernel::KnowledgeGroupType* pKnowledgeGroupType = Find( asn.type );
}