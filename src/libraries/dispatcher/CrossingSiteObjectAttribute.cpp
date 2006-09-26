// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"

#include "CrossingSiteObjectAttribute.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: CrossingSiteObjectAttribute constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
CrossingSiteObjectAttribute::CrossingSiteObjectAttribute( const Model& model, const ASN1T_MsgObjectCreation& asnMsg )
    : ObjectAttribute_ABC( model, asnMsg )
    , nWidth_            ( 0 )
    , nDepth_            ( 0 )
    , nSpeed_            ( 0 )
    , bNeedsConstruction_( false ) 
{
    if( !asnMsg.m.attributs_specifiquesPresent )
        return;

    if( asnMsg.attributs_specifiques.t == T_AttrObjectSpecific_site_franchissement )
    {
        nWidth_             = asnMsg.attributs_specifiques.u.site_franchissement->largeur;
        nDepth_             = asnMsg.attributs_specifiques.u.site_franchissement->profondeur;
        nSpeed_             = asnMsg.attributs_specifiques.u.site_franchissement->vitesse_courant;
        bNeedsConstruction_ = asnMsg.attributs_specifiques.u.site_franchissement->berges_a_amenager;
    }
}

// -----------------------------------------------------------------------------
// Name: CrossingSiteObjectAttribute destructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
CrossingSiteObjectAttribute::~CrossingSiteObjectAttribute()
{

}  

// -----------------------------------------------------------------------------
// Name: CrossingSiteObjectAttribute::Update
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void CrossingSiteObjectAttribute::Update( const ASN1T_MsgObjectUpdate& asnMsg )
{
    if( !asnMsg.m.attributs_specifiquesPresent )
        return;

    if( asnMsg.attributs_specifiques.t == T_AttrObjectSpecific_site_franchissement )
    {
        nWidth_             = asnMsg.attributs_specifiques.u.site_franchissement->largeur;
        nDepth_             = asnMsg.attributs_specifiques.u.site_franchissement->profondeur;
        nSpeed_             = asnMsg.attributs_specifiques.u.site_franchissement->vitesse_courant;
        bNeedsConstruction_ = asnMsg.attributs_specifiques.u.site_franchissement->berges_a_amenager;
    }
}
