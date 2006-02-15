// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "LogisticRouteAttributes.h"
#include "App.h"
#include "World.h"

// -----------------------------------------------------------------------------
// Name: LogisticRouteAttributes constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
LogisticRouteAttributes::LogisticRouteAttributes()
    : set_( false )
{

}

// -----------------------------------------------------------------------------
// Name: LogisticRouteAttributes destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
LogisticRouteAttributes::~LogisticRouteAttributes()
{

}

// -----------------------------------------------------------------------------
// Name: LogisticRouteAttributes::DoUpdate
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
template< typename T >
void LogisticRouteAttributes::DoUpdate( const T& message )
{
    if( message.m.attributs_specifiquesPresent
     && message.attributs_specifiques.t == T_AttrObjectSpecific_itineraire_logistique )
    {
        set_ = true;
        nLogRouteFlow_      = message.attributs_specifiques.u.itineraire_logistique->debit;
        nLogRouteLength_    = message.attributs_specifiques.u.itineraire_logistique->longueur;
        nLogRouteWidth_     = message.attributs_specifiques.u.itineraire_logistique->largeur;
        nLogRouteMaxWeight_ = message.attributs_specifiques.u.itineraire_logistique->poids_max_supporte;
        bLogRouteEquipped_  = message.attributs_specifiques.u.itineraire_logistique->itineraire_equipe;
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticRouteAttributes::Update
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void LogisticRouteAttributes::Update( const ASN1T_MsgObjectKnowledgeUpdate& message )
{
    DoUpdate( message );
}

// -----------------------------------------------------------------------------
// Name: LogisticRouteAttributes::Update
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void LogisticRouteAttributes::Update( const ASN1T_MsgObjectUpdate& message )
{
    DoUpdate( message );
}

// -----------------------------------------------------------------------------
// Name: LogisticRouteAttributes::Update
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void LogisticRouteAttributes::Update( const ASN1T_MsgObjectCreation& message )
{
    DoUpdate( message );
}
