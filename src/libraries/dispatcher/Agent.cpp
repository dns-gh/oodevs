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
#include "Side.h"
#include "AgentLogMedical.h"
#include "AgentLogMaintenance.h"
#include "AgentLogSupply.h"
#include "Network_Def.h"

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
    , pAutomat_                     ( &model.GetAutomats().Get( msg.oid_automate ) )
    , bPC_                          ( msg.pc )
    , position_                     ()
    , nDirection_                   ( 0 )
    , nHeight_                      ( 0 )
    , nSpeed_                       ( 0 )
    , nOperationalStateValue_       ( 100 )
    , reinforcements_               ( )
    , pReinforced_                  ( 0 )
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
    , pSideSurrenderedTo_           ( 0 )
    , bPrisonner_                   ( false )
    , bRefugeeManaged_              ( false )
    , equipments_                   ()
    , troops_                       ()
    , dotations_                    ()
    , borrowings_                   ()
    , lendings_                     ()
    , pLogMedical_                  ()
    , pLogMaintenance_              ()
    , pLogSupply_                   ()

{
    pAutomat_->GetAgents().Register( *this );
}

// -----------------------------------------------------------------------------
// Name: Agent destructor
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
Agent::~Agent()
{
    pAutomat_->GetAgents().Unregister( *this );
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

    if( asnMsg.m.renduPresent )
        pSideSurrenderedTo_ = asnMsg.rendu == 0 ? 0 : &model_.GetSides().Get( asnMsg.rendu );

    UPDATE_ASN_ATTRIBUTE( prisonnier, bPrisonner_ );
    UPDATE_ASN_ATTRIBUTE( refugie_pris_en_compte, bRefugeeManaged_ );

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
        lendings_.Clear();
        for( uint i = 0; i < asnMsg.equipements_pretes.n; ++i )
            lendings_.Create( model_, i, asnMsg.equipements_pretes.elem[ i ] );
    }

    if( asnMsg.m.equipements_empruntesPresent )
    {
        borrowings_.Clear();
        for( uint i = 0; i < asnMsg.equipements_empruntes.n; ++i )
            borrowings_.Create( model_, i, asnMsg.equipements_empruntes.elem[ i ] );
    }
}

