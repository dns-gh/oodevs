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
#include "MIL_AgentServer.h"
#include "MIL_InhabitantSatisfactions.h"
#include "Entities/Objects/CrowdCapacity.h"
#include "Entities/Inhabitants/MIL_LivingAreaBlock.h"
#include "MIL_LivingArea.h"
#include "MIL_Schedule.h"
#include "Entities/MIL_Army_ABC.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "MT_Tools/MT_Logger.h"
#include "Tools/NET_AsnException.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"
#include "Tools/MIL_AffinitiesMap.h"
#include "Tools/MIL_DictionaryExtensions.h"
#include "Tools/MIL_IDManager.h"
#include "Tools/MIL_MessageParameters.h"
#include <boost/foreach.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_Inhabitant )

#define VECTOR_TO_POINT( point ) geometry::Point2f( static_cast< float >( ( point ).rX_ ), static_cast< float >( ( point ).rY_ ) )

namespace
{
    MIL_IDManager idManager_;
}

template< typename Archive >
void save_construct_data( Archive& archive, const MIL_Inhabitant* population, const unsigned int /*version*/ )
{
    unsigned int nTypeID = population->type_.GetID();
    archive << nTypeID;
}

template< typename Archive >
void load_construct_data( Archive& archive, MIL_Inhabitant* population, const unsigned int /*version*/ )
{
    unsigned int nTypeID;
    archive >> nTypeID;
    const MIL_InhabitantType* pType = MIL_InhabitantType::Find( nTypeID );
    if( pType )
        ::new( population )MIL_Inhabitant( *pType );
}

