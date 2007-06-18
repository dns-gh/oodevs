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
CrossingSiteObjectAttribute::CrossingSiteObjectAttribute( const Model& model, const ASN1T_ObjectAttributesSpecific& asnMsg )
    : ObjectAttribute_ABC( model, asnMsg )
    , nWidth_            ( 0 )
    , nDepth_            ( 0 )
    , nSpeed_            ( 0 )
    , bNeedsConstruction_( false ) 
{
    if( asnMsg.t == T_ObjectAttributesSpecific_site_franchissement )
    {
        nWidth_             = asnMsg.u.site_franchissement->largeur;
        nDepth_             = asnMsg.u.site_franchissement->profondeur;
        nSpeed_             = asnMsg.u.site_franchissement->vitesse_courant;
        bNeedsConstruction_ = asnMsg.u.site_franchissement->berges_a_amenager;
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
void CrossingSiteObjectAttribute::Update( const ASN1T_ObjectAttributesSpecific& asnMsg )
{
    if( asnMsg.t == T_ObjectAttributesSpecific_site_franchissement )
    {
        nWidth_             = asnMsg.u.site_franchissement->largeur;
        nDepth_             = asnMsg.u.site_franchissement->profondeur;
        nSpeed_             = asnMsg.u.site_franchissement->vitesse_courant;
        bNeedsConstruction_ = asnMsg.u.site_franchissement->berges_a_amenager;
    }
}

// -----------------------------------------------------------------------------
// Name: CrossingSiteObjectAttribute::Send
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void CrossingSiteObjectAttribute::Send( ASN1T_ObjectAttributesSpecific& asnMsg ) const
{
    asnMsg.t = nType_;
    asnMsg.u.site_franchissement = new ASN1T_ObjectAttributesCrossingSite();
    
    asnMsg.u.site_franchissement->largeur           = nWidth_;
    asnMsg.u.site_franchissement->profondeur        = nDepth_;
    asnMsg.u.site_franchissement->vitesse_courant   = nSpeed_;
    asnMsg.u.site_franchissement->berges_a_amenager = bNeedsConstruction_;
}

// -----------------------------------------------------------------------------
// Name: CrossingSiteObjectAttribute::AsnDelete
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void CrossingSiteObjectAttribute::AsnDelete( ASN1T_ObjectAttributesSpecific& asnMsg ) const
{
    delete asnMsg.u.itineraire_logistique;
}
