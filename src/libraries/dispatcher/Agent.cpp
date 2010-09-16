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
#include "Model_ABC.h"
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
#include "protocol/ClientPublisher_ABC.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include "clients_kernel/StaticModel.h"
#include "protocol/ClientSenders.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Agent constructor
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
Agent::Agent( Model_ABC& model, const MsgsSimToClient::MsgUnitCreation& msg, const tools::Resolver_ABC< kernel::AgentType >& types )
    : dispatcher::Agent_ABC         ( msg.id().id(), QString( msg.nom().c_str() ) )
    , model_                        ( model )
    , type_                         ( types.Get( msg.type().id() ) )
    , name_                         ( msg.nom() )
    , automat_                      ( &model.Automats().Get( msg.automat().id() ) )
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
    , isMounted_                    ( false )
    , bHumanTransportersAvailable_  ( false )
    , nLastPosture_                 ( MsgsSimToClient::MsgUnitAttributes_Posture_arret )
    , nCurrentPosture_              ( MsgsSimToClient::MsgUnitAttributes_Posture_arret )
    , nPostureCompletion_           ( 100 )
    , nInstallationState_           ( 0 )
    , bNbcProtectionSuitEnabled_    ( false )
    , nbcAgentTypesContaminating_   ()
    , contaminationPercentage_      ( 0 )
    , contaminationQuantity_        ( 0.f )
    , communicationJammed_          ( false )
    , knowledgeGroupJammed_         ( 0 )
    , bRadioRecieverEnabled_        ( true )
    , bRadioEmitterEnabled_         ( true )
    , bRadarEnabled_                ( false )
    , pTransporter_                 ( 0 )
    , nForceRatioState_             ( MsgsSimToClient::ForceRatio_Value_neutre )
    , nCloseCombatState_            ( Common         ::etat_esquive )
    , nOperationalState_            ( Common         ::operationnel )
    , nIndirectFireAvailability_    ( MsgsSimToClient::MsgUnitAttributes_FireAvailability_indisponible       )
    , nRoe_                         ( MsgsSimToClient::RulesOfEngagement_Value_tir_libre                     )
    , nPopulationRoe_               ( MsgsSimToClient::MsgUnitAttributes_PopulationRoe_emploi_force_interdit )
    , nTiredness_                   ( Common         ::normal      )
    , nMorale_                      ( Common         ::bon         )
    , nExperience_                  ( Common         ::experimente )
    , pSideSurrenderedTo_           ( 0 )
    , bPrisonner_                   ( false )
    , bRefugeeManaged_              ( false )
    , decisionalInfos_              ( model )
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
    RegisterSelf( *this );
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
// Name: Agent::SetSuperior
// Created: SBO 2010-06-14
// -----------------------------------------------------------------------------
void Agent::SetSuperior( dispatcher::Automat_ABC& superior )
{
    if( automat_ )
        automat_->Remove( *this );
    automat_ = &superior;
    automat_->Register( *this );
}

// -----------------------------------------------------------------------------
// Name: Agent::DoUpdate
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void Agent::DoUpdate( const MsgsSimToClient::MsgUnitCreation& msg )
{
    if( automat_->GetId() != msg.automat().id() )
        SetSuperior( model_.Automats().Get( msg.automat().id() ) );
    decisionalInfos_.Clear();
}

