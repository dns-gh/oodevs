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
#include "Tools/MIL_Tools.h"
#include "Network/NET_ASN_Messages.h"
#include "Network/NET_ASN_Tools.h"

MIL_MOSIDManager MIL_PopulationConcentration::idManager_;

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration constructor
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
MIL_PopulationConcentration::MIL_PopulationConcentration( MIL_Population& population, MIL_InputArchive& archive )
    : population_      ( population )
    , nID_             ( idManager_.GetFreeSimID() )
    , position_        ()
    , rNbrAliveHumans_ ( 0 )
    , rNbrDeadHumans_  ( 0 )
    , pAttitude_       ( 0 )
    , pPullingFlow_    ( 0 )
    , bHumansUpdated_  ( false )
    , bAttitudeUpdated_( false )
{
    // Position
    std::string strPosition;
    archive.ReadField( "Position", strPosition );
    MIL_Tools::ConvertCoordMosToSim( strPosition, position_ );

    archive.ReadField( "NombreHumains", rNbrAliveHumans_ );

    std::string strAttitude;
    archive.ReadField( "Attitude", strAttitude );
    pAttitude_ = MIL_PopulationAttitude::Find( strAttitude );
    if( !pAttitude_ )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown attitude", archive.GetContext() );

//    SendCreation();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration destructor
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
MIL_PopulationConcentration::~MIL_PopulationConcentration()
{
    idManager_.ReleaseSimID( nID_ );

    assert( !pPullingFlow_ );

    NET_ASN_MsgPopulationConcentrationDestruction asnMsg;
    asnMsg.GetAsnMsg().oid_concentration = nID_;
    asnMsg.GetAsnMsg().oid_population    = population_.GetID();
    asnMsg.Send();
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
    if( rNbrAliveHumans_ <= 0. ) //$$$$ nNbrDeadHumans_ 
    {
        pPullingFlow_->UnregisterSourceConcentration( *this );
        pPullingFlow_ = 0;
        return false;
    }
    return true;
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
    if( pPullingFlow_ )
        return;

    pPullingFlow_ = &population_.CreateFlow( *this );
    pPullingFlow_->Move( destination );
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
