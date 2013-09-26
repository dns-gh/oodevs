// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_PopulationConcentration.h"
#include "MIL_PopulationAttitude.h"
#include "MIL_PopulationFlow.h"
#include "MIL_Population.h"
#include "MIL_PopulationType.h"
#include "Entities/Orders/MIL_Report.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Objects/AnimatorAttribute.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/Objects/PopulationAttribute.h"
#include "Network/NET_ASN_Tools.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"
#include "simulation_kernel/PopulationCollisionNotificationHandler_ABC.h"
#include "simulation_terrain/TER_World.h"
#include "MT_Tools/MT_Logger.h"
#include "Tools/MIL_Tools.h"
#include "Tools/MIL_IDManager.h"
#include <boost/make_shared.hpp>
#include <boost/serialization/set.hpp>
#include <boost/serialization/vector.hpp>
#include <xeumeuleu/xml.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_PopulationConcentration )

MIL_IDManager MIL_PopulationConcentration::idManager_;

template< typename Archive >
void save_construct_data( Archive& archive, const MIL_PopulationConcentration* concentration, const unsigned int /*version*/ )
{
    MIL_Population* const pPopulation = &concentration->GetPopulation();
    unsigned int nID = concentration->GetID();
    archive << pPopulation << nID;
}

template< typename Archive >
void load_construct_data( Archive& archive, MIL_PopulationConcentration* concentration, const unsigned int /*version*/ )
{
    MIL_Population* pPopulation;
    unsigned int nID;
    archive >> pPopulation >> nID;
    ::new( concentration )MIL_PopulationConcentration( *pPopulation, nID );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration constructor
// Created: JSR 2010-11-17
// -----------------------------------------------------------------------------
MIL_PopulationConcentration::MIL_PopulationConcentration( MIL_Population& population, unsigned int id )
    : MIL_PopulationElement_ABC( population, id )
    , TER_PopulationConcentration_ABC()
    , pPullingFlow_        ( 0 )
    , rPullingFlowsDensity_( population.GetDefaultFlowDensity() )
    , pSplittingObject_    ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration constructor
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
MIL_PopulationConcentration::MIL_PopulationConcentration( MIL_Population& population, xml::xistream& xis )
    : MIL_PopulationElement_ABC( population, idManager_.GetId() )
    , pPullingFlow_        ( 0 )
    , rPullingFlowsDensity_( population.GetDefaultFlowDensity() )
    , pSplittingObject_    ( 0 )
{
    // Position
    MIL_Tools::ConvertCoordMosToSim( xis.attribute< std::string >( "position" ), position_ );
    xis.start( "composition" );
    PushHumans( MIL_PopulationHumans( xis ) );
    xis.end();
    UpdateLocation();
    UpdateDensity();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration constructor
// Created: NLD 2005-10-05
// -----------------------------------------------------------------------------
MIL_PopulationConcentration::MIL_PopulationConcentration( MIL_Population& population, const MT_Vector2D& position, unsigned int nHumans )
    : MIL_PopulationElement_ABC( population, idManager_.GetId() )
    , position_            ( position )
    , pPullingFlow_        ( 0 )
    , rPullingFlowsDensity_( population.GetDefaultFlowDensity() )
    , pSplittingObject_    ( 0 )
{
    PushHumans( MIL_PopulationHumans( nHumans ) );
    UpdateLocation();
    UpdateDensity();
    SendCreation();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration constructor
// Created: BCI 2011-03-21
// -----------------------------------------------------------------------------
MIL_PopulationConcentration::MIL_PopulationConcentration( MIL_Population& population, const MT_Vector2D& position, const MIL_PopulationHumans& humans )
    : MIL_PopulationElement_ABC( population, idManager_.GetId() )
    , position_            ( position )
    , pPullingFlow_        ( 0 )
    , rPullingFlowsDensity_( population.GetDefaultFlowDensity() )
    , pSplittingObject_    ( 0 )
{
    PushHumans( humans );
    UpdateLocation();
    UpdateDensity();
    SendCreation();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration destructor
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
MIL_PopulationConcentration::~MIL_PopulationConcentration()
{
    assert( !pPullingFlow_ );
    assert( pushingFlows_.empty() );
    SendDestruction();
    RemoveFromPatch();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::Update
// Created: NLD 2005-10-04
// -----------------------------------------------------------------------------
bool MIL_PopulationConcentration::Update()
{
    if( pSplittingObject_ && pSplittingObject_->IsMarkedForDestruction() )
        pSplittingObject_ = 0;
    ClearCollisions();
    if( !IsValid() )
    {
        if( pPullingFlow_ )
        {
            pPullingFlow_->UnregisterSourceConcentration();
            pPullingFlow_ = 0;
        }
        RemoveFromPatch();
        return false;
    }
    if( HasHumansChanged() )
    {
        UpdateLocation();
        UpdateDensity ();
    }
    UpdateCollisions();
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::UpdateLocation
// Created: NLD 2005-10-07
// -----------------------------------------------------------------------------
void MIL_PopulationConcentration::UpdateLocation()
{
    assert( GetPopulation().GetType().GetConcentrationDensity() );
    double rSurface = static_cast< double >( GetTotalLivingHumans() ) / GetPopulation().GetType().GetConcentrationDensity();
    location_.Reset( TER_Localisation( position_, std::sqrt( rSurface / MT_PI ) ) );
    UpdatePatch();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::NotifyCollision
// Created: NLD 2005-11-03
// -----------------------------------------------------------------------------
void MIL_PopulationConcentration::NotifyCollision( MIL_Agent_ABC& agent )
{
    agent.Apply( &PopulationCollisionNotificationHandler_ABC::NotifyConcentrationCollision, *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::MagicMove
// Created: NLD 2005-12-06
// -----------------------------------------------------------------------------
void MIL_PopulationConcentration::MagicMove( const MT_Vector2D& destination )
{
    if( position_ != destination )
    {
        MIL_PopulationConcentration& newConcentration = GetPopulation().GetConcentration( destination );
        newConcentration.PushHumans( PullHumans( GetAllHumans() ) );
    }
    if( pPullingFlow_ )
    {
        pPullingFlow_->UnregisterSourceConcentration();
        pPullingFlow_ = 0;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::Move
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
void MIL_PopulationConcentration::Move( const MT_Vector2D& destination )
{
    if( pPullingFlow_ || IsNearPosition( destination ) )
        return;
    pPullingFlow_ = &GetPopulation().CreateFlow( *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::IsNearPosition
// Created: NLD 2005-10-05
// -----------------------------------------------------------------------------
bool MIL_PopulationConcentration::IsNearPosition( const MT_Vector2D& position ) const
{
    static double rPrecision = 100.;
    return position_.Distance( position ) <= rPrecision;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::RegisterPushingFlow
// Created: NLD 2005-10-05
// -----------------------------------------------------------------------------
void MIL_PopulationConcentration::RegisterPushingFlow( MIL_PopulationFlow& flow )
{
    if( !pushingFlows_.insert( &flow ).second )
        MT_LOG_ERROR_MSG( __FUNCTION__ << " : Insert failed" );
    SetAttitude( flow.GetAttitude() );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::GetSafetyPosition
// Created: SBO 2005-12-16
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > MIL_PopulationConcentration::GetSafetyPosition( const MIL_AgentPion& agent, double rMinDistance, double rSeed ) const
{
    const MT_Vector2D& agentPosition = agent.GetRole< PHY_RoleInterface_Location >().GetPosition();
    MT_Vector2D evadeDirection = ( agentPosition - position_ ).Normalize().Rotate( rSeed );
    if( evadeDirection.IsZero() )
        evadeDirection = -agent.GetOrderManager().GetDirDanger();
    auto safetyPos = boost::make_shared< MT_Vector2D >( location_.GetCircleCenter() + evadeDirection * ( location_.GetCircleRadius() + rMinDistance ) );
    TER_World::GetWorld().ClipPointInsideWorld( *safetyPos );
    return safetyPos;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::GetPullingFlowsDensity
// Created: NLD 2005-12-11
// -----------------------------------------------------------------------------
double MIL_PopulationConcentration::GetPullingFlowsDensity() const
{
    if( pSplittingObject_ )
        return pSplittingObject_->GetAttribute< PopulationAttribute >().GetDensity();
    return rPullingFlowsDensity_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::SetPullingFlowsDensity
// Created: NLD 2005-12-11
// -----------------------------------------------------------------------------
void MIL_PopulationConcentration::SetPullingFlowsDensity( const MIL_Object_ABC& splittingObject )
{
    pSplittingObject_ = &splittingObject;

    //$$$ RC TMP
    if( pSplittingObject_->GetAttribute< PopulationAttribute >().GetDensity() == 0. )
        MIL_Report::PostEvent( GetPopulation(), report::eRC_Bloquee );
    else
    {
        const AnimatorAttribute* animatorAttribute = pSplittingObject_->RetrieveAttribute<AnimatorAttribute>();
        if( animatorAttribute && animatorAttribute->GetAnimators().size() > 0 )
            MIL_Report::PostEvent( GetPopulation(), report::eRC_Filtree );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::SendCreation
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
void MIL_PopulationConcentration::SendCreation(  unsigned int context ) const
{
    client::CrowdConcentrationCreation asnMsg;
    asnMsg().mutable_concentration()->set_id( GetID() );
    asnMsg().mutable_crowd()->set_id( GetPopulation().GetID() );
    NET_ASN_Tools::WritePoint( position_, *asnMsg().mutable_position() );
    asnMsg.Send( NET_Publisher_ABC::Publisher(), context );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::SendDestruction
// Created: NLD 2005-10-05
// -----------------------------------------------------------------------------
void MIL_PopulationConcentration::SendDestruction() const
{
    client::CrowdConcentrationDestruction asnMsg;
    asnMsg().mutable_concentration()->set_id( GetID() );
    asnMsg().mutable_crowd()->set_id( GetPopulation().GetID() );
    asnMsg.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::SendFullState
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
void MIL_PopulationConcentration::SendFullState() const
{
    client::CrowdConcentrationUpdate asnMsg;
    asnMsg().mutable_concentration()->set_id( GetID() );
    asnMsg().mutable_crowd()->set_id( GetPopulation().GetID() );
    asnMsg().set_healthy( GetHealthyHumans() );
    asnMsg().set_wounded( GetWoundedHumans() );
    asnMsg().set_contaminated( GetContaminatedHumans() );
    asnMsg().set_dead( GetDeadHumans() );
    asnMsg().set_attitude( GetAttitude().GetAsnID() );
    asnMsg.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::SendChangedState
// Created: NLD 2005-10-04
// -----------------------------------------------------------------------------
void MIL_PopulationConcentration::SendChangedState() const
{
    if( !HasChanged() )
        return;
    client::CrowdConcentrationUpdate asnMsg;
    asnMsg().mutable_concentration()->set_id( GetID() );
    asnMsg().mutable_crowd()->set_id( GetPopulation().GetID() );
    if( HasAttitudeChanged() )
        asnMsg().set_attitude( GetAttitude().GetAsnID() );
    if( HasHumansChanged() )
    {
        asnMsg().set_healthy( GetHealthyHumans() );
        asnMsg().set_wounded( GetWoundedHumans() );
        asnMsg().set_contaminated( GetContaminatedHumans() );
        asnMsg().set_dead( GetDeadHumans() );
    }
    asnMsg.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::load
// Created: SBO 2005-10-18
// -----------------------------------------------------------------------------
void MIL_PopulationConcentration::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< TER_PopulationConcentration_ABC >( *this );
    file >> boost::serialization::base_object< MIL_PopulationElement_ABC       >( *this );
    file >> position_
         >> pPullingFlow_
         >> pushingFlows_
         >> rPullingFlowsDensity_
         >> const_cast< MIL_Object_ABC*& >( pSplittingObject_ );
    idManager_.GetId( MIL_PopulationElement_ABC::GetID(), true );
    UpdateLocation();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::save
// Created: SBO 2005-10-18
// -----------------------------------------------------------------------------
void MIL_PopulationConcentration::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< TER_PopulationConcentration_ABC >( *this );
    file << boost::serialization::base_object< MIL_PopulationElement_ABC       >( *this );
    file << position_
         << pPullingFlow_
         << pushingFlows_
         << rPullingFlowsDensity_
         << pSplittingObject_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::GetSplittingObject
// Created: NLD 2006-04-27
// -----------------------------------------------------------------------------
const MIL_Object_ABC* MIL_PopulationConcentration::GetSplittingObject() const
{
    return pSplittingObject_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::GetPosition
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
const MT_Vector2D& MIL_PopulationConcentration::GetPosition() const
{
    return position_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::Clean
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
void MIL_PopulationConcentration::Clean()
{
    MIL_PopulationElement_ABC::Clean();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::HasChanged
// Created: NLD 2005-10-04
// -----------------------------------------------------------------------------
bool MIL_PopulationConcentration::HasChanged() const
{
    return HasAttitudeChanged() || HasHumansChanged();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::UnregisterPushingFlow
// Created: NLD 2005-10-05
// -----------------------------------------------------------------------------
void MIL_PopulationConcentration::UnregisterPushingFlow( MIL_PopulationFlow& flow )
{
    if( pushingFlows_.erase( &flow ) != 1 )
        MT_LOG_ERROR_MSG( __FUNCTION__ << " : Erase failed" );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::GetLocation
// Created: NLD 2005-10-07
// -----------------------------------------------------------------------------
const TER_Localisation& MIL_PopulationConcentration::GetLocation() const
{
    return location_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::GetSecuringPoint
// Created: SBO 2005-12-16
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > MIL_PopulationConcentration::GetSecuringPoint( const MIL_Agent_ABC& /*securingAgent*/ ) const
{
    return boost::make_shared< MT_Vector2D >( position_ ); // $$$$ SBO 2005-12-16: should maybe return a random point into concentration
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::IsValid
// Created: NLD 2005-10-13
// -----------------------------------------------------------------------------
bool MIL_PopulationConcentration::IsValid() const
{
    return GetAllHumans() > 0. || !pushingFlows_.empty();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::CanBePerceived
// Created: NLD 2005-12-07
// -----------------------------------------------------------------------------
bool MIL_PopulationConcentration::CanBePerceived() const
{
    return IsValid();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::GetDefaultDensity
// Created: SLG 2011-02-11
// -----------------------------------------------------------------------------
double MIL_PopulationConcentration::GetDefaultDensity( const MIL_PopulationType& type ) const
{
    return type.GetConcentrationDensity();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::Intersect2DWithCircle
// Created: SLI 2012-09-26
// -----------------------------------------------------------------------------
bool MIL_PopulationConcentration::Intersect2DWithCircle( const MT_Vector2D& vCircleCenter, double rRadius, std::vector< MT_Vector2D >& /*shape*/ ) const
{
    return TER_PopulationConcentration_ABC::Intersect2DWithCircle( vCircleCenter, rRadius );
}
