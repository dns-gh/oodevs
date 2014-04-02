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
#include "clients_kernel/DecisionalModel.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include "protocol/ClientSenders.h"

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
    , nDirection_                 ( 0 )
    , nSensorsDirection_          ( 0 )
    , nHeight_                    ( 0 )
    , nAltitude_                  ( 0 )
    , nSpeed_                     ( 0 )
    , nOperationalStateValue_     ( 100 )
    , pReinforced_                ( 0 )
    , bPC_                        ( msg.pc() != 0 )
    , bDead_                      ( false )
    , bNeutralized_               ( false )
    , bStealthModeEnabled_        ( false )
    , bUnderground_               ( false )
    , isMounted_                  ( false )
    , bHumanTransportersAvailable_( false )
    , bNbcProtectionSuitEnabled_  ( false )
    , communicationJammed_        ( false )
    , bRadioRecieverEnabled_      ( true )
    , bRadioEmitterEnabled_       ( true )
    , bRadarEnabled_              ( false )
    , bPrisonner_                 ( false )
    , bRefugeeManaged_            ( false )
    , contaminated_               ( false )
    , nLastPosture_               ( sword::UnitAttributes::stopping )
    , nCurrentPosture_            ( sword::UnitAttributes::stopping )
    , nPostureCompletion_         ( 100 )
    , nInstallationState_         ( 0 )
    , decontaminationPercentage_  ( 100 )
    , contaminationPercentage_    ( 0 )
    , contaminationQuantity_      ( 0.f )
    , dose_                       ( 0.f )
    , knowledgeGroupJammed_       ( 0 )
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
    , nStress_                    ( sword::UnitAttributes::calm )
    , pSideSurrenderedTo_         ( 0 )
    , decisionalInfos_            ( model )
    , pLogMedical_                ( 0 )
    , pLogMaintenance_            ( 0 )
    , pLogSupply_                 ( 0 )
    , order_                      ( 0 )
    , transportedCrowd_           ( -1 )
    , statisfaction_              ( 0 )
    , humanRepartition_           ( 0 )
    , decisionalModel_            ( type_.GetDecisionalModel().GetName() )
    , brainDebug_                 ( false )
    , app6Symbol_                 ( "" )
    , level_                      ( eNatureLevel_None )
{
    if( msg.has_repartition() )
        humanRepartition_.reset( new HumanRepartition( msg.repartition().male(), msg.repartition().female(), msg.repartition().children() ) );

    automat_->Register( *this );
    if( msg.has_color() )
        color_.reset( new sword::RgbColor( msg.color() ) );

    if( msg.has_level() )
        level_ = static_cast< E_NatureLevel >( msg.level() );
    if( msg.has_app6symbol() )
        app6Symbol_ = msg.app6symbol();
    if( msg.has_extension() )
        for( int i = 0; i < msg.extension().entries_size(); ++i )
            extensions_[ msg.extension().entries( i ).name() ] = msg.extension().entries( i ).value();

    AddExtension( *this );
}

