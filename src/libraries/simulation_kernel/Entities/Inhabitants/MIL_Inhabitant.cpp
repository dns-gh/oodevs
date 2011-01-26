// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_Inhabitant.h"
#include "MIL_InhabitantType.h"
#include "Entities/MIL_EntityManager.h"
#include "MIL_AgentServer.h"
#include "Entities/Objects/MedicalCapacity.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "MIL_LivingArea.h"
#include "MIL_Schedule.h"
#include "Entities/MIL_Army_ABC.h"
#include "Network/NET_AsnException.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"
#include "tools/MIL_IDManager.h"
#include <xeumeuleu/xml.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/foreach.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_Inhabitant )

namespace
{
    MIL_IDManager idManager_;
    template< typename R >
    void SaveRole( const MIL_Inhabitant& population, MIL_CheckPointOutArchive& file )
    {
        const R* const role = &population.GetRole< R >();
        file << role;
    }
}

template< typename Archive >
void save_construct_data( Archive& archive, const MIL_Inhabitant* population, const unsigned int /*version*/ )
{
    unsigned int nTypeID = population->GetType().GetID();
    archive << nTypeID;
}

template< typename Archive >
void load_construct_data( Archive& archive, MIL_Inhabitant* population, const unsigned int /*version*/ )
{
    unsigned int nTypeID;
    archive >> nTypeID;
    const MIL_InhabitantType* pType = MIL_InhabitantType::Find( nTypeID );
    if( pType )
        ::new( population )MIL_Inhabitant( *pType);
}

// -----------------------------------------------------------------------------
// Name: MIL_Inhabitant constructor
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
MIL_Inhabitant::MIL_Inhabitant( xml::xistream& xis, const MIL_InhabitantType& type, MIL_Army_ABC& army )
    : MIL_Entity_ABC( xis )
    , pType_                      ( &type )
    , nID_                        ( xis.attribute< unsigned int >( "id" ) )
    , pArmy_                      ( &army )
    , nNbrHealthyHumans_          ( 0 )
    , nNbrDeadHumans_             ( 0 )
    , nNbrWoundedHumans_          ( 0 )
    , healthNeed_                 ( 0 )
    , healthSatisfaction_         ( 0 )
    , safetySatisfaction_         ( 1.f )
    , lastSafetySatisfaction_     ( 1.f )
    , healthSatisfactionChanged_  ( false )
    , healthStateChanged_         ( false )
{
    float totalArea = 0.f;
    idManager_.Lock( nID_ );
    xis >> xml::start( "composition" )
            >> xml::attribute( "healthy", nNbrHealthyHumans_ )
            >> xml::attribute( "wounded", nNbrWoundedHumans_ )
            >> xml::attribute( "dead", nNbrDeadHumans_ )
        >> xml::end
        >> xml::start( "information" )
            >> xml::optional >> text_
        >> xml::end
        >> xml::start( "health-need" )
            >> xml::attribute( "quantity", healthNeed_ )
        >> xml::end
        >> xml::optional
        >> xml::start( "extensions" )
            >> xml::list( "entry", *this, &MIL_Inhabitant::ReadExtension )
        >> xml::end;
    unsigned long population = nNbrHealthyHumans_ + nNbrWoundedHumans_ + nNbrDeadHumans_;
    pLivingArea_.reset( new MIL_LivingArea( xis, population, nID_ ) );
    pLivingArea_->Register( *this );
    pSchedule_.reset( new MIL_Schedule( *pLivingArea_ ) );
    pType_->InitializeSchedule( *pSchedule_ );
    pArmy_->RegisterInhabitant( *this );
    ComputeHealthSatisfaction();
}

