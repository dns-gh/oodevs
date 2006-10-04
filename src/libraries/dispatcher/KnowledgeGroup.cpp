// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"

#include "KnowledgeGroup.h"

#include "Side.h"
#include "Publisher_ABC.h"
#include "Network_Def.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup constructor
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
KnowledgeGroup::KnowledgeGroup( Model& /*model*/, unsigned int nID, Side& side )
    : nID_     ( nID )
    , side_    ( side )
    , automats_()
{
    side_.GetKnowledgeGroups().Register( *this );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup destructor
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
KnowledgeGroup::~KnowledgeGroup()
{
    side_.GetKnowledgeGroups().Unregister( *this );
}

// =============================================================================
// MAIN
// =============================================================================

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::SendCreation
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void KnowledgeGroup::SendCreation( Publisher_ABC& publisher ) const
{
    DIN::DIN_BufferedMessage msg = publisher.GetDinMsg();
    msg << side_.GetID()
        << nID_;        
    publisher.Send( eMsgKnowledgeGroup, msg );
}
