// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"

#include "Formation.h"

#include "Network_Def.h"
#include "Publisher_ABC.h"
#include "Model.h"
#include "Side.h"
#include "Automat.h"
#include "ModelVisitor_ABC.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Formation constructor
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
Formation::Formation( Model& model, const ASN1T_MsgFormationCreation& msg )
    : model_          ( model )
    , nID_            ( msg.oid )
    , side_           ( model.GetSides().Get( msg.oid_camp ) )
    , nLevel_         ( msg.niveau )
    , pParent_        ( msg.m.oid_formation_parentePresent ? &model.GetFormations().Get( msg.oid_formation_parente ) : 0 )
    , strName_        ( msg.nom )
    , automats_       ()
    , subordinates_   ()
{
    if( pParent_ )
        pParent_->GetSubordinates().Register( *this );
    else
        side_.GetFormations().Register( *this );
}

// -----------------------------------------------------------------------------
// Name: Formation destructor
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
Formation::~Formation()
{
    if( pParent_ )
        pParent_->GetSubordinates().Unregister( *this );
    else
        side_.GetFormations().Unregister( *this );
}

// =============================================================================
// MAIN
// =============================================================================

// -----------------------------------------------------------------------------
// Name: Formation::Update
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void Formation::Update( const ASN1T_MsgFormationCreation& )
{
    FlagUpdate();
}

// -----------------------------------------------------------------------------
// Name: Formation::SendCreation
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void Formation::SendCreation( Publisher_ABC& publisher ) const
{
    AsnMsgSimToClientFormationCreation asn;

    asn().oid      = nID_;
    asn().oid_camp = side_.GetID();
    asn().nom      = strName_.c_str();
    asn().niveau   = nLevel_;

    if( pParent_ )
    {
        asn().m.oid_formation_parentePresent = 1;
        asn().oid_formation_parente          = pParent_->GetID();
    }

    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: Formation::SendFullUpdate
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void Formation::SendFullUpdate( Publisher_ABC& ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Formation::Accept
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void Formation::Accept( ModelVisitor_ABC& visitor )
{
    visitor.Visit( *this );
    subordinates_.Apply( std::mem_fun_ref( &Formation::Accept ), visitor );
    automats_    .Apply( std::mem_fun_ref( &Automat  ::Accept ), visitor );
}