#define UPDATE_ASN_ATTRIBUTE( ASN, CPP ) \
    if( asnMsg.has_##ASN##() )        \
        CPP = asnMsg.##ASN##();

// -----------------------------------------------------------------------------
// Name: Agent::DoUpdate
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void Agent::DoUpdate( const MsgsSimToClient::MsgUnitAttributes& asnMsg )
{
    if( asnMsg.has_position() )
        position_.Set( asnMsg.position().latitude(), asnMsg.position().longitude() );
    if( asnMsg.has_direction() )
        nDirection_ = asnMsg.direction().heading();

    UPDATE_ASN_ATTRIBUTE( hauteur  , nHeight_    );
    UPDATE_ASN_ATTRIBUTE( altitude , nAltitude_  );
    UPDATE_ASN_ATTRIBUTE( vitesse  , nSpeed_     );
    UPDATE_ASN_ATTRIBUTE( etat_operationnel_brut, nOperationalStateValue_ );

    if( asnMsg.has_reinforcements() )
    {
        reinforcements_.Clear();
        for( int i = 0; i < asnMsg.reinforcements().elem_size(); ++i )
        {
            const kernel::Agent_ABC& agent = model_.Agents().Get( asnMsg.reinforcements().elem( i ).id() );
            reinforcements_.Register( agent.GetId(), agent );
        }
    }

    if( asnMsg.has_reinforced_unit() )
        pReinforced_ = asnMsg.reinforced_unit().id() == 0 ? 0 : &model_.Agents().Get( asnMsg.reinforced_unit().id() );

    UPDATE_ASN_ATTRIBUTE( mort, bDead_ );
    UPDATE_ASN_ATTRIBUTE( neutralise,  bNeutralized_ );
    UPDATE_ASN_ATTRIBUTE( mode_furtif_actif, bStealthModeEnabled_ );
    UPDATE_ASN_ATTRIBUTE( embarque, isMounted_ );
    UPDATE_ASN_ATTRIBUTE( transporteurs_disponibles, bHumanTransportersAvailable_ );
    UPDATE_ASN_ATTRIBUTE( posture_old, nLastPosture_ );
    UPDATE_ASN_ATTRIBUTE( posture_new, nCurrentPosture_ );
    UPDATE_ASN_ATTRIBUTE( posture_pourcentage, nPostureCompletion_ );
    UPDATE_ASN_ATTRIBUTE( etat_installation, nInstallationState_ );
    UPDATE_ASN_ATTRIBUTE( en_tenue_de_protection_nbc, bNbcProtectionSuitEnabled_ );

    if( asnMsg.has_etat_contamination() )
    {
        if( asnMsg.etat_contamination().has_percentage() )
            contaminationPercentage_ = asnMsg.etat_contamination().percentage();
        if( asnMsg.etat_contamination().has_quantity() )
            contaminationQuantity_   = asnMsg.etat_contamination().quantity();
    }
    if( asnMsg.has_contamine_par_agents_nbc() )
    {
        nbcAgentTypesContaminating_.clear();
        for( int i = 0; i < asnMsg.contamine_par_agents_nbc().elem_size(); ++i )
            nbcAgentTypesContaminating_.push_back( asnMsg.contamine_par_agents_nbc().elem( i ).id() );
    }
    if( asnMsg.has_communications() )
    {
        if( asnMsg.communications().has_jammed() )
            communicationJammed_ = asnMsg.communications().jammed();
        if( asnMsg.communications().has_knowledge_group() )
            knowledgeGroupJammed_ = asnMsg.communications().knowledge_group().id();
    }

    UPDATE_ASN_ATTRIBUTE( radio_emitter_disabled, bRadioEmitterEnabled_ );
    UPDATE_ASN_ATTRIBUTE( radio_receiver_disabled, bRadioRecieverEnabled_ );
    UPDATE_ASN_ATTRIBUTE( radar_actif, bRadarEnabled_ );

    if( asnMsg.has_transported_units() )
    {
        transportedAgents_.Clear();
        for( int i = 0; i < asnMsg.transported_units().elem_size(); ++i )
        {
            const kernel::Agent_ABC& agent = model_.Agents().Get( asnMsg.transported_units().elem( i ).id() );
            transportedAgents_.Register( agent.GetId(), agent );
        }
    }

    if( asnMsg.has_transporting_unit() )
        pTransporter_ = asnMsg.transporting_unit().id() == 0 ? 0 : &model_.Agents().Get( asnMsg.transporting_unit().id() );

    UPDATE_ASN_ATTRIBUTE( rapport_de_force, nForceRatioState_ );
    UPDATE_ASN_ATTRIBUTE( combat_de_rencontre, nCloseCombatState_ );
    UPDATE_ASN_ATTRIBUTE( etat_operationnel, nOperationalState_ );
    UPDATE_ASN_ATTRIBUTE( disponibilite_au_tir_indirect, nIndirectFireAvailability_ );
    UPDATE_ASN_ATTRIBUTE( roe, nRoe_ );
    UPDATE_ASN_ATTRIBUTE( roe_population, nPopulationRoe_ );
    UPDATE_ASN_ATTRIBUTE( fatigue, nTiredness_ );
    UPDATE_ASN_ATTRIBUTE( moral, nMorale_ );
    UPDATE_ASN_ATTRIBUTE( experience, nExperience_ );

    if( asnMsg.has_surrendered_unit()  )
        pSideSurrenderedTo_ = asnMsg.surrendered_unit().id() == 0 ? 0 : &model_.Sides().Get( asnMsg.surrendered_unit().id() );

    UPDATE_ASN_ATTRIBUTE( prisonnier, bPrisonner_ );
    UPDATE_ASN_ATTRIBUTE( refugie_pris_en_compte, bRefugeeManaged_ );

    if( asnMsg.has_dotation_eff_materiel()  )
    {
        for( int i = 0; i < asnMsg.dotation_eff_materiel().elem_size(); ++i )
        {
            const MsgsSimToClient::EquipmentDotations_EquipmentDotation& asn = asnMsg.dotation_eff_materiel().elem( i );
            Equipment* pEquipment = equipments_.Find( asn.type().id() );
            if( pEquipment )
                pEquipment->Update( asn );
            else
            {
                pEquipment = new Equipment( asn );
                equipments_.Register( asn.type().id(), *pEquipment );
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
                pHumans = new Humans( asn );
                troops_.Register( asn.rang(), *pHumans );
            }
        }
    }

    if( asnMsg.has_dotation_eff_ressource() )
    {
        for( int i = 0; i < asnMsg.dotation_eff_ressource().elem_size(); ++i )
        {
            const MsgsSimToClient::ResourceDotations_ResourceDotation& asn = asnMsg.dotation_eff_ressource().elem().Get(i);
            Dotation* pDotation = dotations_.Find( asn.type().id() );
            if( pDotation )
                pDotation->Update( asn );
            else
            {
                pDotation = new Dotation( asn );
                dotations_.Register( asn.type().id(), *pDotation );
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
}

// -----------------------------------------------------------------------------
// Name: Agent::DoUpdate
// Created: AGE 2007-06-18
// -----------------------------------------------------------------------------
void Agent::DoUpdate( const MsgsSimToClient::MsgDecisionalState& asnMsg )
{
    decisionalInfos_.Update( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: Agent::DoUpdate
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void Agent::DoUpdate( const MsgsSimToClient::MsgLogMedicalState& asnMsg )
{
    if( !pLogMedical_ )
        pLogMedical_ = new AgentLogMedical( model_, *this, asnMsg );
    else
        pLogMedical_->Update( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: Agent::DoUpdate
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void Agent::DoUpdate( const MsgsSimToClient::MsgLogMaintenanceState& asnMsg )
{
    if( !pLogMaintenance_ )
        pLogMaintenance_ = new AgentLogMaintenance( model_, *this, asnMsg );
    else
        pLogMaintenance_->Update( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: Agent::DoUpdate
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void Agent::DoUpdate( const MsgsSimToClient::MsgLogSupplyState& asnMsg )
{
    if( !pLogSupply_ )
        pLogSupply_ = new AgentLogSupply( *this, asnMsg );
    else
        pLogSupply_->Update( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: Agent::DoUpdate
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void Agent::DoUpdate( const Common::MsgUnitChangeSuperior& asnMsg )
{
    SetSuperior( model_.Automats().Get( asnMsg.parent().id() ) );
}

// -----------------------------------------------------------------------------
// Name: Agent::DoUpdate
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void Agent::DoUpdate( const Common::MsgUnitOrder& message )
{
    order_.reset();
    if( message.type().id() != 0 )
        order_.reset( new AgentOrder( message ) );
}

// -----------------------------------------------------------------------------
// Name: Agent::SendCreation
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void Agent::SendCreation( ClientPublisher_ABC& publisher ) const
{
    client::UnitCreation message;
    message().mutable_id()->set_id( GetId() );
    message().mutable_type()->set_id( type_.GetId() );
    message().set_nom( name_ );
    message().mutable_automat()->set_id( automat_->GetId() );
    message().set_pc( bPC_ );
    message.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: Agent::SendFullUpdate
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void Agent::SendFullUpdate( ClientPublisher_ABC& publisher ) const
{
    { // Attributes $$$
        client::UnitAttributes asn;
        asn().mutable_id()->set_id( GetId() );
        asn().mutable_position()->set_latitude( position_.X() );
        asn().mutable_position()->set_longitude( position_.Y() );
        asn().mutable_direction()->set_heading( nDirection_ );
        asn().set_hauteur( nHeight_ );
        asn().set_altitude( nAltitude_ );
        asn().set_vitesse( nSpeed_ );
        asn().set_etat_operationnel_brut( nOperationalStateValue_ );

        {
            for( tools::Iterator< const kernel::Agent_ABC& > it = reinforcements_.CreateIterator(); it.HasMoreElements(); )
                asn().mutable_reinforced_unit()->set_id( it.NextElement().GetId() );
        }

        asn().mutable_reinforced_unit()->set_id( pReinforced_ ? pReinforced_->GetId() : 0 );
        asn().set_mort( bDead_ );
        asn().set_neutralise( bNeutralized_ );
        asn().set_mode_furtif_actif( bStealthModeEnabled_ );
        asn().set_embarque( isMounted_ );
        asn().set_transporteurs_disponibles( bHumanTransportersAvailable_ );
        asn().set_posture_old( nLastPosture_ );
        asn().set_posture_new( nCurrentPosture_ );
        asn().set_posture_pourcentage( nPostureCompletion_ );
        asn().set_etat_installation( nInstallationState_ );
        asn().set_en_tenue_de_protection_nbc( bNbcProtectionSuitEnabled_ );

        {
            for( std::vector< unsigned int >::const_iterator it = nbcAgentTypesContaminating_.begin(); it != nbcAgentTypesContaminating_.end(); ++it )
            {
                Common::NBCAgentType& data = *asn().mutable_contamine_par_agents_nbc()->add_elem();
                data.set_id( *it );
            }
        }

        asn().mutable_etat_contamination()->set_percentage( contaminationPercentage_ );
        asn().mutable_etat_contamination()->set_quantity( contaminationQuantity_ );

        asn().mutable_communications()->set_jammed( communicationJammed_ );
        asn().mutable_communications()->mutable_knowledge_group()->set_id( knowledgeGroupJammed_ );

        asn().set_radio_emitter_disabled( bRadioEmitterEnabled_ );
        asn().set_radio_receiver_disabled( bRadioRecieverEnabled_ );
        asn().set_radar_actif( bRadarEnabled_ );

        {
            for( tools::Iterator< const kernel::Agent_ABC& > it = transportedAgents_.CreateIterator(); it.HasMoreElements(); )
                asn().mutable_transported_units()->add_elem()->set_id( it.NextElement().GetId() );
        }

        asn().mutable_transporting_unit()->set_id( pTransporter_ ? pTransporter_->GetId() : 0 );

        asn().set_rapport_de_force ( nForceRatioState_ );
        asn().set_combat_de_rencontre ( nCloseCombatState_ );
        asn().set_etat_operationnel ( nOperationalState_ );
        asn().set_disponibilite_au_tir_indirect ( nIndirectFireAvailability_ );
        asn().set_roe ( nRoe_ );
        asn().set_roe_population ( nPopulationRoe_ );
        asn().set_fatigue ( nTiredness_ );
        asn().set_moral ( nMorale_ );
        asn().set_experience ( nExperience_ );
        asn().mutable_surrendered_unit()->set_id( pSideSurrenderedTo_ ? pSideSurrenderedTo_->GetId() : 0 );
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
    }

    if( automat_ )
    {
        client::UnitChangeSuperior asn;
        asn().mutable_unit()->set_id( GetId() );
        asn().mutable_parent()->set_id( automat_->GetId() );
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
    asn().mutable_id()->set_id( GetId() );
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

// -----------------------------------------------------------------------------
// Name: Agent::GetSuperior
// Created: SBO 2010-06-07
// -----------------------------------------------------------------------------
const dispatcher::Automat_ABC& Agent::GetSuperior() const
{
    return *automat_;
}

// -----------------------------------------------------------------------------
// Name: Agent::GetOperationalState
// Created: SBO 2010-06-07
// -----------------------------------------------------------------------------
Common::EnumOperationalStatus Agent::GetOperationalState() const
{
    return nOperationalState_;
}

// -----------------------------------------------------------------------------
// Name: Agent::GetOperationalStateValue
// Created: SBO 2010-06-07
// -----------------------------------------------------------------------------
unsigned short Agent::GetOperationalStateValue() const
{
    return unsigned short( nOperationalStateValue_ );
}

// -----------------------------------------------------------------------------
// Name: tools::Resolver< dispatcher::Equipment >& Agent::Equipments
// Created: SBO 2010-06-07
// -----------------------------------------------------------------------------
const tools::Resolver< dispatcher::Equipment >& Agent::Equipments() const
{
    return equipments_;
}

// -----------------------------------------------------------------------------
// Name: tools::Resolver< dispatcher::Humans >& Agent::Troops
// Created: SBO 2010-06-07
// -----------------------------------------------------------------------------
const tools::Resolver< dispatcher::Humans >& Agent::Troops() const
{
    return troops_;
}

// -----------------------------------------------------------------------------
// Name: Agent::GetAltitude
// Created: SBO 2010-06-07
// -----------------------------------------------------------------------------
unsigned short Agent::GetAltitude() const
{
    return unsigned short( nAltitude_ );
}

// -----------------------------------------------------------------------------
// Name: Agent::GetSpeed
// Created: SBO 2010-06-07
// -----------------------------------------------------------------------------
unsigned short Agent::GetSpeed() const
{
    return unsigned short( nSpeed_ );
}

// -----------------------------------------------------------------------------
// Name: Agent::GetDirection
// Created: SBO 2010-06-07
// -----------------------------------------------------------------------------
unsigned short Agent::GetDirection() const
{
    return unsigned short( nDirection_ );
}

// -----------------------------------------------------------------------------
// Name: Agent::GetForceRatio
// Created: SBO 2010-06-08
// -----------------------------------------------------------------------------
MsgsSimToClient::ForceRatio_Value Agent::GetForceRatio() const
{
    return nForceRatioState_;
}

// -----------------------------------------------------------------------------
// Name: Agent::GetOrder
// Created: SBO 2010-06-08
// -----------------------------------------------------------------------------
const Order_ABC* Agent::GetOrder() const
{
    return order_.get();
}

// -----------------------------------------------------------------------------
// Name: Agent::IsMounted
// Created: SBO 2010-06-25
// -----------------------------------------------------------------------------
bool Agent::IsMounted() const
{
    return isMounted_;
}
