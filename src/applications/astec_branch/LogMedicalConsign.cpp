// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"

#include "LogMedicalConsign.h"
#include "Agent.h"
#include "LogisticConsigns.h"
#include "Controller.h"
#include "Displayer_ABC.h"

// -----------------------------------------------------------------------------
// Name: LogMedicalConsign constructor
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
LogMedicalConsign::LogMedicalConsign( Controller& controller, const Resolver_ABC< Agent >& resolver, const ASN1T_MsgLogSanteTraitementHumainCreation& message )
    : controller_      ( controller )
    , resolver_        ( resolver )
    , nID_             ( message.oid_consigne )
    , pion_            ( resolver_.Get( message.oid_pion ) )
    , pPionLogHandling_( 0 )
    , wound_           ( E_HumanWound( message.blessure ) )
    , bMentalDeceased_ ( message.blesse_mental )
    , bContaminated_   ( message.contamine_nbc )
    , diagnosed_       ( false )
    , nState_          ( eLogSanteTraitementEtat_Termine )
{
    pion_.Get< LogisticConsigns >().AddConsign( *this );
}

// -----------------------------------------------------------------------------
// Name: LogMedicalConsign destructor
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
LogMedicalConsign::~LogMedicalConsign()
{
    pion_.Get< LogisticConsigns >().RemoveConsign( *this );
    if( pPionLogHandling_ )
        pPionLogHandling_->Get< LogisticConsigns >().TerminateConsign( *this );
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: LogMedicalConsign::Update
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void LogMedicalConsign::Update( const ASN1T_MsgLogSanteTraitementHumainUpdate& message )
{
    if( message.m.oid_pion_log_traitantPresent )
    {
        if( pPionLogHandling_ )
            pPionLogHandling_->Get< LogisticConsigns >().TerminateConsign( *this );
        pPionLogHandling_ = resolver_.Find( message.oid_pion_log_traitant );
        if( pPionLogHandling_ )
            pPionLogHandling_->Get< LogisticConsigns >().HandleConsign( *this );
    }

    if( message.m.blesse_mentalPresent )
        bMentalDeceased_ = message.blesse_mental;
    if( message.m.contamine_nbcPresent )
        bContaminated_   = message.contamine_nbc;
    if( message.m.blessurePresent )
        wound_ = E_HumanWound( message.blessure );
    if( message.m.etatPresent )
        nState_ = E_LogSanteTraitementEtat( message.etat );
    if( message.m.diagnostique_effectuePresent )
        diagnosed_ = message.diagnostique_effectue;

    controller_.Update( *this );
}

// $$$$ AGE 2006-02-28: remove !

// -----------------------------------------------------------------------------
// Name: LogMedicalConsign::GetWoundString
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
const char* LogMedicalConsign::GetWoundString() const
{
    switch( wound_ )
    {
        case EnumHumanWound::non_blesse: return "Non blessé";
        case EnumHumanWound::mort    : return "Mort";
        case EnumHumanWound::blesse_urgence_1 : return "U1";
        case EnumHumanWound::blesse_urgence_2 : return "U2";
        case EnumHumanWound::blesse_urgence_3 : return "U3";
        case EnumHumanWound::blesse_urgence_extreme : return "UE";
        default: return "";
    }
}
// -----------------------------------------------------------------------------
// Name: LogMedicalConsign::GetStateString
// Created: HME 2006-01-30
// -----------------------------------------------------------------------------
const char* LogMedicalConsign::GetStateString() const
{
    switch( nState_ )
    {
        case EnumLogSanteTraitementEtat::attente_disponibilite_ambulance_releve:
            return "En attente d'évacuation";
        case EnumLogSanteTraitementEtat::ambulance_releve_deplacement_aller:
            return "Ambulance en route";
        case EnumLogSanteTraitementEtat::ambulance_releve_chargement:
            return "Ambulance en cours de chargement";
        case EnumLogSanteTraitementEtat::attente_chargement_complet_ambulance_releve:
            return "Ambulance en attente de fin de chargement";
        case EnumLogSanteTraitementEtat::ambulance_releve_deplacement_retour:
            return "Ambulance en retour";
        case EnumLogSanteTraitementEtat::ambulance_releve_dechargement:
            return "Ambulance en cours de déchargement";
        case EnumLogSanteTraitementEtat::attente_disponibilite_medecin_pour_diagnostique:
            return "En attente de diagnostique";
        case EnumLogSanteTraitementEtat::diagnostique:
            return "Diagnostique en cours";
        case EnumLogSanteTraitementEtat::recherche_secteur_tri:
            return "Recherche d'un secteur de tri";
        case EnumLogSanteTraitementEtat::attente_disponibilite_medecin_pour_tri:
            return "En attente de tri";
        case EnumLogSanteTraitementEtat::tri:
            return "Tri en cours";
        case EnumLogSanteTraitementEtat::recherche_secteur_soin:
            return "Recherche d'un secteur de soin";
        case EnumLogSanteTraitementEtat::attente_disponibilite_medecin_pour_soin:
            return "En attente de soins";
        case EnumLogSanteTraitementEtat::soin:
            return "Soins en cours";
        case EnumLogSanteTraitementEtat::attente_disponibilite_ambulance_ramassage:
            return "En attente de ramassage";
        case EnumLogSanteTraitementEtat::ambulance_ramassage_chargement:
            return "Ramassage en cours";
        case EnumLogSanteTraitementEtat::attente_chargement_complet_ambulance_ramassage:
            return "En attente de fin de ramassage";
        case EnumLogSanteTraitementEtat::ambulance_ramassage_deplacement_aller:
            return "Ramassage en route";
        case EnumLogSanteTraitementEtat::ambulance_ramassage_dechargement:
            return "Dechargement du ramassage en cours";
        case EnumLogSanteTraitementEtat::termine:
            return "Terminé";
        default:
            return "";
    }
}

// -----------------------------------------------------------------------------
// Name: LogMedicalConsign::Display
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
void LogMedicalConsign::Display( Displayer_ABC& displayer ) const
{
    displayer.Display( "Consigne :", nID_ )
             .Display( "Pion demandeur :", pion_ )
             .Display( "Pion traitant :", pPionLogHandling_ )
             .Display( "Blessure :", wound_ )// $$$$ AGE 2006-03-21: only if diagnosed
             .Display( "Reac. mental :", bMentalDeceased_ )
             .Display( "Contaminé NBC :", bContaminated_ )
             .Display( "Etat :", nState_ );
}
