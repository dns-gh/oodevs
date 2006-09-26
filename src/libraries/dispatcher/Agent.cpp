// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"

#include "Agent.h"

#include "Model.h"
#include "Automat.h"
#include "Equipment.h"
#include "Dotation.h"
#include "Humans.h"
#include "Loan.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Agent constructor
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
Agent::Agent( Model& model, const ASN1T_MsgPionCreation& msg )
    : model_                        ( model )
    , nID_                          ( msg.oid_pion )
    , nType_                        ( msg.type_pion )
    , strName_                      ( msg.nom )
    , automat_                      ( model.GetAutomats().Get( msg.oid_automate ) )
    , position_                     ()
    , nDirection_                   ( 0 )
    , nHeight_                      ( 0 )
    , nSpeed_                       ( 0 )
    , nOperationalStateValue_       ( 100 )
    , reinforcements_               ( )
    , pReinforced_                  ( 0 )
    , nAutomatState_                ( EnumAutomateState::embraye )  //$$$ Séparation pions/automates
    , bDead_                        ( false )
    , bNeutralized_                 ( false )
    , bStealthModeEnabled_          ( false )
    , bLoaded_                      ( false )
    , bHumanTransportersAvailable_  ( false )
    , nLastPosture_                 ( EnumUnitPosture::posture_arret )
    , nCurrentPosture_              ( EnumUnitPosture::posture_arret )
    , nPostureCompletion_           ( 100 )
    , nInstallationState_           ( 0 )
    , bNbcProtectionSuitEnabled_    ( false )
    , nbcAgentTypesContaminating_   ()
    , nContaminationState_          ( 0 )
    , bCommunicationJammed_         ( false )
    , bBlackoutEnabled_             ( false )
    , bRadarEnabled_                ( false )
    , transportedAgents_            ()
    , pTransporter_                 ( 0 )
    , nForceRatioState_             ( EnumEtatRapFor         ::neutre )
    , nCloseCombatState_            ( EnumEtatCombatRencontre::etat_esquive )
    , nOperationalState_            ( EnumEtatOperationnel   ::operationnel )
    , nIndirectFireAvailability_    ( EnumDisponibiliteAuTir ::indisponible )
    , nRoe_                         ( EnumRoe                ::tir_libre )
    , nRoePopulation_               ( EnumRoePopulation      ::emploi_force_interdit )
    , nTiredness_                   ( EnumUnitFatigue        ::normal )
    , nMorale_                      ( EnumUnitMoral          ::bon )
    , nExperience_                  ( EnumUnitExperience     ::experimente )
    , bSurrendered_                 ( false )
    , bPrisonner_                   ( false )
    , bRefugeeManaged_              ( false )
    , equipments_                   ()
    , troops_                       ()
    , dotations_                    ()
    , borrowings_                   ()
    , lendings_                     ()
{
    automat_.GetAgents().Register( *this );
}

// -----------------------------------------------------------------------------
// Name: Agent destructor
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
Agent::~Agent()
{
    automat_.GetAgents().Unregister( *this );
}

// =============================================================================
// MAIN
// =============================================================================