// -----------------------------------------------------------------------------
// Name: Agent destructor
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
Agent::~Agent()
{
    equipments_.DeleteAll();
    troops_.clear();
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

// -----------------------------------------------------------------------------
// Name: Agent::DoUpdate
// Created: LDC 2013-09-13
// -----------------------------------------------------------------------------
void Agent::DoUpdate( const sword::UnitDestruction& msg )
{
    transportedAgents_.Remove( msg.unit().id() );
    reinforcements_.Remove( msg.unit().id() );
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
    if( message.has_sensors_direction() )
        nSensorsDirection_ = message.sensors_direction().heading();

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

    if( message.has_contamination_agents() )
    {
        nbcAgentTypesContaminating_.clear();
        for( int i = 0; i < message.contamination_agents().elem_size(); ++i )
            nbcAgentTypesContaminating_.push_back( message.contamination_agents().elem( i ).id() );
    }

    if( message.has_contamination_state() )
    {
        sword::ContaminationState state = message.contamination_state();
        if( state.has_decontamination_process() )
            decontaminationPercentage_ = state.decontamination_process();
        if( state.has_percentage() )
            contaminationPercentage_ = state.percentage();
        if( state.has_contaminated() )
            contaminated_ = state.contaminated();
        if( state.has_quantity() )
            contaminationQuantity_= state.quantity();
        if( state.has_dose() )
            dose_ = state.dose();
    }

    if( decontaminationPercentage_ >= 100. )
        nbcAgentTypesContaminating_.clear();

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
    UPDATE_ASN_ATTRIBUTE( experience                , nExperience_ );
    UPDATE_ASN_ATTRIBUTE( stress                    , nStress_ );

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
    {
        troops_.resize( message.human_dotations().elem_size() );
        for( int i = 0; i < message.human_dotations().elem_size(); ++i )
            troops_[ i ].Update( message.human_dotations().elem( i ) );
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

    UPDATE_ASN_ATTRIBUTE( transported_crowd, transportedCrowd_ );

    if( message.has_satisfaction() )
    {
        Satisfaction* pSatisfaction = new Satisfaction;
        pSatisfaction->safety_ = message.satisfaction().safety();
        pSatisfaction->lodging_ = message.satisfaction().lodging();
        pSatisfaction->health_ = message.satisfaction().access_to_health_care();
        statisfaction_.reset( pSatisfaction );
    }
    if( message.has_headquarters() )
        bPC_ = message.headquarters();

    if( message.has_decisional_model() )
        decisionalModel_ = message.decisional_model();
    if( message.has_brain_debug() )
        brainDebug_ = message.brain_debug();

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
    if( !pLogMedical_.get() )
        pLogMedical_.reset( new AgentLogMedical( model_, *this, message ) );
    else
        pLogMedical_->Update( message );
}

// -----------------------------------------------------------------------------
// Name: Agent::DoUpdate
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void Agent::DoUpdate( const sword::LogMaintenanceState& message )
{
    if( !pLogMaintenance_.get() )
        pLogMaintenance_.reset( new AgentLogMaintenance( model_, *this, message ) );
    else
        pLogMaintenance_->Update( message );
}

// -----------------------------------------------------------------------------
// Name: Agent::DoUpdate
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void Agent::DoUpdate( const sword::LogSupplyState& message )
{
    if( !pLogSupply_.get() )
        pLogSupply_.reset( new AgentLogSupply( *this, message ) );
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
// Name: Agent::DoUpdate
// Created: SBO 2011-01-31
// -----------------------------------------------------------------------------
void Agent::DoUpdate( const sword::UnitDetection& message )
{
    if( message.current_visibility() == sword::UnitVisibility_Level_invisible )
        unitDetections_.erase( message.detected_unit().id() );
    else
    {
        UnitDetectionData data;
        data.currentLevel_ = message.current_visibility();
        data.maxLevel_ = message.max_visibility();
        unitDetections_[ message.detected_unit().id() ] = data;
    }
}

// -----------------------------------------------------------------------------
// Name: Agent::DoUpdate
// Created: SBO 2011-01-31
// -----------------------------------------------------------------------------
void Agent::DoUpdate( const sword::ObjectDetection& message )
{
    if( message.visibility() == sword::UnitVisibility_Level_invisible )
        objectDetections_.erase( message.detected_object().id() );
    else
        objectDetections_[ message.detected_object().id() ] = message.visibility();
}

// -----------------------------------------------------------------------------
// Name: Agent::DoUpdate
// Created: LDC 2012-09-04
// -----------------------------------------------------------------------------
void Agent::DoUpdate( const sword::CrowdConcentrationDetection& message )
{
    CrowdConcentrationDetectionData data;
    data.crowdId_ = message.detected_crowd().id();
    data.level_ = message.visibility();
    crowdConcentrationDetections_[ message.detected_concentration().id() ] = data;
}

// -----------------------------------------------------------------------------
// Name: Agent::DoUpdate
// Created: LDC 2012-09-04
// -----------------------------------------------------------------------------
void Agent::DoUpdate( const sword::CrowdFlowDetection& message )
{
    CrowdFlowDetectionData data;
    data.crowdId_ = message.detected_crowd().id();
    data.path_.Update( message.visible_flow().location() );
    crowdFlowDetections_[ message.detected_flow().id() ] = data;
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
    if( color_ )
        *message().mutable_color() = *color_;
    if( humanRepartition_.get() )
    {
        message().mutable_repartition()->set_male( humanRepartition_->male_ );
        message().mutable_repartition()->set_female( humanRepartition_->female_ );
        message().mutable_repartition()->set_children( humanRepartition_->children_ );
    }
    if( !app6Symbol_.empty() )
        message().set_app6symbol( app6Symbol_ );
    if( level_ != eNatureLevel_None )
        message().set_level( sword::EnumNatureLevel( level_ ) );
    for( auto it = extensions_.begin(); it !=  extensions_.end(); ++it )
    {
        sword::Extension_Entry* entry = message().mutable_extension()->add_entries();
        entry->set_name( it->first );
        entry->set_value( it->second );
    }
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
        asn().mutable_sensors_direction()->set_heading( nSensorsDirection_ );
        asn().set_height( nHeight_ );
        asn().set_altitude( nAltitude_ );
        asn().set_speed( nSpeed_ );
        asn().set_raw_operational_state( nOperationalStateValue_ );
        for( auto it = reinforcements_.CreateIterator(); it.HasMoreElements(); )
            asn().mutable_reinforced_unit()->set_id( it.NextElement().GetId() );
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
        asn().mutable_contamination_agents();
        for( auto it = nbcAgentTypesContaminating_.begin(); it != nbcAgentTypesContaminating_.end(); ++it )
            asn().mutable_contamination_agents()->add_elem()->set_id( *it );
        asn().mutable_contamination_state()->set_decontamination_process( decontaminationPercentage_ );
        asn().mutable_contamination_state()->set_percentage( contaminationPercentage_ );
        asn().mutable_contamination_state()->set_contaminated( contaminated_ );
        asn().mutable_contamination_state()->set_quantity( contaminationQuantity_ );
        asn().mutable_contamination_state()->set_dose( dose_ );
        asn().mutable_communications()->set_jammed( communicationJammed_ );
        asn().mutable_communications()->mutable_knowledge_group()->set_id( knowledgeGroupJammed_ );
        asn().set_radio_emitter_disabled( bRadioEmitterEnabled_ );
        asn().set_radio_receiver_disabled( bRadioRecieverEnabled_ );
        asn().set_radar_active( bRadarEnabled_ );
        asn().mutable_transported_units();
        for( auto it = transportedAgents_.CreateIterator(); it.HasMoreElements(); )
            asn().mutable_transported_units()->add_elem()->set_id( it.NextElement().GetId() );
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
        asn().set_stress( nStress_ );
        asn().mutable_surrendered_unit()->set_id( pSideSurrenderedTo_ ? pSideSurrenderedTo_->GetId() : 0 );
        asn().set_prisoner( bPrisonner_ );
        asn().set_refugees_managed( bRefugeeManaged_ );
        for( auto it = equipments_.CreateIterator(); it.HasMoreElements(); )
            it.NextElement().Send( *asn().mutable_equipment_dotations()->add_elem() );
        for( unsigned int i = 0; i < troops_.size(); ++i )
            troops_[ i ].Send( *asn().mutable_human_dotations()->add_elem() );
        for( auto it = dotations_.CreateIterator(); it.HasMoreElements(); )
            it.NextElement().Send( *asn().mutable_resource_dotations()->add_elem() );
        for( auto it = borrowings_.CreateIterator(); it.HasMoreElements(); )
            it.NextElement().Send( *asn().mutable_borrowed_equipments()->add_elem() );
        for( auto it = lendings_.CreateIterator(); it.HasMoreElements(); )
            it.NextElement().Send( *asn().mutable_lent_equipments()->add_elem() );
        for( auto it = extensions_.begin(); it !=  extensions_.end(); ++it )
        {
            sword::Extension_Entry* entry = asn().mutable_extension()->add_entries();
            entry->set_name( it->first );
            entry->set_value( it->second );
        }
        asn().set_critical_intelligence( criticalIntelligence_ );
        for( auto it = affinities_.cbegin(); it != affinities_.cend(); ++it )
        {
            sword::PartyAdhesion& adhesion = *asn().mutable_adhesions()->add_adhesion();
            adhesion.mutable_party()->set_id( it->first );
            adhesion.set_value( it->second );
        }
        if( transportedCrowd_ != -1 )
            asn().set_transported_crowd( transportedCrowd_ );
        if( statisfaction_.get() )
        {
            asn().mutable_satisfaction()->set_lodging( statisfaction_->lodging_ );
            asn().mutable_satisfaction()->set_safety ( statisfaction_->safety_ );
            asn().mutable_satisfaction()->set_access_to_health_care ( statisfaction_->health_ );
        }
        asn().set_decisional_model( decisionalModel_ );
        asn().set_brain_debug( brainDebug_ );
        asn().set_headquarters( bPC_ );

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
    if( pLogMedical_.get() )
        pLogMedical_->Send( publisher );

    if( pLogMaintenance_.get() )
        pLogMaintenance_->Send( publisher );

    if( pLogSupply_.get() )
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

    for( auto it = unitDetections_.begin(); it != unitDetections_.end(); ++it )
    {
        client::UnitDetection msg;
        msg().mutable_observer()->set_id( GetId() );
        msg().mutable_detected_unit()->set_id( it->first );
        msg().set_current_visibility( it->second.currentLevel_ );
        msg().set_max_visibility( it->second.maxLevel_ );
        msg.Send( publisher );
    }
    for( auto it = objectDetections_.begin(); it != objectDetections_.end(); ++it )
    {
        client::ObjectDetection msg;
        msg().mutable_observer()->set_id( GetId() );
        msg().mutable_detected_object()->set_id( it->first );
        msg().set_visibility( it->second );
        msg.Send( publisher );
    }
    for( auto it = crowdConcentrationDetections_.begin(); it != crowdConcentrationDetections_.end(); ++it )
    {
        client::CrowdConcentrationDetection msg;
        msg().mutable_observer()->set_id( GetId() );
        msg().mutable_detected_concentration()->set_id( it->first );
        msg().mutable_detected_crowd()->set_id( it->second.crowdId_ );
        msg().set_visibility( it->second.level_ );
        msg.Send( publisher );
    }
    for( auto it = crowdFlowDetections_.begin(); it != crowdFlowDetections_.end(); ++it )
    {
        client::CrowdFlowDetection msg;
        msg().mutable_observer()->set_id( GetId() );
        msg().mutable_detected_flow()->set_id( it->first );
        msg().mutable_detected_crowd()->set_id( it->second.crowdId_ );
        it->second.path_.Send( *msg().mutable_visible_flow()->mutable_location() );
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
const std::vector< dispatcher::Humans >& Agent::Troops() const
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

// -----------------------------------------------------------------------------
// Name: Agent::GetExtension
// Created: MMC 2012-08-10
// -----------------------------------------------------------------------------
bool Agent::GetExtension( const std::string& key, std::string& result ) const
{
    auto it = extensions_.find( key );
    if( it == extensions_.end() )
        return false;
    result = it->second;
    return true;
}
