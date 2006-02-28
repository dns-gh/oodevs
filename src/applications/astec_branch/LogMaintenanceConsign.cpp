// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: NLD 2004-03-18 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/LogMaintenanceConsign.cpp $
// $Author: Age $
// $Modtime: 6/04/05 10:54 $
// $Revision: 2 $
// $Workfile: LogMaintenanceConsign.cpp $
//
// *****************************************************************************

#include "astec_pch.h"
#include "LogMaintenanceConsign.h"
#include "Agent.h"
#include "LogisticConsigns.h"
#include "Controller.h"
#include "Displayer_ABC.h"
#include "ValuedListItem.h"

// -----------------------------------------------------------------------------
// Name: LogMaintenanceConsign constructor
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
LogMaintenanceConsign::LogMaintenanceConsign( Controller& controller, const Resolver_ABC< Agent >& resolver, const ASN1T_MsgLogMaintenanceTraitementEquipementCreation& message )
    : controller_( controller )
    , resolver_( resolver )
    , nID_             ( message.oid_consigne )
    , pion_            ( resolver_.Get( message.oid_pion ) )
    , pPionLogHandling_( 0 )
    , nEquipmentTypeID_( message.type_equipement )
    , nBreakdownTypeID_( message.type_panne )
    , nState_          ( EnumLogMaintenanceTraitementEtat::termine )
{
    pion_.Get< LogisticConsigns >().AddConsign( *this );
}

// -----------------------------------------------------------------------------
// Name: LogMaintenanceConsign destructor
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
LogMaintenanceConsign::~LogMaintenanceConsign()
{
    pion_.Get< LogisticConsigns >().RemoveConsign( *this );
    if( pPionLogHandling_ )
        pPionLogHandling_->Get< LogisticConsigns >().TerminateConsign( *this );
}


// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: LogMaintenanceConsign::Update
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void LogMaintenanceConsign::Update( const ASN1T_MsgLogMaintenanceTraitementEquipementUpdate& message )
{
    if( pPionLogHandling_ )
        pPionLogHandling_->Get< LogisticConsigns >().TerminateConsign( *this );
    pPionLogHandling_ = resolver_.Find( message.oid_pion_log_traitant );
    if( pPionLogHandling_ )
        pPionLogHandling_->Get< LogisticConsigns >().HandleConsign( *this );
    nState_ = message.etat;
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: LogMaintenanceConsign::GetStateString
// Created: HME 2006-01-30
// -----------------------------------------------------------------------------
const char* LogMaintenanceConsign::GetStateString() const
{
    switch( nState_ )
    {
        case EnumLogMaintenanceTraitementEtat::deplacement_vers_chaine:
            return "En déplacement vers la chaine";
        case EnumLogMaintenanceTraitementEtat::attente_disponibilite_remorqueur: 
            return "En attente d'un remorqueur";
        case EnumLogMaintenanceTraitementEtat::remorqueur_deplacement_aller: 
            return "Remorqueur en route";
        case EnumLogMaintenanceTraitementEtat::remorqueur_chargement: 
            return "Remorqueur en cours de chargement";
        case EnumLogMaintenanceTraitementEtat::remorqueur_deplacement_retour: 
            return "Remorqueur en retour";
        case EnumLogMaintenanceTraitementEtat::remorqueur_dechargement: 
            return "Remorqueur en cours de déchargement";
        case EnumLogMaintenanceTraitementEtat::diagnostique: 
            return "Diagnostique en cours";
        case EnumLogMaintenanceTraitementEtat::attente_prise_en_charge_par_niveau_superieur: 
            return "En attente de prise en charge par le niveau supérieur";
        case EnumLogMaintenanceTraitementEtat::attente_disponibilite_pieces: 
            return "En attente de pièces";
        case EnumLogMaintenanceTraitementEtat::attente_disponibilite_reparateur: 
            return "En attente d'un réparateur";
        case EnumLogMaintenanceTraitementEtat::reparation: 
            return "En cours de réparation";
        case EnumLogMaintenanceTraitementEtat::attente_retour_pion: 
            return "En attente de retour";
        case EnumLogMaintenanceTraitementEtat::retour_pion: 
            return "Retour en cours";
        case EnumLogMaintenanceTraitementEtat::termine: 
            return "Terminé";
        default:
            return "";
    }
}

// -----------------------------------------------------------------------------
// Name: LogMaintenanceConsign::Display
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
void LogMaintenanceConsign::Display( Displayer_ABC& displayer ) const
{
    displayer.Display( "Consigne :", nID_ )
             .Display( "Pion demandeur :", pion_ )
             .Display( "Pion traitant :", pPionLogHandling_ )
             .Display( "Type d'équipement :", nEquipmentTypeID_ )
             .Display( "Type de panne :", nBreakdownTypeID_ )
             .Display( "Etat :", nState_ );
}
