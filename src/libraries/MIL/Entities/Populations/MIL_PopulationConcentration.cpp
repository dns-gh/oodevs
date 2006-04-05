// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: NLD 2005-09-28 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#include "MIL_pch.h"
#include "MIL_PopulationConcentration.h"

#include "MIL_PopulationAttitude.h"
#include "MIL_PopulationFlow.h"
#include "MIL_Population.h"
#include "MIL_PopulationType.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Objects/MIL_RealObject_ABC.h"
#include "Tools/MIL_Tools.h"
#include "Tools/MIL_IDManager.h"
#include "Network/NET_ASN_Messages.h"
#include "Network/NET_ASN_Tools.h"
#include "CheckPoints/MIL_CheckPointSerializationHelpers.h"

BOOST_CLASS_EXPORT_GUID( MIL_PopulationConcentration, "MIL_PopulationConcentration" )

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration constructor
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
MIL_PopulationConcentration::MIL_PopulationConcentration( MIL_Population& population, MIL_InputArchive& archive )
    : MIL_PopulationElement_ABC      ( population, MIL_IDManager::populationConcentrations_.GetFreeSimID() )
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
    archive.ReadField( "Position", strPosition );
    MIL_Tools::ConvertCoordMosToSim( strPosition, position_ );
    MT_Float rNbrHumans;
    archive.ReadField( "NombreHumains", rNbrHumans, CheckValueGreater( 0. ) );
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
    : MIL_PopulationElement_ABC      (  population, MIL_IDManager::populationConcentrations_.GetFreeSimID() )
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
// Name: MIL_PopulationConcentration constructor
// Created: SBO 2005-10-18
// -----------------------------------------------------------------------------
MIL_PopulationConcentration::MIL_PopulationConcentration()
    : MIL_PopulationElement_ABC      ()
    , TER_PopulationConcentration_ABC()
    , position_                      ()
    , location_                      ()
    , pPullingFlow_                  ( 0 )
    , pushingFlows_                  ()
    , rPullingFlowsDensity_          ( 0. )
    , pSplittingObject_              ( 0 )
{
    // NOTHING
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

// =============================================================================
// UPDATE
// =============================================================================

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
    agent.GetRole< PHY_RoleInterface_Location >().NotifyPopulationCollision( *this );
}

// =============================================================================
// ACTIONS
// =============================================================================

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

// =============================================================================
// TOOLS
// =============================================================================

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
    bool bOut = pushingFlows_.insert( &flow ).second;
    assert( bOut );
    SetAttitude( flow.GetAttitude() );   
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::GetSafetyPosition
// Created: SBO 2005-12-16
// -----------------------------------------------------------------------------
MT_Vector2D MIL_PopulationConcentration::GetSafetyPosition( const MIL_AgentPion& agent, MT_Float rMinDistance, MT_Float rSeed ) const
{
    const MT_Vector2D& agentPosition = agent.GetRole< PHY_RolePion_Location >().GetPosition();
    MT_Vector2D evadeDirection = ( agentPosition - position_ ).Normalize().Rotate( rSeed );

    if( evadeDirection.IsZero() )
        evadeDirection = -agent.GetDirDanger();

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
    {
        const MT_Float rObjectPullingDensity = pSplittingObject_->GetExitingPopulationDensity();
        if( rObjectPullingDensity != std::numeric_limits< MT_Float >::max() )
            return rObjectPullingDensity;
    }
    return rPullingFlowsDensity_;  
}


// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::SendCreation
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
void MIL_PopulationConcentration::SendCreation() const
{
    NET_ASN_MsgPopulationConcentrationCreation asnMsg;
    asnMsg.GetAsnMsg().oid_concentration = GetID();
    asnMsg.GetAsnMsg().oid_population    = GetPopulation().GetID();
    NET_ASN_Tools::WritePoint( position_, asnMsg.GetAsnMsg().position ); 
    asnMsg.Send();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::SendDestruction
// Created: NLD 2005-10-05
// -----------------------------------------------------------------------------
void MIL_PopulationConcentration::SendDestruction() const
{
    NET_ASN_MsgPopulationConcentrationDestruction asnMsg;
    asnMsg.GetAsnMsg().oid_concentration = GetID();
    asnMsg.GetAsnMsg().oid_population    = GetPopulation().GetID();
    asnMsg.Send();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::SendFullState
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
void MIL_PopulationConcentration::SendFullState() const
{
    NET_ASN_MsgPopulationConcentrationUpdate asnMsg;
    asnMsg.GetAsnMsg().oid_concentration = GetID();
    asnMsg.GetAsnMsg().oid_population    = GetPopulation().GetID();

    asnMsg.GetAsnMsg().m.attitudePresent           = 1;
    asnMsg.GetAsnMsg().m.nb_humains_mortsPresent   = 1;
    asnMsg.GetAsnMsg().m.nb_humains_vivantsPresent = 1;
    
    asnMsg.GetAsnMsg().attitude           = GetAttitude().GetAsnID();
    asnMsg.GetAsnMsg().nb_humains_morts   = GetPopulation().GetBoundedPeople( GetNbrDeadHumans () );
    asnMsg.GetAsnMsg().nb_humains_vivants = GetPopulation().GetBoundedPeople( GetNbrAliveHumans() );

    asnMsg.Send();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::SendChangedState
// Created: NLD 2005-10-04
// -----------------------------------------------------------------------------
void MIL_PopulationConcentration::SendChangedState() const
{
    if( !HasChanged() )
        return;

    NET_ASN_MsgPopulationConcentrationUpdate asnMsg;
    asnMsg.GetAsnMsg().oid_concentration = GetID();
    asnMsg.GetAsnMsg().oid_population    = GetPopulation().GetID();

    if( HasAttitudeChanged() )
    {
        asnMsg.GetAsnMsg().m.attitudePresent = 1;
        asnMsg.GetAsnMsg().attitude          = GetAttitude().GetAsnID();
    }

    if( HasHumansChanged() )
    {
        asnMsg.GetAsnMsg().m.nb_humains_mortsPresent   = 1;
        asnMsg.GetAsnMsg().m.nb_humains_vivantsPresent = 1;
    
        asnMsg.GetAsnMsg().nb_humains_morts   = GetPopulation().GetBoundedPeople( GetNbrDeadHumans () );
        asnMsg.GetAsnMsg().nb_humains_vivants = GetPopulation().GetBoundedPeople( GetNbrAliveHumans() );
    }

    asnMsg.Send();
}

// =============================================================================
// CHECKPOINT
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::load
// Created: SBO 2005-10-18
// -----------------------------------------------------------------------------
void MIL_PopulationConcentration::load( MIL_CheckPointInArchive& file, const uint )
{
    file >> boost::serialization::base_object< TER_PopulationConcentration_ABC >( *this );
    file >> boost::serialization::base_object< MIL_PopulationElement_ABC       >( *this );
    
    file >> position_
         >> location_
         >> pPullingFlow_
         >> pushingFlows_
         >> rPullingFlowsDensity_
         >> const_cast< MIL_RealObject_ABC* >( pSplittingObject_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::save
// Created: SBO 2005-10-18
// -----------------------------------------------------------------------------
void MIL_PopulationConcentration::save( MIL_CheckPointOutArchive& file, const uint ) const
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
