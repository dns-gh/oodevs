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
#include "AgentOrder.h"
#include "ClientPublisher_ABC.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/ModelVisitor_ABC.h"

#include "protocol/clientsenders.h"

////using namespace Common;
//using namespace MsgsSimToClient;

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Agent constructor
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
Agent::Agent( Model& model, const MsgsSimToClient::MsgUnitCreation& msg )
    : SimpleEntity< kernel::Agent_ABC >( msg.oid(), QString(msg.nom().c_str()) )
    , model_                        ( model )
    , type_                         ( model.GetAgentTypes().Get( msg.type_pion() ) )
    , name_                         ( msg.nom() )
    , automat_                      ( &model.automats_.Get( msg.oid_automate() ) )
    , bPC_                          ( msg.pc() != 0 )
    , nDirection_                   ( 0 )
    , nHeight_                      ( 0 )
    , nAltitude_                    ( 0 )
    , nSpeed_                       ( 0 )
    , nOperationalStateValue_       ( 100 )
    , pReinforced_                  ( 0 )
    , bDead_                        ( false )
    , bNeutralized_                 ( false )
    , bStealthModeEnabled_          ( false )
    , bLoaded_                      ( false )
    , bHumanTransportersAvailable_  ( false )
    , nLastPosture_                 ( MsgsSimToClient   ::MsgUnitAttributes_Posture_arret )
    , nCurrentPosture_              ( MsgsSimToClient   ::MsgUnitAttributes_Posture_arret )
    , nPostureCompletion_           ( 100 )
    , nInstallationState_           ( 0 )
    , bNbcProtectionSuitEnabled_    ( false )
    , nbcAgentTypesContaminating_   ()
    , contamination_                ()
    , communicationJammed_          ( false )
    , knowledgeGroupJammed_         ( 0 )
    , bBlackoutEnabled_             ( false )
    , bRadarEnabled_                ( false )
    , pTransporter_                 ( 0 )
    , nForceRatioState_             ( MsgsSimToClient   ::ForceRatio_Value_neutre                                   )
    , nCloseCombatState_            ( Common            ::EnumMeetingEngagementStatus::etat_esquive                 )
    , nOperationalState_            ( Common            ::EnumOperationalStatus::operationnel                       )
    , nIndirectFireAvailability_    ( MsgsSimToClient   ::MsgUnitAttributes_FireAvailability_indisponible           )
    , nRoe_                         ( MsgsSimToClient   ::RulesOfEngagement_Value_tir_libre                         )
    , nPopulationRoe_               ( MsgsSimToClient   ::MsgUnitAttributes_PopulationRoe_emploi_force_interdit     )
    , nTiredness_                   ( Common            ::EnumUnitTiredness::normal                                 )
    , nMorale_                      ( Common            ::EnumUnitMorale::bon                                       )
    , nExperience_                  ( Common            ::EnumUnitExperience::experimente                           )
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
    , order_                        ( 0 )
{
    automat_->Register( *this );
}

