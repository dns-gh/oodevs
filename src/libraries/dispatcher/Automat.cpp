// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"

#include "Automat.h"

#include "Side.h"
#include "Model.h"
#include "KnowledgeGroup.h"
#include "Network_Def.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Automat constructor
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
Automat::Automat( Model& model, const ASN1T_MsgAutomateCreation& msg )
    : nID_           ( msg.oid_automate )
    , nType_         ( msg.type_automate )
    , strName_       ( msg.nom )
    , side_          ( model.GetSides          ().Get( msg.oid_camp ) )
    , knowledgeGroup_( model.GetKnowledgeGroups().Get( msg.oid_groupe_connaissance ) )
    , agents_        ()
{
    /*
    oid-tc2                  Automate OPTIONAL, $$$
    oid-maintenance          Automate OPTIONAL, $$$
    oid-sante                Automate OPTIONAL, $$$
    oid-ravitaillement       Automate OPTIONAL  $$$
    */

    knowledgeGroup_.GetAutomats().Register( *this );
    side_          .GetAutomats().Register( *this );
}

// -----------------------------------------------------------------------------
// Name: Automat destructor
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
Automat::~Automat()
{
    knowledgeGroup_.GetAutomats().Unregister( *this );
    side_          .GetAutomats().Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: Automat::SendCreation
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void Automat::SendCreation( Publisher_ABC& publisher ) const
{
    AsnMsgInClientAutomateCreation asn;
    asn().oid_automate            = nID_;
    asn().type_automate           = nType_;
    asn().nom                     = strName_.c_str(); // !! pointeur sur const char*
    asn().oid_camp                = side_.GetID();
    asn().oid_groupe_connaissance = knowledgeGroup_.GetID();
//    asn().oid_tc2;
//    asn().oid_maintenance;
//    asn().oid_sante;
//    asn().oid_ravitaillement;
    asn.Send( publisher );
}