// -----------------------------------------------------------------------------
// Name: Agent::Update
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void Agent::Update( const ASN1T_MsgLogSanteEtat& asnMsg )
{
    if( !pLogMedical_ )
        pLogMedical_ = new AgentLogMedical( model_, *this, asnMsg );
    else
        pLogMedical_->Update( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: Agent::Update
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void Agent::Update( const ASN1T_MsgLogMaintenanceEtat& asnMsg )
{
    if( !pLogMaintenance_ )
        pLogMaintenance_ = new AgentLogMaintenance( model_, *this, asnMsg );
    else
        pLogMaintenance_->Update( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: Agent::Update
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void Agent::Update( const ASN1T_MsgLogRavitaillementEtat& asnMsg )
{
    if( !pLogSupply_ )
        pLogSupply_ = new AgentLogSupply( model_, *this, asnMsg );
    else
        pLogSupply_->Update( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: Agent::Update
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void Agent::Update( const ASN1T_MsgChangeAutomate& asnMsg )
{
    pAutomat_->GetAgents().Unregister( *this );
    pAutomat_ = &model_.GetAutomats().Get( asnMsg.oid_automate );
    pAutomat_->GetAgents().Register( *this );
}
    
// -----------------------------------------------------------------------------
// Name: Agent::Update
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void Agent::Update( const ASN1T_MsgChangeAutomateAck& asnMsg )
{
    if( asnMsg.error_code == EnumChangeAutomateErrorCode::no_error )
    {
        pAutomat_->GetAgents().Unregister( *this );
        pAutomat_ = &model_.GetAutomats().Get( asnMsg.oid_automate );
        pAutomat_->GetAgents().Register( *this );
    }
}

// -----------------------------------------------------------------------------
// Name: Agent::SendCreation
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void Agent::SendCreation( Publisher_ABC& publisher ) const
{
    AsnMsgInClientPionCreation asn;
    asn().oid_pion      = nID_;
    asn().type_pion     = nType_;
    asn().nom           = strName_.c_str(); // !! pointeur sur const char*
    asn().oid_automate  = pAutomat_->GetID();
    asn().pc            = bPC_;
    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: Agent::SendFullUpdate
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void Agent::SendFullUpdate( Publisher_ABC& publisher ) const
{
    { // Attributes $$$
        AsnMsgInClientUnitAttributes asn;

        asn().oid_pion = nID_;     

        asn().m.dotation_eff_materielPresent = 1;
        asn().m.dotation_eff_personnelPresent = 1;
        asn().m.dotation_eff_ressourcePresent = 1;
        asn().m.equipements_empruntesPresent = 1;
        asn().m.equipements_pretesPresent = 1;
        asn().m.positionPresent = 1;
        asn().m.directionPresent = 1;
        asn().m.hauteurPresent = 1;
        asn().m.vitessePresent = 1;
        asn().m.etat_operationnel_brutPresent = 1;
        asn().m.pions_renforcantPresent = 1;
        asn().m.pion_renforcePresent = 1;
        asn().m.mortPresent = 1;
        asn().m.neutralisePresent = 1;
        asn().m.mode_furtif_actifPresent = 1;
        asn().m.embarquePresent = 1;
        asn().m.transporteurs_disponiblesPresent = 1;
        asn().m.posture_oldPresent = 1;
        asn().m.posture_newPresent = 1;
        asn().m.posture_pourcentagePresent = 1;
        asn().m.etat_installationPresent = 1;
        asn().m.en_tenue_de_protection_nbcPresent = 1;
        asn().m.contamine_par_agents_nbcPresent = 1;
        asn().m.etat_contaminationPresent = 1;
        asn().m.communications_brouilleesPresent = 1;
        asn().m.silence_radioPresent = 1;
        asn().m.radar_actifPresent = 1;
        asn().m.pions_transportesPresent = 1;
        asn().m.pion_transporteurPresent = 1;
        asn().m.rapport_de_forcePresent = 1;
        asn().m.combat_de_rencontrePresent = 1;
        asn().m.etat_operationnelPresent = 1;
        asn().m.disponibilite_au_tir_indirectPresent = 1;
        asn().m.roePresent = 1;
        asn().m.roe_populationPresent = 1;
        asn().m.fatiguePresent = 1;
        asn().m.moralPresent = 1;
        asn().m.experiencePresent = 1;
        asn().m.renduPresent = 1;
        asn().m.prisonnierPresent = 1;
        asn().m.refugie_pris_en_comptePresent = 1;

        position_.Send( asn().position );

        asn().direction = nDirection_;
        asn().hauteur = nHeight_;
        asn().vitesse = nSpeed_;
        asn().etat_operationnel_brut = nOperationalStateValue_;

        reinforcements_.Send< ASN1T_ListAgent, ASN1T_Agent >( asn().pions_renforcant );

        asn().pion_renforce = pReinforced_ ? pReinforced_->GetID() : 0 ;
        asn().mort = bDead_;
        asn().neutralise =  bNeutralized_;
        asn().mode_furtif_actif = bStealthModeEnabled_;
        asn().embarque = bLoaded_;
        asn().transporteurs_disponibles = bHumanTransportersAvailable_;
        asn().posture_old = nLastPosture_;
        asn().posture_new = nCurrentPosture_;
        asn().posture_pourcentage = nPostureCompletion_;
        asn().etat_installation = nInstallationState_;
        asn().en_tenue_de_protection_nbc = bNbcProtectionSuitEnabled_;

        SendContainerValues< ASN1T_ListOID, ASN1T_OID, T_OIDVector >( nbcAgentTypesContaminating_, asn().contamine_par_agents_nbc ); 

        asn().etat_contamination = nContaminationState_;
        asn().communications_brouillees = bCommunicationJammed_;
        asn().silence_radio = bBlackoutEnabled_;
        asn().radar_actif = bRadarEnabled_;

        transportedAgents_.Send< ASN1T_ListAgent, ASN1T_Agent >( asn().pions_transportes );

        asn().pion_transporteur = pTransporter_ ? pTransporter_->GetID() : 0;

        asn().rapport_de_force = nForceRatioState_;
        asn().combat_de_rencontre = nCloseCombatState_;
        asn().etat_operationnel = nOperationalState_;
        asn().disponibilite_au_tir_indirect = nIndirectFireAvailability_;
        asn().roe = nRoe_;
        asn().roe_population = nRoePopulation_;
        asn().fatigue = nTiredness_;
        asn().moral = nMorale_;
        asn().experience = nExperience_;
        asn().rendu = pSideSurrenderedTo_ ? 0 : pSideSurrenderedTo_->GetID();
        asn().prisonnier = bPrisonner_;
        asn().refugie_pris_en_compte = bRefugeeManaged_;

        equipments_ .Send< ASN1T__SeqOfDotationEquipement, ASN1T_DotationEquipement >( asn().dotation_eff_materiel  );
        troops_     .Send< ASN1T__SeqOfDotationPersonnel , ASN1T_DotationPersonnel  >( asn().dotation_eff_personnel );
        dotations_  .Send< ASN1T__SeqOfDotationRessource , ASN1T_DotationRessource  >( asn().dotation_eff_ressource );
        borrowings_ .Send< ASN1T__SeqOfEquipementEmprunte, ASN1T_EquipementEmprunte >( asn().equipements_empruntes  );
        lendings_   .Send< ASN1T__SeqOfEquipementPrete   , ASN1T_EquipementPrete    >( asn().equipements_pretes     );

        asn.Send( publisher );

        if( asn().m.pions_renforcantPresent && asn().pions_renforcant.n > 0 )
            delete [] asn().pions_renforcant.elem;
        if( asn().m.contamine_par_agents_nbcPresent && asn().contamine_par_agents_nbc.n > 0 )
            delete [] asn().contamine_par_agents_nbc.elem;
        if( asn().m.pions_transportesPresent && asn().pions_transportes.n > 0 )
            delete [] asn().pions_transportes.elem;
        if( asn().m.dotation_eff_materielPresent && asn().dotation_eff_materiel.n > 0 )
            delete [] asn().dotation_eff_materiel.elem;
        if( asn().m.dotation_eff_personnelPresent && asn().dotation_eff_personnel.n > 0 )
            delete [] asn().dotation_eff_personnel.elem;
        if( asn().m.dotation_eff_ressourcePresent && asn().dotation_eff_ressource.n > 0 )
            delete [] asn().dotation_eff_ressource.elem;
        if( asn().m.equipements_empruntesPresent && asn().equipements_empruntes.n > 0 )
            delete [] asn().equipements_empruntes.elem;
        if( asn().m.equipements_pretesPresent && asn().equipements_pretes.n > 0 )
            delete [] asn().equipements_pretes.elem;
    }

    // Log
    if( pLogMedical_ )
        pLogMedical_->Send( publisher );

    if( pLogMaintenance_ )
        pLogMaintenance_->Send( publisher );

    if( pLogSupply_ )
        pLogSupply_->Send( publisher );
}
