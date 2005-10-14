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
#include "Tools/MIL_Tools.h"
#include "Network/NET_ASN_Messages.h"
#include "Network/NET_ASN_Tools.h"

MIL_MOSIDManager MIL_PopulationConcentration::idManager_;

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration constructor
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
MIL_PopulationConcentration::MIL_PopulationConcentration( MIL_Population& population, MIL_InputArchive& archive )
    : TER_PopulationConcentration_ABC()
    , population_                    ( population )
    , nID_                           ( idManager_.GetFreeSimID() )
    , position_                      ()
    , location_                      ()
    , rNbrAliveHumans_               ( 0 )
    , rNbrDeadHumans_                ( 0 )
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
    // SendCreation()
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration constructor
// Created: NLD 2005-10-05
// -----------------------------------------------------------------------------
MIL_PopulationConcentration::MIL_PopulationConcentration( MIL_Population& population, const MT_Vector2D& position )
    : TER_PopulationConcentration_ABC()
    , population_                    ( population )
    , nID_                           ( idManager_.GetFreeSimID() )
    , position_                      ( position )
    , location_                      ()
    , rNbrAliveHumans_               ( 0 )
    , rNbrDeadHumans_                ( 0 )
    , pAttitude_                     ( &population.GetDefaultAttitude() )
    , pPullingFlow_                  ( 0 )
    , pushingFlows_                  ()
    , bHumansUpdated_                ( true )
    , bAttitudeUpdated_              ( true )
{
    UpdateLocation();
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
        UpdateLocation();
    return true;
}


// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::UpdateLocation
// Created: NLD 2005-10-07
// -----------------------------------------------------------------------------
void MIL_PopulationConcentration::UpdateLocation()
{
    assert( population_.GetType().GetConcentrationDensity() );
    MT_Float rSurface = rNbrAliveHumans_ / population_.GetType().GetConcentrationDensity();
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

    pPullingFlow_ = &population_.CreateFlow( *this );
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
    NET_ASN_MsgPopulationConcentrationCreation asnMsg;
    asnMsg.GetAsnMsg().oid_concentration = nID_;
    asnMsg.GetAsnMsg().oid_population    = population_.GetID();
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
    asnMsg.GetAsnMsg().oid_concentration = nID_;
    asnMsg.GetAsnMsg().oid_population    = population_.GetID();
    asnMsg.Send();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::SendFullState
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
void MIL_PopulationConcentration::SendFullState() const
{
    assert( pAttitude_ );

    NET_ASN_MsgPopulationConcentrationUpdate asnMsg;
    asnMsg.GetAsnMsg().oid_concentration = nID_;
    asnMsg.GetAsnMsg().oid_population    = population_.GetID();

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
    assert( pAttitude_ );

    if( !HasChanged() )
        return;

    NET_ASN_MsgPopulationConcentrationUpdate asnMsg;
    asnMsg.GetAsnMsg().oid_concentration = nID_;
    asnMsg.GetAsnMsg().oid_population    = population_.GetID();

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
