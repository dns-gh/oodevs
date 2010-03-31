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
#include "CheckPoints/MIL_CheckPointSerializationHelpers.h"
#include "Entities/Orders/MIL_Report.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/Objects/PopulationAttribute.h"
#include "Tools/MIL_IDManager.h"
#include "Network/NET_ASN_Tools.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"
#include "simulation_kernel/PopulationCollisionNotificationHandler_ABC.h"
#include "Tools/MIL_Tools.h"
#include "Tools/MIL_IDManager.h"
#include <xeumeuleu/xml.h>

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_PopulationConcentration )

MIL_IDManager MIL_PopulationConcentration::idManager_;
template< typename Archive >
void save_construct_data( Archive& archive, const MIL_PopulationConcentration* concentration, const unsigned int /*version*/ )
{
    MIL_Population* const pPopulation = &concentration->GetPopulation();
    archive << pPopulation << concentration->position_;
}

template< typename Archive >
void load_construct_data( Archive& archive, MIL_PopulationConcentration* concentration, const unsigned int /*version*/ )
{
    MIL_Population* pPopulation;
    MT_Vector2D position;
    archive >> pPopulation >> position;
    ::new( concentration )MIL_PopulationConcentration( *pPopulation, position);
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration constructor
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
MIL_PopulationConcentration::MIL_PopulationConcentration( MIL_Population& population, xml::xistream& xis )
    : MIL_PopulationElement_ABC      ( population, idManager_.GetFreeId() )
    , TER_PopulationConcentration_ABC()
    , position_                      ()
    , location_                      ()
    , pPullingFlow_                  ( 0 )
    , pushingFlows_                  ()
    , rPullingFlowsDensity_          ( population.GetDefaultFlowDensity() )
    , pSplittingObject_              ( 0 )
{
    // Position
    std::string strPosition;
    xis >> xml::attribute( "position", strPosition );
    MIL_Tools::ConvertCoordMosToSim( strPosition, position_ );
    MT_Float rNbrHumans;
    xis >> xml::attribute( "humans", rNbrHumans );
    if( rNbrHumans <= 0. )
        xis.error( "rNbrHumans is not greater than 0." );

    PushHumans( T_Humans( rNbrHumans, 0. ) );
        
    UpdateLocation();
    UpdateDensity ();
    // SendCreation()
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration constructor
// Created: NLD 2005-10-05
// -----------------------------------------------------------------------------
MIL_PopulationConcentration::MIL_PopulationConcentration( MIL_Population& population, const MT_Vector2D& position )
    : MIL_PopulationElement_ABC      ( population, idManager_.GetFreeId() )
    , TER_PopulationConcentration_ABC()
    , position_                      ( position )
    , location_                      ()
    , pPullingFlow_                  ( 0 )
    , pushingFlows_                  ()
    , rPullingFlowsDensity_          ( population.GetDefaultFlowDensity() )
    , pSplittingObject_              ( 0 )
{
    UpdateLocation();
    UpdateDensity ();
    SendCreation  ();
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
            pPullingFlow_->UnregisterSourceConcentration( *this );
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
    MT_Float rSurface = GetNbrAliveHumans() / GetPopulation().GetType().GetConcentrationDensity();
    location_.Reset( TER_Localisation( position_, std::sqrt( rSurface / MT_PI ) ) );
    UpdatePatch();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::NotifyCollision
// Created: NLD 2005-11-03
// -----------------------------------------------------------------------------
void MIL_PopulationConcentration::NotifyCollision( MIL_Agent_ABC& agent )
{
    agent.Apply( &population::PopulationCollisionNotificationHandler_ABC::NotifyConcentrationCollision, *this );
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
        newConcentration.PushHumans( PullHumans( GetNbrHumans() ) );
    }

    if( pPullingFlow_ )
    {
        pPullingFlow_->UnregisterSourceConcentration( *this );
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
    pPullingFlow_->Move( destination );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::IsNearPosition
// Created: NLD 2005-10-05
// -----------------------------------------------------------------------------
bool MIL_PopulationConcentration::IsNearPosition( const MT_Vector2D& position ) const
{
    static MT_Float rPrecision = 100.;
    return position_.Distance( position ) <= rPrecision;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::RegisterPushingFlow
// Created: NLD 2005-10-05
// -----------------------------------------------------------------------------
void MIL_PopulationConcentration::RegisterPushingFlow( MIL_PopulationFlow& flow )
{
    if( ! pushingFlows_.insert( &flow ).second )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Insert failed" );
    SetAttitude( flow.GetAttitude() );   
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::GetSafetyPosition
// Created: SBO 2005-12-16
// -----------------------------------------------------------------------------
MT_Vector2D MIL_PopulationConcentration::GetSafetyPosition( const MIL_AgentPion& agent, MT_Float rMinDistance, MT_Float rSeed ) const
{
    const MT_Vector2D& agentPosition = agent.GetRole< PHY_RoleInterface_Location >().GetPosition();
    MT_Vector2D evadeDirection = ( agentPosition - position_ ).Normalize().Rotate( rSeed );

    if( evadeDirection.IsZero() )
        evadeDirection = -agent.GetOrderManager().GetDirDanger();

    MT_Vector2D safetyPos = location_.GetCircleCenter() + evadeDirection * ( location_.GetCircleRadius() + rMinDistance );
    TER_World::GetWorld().ClipPointInsideWorld( safetyPos );
    return safetyPos;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::GetPullingFlowsDensity
// Created: NLD 2005-12-11
// -----------------------------------------------------------------------------
MT_Float MIL_PopulationConcentration::GetPullingFlowsDensity() const
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
        MIL_Report::PostEvent( GetPopulation(), MIL_Report::eReport_Blocked );
    else
        MIL_Report::PostEvent( GetPopulation(), MIL_Report::eReport_Filtered );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::SendCreation
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
void MIL_PopulationConcentration::SendCreation() const
{
    client::PopulationConcentrationCreation asnMsg;
    asnMsg().set_oid( GetID() );
    asnMsg().set_oid_population( GetPopulation().GetID() );
    NET_ASN_Tools::WritePoint( position_, *asnMsg().mutable_position() ); 
    asnMsg.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::SendDestruction
// Created: NLD 2005-10-05
// -----------------------------------------------------------------------------
void MIL_PopulationConcentration::SendDestruction() const
{
    client::PopulationConcentrationDestruction asnMsg;
    asnMsg().set_oid( GetID() );
    asnMsg().set_oid_population( GetPopulation().GetID() );
    asnMsg.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::SendFullState
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
void MIL_PopulationConcentration::SendFullState( MIL_Population::sPeopleCounter& peopleCounter ) const
{
    client::PopulationConcentrationUpdate asnMsg;
    asnMsg().set_oid( GetID() );
    asnMsg().set_oid_population    ( GetPopulation().GetID() );
    asnMsg().set_attitude          ( GetAttitude().GetAsnID() );
    asnMsg().set_nb_humains_morts  ( peopleCounter.GetBoundedPeople( GetNbrDeadHumans () ) );
    asnMsg().set_nb_humains_vivants( peopleCounter.GetBoundedPeople( GetNbrAliveHumans() ) );

    asnMsg.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::SendChangedState
// Created: NLD 2005-10-04
// -----------------------------------------------------------------------------
void MIL_PopulationConcentration::SendChangedState( MIL_Population::sPeopleCounter& peopleCounter ) const
{
    if( !HasChanged() )
        return;

    client::PopulationConcentrationUpdate asnMsg;
    asnMsg().set_oid( GetID() );
    asnMsg().set_oid_population( GetPopulation().GetID() );

    if( HasAttitudeChanged() )
    {
        asnMsg().set_attitude( GetAttitude().GetAsnID() );
    }

    if( HasHumansChanged() )
    {
        asnMsg().set_nb_humains_morts  ( peopleCounter.GetBoundedPeople( GetNbrDeadHumans () ) );
        asnMsg().set_nb_humains_vivants( peopleCounter.GetBoundedPeople( GetNbrAliveHumans() ) );
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
         >> location_
         >> pPullingFlow_
         >> pushingFlows_
         >> rPullingFlowsDensity_
         >> const_cast< MIL_Object_ABC*& >( pSplittingObject_ );
    idManager_.Lock( MIL_PopulationElement_ABC::GetID() );
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
         << location_
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
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Erase failed" );
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
MT_Vector2D MIL_PopulationConcentration::GetSecuringPoint( const MIL_Agent_ABC& /*securingAgent*/ ) const
{
    return position_; // $$$$ SBO 2005-12-16: should maybe return a random point into concentration
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::IsValid
// Created: NLD 2005-10-13
// -----------------------------------------------------------------------------
bool MIL_PopulationConcentration::IsValid() const
{
    return GetNbrHumans() > 0. || !pushingFlows_.empty();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::CanBePerceived
// Created: NLD 2005-12-07
// -----------------------------------------------------------------------------
bool MIL_PopulationConcentration::CanBePerceived() const
{
    return IsValid();
}