// -----------------------------------------------------------------------------
// Name: MIL_Inhabitant constructor
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
MIL_Inhabitant::MIL_Inhabitant( const MIL_InhabitantType& type )
    : MIL_Entity_ABC( type.GetName() )
    , pType_                      ( &type )
    , nID_                        ( 0 )
    , pArmy_                      ( 0 )
    , pLivingArea_                ( 0 )
    , pSchedule_                  ( 0 )
    , nNbrHealthyHumans_          ( 0 )
    , nNbrDeadHumans_             ( 0 )
    , nNbrWoundedHumans_          ( 0 )
    , healthNeed_                 ( 0 )
    , healthSatisfaction_         ( 0 )
    , safetySatisfaction_         ( 0 )
    , lastSafetySatisfaction_     ( 0 )
    , healthSatisfactionChanged_  ( false )
    , healthStateChanged_         ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Inhabitant destructor
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
MIL_Inhabitant::~MIL_Inhabitant()
{
    if( pArmy_ )
        pArmy_->UnregisterInhabitant( *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_Inhabitant::load
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
void MIL_Inhabitant::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    MIL_LivingArea* pLivingArea;
    file >> boost::serialization::base_object< MIL_Entity_ABC >( *this );
    file >> const_cast< unsigned int& >( nID_ )
         >> const_cast< MIL_Army_ABC*& >( pArmy_ );
    idManager_.Lock( nID_ );
    file >> text_
         >> nNbrHealthyHumans_
         >> nNbrDeadHumans_
         >> nNbrWoundedHumans_
         >> healthNeed_
         >> healthSatisfaction_
         >> safetySatisfaction_
         >> pLivingArea;
    pLivingArea_.reset( pLivingArea );
    unsigned int size;
    file >> size;
    std::string first;
    std::string second;
    for( unsigned int i = 0; i < size; ++i )
    {
        file >> first
             >> second;
        extensions_[ first ] = second;
    }
    pSchedule_.reset( new MIL_Schedule( *pLivingArea_ ) );
    pType_->InitializeSchedule( *pSchedule_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_Inhabitant::save
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
void MIL_Inhabitant::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    const MIL_LivingArea* const pLivingArea = pLivingArea_.get();
    file << boost::serialization::base_object< MIL_Entity_ABC >( *this );
    file << nID_
         << pArmy_
         << text_
         << nNbrHealthyHumans_
         << nNbrDeadHumans_
         << nNbrWoundedHumans_
         << healthNeed_
         << healthSatisfaction_
         << safetySatisfaction_
         << pLivingArea;
    unsigned int size;
    size = extensions_.size();
    file << size;
    for( CIT_Extensions it = extensions_.begin(); it != extensions_.end(); ++it )
        file << it->first
             << it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_Inhabitant::WriteODB
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
void MIL_Inhabitant::WriteODB( xml::xostream& xos ) const
{
    assert( pType_ );
    assert( pArmy_ );
    xos << xml::start( "inhabitant" );
    MIL_Entity_ABC::WriteODB ( xos ) ;
    xos << xml::attribute( "id", nID_ )
        << xml::attribute( "type", pType_->GetName() )
        << xml::start( "composition" )
            << xml::attribute( "healthy", nNbrHealthyHumans_ )
            << xml::attribute( "wounded", nNbrWoundedHumans_ )
            << xml::attribute( "dead", nNbrDeadHumans_ )
        << xml::end
        << xml::start( "health-need" )
            << xml::attribute( "quantity", healthNeed_ )
        << xml::end
        << xml::content( "information", text_ )
        << xml::start( "living-area" );
    pLivingArea_->WriteODB( xos );
        xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: MIL_Inhabitant::ReadExtension
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
void MIL_Inhabitant::ReadExtension( xml::xistream& xis )
{
    extensions_[ xis.attribute< std::string >( "key" ) ] = xis.attribute< std::string >( "value" );
}

// -----------------------------------------------------------------------------
// Name: MIL_Inhabitant::SendCreation
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
void MIL_Inhabitant::SendCreation() const
{
    client::PopulationCreation msg;
    msg().mutable_id()->set_id( nID_ );
    msg().mutable_type()->set_id( pType_->GetID() );
    msg().mutable_party()->set_id( pArmy_->GetID() );
    msg().set_text( text_ );
    msg().set_name( GetName() );
    BOOST_FOREACH( const T_Extensions::value_type& extension, extensions_ )
    {
        sword::Extension_Entry* entry = msg().mutable_extension()->add_entries();
        entry->set_name( extension.first );
        entry->set_value( extension.second );
    }
    pLivingArea_->SendCreation( msg );
    msg.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: MIL_Inhabitant::SendFullState
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
void MIL_Inhabitant::SendFullState() const
{
    client::PopulationUpdate msg;
    msg().mutable_id()->set_id( nID_ );
    msg().set_healthy( nNbrHealthyHumans_ );
    msg().set_dead( nNbrDeadHumans_ );
    msg().set_wounded( nNbrWoundedHumans_ );
    msg().mutable_satisfaction()->set_health( healthSatisfaction_ );
    msg().mutable_satisfaction()->set_safety( safetySatisfaction_ );
    msg.Send( NET_Publisher_ABC::Publisher() );
    pLivingArea_->SendFullState();
}

// -----------------------------------------------------------------------------
// Name: MIL_Inhabitant::UpdateState
// Created: JSR 2011-01-14
// -----------------------------------------------------------------------------
void MIL_Inhabitant::UpdateState()
{
    pSchedule_->Update( MIL_AgentServer::GetWorkspace().GetRealTime(), MIL_AgentServer::GetWorkspace().GetTickDuration() );
    safetySatisfaction_ = std::min( 1.f, safetySatisfaction_ + MIL_AgentServer::GetWorkspace().GetTickDuration() * pType_->GetSafetyGainPerHour() / 3600 );
}

// -----------------------------------------------------------------------------
// Name: MIL_Inhabitant::UpdateNetwork
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
void MIL_Inhabitant::UpdateNetwork()
{
    bool safetyChanged = std::abs( lastSafetySatisfaction_ - safetySatisfaction_ ) > 0.01f;
    if( healthSatisfactionChanged_ || safetyChanged || healthStateChanged_ )
    {
        client::PopulationUpdate msg;
        msg().mutable_id()->set_id( nID_ );
        if( healthSatisfactionChanged_ )
        {
            msg().mutable_satisfaction()->set_health( healthSatisfaction_ );
            healthSatisfactionChanged_ = false;
        }
        if( safetyChanged )
        {
            msg().mutable_satisfaction()->set_safety( safetySatisfaction_ );
            lastSafetySatisfaction_ = safetySatisfaction_;
        }
        if( healthStateChanged_ )
        {
            msg().set_healthy( nNbrHealthyHumans_ );
            msg().set_wounded( nNbrWoundedHumans_ );
            msg().set_dead( nNbrDeadHumans_ );
            healthStateChanged_ = false;
        }
        msg.Send( NET_Publisher_ABC::Publisher() );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Inhabitant::OnReceiveInhabitantMagicAction
// Created: ABR 2011-01-26
// -----------------------------------------------------------------------------
void MIL_Inhabitant::OnReceiveInhabitantMagicAction( const sword::UnitMagicAction& msg )
{
    client::ChangePopulationMagicActionAck ack;
    ack().set_error_code( sword::ChangePopulationMagicActionAck::no_error );
    try
    {
        switch( msg.type() )
        {
        case sword::UnitMagicAction::inhabitant_change_health_state:
            OnReceiveMsgChangeHealthState( msg );
            break;
        case sword::UnitMagicAction::inhabitant_change_adhesion_list:
            OnReceiveMsgChangeAdhesionList( msg );
            break;
        default:
            assert( false );
        }
    }
    catch( NET_AsnException< sword::ChangePopulationMagicActionAck::ErrorCode >& e )
    {
        ack().set_error_code( e.GetErrorID() );
    }
    ack.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: MIL_Inhabitant::OnReceiveMsgChangeHealthState
// Created: ABR 2011-01-26
// -----------------------------------------------------------------------------
void MIL_Inhabitant::OnReceiveMsgChangeHealthState( const sword::UnitMagicAction& msg )
{
    if( !msg.has_parameters() || msg.parameters().elem_size() != 3)
        throw NET_AsnException< sword::ChangePopulationMagicActionAck_ErrorCode >( sword::ChangePopulationMagicActionAck::error_invalid_parameter );

    const sword::MissionParameter& healthy = msg.parameters().elem( 0 );
    const sword::MissionParameter& wounded = msg.parameters().elem( 1 );
    const sword::MissionParameter& dead    = msg.parameters().elem( 2 );

    if( healthy.value_size() != 1 || !healthy.value().Get( 0 ).has_quantity() ||
        wounded.value_size() != 1 || !wounded.value().Get( 0 ).has_quantity() ||
        dead.value_size() != 1    || !dead.value().Get( 0 ).has_quantity() )
        throw NET_AsnException< sword::ChangePopulationMagicActionAck_ErrorCode >( sword::ChangePopulationMagicActionAck::error_invalid_parameter );

    if( healthy.value().Get( 0 ).quantity() < 0 || 
        wounded.value().Get( 0 ).quantity() < 0 || 
        dead.value().Get( 0 ).quantity() < 0 )
        throw NET_AsnException< sword::ChangePopulationMagicActionAck_ErrorCode >( sword::ChangePopulationMagicActionAck::error_invalid_number );

    nNbrHealthyHumans_ = healthy.value().Get( 0 ).quantity();
    nNbrWoundedHumans_ = wounded.value().Get( 0 ).quantity();
    nNbrDeadHumans_    = dead.value().Get( 0 ).quantity();
    healthStateChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: MIL_Inhabitant::OnReceiveMsgChangeAdhesionList
// Created: ABR 2011-01-26
// -----------------------------------------------------------------------------
void MIL_Inhabitant::OnReceiveMsgChangeAdhesionList( const sword::UnitMagicAction& msg )
{

}

// -----------------------------------------------------------------------------
// Name: MIL_Inhabitant::GetID
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
unsigned int MIL_Inhabitant::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Inhabitant::GetType
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
const MIL_InhabitantType& MIL_Inhabitant::GetType() const
{
    assert( pType_ );
    return *pType_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Inhabitant::GetArmy
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
MIL_Army_ABC& MIL_Inhabitant::GetArmy() const
{
    assert( pArmy_ );
    return *pArmy_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Inhabitant::NotifyStructuralStateChanged
// Created: JSR 2011-01-14
// -----------------------------------------------------------------------------
void MIL_Inhabitant::NotifyStructuralStateChanged( unsigned int /*structuralState*/, const MIL_Object_ABC& object )
{
    if( object.Retrieve< MedicalCapacity >() )
        ComputeHealthSatisfaction();
}

// -----------------------------------------------------------------------------
// Name: MIL_Inhabitant::NotifyFired
// Created: JSR 2011-01-25
// -----------------------------------------------------------------------------
void MIL_Inhabitant::NotifyFired()
{
    safetySatisfaction_ = std::max( 0.f, safetySatisfaction_ - pType_->GetSafetyLossOnFire() );
}

// -----------------------------------------------------------------------------
// Name: MIL_Inhabitant::ComputeHealthSatisfaction
// Created: JSR 2011-01-14
// -----------------------------------------------------------------------------
void MIL_Inhabitant::ComputeHealthSatisfaction()
{
    float healthSatisfaction;
    if( healthNeed_ == 0 )
        healthSatisfaction = 1.f;
    else
        healthSatisfaction = std::min( 1.f, pLivingArea_->HealthCount() / healthNeed_ );
    if( healthSatisfaction != healthSatisfaction_ )
    {
        healthSatisfactionChanged_ = true;
        healthSatisfaction_ = healthSatisfaction;
    }
}
