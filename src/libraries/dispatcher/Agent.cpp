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
#include <boost/foreach.hpp>

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Agent constructor
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
Agent::Agent( Model_ABC& model, const sword::UnitCreation& msg, const tools::Resolver_ABC< kernel::AgentType >& types )
    : dispatcher::Agent_ABC( msg.unit().id(), QString( msg.name().c_str() ) )
    , model_                      ( model )
    , type_                       ( types.Get( msg.type().id() ) )
    , name_                       ( msg.name() )
    , automat_                    ( &model.Automats().Get( msg.automat().id() ) )
    , bPC_                        ( msg.pc() != 0 )
    , nDirection_                 ( 0 )
    , nHeight_                    ( 0 )
    , nAltitude_                  ( 0 )
    , nSpeed_                     ( 0 )
    , nOperationalStateValue_     ( 100 )
    , pReinforced_                ( 0 )
    , bDead_                      ( false )
    , bNeutralized_               ( false )
    , bStealthModeEnabled_        ( false )
    , bUnderground_               ( false )
    , isMounted_                  ( false )
    , bHumanTransportersAvailable_( false )
    , nLastPosture_               ( sword::UnitAttributes::stopping )
    , nCurrentPosture_            ( sword::UnitAttributes::stopping )
    , nPostureCompletion_         ( 100 )
    , nInstallationState_         ( 0 )
    , bNbcProtectionSuitEnabled_  ( false )
    , contaminationPercentage_    ( 0 )
    , contaminationQuantity_      ( 0.f )
    , communicationJammed_        ( false )
    , knowledgeGroupJammed_       ( 0 )
    , bRadioRecieverEnabled_      ( true )
    , bRadioEmitterEnabled_       ( true )
    , bRadarEnabled_              ( false )
    , pTransporter_               ( 0 )
    , nForceRatioState_           ( sword::ForceRatio::neutral )
    , nCloseCombatState_          ( sword::avoiding )
    , nOperationalState_          ( sword::operational )
    , nIndirectFireAvailability_  ( sword::UnitAttributes::fire_unavailable )
    , nRoe_                       ( sword::RulesOfEngagement::free_fire )
    , nPopulationRoe_             ( sword::UnitAttributes::no_force )
    , nTiredness_                 ( sword::UnitAttributes::rested )
    , nMorale_                    ( sword::UnitAttributes::high )
    , nExperience_                ( sword::UnitAttributes::expert )
    , pSideSurrenderedTo_         ( 0 )
    , bPrisonner_                 ( false )
    , bRefugeeManaged_            ( false )
    , decisionalInfos_            ( model )
    , pLogMedical_                ( 0 )
    , pLogMaintenance_            ( 0 )
    , pLogSupply_                 ( 0 )
    , order_                      ( 0 )
{
    automat_->Register( *this );
    if( msg.has_color() )
        color_ = msg.color();
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
void Agent::DoUpdate( const sword::UnitCreation& msg )
{
    if( automat_->GetId() != msg.automat().id() )
        SetSuperior( model_.Automats().Get( msg.automat().id() ) );
    decisionalInfos_.Clear();
    reinforcements_.Clear();
    transportedAgents_.Clear();
}

#define UPDATE_ASN_ATTRIBUTE( ASN, CPP ) \
    if( message.has_##ASN##() )        \
        CPP = message.##ASN##();

// -----------------------------------------------------------------------------
// Name: Agent::DoUpdate
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void Agent::DoUpdate( const sword::UnitAttributes& message )
{
    if( message.has_position() )
        position_.Set( message.position().latitude(), message.position().longitude() );
    if( message.has_direction() )
        nDirection_ = message.direction().heading();

    UPDATE_ASN_ATTRIBUTE( height, nHeight_ );
    UPDATE_ASN_ATTRIBUTE( altitude, nAltitude_ );
    UPDATE_ASN_ATTRIBUTE( speed, nSpeed_ );
    UPDATE_ASN_ATTRIBUTE( raw_operational_state, nOperationalStateValue_ );

    if( message.has_reinforcements() )
    {
        reinforcements_.Clear();
        for( int i = 0; i < message.reinforcements().elem_size(); ++i )
        {
            const kernel::Agent_ABC& agent = model_.Agents().Get( message.reinforcements().elem( i ).id() );
            reinforcements_.Register( agent.GetId(), agent );
        }
    }

    if( message.has_reinforced_unit() )
        pReinforced_ = message.reinforced_unit().id() == 0 ? 0 : &model_.Agents().Get( message.reinforced_unit().id() );

    UPDATE_ASN_ATTRIBUTE( dead                  , bDead_ );
    UPDATE_ASN_ATTRIBUTE( neutralized           , bNeutralized_ );
    UPDATE_ASN_ATTRIBUTE( stealth               , bStealthModeEnabled_ );
    UPDATE_ASN_ATTRIBUTE( underground           , bUnderground_ );
    UPDATE_ASN_ATTRIBUTE( embarked              , isMounted_ );
    UPDATE_ASN_ATTRIBUTE( transporters_available, bHumanTransportersAvailable_ );
    UPDATE_ASN_ATTRIBUTE( old_posture           , nLastPosture_ );
    UPDATE_ASN_ATTRIBUTE( new_posture           , nCurrentPosture_ );
    UPDATE_ASN_ATTRIBUTE( posture_transition    , nPostureCompletion_ );
    UPDATE_ASN_ATTRIBUTE( installation          , nInstallationState_ );
    UPDATE_ASN_ATTRIBUTE( protective_suits      , bNbcProtectionSuitEnabled_ );

    if( message.has_contamination_state() )
    {
        if( message.contamination_state().has_percentage() )
            contaminationPercentage_ = message.contamination_state().percentage();
        if( message.contamination_state().has_quantity() )
            contaminationQuantity_   = message.contamination_state().quantity();
    }
    if( message.has_contamination_agents() )
    {
        nbcAgentTypesContaminating_.clear();
        for( int i = 0; i < message.contamination_agents().elem_size(); ++i )
            nbcAgentTypesContaminating_.push_back( message.contamination_agents().elem( i ).id() );
    }
    if( message.has_communications() )
    {
        if( message.communications().has_jammed() )
            communicationJammed_ = message.communications().jammed();
        if( message.communications().has_knowledge_group() )
            knowledgeGroupJammed_ = message.communications().knowledge_group().id();
    }

    UPDATE_ASN_ATTRIBUTE( radio_emitter_disabled, bRadioEmitterEnabled_ );
    UPDATE_ASN_ATTRIBUTE( radio_receiver_disabled, bRadioRecieverEnabled_ );
    UPDATE_ASN_ATTRIBUTE( radar_active, bRadarEnabled_ );

    if( message.has_transported_units() )
    {
        transportedAgents_.Clear();
        for( int i = 0; i < message.transported_units().elem_size(); ++i )
        {
            const kernel::Agent_ABC& agent = model_.Agents().Get( message.transported_units().elem( i ).id() );
            transportedAgents_.Register( agent.GetId(), agent );
        }
    }

    if( message.has_transporting_unit() )
        pTransporter_ = message.transporting_unit().id() == 0 ? 0 : &model_.Agents().Get( message.transporting_unit().id() );

    UPDATE_ASN_ATTRIBUTE( force_ratio               , nForceRatioState_ );
    UPDATE_ASN_ATTRIBUTE( meeting_engagement        , nCloseCombatState_ );
    UPDATE_ASN_ATTRIBUTE( operational_state         , nOperationalState_ );
    UPDATE_ASN_ATTRIBUTE( indirect_fire_availability, nIndirectFireAvailability_ );
    UPDATE_ASN_ATTRIBUTE( roe                       , nRoe_ );
    UPDATE_ASN_ATTRIBUTE( roe_crowd                 , nPopulationRoe_ );
    UPDATE_ASN_ATTRIBUTE( tiredness                 , nTiredness_ );
    UPDATE_ASN_ATTRIBUTE( morale                    , nMorale_ );
    UPDATE_ASN_ATTRIBUTE( experience, nExperience_ );

    if( message.has_surrendered_unit()  )
        pSideSurrenderedTo_ = message.surrendered_unit().id() == 0 ? 0 : &model_.Sides().Get( message.surrendered_unit().id() );

    UPDATE_ASN_ATTRIBUTE( prisoner        , bPrisonner_ );
    UPDATE_ASN_ATTRIBUTE( refugees_managed, bRefugeeManaged_ );

    if( message.has_equipment_dotations()  )
        for( int i = 0; i < message.equipment_dotations().elem_size(); ++i )
        {
            const sword::EquipmentDotations_EquipmentDotation& asn = message.equipment_dotations().elem( i );
            Equipment* pEquipment = equipments_.Find( asn.type().id() );
            if( pEquipment )
                pEquipment->Update( asn );
            else
            {
                pEquipment = new Equipment( asn );
                equipments_.Register( asn.type().id(), *pEquipment );
            }
        }
    if( message.has_human_dotations() )
        for( int i = 0; i < message.human_dotations().elem_size(); ++i )
        {
            const sword::HumanDotations_HumanDotation& asn = message.human_dotations().elem( i );
            Humans* pHumans = troops_.Find( asn.rank() );
            if( pHumans )
                pHumans->Update( asn );
            else
            {
                pHumans = new Humans( asn );
                troops_.Register( asn.rank(), *pHumans );
            }
        }
    if( message.has_resource_dotations() )
        for( int i = 0; i < message.resource_dotations().elem_size(); ++i )
        {
            const sword::ResourceDotations_ResourceDotation& asn = message.resource_dotations().elem().Get(i);
            Dotation* pDotation = dotations_.Find( asn.type().id() );
            if( pDotation )
                pDotation->Update( asn );
            else
            {
                pDotation = new Dotation( asn );
                dotations_.Register( asn.type().id(), *pDotation );
            }
        }
    if( message.has_lent_equipments() )
    {
        lendings_.DeleteAll();
        for( int i = 0; i < message.lent_equipments().elem_size(); ++i )
        {
            Loan* loan = new Loan( model_, message.lent_equipments().elem( i ) );
            lendings_.Register( i, *loan );
        }
    }
    if( message.has_borrowed_equipments() )
    {
        borrowings_.DeleteAll();
        for( int i = 0; i < message.borrowed_equipments().elem_size(); ++i )
        {
            Loan* loan = new Loan( model_, message.borrowed_equipments().elem( i ) );
            borrowings_.Register( i, *loan );
        }
    }
    if( message.has_extension() )
        for( int i = 0; i < message.extension().entries_size(); ++i )
            extensions_[ message.extension().entries( i ).name() ] = message.extension().entries( i ).value();

    UPDATE_ASN_ATTRIBUTE( critical_intelligence, criticalIntelligence_ );

    if( message.has_adhesions() )
        for( int i = 0; i < message.adhesions().adhesion_size(); ++i )
        {
            const sword::PartyAdhesion& adhesion = message.adhesions().adhesion( i );
            affinities_[ adhesion.party().id() ] = adhesion.value();
        }

    Observable< sword::UnitAttributes >::Notify( message );
}

// -----------------------------------------------------------------------------
// Name: Agent::DoUpdate
// Created: AGE 2007-06-18
// -----------------------------------------------------------------------------
void Agent::DoUpdate( const sword::DecisionalState& message )
{
    decisionalInfos_.Update( message );
}

// -----------------------------------------------------------------------------
// Name: Agent::DoUpdate
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void Agent::DoUpdate( const sword::LogMedicalState& message )
{
    if( !pLogMedical_ )
        pLogMedical_ = new AgentLogMedical( model_, *this, message );
    else
        pLogMedical_->Update( message );
}

// -----------------------------------------------------------------------------
// Name: Agent::DoUpdate
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void Agent::DoUpdate( const sword::LogMaintenanceState& message )
{
    if( !pLogMaintenance_ )
        pLogMaintenance_ = new AgentLogMaintenance( model_, *this, message );
    else
        pLogMaintenance_->Update( message );
}

// -----------------------------------------------------------------------------
// Name: Agent::DoUpdate
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void Agent::DoUpdate( const sword::LogSupplyState& message )
{
    if( !pLogSupply_ )
        pLogSupply_ = new AgentLogSupply( *this, message );
    else
        pLogSupply_->Update( message );
}

// -----------------------------------------------------------------------------
// Name: Agent::DoUpdate
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void Agent::DoUpdate( const sword::UnitChangeSuperior& message )
{
    SetSuperior( model_.Automats().Get( message.parent().id() ) );
}

// -----------------------------------------------------------------------------
// Name: Agent::DoUpdate
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void Agent::DoUpdate( const sword::UnitOrder& message )
{
    order_.reset();
    if( message.type().id() != 0 )
        order_.reset( new AgentOrder( message ) );
}

// -----------------------------------------------------------------------------
// Name: Agent::DoUpdate
// Created: NLD 2010-12-27
// -----------------------------------------------------------------------------
void Agent::DoUpdate( const sword::UnitPathFind& message )
{
    currentPath_.Update( message.path().location() );
    Observable< sword::UnitPathFind >::Notify( message );
}

// -----------------------------------------------------------------------------
// Name: Agent::DoUpdate
// Created: SBO 2011-01-31
// -----------------------------------------------------------------------------
void Agent::DoUpdate( const sword::UnitEnvironmentType& message )
{
    Observable< sword::UnitEnvironmentType >::Notify( message );
}

// -----------------------------------------------------------------------------
// Name: Agent::SendCreation
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void Agent::SendCreation( ClientPublisher_ABC& publisher ) const
{
    client::UnitCreation message;
    message().mutable_unit()->set_id( GetId() );
    message().mutable_type()->set_id( type_.GetId() );
    message().set_name( name_ );
    message().mutable_automat()->set_id( automat_->GetId() );
    message().set_pc( bPC_ );
    if( color_.IsInitialized() )
        *message().mutable_color() = color_;
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
        asn().mutable_unit()->set_id( GetId() );
        asn().mutable_position()->set_latitude( position_.X() );
        asn().mutable_position()->set_longitude( position_.Y() );
        asn().mutable_direction()->set_heading( nDirection_ );
        asn().set_height( nHeight_ );
        asn().set_altitude( nAltitude_ );
        asn().set_speed( nSpeed_ );
        asn().set_raw_operational_state( nOperationalStateValue_ );

        {
            for( tools::Iterator< const kernel::Agent_ABC& > it = reinforcements_.CreateIterator(); it.HasMoreElements(); )
                asn().mutable_reinforced_unit()->set_id( it.NextElement().GetId() );
        }

        asn().mutable_reinforced_unit()->set_id( pReinforced_ ? pReinforced_->GetId() : 0 );
        asn().set_dead( bDead_ );
        asn().set_neutralized( bNeutralized_ );
        asn().set_stealth( bStealthModeEnabled_ );
        asn().set_underground( bUnderground_ );
        asn().set_embarked( isMounted_ );
        asn().set_transporters_available( bHumanTransportersAvailable_ );
        asn().set_old_posture( nLastPosture_ );
        asn().set_new_posture( nCurrentPosture_ );
        asn().set_posture_transition( nPostureCompletion_ );
        asn().set_installation( nInstallationState_ );
        asn().set_protective_suits( bNbcProtectionSuitEnabled_ );
        {
            for( std::vector< unsigned int >::const_iterator it = nbcAgentTypesContaminating_.begin(); it != nbcAgentTypesContaminating_.end(); ++it )
            {
                sword::NBCAgentType& data = *asn().mutable_contamination_agents()->add_elem();
                data.set_id( *it );
            }
        }
        asn().mutable_contamination_state()->set_percentage( contaminationPercentage_ );
        asn().mutable_contamination_state()->set_quantity( contaminationQuantity_ );
        asn().mutable_communications()->set_jammed( communicationJammed_ );
        asn().mutable_communications()->mutable_knowledge_group()->set_id( knowledgeGroupJammed_ );
        asn().set_radio_emitter_disabled( bRadioEmitterEnabled_ );
        asn().set_radio_receiver_disabled( bRadioRecieverEnabled_ );
        asn().set_radar_active( bRadarEnabled_ );
        {
            asn().mutable_transported_units();
            for( tools::Iterator< const kernel::Agent_ABC& > it = transportedAgents_.CreateIterator(); it.HasMoreElements(); )
                asn().mutable_transported_units()->add_elem()->set_id( it.NextElement().GetId() );
        }
        asn().mutable_transporting_unit()->set_id( pTransporter_ ? pTransporter_->GetId() : 0 );
        asn().set_force_ratio( nForceRatioState_ );
        asn().set_meeting_engagement( nCloseCombatState_ );
        asn().set_operational_state( nOperationalState_ );
        asn().set_indirect_fire_availability( nIndirectFireAvailability_ );
        asn().set_roe( nRoe_ );
        asn().set_roe_crowd( nPopulationRoe_ );
        asn().set_tiredness( nTiredness_ );
        asn().set_morale( nMorale_ );
        asn().set_experience( nExperience_ );
        asn().mutable_surrendered_unit()->set_id( pSideSurrenderedTo_ ? pSideSurrenderedTo_->GetId() : 0 );
        asn().set_prisoner( bPrisonner_ );
        asn().set_refugees_managed( bRefugeeManaged_ );
        {
            for( tools::Iterator< const Equipment& > it = equipments_.CreateIterator(); it.HasMoreElements(); )
                it.NextElement().Send( *asn().mutable_equipment_dotations()->add_elem() );
        }
        {
            for( tools::Iterator< const Humans& > it = troops_.CreateIterator(); it.HasMoreElements(); )
                it.NextElement().Send( *asn().mutable_human_dotations()->add_elem() );
        }
        {
            for( tools::Iterator< const Dotation& > it = dotations_.CreateIterator(); it.HasMoreElements(); )
                it.NextElement().Send( *asn().mutable_resource_dotations()->add_elem() );
        }
        {
            for( tools::Iterator< const Loan& > it = borrowings_.CreateIterator(); it.HasMoreElements(); )
                it.NextElement().Send( *asn().mutable_borrowed_equipments()->add_elem() );
        }
        {
            for( tools::Iterator< const Loan& > it = lendings_.CreateIterator(); it.HasMoreElements(); )
                it.NextElement().Send( *asn().mutable_lent_equipments()->add_elem() );
        }
        for( std::map< std::string, std::string >::const_iterator it = extensions_.begin(); it !=  extensions_.end(); ++it )
        {
            sword::Extension_Entry* entry = asn().mutable_extension()->add_entries();
            entry->set_name( it->first );
            entry->set_value( it->second );
        }
        asn().set_critical_intelligence( criticalIntelligence_ );
        BOOST_FOREACH( const T_Affinities::value_type& affinity, affinities_ )
        {
            sword::PartyAdhesion& adhesion = *asn().mutable_adhesions()->add_adhesion();
            adhesion.mutable_party()->set_id( affinity.first );
            adhesion.set_value( affinity.second );
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

    // Path
    if( !currentPath_.IsEmpty() )
    {
        client::UnitPathFind msg;
        msg().mutable_unit()->set_id( GetId() );
        currentPath_.Send( *msg().mutable_path()->mutable_location() );
        msg.Send( publisher );
    }
}

// -----------------------------------------------------------------------------
// Name: Agent::SendDestruction
// Created: AGE 2007-04-24
// -----------------------------------------------------------------------------
void Agent::SendDestruction( ClientPublisher_ABC& publisher ) const
{
    client::UnitDestruction asn;
    asn().mutable_unit()->set_id( GetId() );
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
sword::EnumOperationalStatus Agent::GetOperationalState() const
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
// Name: Agent::Equipments
// Created: SBO 2010-06-07
// -----------------------------------------------------------------------------
const tools::Resolver< dispatcher::Equipment >& Agent::Equipments() const
{
    return equipments_;
}

// -----------------------------------------------------------------------------
// Name: Agent::Troops
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
sword::ForceRatio_Value Agent::GetForceRatio() const
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
