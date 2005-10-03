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
#include "MIL_PopulationFlow.h"

#include "MIL_Population.h"
#include "Network/NET_ASN_Messages.h"

MIL_MOSIDManager MIL_PopulationFlow::idManager_;

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow constructor
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
MIL_PopulationFlow::MIL_PopulationFlow( const MIL_Population& population, MIL_PopulationConcentration& sourceConcentration )
    : population_          ( population )
    , nID_                 ( idManager_.GetFreeSimID() )
    , pSourceConcentration_( &sourceConcentration )
    , pDestConcentration_  ( 0 )
    , destination_         ( )
    , pCurrentPath_        ( 0 )
    , bHasMoved_           ( false )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow destructor
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
MIL_PopulationFlow::~MIL_PopulationFlow()
{
    idManager_.ReleaseSimID( nID_ );
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::Move
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
void MIL_PopulationFlow::Move( const MT_Vector2D& destination )
{
    /*if( bHasMoved_ ) 
        return;

    if( destination != destination_ )
    {
        destination_ = destination;

        if( pCurrentPath_ )
        {
            pCurrentPath_->Cancel();
            pCurrentPath_->DecRef();
            pCurrentPath_ = 0;
        }

        pCurrentPath_ = new DEC_Population_Path( *this, destination_ );
        pCurrentPath_->IncRef();
        MIL_AgentServer::GetWorkspace().GetPathFindManager().StartCompute( *pCurrentPath_ );
    }

    DEC_Agent_Path::E_State nPathState = path.GetState();
    if( nPathState == DEC_Agent_Path::eInvalid || nPathState == DEC_Agent_Path::eImpossible || nPathState == DEC_Agent_Path::eCanceled )
        return;
    
    bHasMoved_ = true;
    if( nPathState == DEC_Agent_Path::eComputing )
        return;



    */

}


// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::SendCreation
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
void MIL_PopulationFlow::SendCreation() const
{
    NET_ASN_MsgPopulationFluxCreation asnMsg;
    asnMsg.GetAsnMsg().oid_flux       = nID_;
    asnMsg.GetAsnMsg().oid_population = population_.GetID();
    asnMsg.Send();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::SendFullState
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
void MIL_PopulationFlow::SendFullState() const
{
    std::cout << "MIL_PopulationFlow::SendFullState TODO DUMBASS" << std::endl;
    
//    assert( false );

    NET_ASN_MsgPopulationFluxUpdate asnMsg;
    asnMsg.GetAsnMsg().oid_flux       = nID_;
    asnMsg.GetAsnMsg().oid_population = population_.GetID();

    //$$$

    asnMsg.Send();
}

