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
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Tools/MIL_Tools.h"
#include "Network/NET_ASN_Messages.h"
#include "Network/NET_ASN_Tools.h"
#include "CheckPoints/MIL_CheckPointSerializationHelpers.h"

MIL_MOSIDManager MIL_PopulationConcentration::idManager_;

BOOST_CLASS_EXPORT_GUID( MIL_PopulationConcentration, "MIL_PopulationConcentration" )

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration constructor
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
MIL_PopulationConcentration::MIL_PopulationConcentration( MIL_Population& population, MIL_InputArchive& archive )
    : TER_PopulationConcentration_ABC()
    , pPopulation_                   ( &population )
    , nID_                           ( idManager_.GetFreeSimID() )
    , position_                      ()
    , location_                      ()
    , rNbrAliveHumans_               ( 0. )
    , rNbrDeadHumans_                ( 0. )
    , rDensity_                      ( 0. )
    , pAttitude_                     ( &population.GetDefaultAttitude() )
    , pPullingFlow_                  ( 0 )
    , pushingFlows_                  ()
    , bHumansUpdated_                ( true )
    , bAttitudeUpdated_              ( true )
{
    // Position
    std::string strPosition;
    archive.ReadField( "Position", strPosition );
    MIL_Tools::ConvertCoordMosToSim( strPosition, position_ );
    archive.ReadField( "NombreHumains", rNbrAliveHumans_ );
    UpdateLocation();
    UpdateDensity ();
    // SendCreation()
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration constructor
// Created: NLD 2005-10-05
// -----------------------------------------------------------------------------
MIL_PopulationConcentration::MIL_PopulationConcentration( MIL_Population& population, const MT_Vector2D& position )
    : TER_PopulationConcentration_ABC()
    , pPopulation_                   ( &population )
    , nID_                           ( idManager_.GetFreeSimID() )
    , position_                      ( position )
    , location_                      ()
    , rNbrAliveHumans_               ( 0. )
    , rNbrDeadHumans_                ( 0. )
    , rDensity_                      ( 0. )
    , pAttitude_                     ( &population.GetDefaultAttitude() )
    , pPullingFlow_                  ( 0 )
    , pushingFlows_                  ()
    , bHumansUpdated_                ( true )
    , bAttitudeUpdated_              ( true )
{
    UpdateLocation();
    UpdateDensity ();
    SendCreation();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration constructor
// Created: SBO 2005-10-18
// -----------------------------------------------------------------------------
MIL_PopulationConcentration::MIL_PopulationConcentration()
    : TER_PopulationConcentration_ABC()
    , pPopulation_                   ( 0 )
    , nID_                           ( 0 )
    , position_                      ()
    , location_                      ()
    , rNbrAliveHumans_               ( 0 )
    , rNbrDeadHumans_                ( 0 )
    , rDensity_                      ( 0. )
    , pAttitude_                     ( 0 )
    , pPullingFlow_                  ( 0 )
    , pushingFlows_                  ()
    , bHumansUpdated_                ( true )
    , bAttitudeUpdated_              ( true )
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

    idManager_.ReleaseSimID( nID_ );
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
    //$$$$ nNbrDeadHumans_ 
    if( !IsValid() )
    {
        pPullingFlow_->UnregisterSourceConcentration( *this );
        pPullingFlow_ = 0;
        RemoveFromPatch();
        return false;
    }
    if( bHumansUpdated_ )
    {
        UpdateLocation();
        UpdateDensity ();
    }

    // Collisions
    TER_Agent_ABC::T_AgentPtrVector agents;
    TER_World::GetWorld().GetAgentManager().GetListWithinLocalisation( location_, agents, 100. ); //$$$ TEST
    for( TER_Agent_ABC::CIT_AgentPtrVector it = agents.begin(); it != agents.end(); ++it )
    {
        PHY_RoleInterface_Location& agent = static_cast< PHY_RoleInterface_Location& >( **it );
        agent.NotifyPopulationCollision( *this );
    }

    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::UpdateLocation
// Created: NLD 2005-10-07
// -----------------------------------------------------------------------------
void MIL_PopulationConcentration::UpdateLocation()
{
    assert( pPopulation_ && pPopulation_->GetType().GetConcentrationDensity() );
    MT_Float rSurface = rNbrAliveHumans_ / pPopulation_->GetType().GetConcentrationDensity();
    location_.Reset( TER_Localisation( position_, std::sqrt( rSurface / MT_PI ) ) );
    UpdatePatch();
}

// =============================================================================
// ACTIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::Move
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
void MIL_PopulationConcentration::Move( const MT_Vector2D& destination )
{
    if( pPullingFlow_ || IsNearPosition( destination ) )
        return;

    assert( pPopulation_ );
    pPullingFlow_ = &pPopulation_->CreateFlow( *this );
    pPullingFlow_->Move( destination );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::SetAttitude
// Created: SBO 2005-10-25
// -----------------------------------------------------------------------------
void MIL_PopulationConcentration::SetAttitude( const MIL_PopulationAttitude& attitude )
{
    if( pAttitude_ != &attitude )
    {
        bAttitudeUpdated_ = true;
        pAttitude_ = &attitude;
    }
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

// =============================================================================
// PION EFFECTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::GetPionMaxSpeed
// Created: NLD 2005-10-21
// -----------------------------------------------------------------------------
MT_Float MIL_PopulationConcentration::GetPionMaxSpeed( const PHY_Volume& pionVolume ) const
{
    assert( pAttitude_ );
    return pPopulation_->GetPionMaxSpeed( *pAttitude_, rDensity_, pionVolume );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::GetPionReloadingTimeFactor
// Created: NLD 2005-11-02
// -----------------------------------------------------------------------------
MT_Float MIL_PopulationConcentration::GetPionReloadingTimeFactor() const
{
    return pPopulation_->GetPionReloadingTimeFactor( rDensity_ );
}

// =============================================================================
// HUMANS MANAGEMENT
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::PushHumans
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
MT_Float MIL_PopulationConcentration::PushHumans( MT_Float rNbr )
{
    bHumansUpdated_ = true;
    rNbrAliveHumans_ += rNbr;
    return rNbr;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::PullHumans
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
MT_Float MIL_PopulationConcentration::PullHumans( MT_Float rNbr )
{
    bHumansUpdated_ = true;
    const MT_Float rNbrTmp = std::min( rNbr, rNbrAliveHumans_ );
    rNbrAliveHumans_ -= rNbrTmp;
    return rNbrTmp;
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
    assert( pPopulation_ );

    NET_ASN_MsgPopulationConcentrationCreation asnMsg;
    asnMsg.GetAsnMsg().oid_concentration = nID_;
    asnMsg.GetAsnMsg().oid_population    = pPopulation_->GetID();
    NET_ASN_Tools::WritePoint( position_, asnMsg.GetAsnMsg().position ); 
    asnMsg.Send();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::SendDestruction
// Created: NLD 2005-10-05
// -----------------------------------------------------------------------------
void MIL_PopulationConcentration::SendDestruction() const
{
    assert( pPopulation_ );

    NET_ASN_MsgPopulationConcentrationDestruction asnMsg;
    asnMsg.GetAsnMsg().oid_concentration = nID_;
    asnMsg.GetAsnMsg().oid_population    = pPopulation_->GetID();
    asnMsg.Send();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::SendFullState
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
void MIL_PopulationConcentration::SendFullState() const
{
    assert( pPopulation_ );
    assert( pAttitude_ );

    NET_ASN_MsgPopulationConcentrationUpdate asnMsg;
    asnMsg.GetAsnMsg().oid_concentration = nID_;
    asnMsg.GetAsnMsg().oid_population    = pPopulation_->GetID();

    asnMsg.GetAsnMsg().m.attitudePresent           = 1;
    asnMsg.GetAsnMsg().m.nb_humains_mortsPresent   = 1;
    asnMsg.GetAsnMsg().m.nb_humains_vivantsPresent = 1;
    
    asnMsg.GetAsnMsg().attitude = pAttitude_->GetAsnID();
    asnMsg.GetAsnMsg().nb_humains_morts   = (uint)rNbrDeadHumans_;
    asnMsg.GetAsnMsg().nb_humains_vivants = (uint)rNbrAliveHumans_;

    asnMsg.Send();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::SendChangedState
// Created: NLD 2005-10-04
// -----------------------------------------------------------------------------
void MIL_PopulationConcentration::SendChangedState() const
{
    assert( pPopulation_ );
    assert( pAttitude_ );

    if( !HasChanged() )
        return;

    NET_ASN_MsgPopulationConcentrationUpdate asnMsg;
    asnMsg.GetAsnMsg().oid_concentration = nID_;
    asnMsg.GetAsnMsg().oid_population    = pPopulation_->GetID();

    if( bAttitudeUpdated_ )
    {
        asnMsg.GetAsnMsg().m.attitudePresent           = 1;
        asnMsg.GetAsnMsg().attitude = pAttitude_->GetAsnID();
    }

    if( bHumansUpdated_ )
    {
        asnMsg.GetAsnMsg().m.nb_humains_mortsPresent   = 1;
        asnMsg.GetAsnMsg().m.nb_humains_vivantsPresent = 1;
    
        asnMsg.GetAsnMsg().nb_humains_morts   = (uint)rNbrDeadHumans_;
        asnMsg.GetAsnMsg().nb_humains_vivants = (uint)rNbrAliveHumans_;
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
    
    file >> pPopulation_
         >> nID_
         >> position_
         >> location_
         >> rNbrAliveHumans_
         >> rNbrDeadHumans_
         >> rDensity_;

    if ( !idManager_.IsMosIDValid( nID_ ) )
        idManager_.LockSimID( nID_ );

    uint nAttitudeID;
    file >> nAttitudeID;
    pAttitude_ = MIL_PopulationAttitude::Find( nAttitudeID );
    assert( pAttitude_ );

    file >> pPullingFlow_
         >> pushingFlows_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::save
// Created: SBO 2005-10-18
// -----------------------------------------------------------------------------
void MIL_PopulationConcentration::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    file << boost::serialization::base_object< TER_PopulationConcentration_ABC >( *this );

    file << pPopulation_
         << nID_
         << position_
         << location_
         << rNbrAliveHumans_
         << rNbrDeadHumans_
         << rDensity_
         << pAttitude_->GetID()
         << pPullingFlow_
         << pushingFlows_
    ;
}
