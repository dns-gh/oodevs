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
LogisticRouteObjectAttribute::LogisticRouteObjectAttribute( const Model& model, const ASN1T_MsgObjectCreation& asnMsg )
    : ObjectAttribute_ABC( model, asnMsg )
    , bEquipped_         ( false )
    , nMaxWeight_        ( 0 )
    , nWidth_            ( 0 )
    , nLength_           ( 0 )
    , nFlow_             ( 0 )
{
    if( !asnMsg.m.attributs_specifiquesPresent )
        return;

    if( asnMsg.attributs_specifiques.t == T_AttrObjectSpecific_itineraire_logistique )
    {
        bEquipped_  = asnMsg.attributs_specifiques.u.itineraire_logistique->itineraire_equipe;
        nMaxWeight_ = asnMsg.attributs_specifiques.u.itineraire_logistique->poids_max_supporte;
        nWidth_     = asnMsg.attributs_specifiques.u.itineraire_logistique->largeur;
        nLength_    = asnMsg.attributs_specifiques.u.itineraire_logistique->longueur;
        nFlow_      = asnMsg.attributs_specifiques.u.itineraire_logistique->debit;
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
void LogisticRouteObjectAttribute::Update( const ASN1T_MsgObjectUpdate& asnMsg )
{
    if( !asnMsg.m.attributs_specifiquesPresent )
        return;

    if( asnMsg.attributs_specifiques.t == T_AttrObjectSpecific_itineraire_logistique )
    {
        bEquipped_  = asnMsg.attributs_specifiques.u.itineraire_logistique->itineraire_equipe;
        nMaxWeight_ = asnMsg.attributs_specifiques.u.itineraire_logistique->poids_max_supporte;
        nWidth_     = asnMsg.attributs_specifiques.u.itineraire_logistique->largeur;
        nLength_    = asnMsg.attributs_specifiques.u.itineraire_logistique->longueur;
        nFlow_      = asnMsg.attributs_specifiques.u.itineraire_logistique->debit;
    }
}