// -----------------------------------------------------------------------------
// Name: MIL_Inhabitant constructor
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
MIL_Inhabitant::MIL_Inhabitant( xml::xistream& xis, const MIL_InhabitantType& type, MIL_Army_ABC& army )
    : MIL_Entity_ABC( xis, idManager_.GetId( xis.attribute< unsigned int >( "id" ), true ) )
    , type_                   ( type )
    , pArmy_                  ( &army )
    , movingObjectId_         ( 0 )
    , nNbrHealthyHumans_      ( 0 )
    , nNbrDeadHumans_         ( 0 )
    , nNbrWoundedHumans_      ( 0 )
    , healthStateChanged_     ( false )
    , pSatisfactions_         ( new MIL_InhabitantSatisfactions( xis ) )
    , pAffinities_            ( new MIL_AffinitiesMap( xis ) )
    , pExtensions_            ( new MIL_DictionaryExtensions( xis ) )
{
    xis >> xml::start( "composition" )
            >> xml::attribute( "healthy", nNbrHealthyHumans_ )
            >> xml::attribute( "wounded", nNbrWoundedHumans_ )
            >> xml::attribute( "dead", nNbrDeadHumans_ )
        >> xml::end
        >> xml::start( "information" )
            >> xml::optional >> text_
        >> xml::end;
    pLivingArea_.reset( new MIL_LivingArea( xis, nNbrHealthyHumans_ + nNbrWoundedHumans_ + nNbrDeadHumans_, *this ) );
    pArmy_->RegisterInhabitant( *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_Inhabitant constructor
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
MIL_Inhabitant::MIL_Inhabitant( const MIL_InhabitantType& type )
    : MIL_Entity_ABC( type.GetName(), 0 )
    , type_                   ( type )
    , pArmy_                  ( 0 )
    , movingObjectId_         ( 0 )
    , pLivingArea_            ( 0 )
    , pSchedule_              ( 0 )
    , pSatisfactions_         ( 0 )
    , pAffinities_            ( 0 )
    , pExtensions_            ( 0 )
    , nNbrHealthyHumans_      ( 0 )
    , nNbrDeadHumans_         ( 0 )
    , nNbrWoundedHumans_      ( 0 )
    , healthStateChanged_     ( false )
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
// Name: MIL_Inhabitant::Finalize
// Created: LMT 2011-07-21
// -----------------------------------------------------------------------------
void MIL_Inhabitant::Finalize()
{
    pLivingArea_->Finalize();
    pSchedule_.reset( new MIL_Schedule( *pLivingArea_ ) );
    type_.InitializeSchedule( *pSchedule_ );
    pSatisfactions_->ComputeHealthSatisfaction( pLivingArea_->HealthCount() );
}

// -----------------------------------------------------------------------------
// Name: MIL_Inhabitant::load
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
void MIL_Inhabitant::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    MIL_LivingArea* pLivingArea;
    MIL_InhabitantSatisfactions* pSatisfactions;
    MIL_AffinitiesMap* pAffinities;
    MIL_DictionaryExtensions* pExtensions;
    file >> boost::serialization::base_object< MIL_Entity_ABC >( *this );
    file >> const_cast< MIL_Army_ABC*& >( pArmy_ );
    idManager_.GetId( GetID(), true );
    file >> text_
         >> nNbrHealthyHumans_
         >> nNbrDeadHumans_
         >> nNbrWoundedHumans_
         >> pSatisfactions
         >> pLivingArea
         >> pAffinities
         >> pExtensions
         >> movingObjectId_;
    pLivingArea_.reset( pLivingArea );
    pSatisfactions_.reset( pSatisfactions );
    pAffinities_.reset( pAffinities );
    pExtensions_.reset( pExtensions );
    pSchedule_.reset( new MIL_Schedule( *pLivingArea_ ) );
    if( movingObjectId_ )
        pSchedule_->SetMovingAfterCheckpoint();
    type_.InitializeSchedule( *pSchedule_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_Inhabitant::save
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
void MIL_Inhabitant::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    const MIL_LivingArea* const pLivingArea = pLivingArea_.get();
    const MIL_InhabitantSatisfactions* const pSatisfactions = pSatisfactions_.get();
    const MIL_AffinitiesMap* const pAffinities = pAffinities_.get();
    const MIL_DictionaryExtensions* const pExtensions = pExtensions_.get();
    file << boost::serialization::base_object< MIL_Entity_ABC >( *this );
    file << pArmy_
         << text_
         << nNbrHealthyHumans_
         << nNbrDeadHumans_
         << nNbrWoundedHumans_
         << pSatisfactions
         << pLivingArea
         << pAffinities
         << pExtensions
         << movingObjectId_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Inhabitant::WriteODB
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
void MIL_Inhabitant::WriteODB( xml::xostream& xos ) const
{
    assert( pArmy_ );
    xos << xml::start( "inhabitant" );
    MIL_Entity_ABC::WriteODB ( xos ) ;
    xos << xml::attribute( "id", GetID() )
        << xml::attribute( "type", type_.GetName() )
        << xml::start( "composition" )
            << xml::attribute( "healthy", nNbrHealthyHumans_ )
            << xml::attribute( "wounded", nNbrWoundedHumans_ )
            << xml::attribute( "dead", nNbrDeadHumans_ )
        << xml::end;
    pSatisfactions_->WriteODB( xos );
    xos << xml::content( "information", text_ );
    pLivingArea_->WriteODB( xos );
    pAffinities_->WriteODB( xos );
    pExtensions_->WriteODB( xos );
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: MIL_Inhabitant::SendCreation
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
void MIL_Inhabitant::SendCreation() const
{
    client::PopulationCreation msg;
    msg().mutable_id()->set_id( GetID() );
    msg().mutable_type()->set_id( type_.GetID() );
    msg().mutable_party()->set_id( pArmy_->GetID() );
    msg().set_text( text_ );
    msg().set_name( GetName() );
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
    msg().mutable_id()->set_id( GetID() );
    msg().set_healthy( nNbrHealthyHumans_ );
    msg().set_dead( nNbrDeadHumans_ );
    msg().set_wounded( nNbrWoundedHumans_ );
    if( pSchedule_.get() )
        pSchedule_->SendFullState( msg );
    pLivingArea_->SendFullState( msg );
    pSatisfactions_->SendFullState( msg );
    pAffinities_->SendFullState( msg );
    pExtensions_->SendFullState( msg );
    msg.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: MIL_Inhabitant::UpdateState
// Created: JSR 2011-01-14
// -----------------------------------------------------------------------------
void MIL_Inhabitant::UpdateState()
{
    try
    {
        pSchedule_->Update( MIL_Time_ABC::GetTime().GetRealTime(), MIL_Time_ABC::GetTime().GetTickDuration() );
        // $$$$ _RC_ JSR 2011-03-23: L'objet doit être créé dans le livingarea, dans startmoving, et détruit dans finishmoving!!!
        if( pSchedule_->IsMoving() && !movingObjectId_ )
            CreateInhabitantMovingObject();
        if( !pSchedule_->IsMoving() && movingObjectId_ )
            DestroyInhabitantMovingObject();
        pSatisfactions_->IncreaseSafety( type_.GetSafetyGainPerHour() );
        pSatisfactions_->SetLodgingSatisfaction( pLivingArea_->ComputeOccupationFactor() );
        const MIL_InhabitantType::T_ConsumptionsMap& consumptions = type_.GetConsumptions();
        MIL_LivingArea::T_Blocks angryBlocks;
        for( auto it = consumptions.begin(); it != consumptions.end(); ++it )
        {
            float satisfaction = pLivingArea_->Consume( *it->first, static_cast< unsigned int >( it->second ), angryBlocks );
            pSatisfactions_->SetResourceSatisfaction( *it->first, satisfaction );
        }
        MIL_LivingArea::T_PersonsPerAccomodation occupations;
        pLivingArea_->GetUsagesOccupation( occupations );
        pSatisfactions_->ComputeMotivationSatisfactions( occupations, nNbrHealthyHumans_ + nNbrWoundedHumans_ );
        pSatisfactions_->ComputeHealthSatisfaction( pLivingArea_->HealthCount() );

        // $$$$ BCI 2011-03-18: gestion des foules en colère => comment faire pour ne pas mettre ça en dur dans la sim?
        if( !type_.GetAngryCrowdMissionType().empty() )
            BOOST_FOREACH( MIL_LivingAreaBlock* block, angryBlocks )
                block->ManageAngryCrowd( type_, *pArmy_ );
    }
    catch( const std::exception& e )
    {
        MT_LOG_ERROR_MSG( "Error updating inhabitant " << GetID() << " : " << tools::GetExceptionMsg( e ) );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Inhabitant::UpdateNetwork
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
void MIL_Inhabitant::UpdateNetwork()
{
    try
    {
        client::PopulationUpdate msg;
        msg().mutable_id()->set_id( GetID() );
        if( healthStateChanged_ )
        {
            msg().set_healthy( nNbrHealthyHumans_ );
            msg().set_wounded( nNbrWoundedHumans_ );
            msg().set_dead( nNbrDeadHumans_ );
        }
        if( pSchedule_.get() )
            pSchedule_->UpdateNetwork( msg );
        pLivingArea_->UpdateNetwork( msg );
        pSatisfactions_->UpdateNetwork( msg );
        pAffinities_->UpdateNetwork( msg );
        pExtensions_->UpdateNetwork( msg );
        if( healthStateChanged_ || msg().occupations_size() > 0 || msg().has_satisfaction() || msg().has_adhesions() || msg().has_extension() || msg().has_motivation() )
            msg.Send( NET_Publisher_ABC::Publisher() );
        healthStateChanged_ = false;
    }
    catch( const std::exception& e )
    {
        MT_LOG_ERROR_MSG( "Error updating network for inhabitant " << GetID() << " : " << tools::GetExceptionMsg( e ) );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Inhabitant::OnReceiveInhabitantMagicAction
// Created: ABR 2011-01-26
// -----------------------------------------------------------------------------
void MIL_Inhabitant::OnReceiveUnitMagicAction( const sword::UnitMagicAction& msg )
{
    switch( msg.type() )
    {
    case sword::UnitMagicAction::inhabitant_change_health_state:
        OnReceiveMsgChangeHealthState( msg );
        break;
    case sword::UnitMagicAction::inhabitant_change_affinities:
        pAffinities_->OnReceiveMsgChangeAffinities( msg );
        break;
    case sword::UnitMagicAction::change_extension:
        pExtensions_->OnReceiveMsgChangeExtensions( msg );
        break;
    case sword::UnitMagicAction::rename:
        OnReceiveRename( msg.parameters() );
        break;
    default:
        throw MASA_EXCEPTION_ASN( sword::UnitActionAck_ErrorCode,
            sword::UnitActionAck::error_invalid_unit );
    }
}

namespace
{
    void CheckQuantity( const sword::MissionParameters& parameters, unsigned int index )
    {
        const sword::MissionParameter& parameter = parameters.elem( index );
        if( !parameter.value( 0 ).has_quantity() )
            throw MASA_BADPARAM_MAGICACTION( "parameters[" + boost::lexical_cast< std::string >( index )
            + "] must be a Quantity" );
        int quantity = parameter.value( 0 ).quantity();
        if( quantity < 0 )
            throw MASA_BADPARAM_MAGICACTION( "parameters[" + boost::lexical_cast< std::string >( index )
                + "] must be a positive number" );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Inhabitant::OnReceiveMsgChangeHealthState
// Created: ABR 2011-01-26
// -----------------------------------------------------------------------------
void MIL_Inhabitant::OnReceiveMsgChangeHealthState( const sword::UnitMagicAction& msg )
{
    if( !msg.has_parameters() || msg.parameters().elem_size() != 3 )
        throw MASA_BADPARAM_MAGICACTION( "invalid parameters count, 3 parameters expected" );

    const sword::MissionParameters& parameters = msg.parameters();
    CheckQuantity( parameters, 0 );
    CheckQuantity( parameters, 1 );
    CheckQuantity( parameters, 2 );

    nNbrHealthyHumans_ = parameters.elem( 0 ).value( 0 ).quantity();
    nNbrWoundedHumans_ = parameters.elem( 1 ).value( 0 ).quantity();
    nNbrDeadHumans_ = parameters.elem( 2 ).value( 0 ).quantity();

    healthStateChanged_ = true;
    pLivingArea_->DistributeHumans( nNbrHealthyHumans_ + nNbrWoundedHumans_ + nNbrDeadHumans_ );
    if( movingObjectId_ )
    {
        MIL_Object_ABC* obj = MIL_AgentServer::GetWorkspace().GetEntityManager().FindObject( movingObjectId_ );
        if( obj )
            obj->Get< CrowdCapacity >().SetDensityFactor( static_cast< double >( nNbrHealthyHumans_ ) /( nNbrHealthyHumans_ + nNbrWoundedHumans_ + nNbrDeadHumans_ ) );
    }
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
// Name: MIL_Inhabitant::NotifyFired
// Created: JSR 2011-01-25
// -----------------------------------------------------------------------------
void MIL_Inhabitant::NotifyFired()
{
    pSatisfactions_->DecreaseSafety( type_.GetSafetyLossOnFire() );
}

// -----------------------------------------------------------------------------
// Name: MIL_Inhabitant::NotifyAlerted
// Created: BCI 2011-02-01
// -----------------------------------------------------------------------------
void MIL_Inhabitant::NotifyAlerted( const TER_Localisation& localisation )
{
    pLivingArea_->Alert( localisation );
}

// -----------------------------------------------------------------------------
// Name: MIL_Inhabitant::NotifyAlerted
// Created: CCD 2012-01-11
// -----------------------------------------------------------------------------
void MIL_Inhabitant::NotifyUndoAlerted( const TER_Localisation& localisation )
{
    pLivingArea_->Alert( localisation, false );
}

// -----------------------------------------------------------------------------
// Name: MIL_Inhabitant::NotifyConfined
// Created: BCI 2011-02-18
// -----------------------------------------------------------------------------
void MIL_Inhabitant::NotifyConfined( const TER_Localisation& localisation )
{
    pLivingArea_->Confine( localisation );
}

// -----------------------------------------------------------------------------
// Name: MIL_Inhabitant::NotifyUndoConfined
// Created: CCD 2012-01-11
// -----------------------------------------------------------------------------
void MIL_Inhabitant::NotifyUndoConfined( const TER_Localisation& localisation )
{
    pLivingArea_->Confine( localisation, false );
}

// -----------------------------------------------------------------------------
// Name: MIL_Inhabitant::NotifyEvacuate
// Created: CCD 2012-01-13
// -----------------------------------------------------------------------------
void MIL_Inhabitant::NotifyEvacuated( const TER_Localisation& localisation )
{
    pLivingArea_->Evacuate( localisation );
}

// -----------------------------------------------------------------------------
// Name: MIL_Inhabitant::NotifyEvacuate
// Created: CCD 2012-01-18
// -----------------------------------------------------------------------------
void MIL_Inhabitant::NotifyUndoEvacuated( const TER_Localisation& localisation )
{
    pLivingArea_->Evacuate( localisation, false );
}

// -----------------------------------------------------------------------------
// Name: MIL_Inhabitant::IsAlerted
// Created: BCI 2011-02-18
// -----------------------------------------------------------------------------
bool MIL_Inhabitant::IsAlerted( const TER_Localisation& localisation ) const
{
    return pLivingArea_->IsAlerted( localisation );
}

// -----------------------------------------------------------------------------
// Name: MIL_Inhabitant::IsConfined
// Created: CCD 2012-01-10
// -----------------------------------------------------------------------------
bool MIL_Inhabitant::IsConfined( const TER_Localisation& localisation ) const
{
    return pLivingArea_->IsConfined( localisation );
}

// -----------------------------------------------------------------------------
// Name: MIL_Inhabitant::IsEvacuated
// Created: CCD 2012-01-13
// -----------------------------------------------------------------------------
bool MIL_Inhabitant::IsEvacuated( const TER_Localisation& localisation ) const
{
    return pLivingArea_->IsEvacuated( localisation );
}

// -----------------------------------------------------------------------------
// Name: MIL_Inhabitant::ComputeSpeed
// Created: SLG 2011-01-27
// -----------------------------------------------------------------------------
void MIL_Inhabitant::CreateInhabitantMovingObject()
{
    if( nNbrHealthyHumans_ + nNbrWoundedHumans_ + nNbrDeadHumans_ == 0 )
        return;

    T_PointVector hull = pLivingArea_->ComputeMovingArea();
    if( hull.size() < 3 )
        return;

    MIL_Object_ABC* obj = MIL_AgentServer::GetWorkspace().GetEntityManager().CreateObject( "population moving", pArmy_, TER_Localisation( TER_Localisation::ePolygon, hull ) );
    CrowdCapacity* capacity = new CrowdCapacity( type_.GetAssociatedCrowdType(), static_cast< double >( nNbrHealthyHumans_ ) /( nNbrHealthyHumans_ + nNbrWoundedHumans_ + nNbrDeadHumans_ ) );
    capacity->Register( *obj );
    movingObjectId_ = obj->GetID();
}

// -----------------------------------------------------------------------------
// Name: MIL_Inhabitant::DestroyInhabitantMovingObject
// Created: SLG 2011-02-02
// -----------------------------------------------------------------------------
void MIL_Inhabitant::DestroyInhabitantMovingObject()
{
    MIL_Object_ABC* obj = MIL_AgentServer::GetWorkspace().GetEntityManager().FindObject( movingObjectId_ );
    if( obj )
        obj->MarkForDestruction();
    movingObjectId_ = 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_Inhabitant::ReStartMotivation
// Created: ABR 2011-03-22
// -----------------------------------------------------------------------------
void MIL_Inhabitant::ReStartMotivation()
{
    pSchedule_->RestartLastEvent();
}

// -----------------------------------------------------------------------------
// Name: MIL_Inhabitant::GetAffinity
// Created: ABR 2011-03-28
// -----------------------------------------------------------------------------
float MIL_Inhabitant::GetAffinity( unsigned long teamID ) const
{
    return pAffinities_->GetAffinity( teamID );
}

// -----------------------------------------------------------------------------
// Name: MIL_Inhabitant::CanEmitReports
// Created: NPT 2013-02-07
// -----------------------------------------------------------------------------
bool MIL_Inhabitant::CanEmitReports() const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_Inhabitant::OnReceiveRename
// Created: ABR 2014-08-27
// -----------------------------------------------------------------------------
void MIL_Inhabitant::OnReceiveRename( const sword::MissionParameters& parameters )
{
    protocol::CheckCount( parameters, 1 );
    const auto& name = protocol::GetString( parameters, 0 );
    SetName( name );
    client::PopulationUpdate asn;
    asn().mutable_id()->set_id( GetID() );
    asn().set_name( name.c_str() );
    asn.Send( NET_Publisher_ABC::Publisher() );
}
