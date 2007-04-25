// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"

#include "Side.h"

#include "Network_Def.h"
#include "Publisher_ABC.h"
#include "Model.h"
#include "Formation.h"
#include "KnowledgeGroup.h"
#include "Object.h"
#include "Population.h"
#include "ModelVisitor_ABC.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Side constructor
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
Side::Side( Model& model, const ASN1T_MsgSideCreation& msg )
    : model_          ( model    )
    , nID_            ( msg.oid  )
    , strName_        ( msg.nom  )
    , nType_          ( msg.type )
    , knowledgeGroups_()
    , formations_     ()
    , diplomacies_    ()    
	, objects_		  ()
	, populations_    ()
{
}

// -----------------------------------------------------------------------------
// Name: Side destructor
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
Side::~Side()
{
}

// =============================================================================
// MAIN
// =============================================================================

// -----------------------------------------------------------------------------
// Name: Side::Update
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void Side::Update( const ASN1T_MsgSideCreation& asnMsg )
{
    FlagUpdate();
}

// -----------------------------------------------------------------------------
// Name: Side::Update
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void Side::Update( const ASN1T_MsgChangeDiplomatie& asnMsg )
{
    FlagUpdate();

    Side& side = model_.GetSides().Get( asnMsg.oid_camp2 );   
    diplomacies_[ &side ] = asnMsg.diplomatie;
}

// -----------------------------------------------------------------------------
// Name: Side::Update
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void Side::Update( const ASN1T_MsgChangeDiplomatieAck& asnMsg )
{
    FlagUpdate();

    Side& side = model_.GetSides().Get( asnMsg.oid_camp2 );   
    diplomacies_[ &side ] = asnMsg.diplomatie;
}

// -----------------------------------------------------------------------------
// Name: Side::SendCreation
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void Side::SendCreation( Publisher_ABC& publisher ) const
{
    AsnMsgSimToClientSideCreation asn;
    asn().oid  = nID_;
    asn().nom  = strName_.c_str();
    asn().type = nType_;
    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: Side::SendFullUpdate
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void Side::SendFullUpdate( Publisher_ABC& publisher ) const
{
    for( CIT_DiplomacyMap it = diplomacies_.begin(); it != diplomacies_.end(); ++it )
    {
        AsnMsgSimToClientChangeDiplomatie asn;
        asn().oid_camp1  = nID_;
        asn().oid_camp2  = it->first->GetID();
        asn().diplomatie = it->second;
        asn.Send( publisher );
    }
}

// -----------------------------------------------------------------------------
// Name: Side::Accept
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void Side::Accept( ModelVisitor_ABC& visitor )
{
    visitor.Visit( *this );
    knowledgeGroups_.Apply( std::mem_fun_ref( &KnowledgeGroup::Accept ), visitor );
	formations_		.Apply( std::mem_fun_ref( &Formation     ::Accept ), visitor );
	objects_		.Apply( std::mem_fun_ref( &Object        ::Accept ), visitor );
	populations_	.Apply( std::mem_fun_ref( &Population    ::Accept ), visitor );
}
