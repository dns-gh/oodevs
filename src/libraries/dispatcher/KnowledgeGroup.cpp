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
#include "Model.h"
#include "Network_Def.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup constructor
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
KnowledgeGroup::KnowledgeGroup( Model& model, const ASN1T_MsgKnowledgeGroupCreation& msg )
    : nID_     ( msg.oid )
    , side_    ( model.GetSides().Get( msg.oid_camp ) )
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
// Name: KnowledgeGroup::Update
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void KnowledgeGroup::Update( const ASN1T_MsgKnowledgeGroupCreation& )
{
    FlagUpdate();
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::SendCreation
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void KnowledgeGroup::SendCreation( ClientPublisher_ABC& publisher ) const
{
    AsnMsgSimToClientKnowledgeGroupCreation asn;
    
    asn().oid      = nID_;
    asn().oid_camp = side_.GetID();

    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::SendFullUpdate
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void KnowledgeGroup::SendFullUpdate( ClientPublisher_ABC& /*publisher*/ ) const
{
	// NOTHING
}