#define UPDATE_ASN_ATTRIBUTE( ASN, CPP ) \
    if( asnMsg.m.##ASN##Present )        \
        CPP = asnMsg.##ASN##;

// -----------------------------------------------------------------------------
// Name: Agent::Update
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void Agent::Update( const ASN1T_MsgUnitAttributes& asnMsg )
{
    UPDATE_ASN_ATTRIBUTE( position , position_   );
    UPDATE_ASN_ATTRIBUTE( direction, nDirection_ );
    UPDATE_ASN_ATTRIBUTE( hauteur  , nHeight_    );
    UPDATE_ASN_ATTRIBUTE( vitesse  , nSpeed_     );
    UPDATE_ASN_ATTRIBUTE( etat_operationnel_brut, nOperationalStateValue_ );

    if( asnMsg.m.pions_renforcantPresent )
    {
        reinforcements_.Clear();
        for( uint i = 0; i < asnMsg.pions_renforcant.n; ++i )
            reinforcements_.Register( model_.GetAgents().Get( asnMsg.pions_renforcant.elem[ i ] ) );
    }

    if( asnMsg.m.pion_renforcePresent )
        pReinforced_ = asnMsg.pion_renforce == 0 ? 0 : &model_.GetAgents().Get( asnMsg.pion_renforce );

    UPDATE_ASN_ATTRIBUTE( etat_automate, nAutomatState_ );
    UPDATE_ASN_ATTRIBUTE( mort, bDead_ );
    UPDATE_ASN_ATTRIBUTE( neutralise,  bNeutralized_ );
    UPDATE_ASN_ATTRIBUTE( mode_furtif_actif, bStealthModeEnabled_ );
    UPDATE_ASN_ATTRIBUTE( embarque, bLoaded_ );
    UPDATE_ASN_ATTRIBUTE( transporteurs_disponibles, bHumanTransportersAvailable_ );
    UPDATE_ASN_ATTRIBUTE( posture_old, nLastPosture_ );
    UPDATE_ASN_ATTRIBUTE( posture_new, nCurrentPosture_ );
    UPDATE_ASN_ATTRIBUTE( posture_pourcentage, nPostureCompletion_ );
    UPDATE_ASN_ATTRIBUTE( etat_installation, nInstallationState_ );
    UPDATE_ASN_ATTRIBUTE( en_tenue_de_protection_nbc, bNbcProtectionSuitEnabled_ );

    if( asnMsg.m.contamine_par_agents_nbcPresent )
    {
        nbcAgentTypesContaminating_.clear();
        for( uint i = 0; i < asnMsg.contamine_par_agents_nbc.n; ++i )
            nbcAgentTypesContaminating_.push_back( asnMsg.contamine_par_agents_nbc.elem[ i ] );
    }

    UPDATE_ASN_ATTRIBUTE( etat_contamination, nContaminationState_ );
    UPDATE_ASN_ATTRIBUTE( communications_brouillees, bCommunicationJammed_ );
    UPDATE_ASN_ATTRIBUTE( silence_radio, bBlackoutEnabled_ );
    UPDATE_ASN_ATTRIBUTE( radar_actif, bRadarEnabled_ );

    if( asnMsg.m.pions_transportesPresent )
    {
        transportedAgents_.Clear();
        for( uint i = 0; i < asnMsg.pions_transportes.n; ++i )
            transportedAgents_.Register( model_.GetAgents().Get( asnMsg.pions_transportes.elem[ i ] ) );
    }

    if( asnMsg.m.pion_transporteurPresent )
        pTransporter_ = asnMsg.pion_transporteur == 0 ? 0 : &model_.GetAgents().Get( asnMsg.pion_transporteur );

    UPDATE_ASN_ATTRIBUTE( rapport_de_force, nForceRatioState_ );
    UPDATE_ASN_ATTRIBUTE( combat_de_rencontre, nCloseCombatState_ );
    UPDATE_ASN_ATTRIBUTE( etat_operationnel, nOperationalState_ );
    UPDATE_ASN_ATTRIBUTE( disponibilite_au_tir_indirect, nIndirectFireAvailability_ );
    UPDATE_ASN_ATTRIBUTE( roe, nRoe_ );
    UPDATE_ASN_ATTRIBUTE( roe_population, nRoePopulation_ );
    UPDATE_ASN_ATTRIBUTE( fatigue, nTiredness_ );
    UPDATE_ASN_ATTRIBUTE( moral, nMorale_ );
    UPDATE_ASN_ATTRIBUTE( experience, nExperience_ );
    UPDATE_ASN_ATTRIBUTE( rendu, bSurrendered_ );
    UPDATE_ASN_ATTRIBUTE( prisonnier, bPrisonner_ );
    UPDATE_ASN_ATTRIBUTE( refugie_pris_en_compte, bRefugeeManaged_ );
}

// -----------------------------------------------------------------------------
// Name: Agent::Update
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void Agent::Update( const ASN1T_MsgUnitDotations& asnMsg )
{
    if( asnMsg.m.dotation_eff_materielPresent )
    {
        for( uint i = 0; i < asnMsg.dotation_eff_materiel.n; ++i )
        {
            const ASN1T_DotationEquipement& asn = asnMsg.dotation_eff_materiel.elem[ i ];
            Equipment* pEquipment = equipments_.Find( asn.type_equipement );
            if( pEquipment )
                pEquipment->Update( asn );
            else
                pEquipment = &equipments_.Create( model_, asn.type_equipement, asn );
        }
    }

    if( asnMsg.m.dotation_eff_personnelPresent )
    {
        for( uint i = 0; i < asnMsg.dotation_eff_personnel.n; ++i )
        {
            const ASN1T_DotationPersonnel& asn = asnMsg.dotation_eff_personnel.elem[ i ];
            Humans* pHumans = troops_.Find( asn.rang );
            if( pHumans  )
                pHumans ->Update( asn );
            else
                pHumans  = &troops_.Create( model_, asn.rang, asn );
        }
    }

    if( asnMsg.m.dotation_eff_ressourcePresent )
    {
        for( uint i = 0; i < asnMsg.dotation_eff_ressource.n; ++i )
        {
            const ASN1T_DotationRessource& asn = asnMsg.dotation_eff_ressource.elem[ i ];
            Dotation* pDotation = dotations_.Find( asn.ressource_id );
            if( pDotation )
                pDotation->Update( asn );
            else
                pDotation = &dotations_.Create( model_, asn.ressource_id, asn );
        }
    }

    if( asnMsg.m.equipements_pretesPresent )
    {
        lendings_.clear();
        for( uint i = 0; i < asnMsg.equipements_pretes.n; ++i )
            lendings_.push_back( Loan( model_, asnMsg.equipements_pretes.elem[ i ] ) );
    }

    if( asnMsg.m.equipements_empruntesPresent )
    {
        borrowings_.clear();
        for( uint i = 0; i < asnMsg.equipements_empruntes.n; ++i )
            borrowings_.push_back( Loan( model_, asnMsg.equipements_empruntes.elem[ i ] ) );
    }
}
