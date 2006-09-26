// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: NLD 2006-09-25 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#include "dispatcher_pch.h"

#include "Automat.h"

#include "Side.h"
#include "Model.h"
#include "KnowledgeGroup.h"

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