// -----------------------------------------------------------------------------
// Name: Agent destructor
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
Agent::~Agent()
{
    equipments_.DeleteAll();
    troops_.DeleteAll();
    dotations_.DeleteAll();
    borrowings_.DeleteAll();
    lendings_.DeleteAll();
    automat_->Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: Agent::ChangeAutomat
// Created: SBO 2008-07-10
// -----------------------------------------------------------------------------
void Agent::ChangeAutomat( unsigned long id )
{
    if( automat_ )
        automat_->Remove( *this );
    automat_ = &model_.Automats().Get( id );
    automat_->Register( *this );
}

// -----------------------------------------------------------------------------
// Name: Agent::Update
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void Agent::Update( const MsgsSimToClient::MsgUnitCreation& msg )
{
    if( automat_->GetId() != msg.oid_automate() )
        ChangeAutomat( msg.oid_automate() );
    decisionalInfos_.Clear();
    ApplyUpdate( msg );
}

#define UPDATE_ASN_ATTRIBUTE( ASN, CPP ) \
    if( asnMsg.has_##ASN##() )        \
        CPP = asnMsg.##ASN##();

// -----------------------------------------------------------------------------
// Name: Agent::Update
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void Agent::Update( const MsgsSimToClient::MsgUnitAttributes& asnMsg )
{
    if( asnMsg.has_position() )
        position_.Set( asnMsg.position().latitude(), asnMsg.position().longitude() );
    //    UPDATE_ASN_ATTRIBUTE( direction, nDirection_ );
    if( asnMsg.has_direction() )
        nDirection_ = asnMsg.direction().heading();

    UPDATE_ASN_ATTRIBUTE( hauteur  , nHeight_    );
    UPDATE_ASN_ATTRIBUTE( altitude , nAltitude_  );
    UPDATE_ASN_ATTRIBUTE( vitesse  , nSpeed_     );
    UPDATE_ASN_ATTRIBUTE( etat_operationnel_brut, nOperationalStateValue_ );

    if( asnMsg.has_pions_renforcant() )
    {
        reinforcements_.Clear();
        for( int i = 0; i < asnMsg.pions_renforcant().elem_size(); ++i )
        {
            const kernel::Agent_ABC& agent = model_.agents_.Get( asnMsg.pions_renforcant().elem( i ).oid() );
            reinforcements_.Register( agent.GetId(), agent );
        }
    }

    if( asnMsg.has_pion_renforce() )
        pReinforced_ = asnMsg.pion_renforce() == 0 ? 0 : &model_.agents_.Get( asnMsg.pion_renforce() );

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

    if( asnMsg.has_contamine_par_agents_nbc() )
    {
        nbcAgentTypesContaminating_.clear();
        for( int i = 0; i < asnMsg.contamine_par_agents_nbc().elem_size(); ++i )
            nbcAgentTypesContaminating_.push_back( asnMsg.contamine_par_agents_nbc().elem( i ) );
    }
            
    UPDATE_ASN_ATTRIBUTE( etat_contamination, contamination_ );

    if( asnMsg.has_communications() )
    {
        if( asnMsg.communications().has_jammed() )
            communicationJammed_ = asnMsg.communications().jammed();
        if( asnMsg.communications().has_knowledge_group() )
            knowledgeGroupJammed_ = asnMsg.communications().knowledge_group();
    }

    UPDATE_ASN_ATTRIBUTE( silence_radio, bBlackoutEnabled_ );
    UPDATE_ASN_ATTRIBUTE( radar_actif, bRadarEnabled_ );

    if( asnMsg.has_pions_transportes() )
    {
        transportedAgents_.Clear();
        for( int i = 0; i < asnMsg.pions_transportes().elem_size(); ++i )
        {
            const kernel::Agent_ABC& agent = model_.agents_.Get( asnMsg.pions_transportes().elem( i ).oid() );
            transportedAgents_.Register( agent.GetId(), agent );
        }
    }

    if( asnMsg.has_pion_transporteur() )
        pTransporter_ = asnMsg.pion_transporteur() == 0 ? 0 : &model_.agents_.Get( asnMsg.pion_transporteur() );

    UPDATE_ASN_ATTRIBUTE( rapport_de_force, nForceRatioState_ );
    UPDATE_ASN_ATTRIBUTE( combat_de_rencontre, nCloseCombatState_ );
    UPDATE_ASN_ATTRIBUTE( etat_operationnel, nOperationalState_ );
    UPDATE_ASN_ATTRIBUTE( disponibilite_au_tir_indirect, nIndirectFireAvailability_ );
    UPDATE_ASN_ATTRIBUTE( roe, nRoe_ );
    UPDATE_ASN_ATTRIBUTE( roe_population, nPopulationRoe_ );
    UPDATE_ASN_ATTRIBUTE( fatigue, nTiredness_ );
    UPDATE_ASN_ATTRIBUTE( moral, nMorale_ );
    UPDATE_ASN_ATTRIBUTE( experience, nExperience_ );

    if( asnMsg.has_rendu()  )
        pSideSurrenderedTo_ = asnMsg.rendu() == 0 ? 0 : &model_.sides_.Get( asnMsg.rendu() );

    UPDATE_ASN_ATTRIBUTE( prisonnier, bPrisonner_ );
    UPDATE_ASN_ATTRIBUTE( refugie_pris_en_compte, bRefugeeManaged_ );

    if( asnMsg.has_dotation_eff_materiel()  )
    {
        for( int i = 0; i < asnMsg.dotation_eff_materiel().elem_size(); ++i )
        {
            const MsgsSimToClient::EquipmentDotations_EquipmentDotation& asn = asnMsg.dotation_eff_materiel().elem( i );
            Equipment* pEquipment = equipments_.Find( asn.type_equipement() );
            if( pEquipment )
                pEquipment->Update( asn );
            else
            {
                pEquipment = new Equipment( model_, asn );
                equipments_.Register( asn.type_equipement(), *pEquipment );
            }
        }
    }

    if( asnMsg.has_dotation_eff_personnel() )
    {
        for( int i = 0; i < asnMsg.dotation_eff_personnel().elem_size(); ++i )
        {
            const MsgsSimToClient::HumanDotations_HumanDotation& asn = asnMsg.dotation_eff_personnel().elem( i );
            Humans* pHumans = troops_.Find( asn.rang() );
            if( pHumans )
                pHumans->Update( asn );
            else
            {
                pHumans = new Humans( model_, asn );
                troops_.Register( asn.rang(), *pHumans );
            }
        }
    }

    if( asnMsg.has_dotation_eff_ressource() )
    {
        for( int i = 0; i < asnMsg.dotation_eff_ressource().elem_size(); ++i )
        {
            const MsgsSimToClient::ResourceDotations_ResourceDotation& asn = asnMsg.dotation_eff_ressource().elem().Get(i);
            Dotation* pDotation = dotations_.Find( asn.ressource_id() );
            if( pDotation )
                pDotation->Update( asn );
            else
            {
                pDotation = new Dotation( asn );
                dotations_.Register( asn.ressource_id(), *pDotation );
            }
        }
    }

    if( asnMsg.has_equipements_pretes() )
    {
        lendings_.DeleteAll();
        for( int i = 0; i < asnMsg.equipements_pretes().elem_size(); ++i )
        {
            Loan* loan = new Loan( model_, asnMsg.equipements_pretes().elem( i ) );
            lendings_.Register( i, *loan );
        }
    }

    if( asnMsg.has_equipements_empruntes() )
    {
        borrowings_.DeleteAll();
        for( int i = 0; i < asnMsg.equipements_empruntes().elem_size(); ++i )
        {
            Loan* loan = new Loan( model_, asnMsg.equipements_empruntes().elem( i ) );
            borrowings_.Register( i, *loan );
        }
    }

    ApplyUpdate( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: Agent::Update
// Created: AGE 2007-06-18
// -----------------------------------------------------------------------------
void Agent::Update( const MsgsSimToClient::MsgDecisionalState& asnMsg )
{
    decisionalInfos_.Update( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: Agent::Update
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void Agent::Update( const MsgsSimToClient::MsgLogMedicalState& asnMsg )
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
void Agent::Update( const MsgsSimToClient::MsgLogMaintenanceState& asnMsg )
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
void Agent::Update( const MsgsSimToClient::MsgLogSupplyState& asnMsg )
{
    if( !pLogSupply_ )
        pLogSupply_ = new AgentLogSupply( *this, asnMsg );
    else
        pLogSupply_->Update( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: Agent::Update
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void Agent::Update( const Common::MsgUnitChangeSuperior& asnMsg )
{
    ChangeAutomat( asnMsg.oid_automate() );
}

// -----------------------------------------------------------------------------
// Name: Agent::Update
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void Agent::Update( const Common::MsgUnitOrder& asnMsg )
{
    order_.reset();
    if( asnMsg.mission() != 0 )
        order_.reset( new AgentOrder( model_, *this, asnMsg ) );
    ApplyUpdate( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: Agent::SendCreation
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void Agent::SendCreation( ClientPublisher_ABC& publisher ) const
{
    client::UnitCreation asn;
    asn().set_oid(GetId());
    asn().set_type_pion(type_.GetId());
    asn().set_nom(name_.c_str());
    asn().set_oid_automate(automat_->GetId());
    asn().set_pc(bPC_);
    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: Agent::SendFullUpdate
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void Agent::SendFullUpdate( ClientPublisher_ABC& publisher ) const
{
    { // Attributes $$$
        client::UnitAttributes asn;

        asn().set_oid(GetId());
        asn().mutable_position()->set_latitude( position_.X() );
        asn().mutable_position()->set_longitude( position_.Y() );

        asn().mutable_direction()->set_heading( nDirection_ );
        asn().set_hauteur(nHeight_);
        asn().set_altitude(nAltitude_);
        asn().set_vitesse(nSpeed_);
        asn().set_etat_operationnel_brut(nOperationalStateValue_);

        {
            for( tools::Iterator< const kernel::Agent_ABC& > it = reinforcements_.CreateIterator(); it.HasMoreElements(); )
                asn().mutable_pions_renforcant()->add_elem()->set_oid( it.NextElement().GetId() );
        }

        asn().set_pion_renforce( pReinforced_ ? pReinforced_->GetId() : 0 );
        asn().set_mort ( bDead_ );
        asn().set_neutralise (  bNeutralized_ );
        asn().set_mode_furtif_actif ( bStealthModeEnabled_ );
        asn().set_embarque ( bLoaded_ );
        asn().set_transporteurs_disponibles ( bHumanTransportersAvailable_ );
        asn().set_posture_old ( nLastPosture_ );
        asn().set_posture_new ( nCurrentPosture_ );
        asn().set_posture_pourcentage ( nPostureCompletion_ );
        asn().set_etat_installation ( nInstallationState_ );
        asn().set_en_tenue_de_protection_nbc ( bNbcProtectionSuitEnabled_ );

        {
            unsigned int i = 0;
            for( std::vector< unsigned int >::const_iterator it = nbcAgentTypesContaminating_.begin(); it != nbcAgentTypesContaminating_.end(); ++it )
                asn().mutable_contamine_par_agents_nbc()->set_elem( i++, *it );
        }

        *asn().mutable_etat_contamination() = contamination_;
        
        asn().mutable_communications()->set_jammed( communicationJammed_ );
        asn().mutable_communications()->set_knowledge_group( knowledgeGroupJammed_ );

        asn().set_silence_radio ( bBlackoutEnabled_ );
        asn().set_radar_actif ( bRadarEnabled_ );

        {
            for( tools::Iterator< const kernel::Agent_ABC& > it = transportedAgents_.CreateIterator(); it.HasMoreElements(); )
                asn().mutable_pions_transportes()->add_elem()->set_oid( it.NextElement().GetId() );
        }

        asn().set_pion_transporteur( pTransporter_ ? pTransporter_->GetId() : 0 );

        asn().set_rapport_de_force ( nForceRatioState_ );
        asn().set_combat_de_rencontre ( nCloseCombatState_ );
        asn().set_etat_operationnel ( nOperationalState_ );
        asn().set_disponibilite_au_tir_indirect ( nIndirectFireAvailability_ );
        asn().set_roe ( nRoe_ );
        asn().set_roe_population ( nPopulationRoe_ );
        asn().set_fatigue ( nTiredness_ );
        asn().set_moral ( nMorale_ );
        asn().set_experience ( nExperience_ );
        asn().set_rendu ( pSideSurrenderedTo_ ? pSideSurrenderedTo_->GetId() : 0 );
        asn().set_prisonnier ( bPrisonner_ );
        asn().set_refugie_pris_en_compte ( bRefugeeManaged_ );

        {
            for( tools::Iterator< const Equipment& > it = equipments_.CreateIterator(); it.HasMoreElements(); )
                it.NextElement().Send( *asn().mutable_dotation_eff_materiel()->add_elem() );
        }
        {
            for( tools::Iterator< const Humans& > it = troops_.CreateIterator(); it.HasMoreElements(); )
                it.NextElement().Send( *asn().mutable_dotation_eff_personnel()->add_elem() );
        }
        {
            for( tools::Iterator< const Dotation& > it = dotations_.CreateIterator(); it.HasMoreElements(); )
                it.NextElement().Send( *asn().mutable_dotation_eff_ressource()->add_elem() );
        }
        {
            for( tools::Iterator< const Loan& > it = borrowings_.CreateIterator(); it.HasMoreElements(); )
                it.NextElement().Send( *asn().mutable_equipements_empruntes()->add_elem() );
        }
        {
            for( tools::Iterator< const Loan& > it = lendings_.CreateIterator(); it.HasMoreElements(); )
                it.NextElement().Send( *asn().mutable_equipements_pretes()->add_elem() );
        }

        asn.Send( publisher );

        if( asn().has_pions_renforcant() && asn().pions_renforcant().elem_size() > 0 )
            asn().mutable_pions_renforcant()->Clear();
        if( asn().has_contamine_par_agents_nbc() && asn().contamine_par_agents_nbc().elem_size() > 0 )
            asn().mutable_contamine_par_agents_nbc()->Clear();
        if( asn().has_pions_transportes() && asn().pions_transportes().elem_size() > 0 )
            asn().mutable_pions_transportes()->Clear();
        if( asn().has_dotation_eff_materiel() && asn().dotation_eff_materiel().elem_size() > 0 )
            asn().mutable_dotation_eff_materiel()->Clear();
        if( asn().has_dotation_eff_personnel() && asn().dotation_eff_personnel().elem_size() > 0 )
            asn().mutable_dotation_eff_personnel()->Clear();
        if( asn().has_dotation_eff_ressource() && asn().dotation_eff_ressource().elem_size() > 0 )
            asn().mutable_dotation_eff_ressource()->Clear();
        if( asn().has_equipements_empruntes() && asn().equipements_empruntes().elem_size() > 0 )
            asn().mutable_equipements_empruntes()->Clear();
        if( asn().has_equipements_pretes() && asn().equipements_pretes().elem_size() > 0 )
            asn().mutable_equipements_pretes()->Clear();
    }

    if( automat_ )
    {
        client::UnitChangeSuperior asn;
        asn().set_oid( GetId() );
        asn().set_oid_automate( automat_->GetId() );
        asn.Send( publisher );
    }

    // Log
    if( pLogMedical_ )
        pLogMedical_->Send( publisher );

    if( pLogMaintenance_ )
        pLogMaintenance_->Send( publisher );

    if( pLogSupply_ )
        pLogSupply_->Send( publisher );

    if( order_.get() )
        order_->Send( publisher );
    else
        AgentOrder::SendNoMission( *this, publisher );

    decisionalInfos_.Send( GetId(), publisher );
}

// -----------------------------------------------------------------------------
// Name: Agent::SendDestruction
// Created: AGE 2007-04-24
// -----------------------------------------------------------------------------
void Agent::SendDestruction( ClientPublisher_ABC& publisher ) const
{
    client::UnitDestruction asn;
    asn().set_oid( GetId() );
    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: Agent::Accept
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
void Agent::Accept( kernel::ModelVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}

// -----------------------------------------------------------------------------
// Name: Agent::GetType
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
const kernel::AgentType& Agent::GetType() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: Agent::IsCommandPost
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
bool Agent::IsCommandPost() const
{
    return bPC_;
}

// -----------------------------------------------------------------------------
// Name: Agent::GetPosition
// Created: MGD 2009-12-21
// -----------------------------------------------------------------------------
const geometry::Point2d& Agent::GetPosition() const
{
    return position_;
}
