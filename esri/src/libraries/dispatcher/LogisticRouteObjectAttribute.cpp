// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"

#include "LogisticRouteObjectAttribute.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: LogisticRouteObjectAttribute constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
LogisticRouteObjectAttribute::LogisticRouteObjectAttribute( const Model& model, const ASN1T_AttrObjectSpecific& asnMsg )
    : ObjectAttribute_ABC( model, asnMsg )
    , bEquipped_         ( false )
    , nMaxWeight_        ( 0 )
    , nWidth_            ( 0 )
    , nLength_           ( 0 )
    , nFlow_             ( 0 )
{
    if( asnMsg.t == T_AttrObjectSpecific_itineraire_logistique )
    {
        bEquipped_  = asnMsg.u.itineraire_logistique->itineraire_equipe;
        nMaxWeight_ = asnMsg.u.itineraire_logistique->poids_max_supporte;
        nWidth_     = asnMsg.u.itineraire_logistique->largeur;
        nLength_    = asnMsg.u.itineraire_logistique->longueur;
        nFlow_      = asnMsg.u.itineraire_logistique->debit;
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticRouteObjectAttribute destructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
LogisticRouteObjectAttribute::~LogisticRouteObjectAttribute()
{

}  

// -----------------------------------------------------------------------------
// Name: LogisticRouteObjectAttribute::Update
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void LogisticRouteObjectAttribute::Update( const ASN1T_AttrObjectSpecific& asnMsg )
{
    if( asnMsg.t == T_AttrObjectSpecific_itineraire_logistique )
    {
        bEquipped_  = asnMsg.u.itineraire_logistique->itineraire_equipe;
        nMaxWeight_ = asnMsg.u.itineraire_logistique->poids_max_supporte;
        nWidth_     = asnMsg.u.itineraire_logistique->largeur;
        nLength_    = asnMsg.u.itineraire_logistique->longueur;
        nFlow_      = asnMsg.u.itineraire_logistique->debit;
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticRouteObjectAttribute::Send
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void LogisticRouteObjectAttribute::Send( ASN1T_AttrObjectSpecific& asnMsg ) const
{
    asnMsg.t = nType_;
    asnMsg.u.itineraire_logistique = new ASN1T_AttrObjectItineraireLogistique();
    
    asnMsg.u.itineraire_logistique->itineraire_equipe  = bEquipped_;
    asnMsg.u.itineraire_logistique->poids_max_supporte = nMaxWeight_;
    asnMsg.u.itineraire_logistique->largeur            = nWidth_;
    asnMsg.u.itineraire_logistique->longueur           = nLength_;
    asnMsg.u.itineraire_logistique->debit              = nFlow_;
}

// -----------------------------------------------------------------------------
// Name: LogisticRouteObjectAttribute::AsnDelete
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void LogisticRouteObjectAttribute::AsnDelete( ASN1T_AttrObjectSpecific& asnMsg ) const
{
    delete asnMsg.u.itineraire_logistique;
}

