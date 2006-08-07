// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_gaming_pch.h"
#include "LogisticRouteAttributes.h"
#include "astec_kernel/Controller.h"
#include "astec_kernel/Displayer_ABC.h"
#include "astec_kernel/Units.h"

// -----------------------------------------------------------------------------
// Name: LogisticRouteAttributes constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
LogisticRouteAttributes::LogisticRouteAttributes( Controller& controller )
    : controller_( controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticRouteAttributes destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
LogisticRouteAttributes::~LogisticRouteAttributes()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticRouteAttributes::UpdateData
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
template< typename T >
void LogisticRouteAttributes::UpdateData( const T& message )
{
    if( message.m.attributs_specifiquesPresent
     && message.attributs_specifiques.t == T_AttrObjectSpecific_itineraire_logistique )
    {
        nLogRouteFlow_      = message.attributs_specifiques.u.itineraire_logistique->debit;
        nLogRouteLength_    = message.attributs_specifiques.u.itineraire_logistique->longueur;
        nLogRouteWidth_     = message.attributs_specifiques.u.itineraire_logistique->largeur;
        nLogRouteMaxWeight_ = message.attributs_specifiques.u.itineraire_logistique->poids_max_supporte;
        bLogRouteEquipped_  = message.attributs_specifiques.u.itineraire_logistique->itineraire_equipe;
        controller_.Update( *this );
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticRouteAttributes::DoUpdate
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void LogisticRouteAttributes::DoUpdate( const ASN1T_MsgObjectKnowledgeUpdate& message )
{
    UpdateData( message );
}

// -----------------------------------------------------------------------------
// Name: LogisticRouteAttributes::DoUpdate
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void LogisticRouteAttributes::DoUpdate( const ASN1T_MsgObjectUpdate& message )
{
    UpdateData( message );
}

// -----------------------------------------------------------------------------
// Name: LogisticRouteAttributes::DoUpdate
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void LogisticRouteAttributes::DoUpdate( const ASN1T_MsgObjectCreation& message )
{
    UpdateData( message );
}

// -----------------------------------------------------------------------------
// Name: LogisticRouteAttributes::Display
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
void LogisticRouteAttributes::Display( Displayer_ABC& displayer ) const
{
    
    displayer.Group( "Itinéraire logistique" )
                .Display( "Equipé:", bLogRouteEquipped_ )
                .Display( "Débit:", nLogRouteFlow_ * Units::vehiclesPerHour )
                .Display( "Largeur:", nLogRouteWidth_ * Units::meters )
                .Display( "Longueur:", nLogRouteLength_ * Units::meters )
                .Display( "Poids supporté:", nLogRouteMaxWeight_ * Units::tons );
}
