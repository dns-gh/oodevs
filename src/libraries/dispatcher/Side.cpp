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

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Side constructor
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
Side::Side( Model& model, const ASN1T_MsgSideCreation& msg )
    : model_          ( model )
    , nID_            ( msg.oid )
    , strName_        ( msg.nom )
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
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void Side::Update( const ASN1T_MsgChangeDiplomatie& asnMsg )
{
    Side& side = model_.GetSides().Get( asnMsg.oid_camp2 );   
    diplomacies_[ &side ] = asnMsg.diplomatie;
}

// -----------------------------------------------------------------------------
// Name: Side::Update
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void Side::Update( const ASN1T_MsgChangeDiplomatieAck& asnMsg )
{
    Side& side = model_.GetSides().Get( asnMsg.oid_camp2 );   
    diplomacies_[ &side ] = asnMsg.diplomatie;
}

// -----------------------------------------------------------------------------
// Name: Side::SendCreation
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void Side::SendCreation( Publisher_ABC& publisher ) const
{
    AsnMsgInClientSideCreation asn;

    asn().oid = nID_;
    asn().nom = strName_.c_str();
    asn.Send( publisher );

	knowledgeGroups_.Apply( std::mem_fun_ref( &KnowledgeGroup::SendCreation ), publisher );
	formations_		.Apply( std::mem_fun_ref( &Formation     ::SendCreation ), publisher );
	objects_		.Apply( std::mem_fun_ref( &Object        ::SendCreation ), publisher );
	populations_	.Apply( std::mem_fun_ref( &Population    ::SendCreation ), publisher );
}

// -----------------------------------------------------------------------------
// Name: Side::SendFullUpdate
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void Side::SendFullUpdate( Publisher_ABC& publisher ) const
{
    for( CIT_DiplomacyMap it = diplomacies_.begin(); it != diplomacies_.end(); ++it )
    {
        AsnMsgInClientChangeDiplomatie asn;
        asn().oid_camp1  = nID_;
        asn().oid_camp2  = it->first->GetID();
        asn().diplomatie = it->second;
        asn.Send( publisher );
    }

	knowledgeGroups_.Apply( std::mem_fun_ref( &KnowledgeGroup::SendFullUpdate ), publisher );
	formations_		.Apply( std::mem_fun_ref( &Formation     ::SendFullUpdate ), publisher );
	objects_		.Apply( std::mem_fun_ref( &Object        ::SendFullUpdate ), publisher );
	populations_	.Apply( std::mem_fun_ref( &Population    ::SendFullUpdate ), publisher );
}
