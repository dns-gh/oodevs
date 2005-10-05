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
#include "MIL_PopulationConcentration.h"
#include "MIL_PopulationAttitude.h"
#include "Decision/Path/Population/DEC_Population_Path.h"
#include "Decision/Path/DEC_PathFind_Manager.h"
#include "Decision/Path/DEC_PathPoint.h"
#include "Network/NET_ASN_Messages.h"
#include "Network/NET_ASN_Tools.h"
#include "Tools/MIL_Tools.h"

MIL_MOSIDManager MIL_PopulationFlow::idManager_;

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow constructor
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
MIL_PopulationFlow::MIL_PopulationFlow( MIL_Population& population, MIL_PopulationConcentration& sourceConcentration )
    : population_          ( population )
    , nID_                 ( idManager_.GetFreeSimID() )
    , pSourceConcentration_( &sourceConcentration )
    , pDestConcentration_  ( 0 )
    , pAttitude_           ( &sourceConcentration.GetAttitude() )
    , destination_         ( )
    , pCurrentPath_        ( 0 )
    , bMoving_             ( false )
    , bHeadMoveFinished_   ( false )
    , flowShape_           ( 2, sourceConcentration.GetPosition() )
    , direction_           ( 0., 1. )
    , rSpeed_              ( 0. )
    , rNbrAliveHumans_     ( 0. )
    , rNbrDeadHumans_      ( 0. )
    , bPathUpdated_        ( true )
    , bFlowShapeUpdated_   ( true ) 
    , bDirectionUpdated_   ( true )
    , bSpeedUpdated_       ( true )
    , bHumansUpdated_      ( true )
    , bAttitudeUpdated_    ( true )
{
    assert( pAttitude_ );
    SendCreation();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow destructor
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
MIL_PopulationFlow::~MIL_PopulationFlow()
{
    assert( !pSourceConcentration_ );
    assert( !pDestConcentration_   );    

    SendDestruction();

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

        if( pDestConcentration_ )
        {
            pDestConcentration_->UnregisterPushingFlow( *this );
            pDestConcentration_ = 0;
        }
    }

    assert( pCurrentPath_ );
    int nOut = PHY_MovingEntity_ABC::Move( *pCurrentPath_ );
    if( nOut == DEC_PathWalker::eFinished )
        bHeadMoveFinished_ = true;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::NotifyMovingOnPathPoint
// Created: NLD 2005-10-04
// -----------------------------------------------------------------------------
void MIL_PopulationFlow::NotifyMovingOnPathPoint( const DEC_PathPoint& point )
{
    // Head position
    assert( !flowShape_.empty() );
    IT_PointList itTmp = flowShape_.end();
    -- itTmp;
    flowShape_.insert( itTmp, point.GetPos() );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::ApplyMove
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
void MIL_PopulationFlow::ApplyMove( const MT_Vector2D& position, const MT_Vector2D& direction, MT_Float rSpeed, MT_Float /*rWalkedDistance*/ )
{
    bMoving_   = true;
    direction_ = direction;
    rSpeed_    = rSpeed;

    SetHeadPosition( position );

    bFlowShapeUpdated_ = true;
    bDirectionUpdated_ = true;
    bSpeedUpdated_     = true;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::UpdateTailPosition
// Created: NLD 2005-10-04
// -----------------------------------------------------------------------------
void MIL_PopulationFlow::UpdateTailPosition( const MT_Float rWalkedDistance )
{
    bFlowShapeUpdated_ =  true;

    /////// $$ A NETTOYER
    MT_Vector2D  vCur   = GetTailPosition();
    IT_PointList itNext = flowShape_.begin();
    ++itNext;

    MT_Vector2D vNext = *itNext;
    MT_Vector2D vDir  = vNext - vCur;
    MT_Float    rDist = rWalkedDistance;

    MT_Float rDirLength = vDir.Magnitude();
    if( rDirLength )
        vDir /= rDirLength;

    while( 1 )
    {
        if( rDist < rDirLength )
        {
            vCur = vCur + ( vDir * rDist );

            IT_PointList itStart = flowShape_.begin();
            ++ itStart;
            flowShape_.erase ( itStart, itNext );
            SetTailPosition( vCur );
            break;
        }
        else
        {
            rDist -= rDirLength;
            vCur   = vNext;
            ++itNext;
            if( itNext == flowShape_.end() )
            {
                IT_PointList itTmp = flowShape_.end();
                --itTmp;
                IT_PointList itStart = flowShape_.begin();
                ++ itStart;
                flowShape_.erase( itStart, itTmp );
                SetTailPosition( GetHeadPosition() );
                break;
            }
            vNext = *itNext;
            vDir = vNext - vCur;
            rDirLength = vDir.Magnitude();
            if( rDirLength )
                vDir /= rDirLength;
        }
    }   
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::Update
// Created: NLD 2005-10-04
// -----------------------------------------------------------------------------
bool MIL_PopulationFlow::Update()
{
    if( bMoving_ )
    {
        const MT_Float rWalkedDistance = population_.GetMaxSpeed() /* * 1.*/; // vitesse en pixel/deltaT = metre/deltaT

        // Tail management
        MT_Float rNbrHumans = rWalkedDistance * population_.GetDefaultFlowDensity();
        if( pSourceConcentration_ )
        {
            rNbrAliveHumans_ += pSourceConcentration_->PullHumans( rNbrHumans );
            bHumansUpdated_ = true;
        }
        else
            UpdateTailPosition( rWalkedDistance );


        // Head management
        if( bHeadMoveFinished_ && !pDestConcentration_ )
        {
            pDestConcentration_ = &population_.GetConcentration( GetHeadPosition() );
            pDestConcentration_->RegisterPushingFlow( *this );
        }

        rNbrHumans = std::min( rNbrHumans, rNbrAliveHumans_ );
        if( pDestConcentration_ )
        {
            rNbrAliveHumans_ -= pDestConcentration_->PushHumans( rNbrHumans );
            bHumansUpdated_ = true;
        }
    }

    // Destruction
    if( pDestConcentration_ && !pSourceConcentration_ && GetHeadPosition() == GetTailPosition() )
    {
        pDestConcentration_->UnregisterPushingFlow( *this );
        pDestConcentration_ = 0;
        return false; // Must be destroyed
    }
    return true;
}

// =============================================================================
// ACCESSORS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::GetMaxSpeed
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
MT_Float MIL_PopulationFlow::GetMaxSpeed() const
{
    return population_.GetMaxSpeed();
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
// Name: MIL_PopulationFlow::SendDestruction
// Created: NLD 2005-10-05
// -----------------------------------------------------------------------------
void MIL_PopulationFlow::SendDestruction() const
{
    NET_ASN_MsgPopulationFluxDestruction asnMsg;
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
    assert( pAttitude_ );

    NET_ASN_MsgPopulationFluxUpdate asnMsg;
    
    asnMsg.GetAsnMsg().oid_flux       = nID_;
    asnMsg.GetAsnMsg().oid_population = population_.GetID();

    if( SerializeCurrentPath( asnMsg.GetAsnMsg().itineraire ) )
        asnMsg.GetAsnMsg().m.itinerairePresent = 1;

    asnMsg.GetAsnMsg().m.fluxPresent               = 1;
    asnMsg.GetAsnMsg().m.attitudePresent           = 1;
    asnMsg.GetAsnMsg().m.directionPresent          = 1;
    asnMsg.GetAsnMsg().m.nb_humains_vivantsPresent = 1;
    asnMsg.GetAsnMsg().m.nb_humains_mortsPresent   = 1;
    asnMsg.GetAsnMsg().m.vitessePresent            = 1;

    NET_ASN_Tools::WritePath     ( flowShape_, asnMsg.GetAsnMsg().flux      );
    NET_ASN_Tools::WriteDirection( direction_, asnMsg.GetAsnMsg().direction );
    asnMsg.GetAsnMsg().attitude           = pAttitude_->GetAsnID();
    asnMsg.GetAsnMsg().vitesse            = (uint)MIL_Tools::ConvertSpeedSimToMos( rSpeed_ );
    asnMsg.GetAsnMsg().nb_humains_vivants = (uint)rNbrAliveHumans_; 
    asnMsg.GetAsnMsg().nb_humains_morts   = (uint)rNbrDeadHumans_;

    asnMsg.Send();

    NET_ASN_Tools::Delete( asnMsg.GetAsnMsg().flux );
    if( asnMsg.GetAsnMsg().m.itinerairePresent )
        NET_ASN_Tools::Delete( asnMsg.GetAsnMsg().itineraire );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::SendChangedState
// Created: NLD 2005-10-04
// -----------------------------------------------------------------------------
#include "Network/NET_AS_MOSServerMsgMgr.h"
void MIL_PopulationFlow::SendChangedState() const
{
//$$$$ DEBUG
    NET_AS_MOSServerMsgMgr& msgMgr = MIL_AgentServer::GetWorkspace().GetAgentServer().GetMessageMgr();
    DIN::DIN_BufferedMessage dinMsg = msgMgr.BuildMessage();
    
    dinMsg << (uint32)6000025;
    dinMsg << (uint32)flowShape_.size();    
    for( CIT_PointList it = flowShape_.begin(); it != flowShape_.end(); ++it )
        dinMsg << *it;
    msgMgr.SendMsgDebugDrawPoints( dinMsg );
//$$$$ DEBUG

    if( !HasChanged() )
        return;

    NET_ASN_MsgPopulationFluxUpdate asnMsg;
    
    asnMsg.GetAsnMsg().oid_flux       = nID_;
    asnMsg.GetAsnMsg().oid_population = population_.GetID();

    if( bPathUpdated_ && SerializeCurrentPath( asnMsg.GetAsnMsg().itineraire ) )
        asnMsg.GetAsnMsg().m.itinerairePresent = 1;

    if( bFlowShapeUpdated_ )
    {
        asnMsg.GetAsnMsg().m.fluxPresent = 1;
        NET_ASN_Tools::WritePath( flowShape_, asnMsg.GetAsnMsg().flux );
    }

    if( bAttitudeUpdated_ )
    {
        assert( pAttitude_ );
        asnMsg.GetAsnMsg().m.attitudePresent = 1;
        asnMsg.GetAsnMsg().attitude          = pAttitude_->GetAsnID();
    }

    if( bDirectionUpdated_ )
    {
        asnMsg.GetAsnMsg().m.directionPresent = 1;
        NET_ASN_Tools::WriteDirection( direction_, asnMsg.GetAsnMsg().direction );
    }

    if( bHumansUpdated_ )
    {
        asnMsg.GetAsnMsg().m.nb_humains_vivantsPresent = 1; 
        asnMsg.GetAsnMsg().m.nb_humains_mortsPresent   = 1;
        asnMsg.GetAsnMsg().nb_humains_vivants          = (uint)rNbrAliveHumans_; 
        asnMsg.GetAsnMsg().nb_humains_morts            = (uint)rNbrDeadHumans_;
    }

    if( bSpeedUpdated_ )
    {
        asnMsg.GetAsnMsg().m.vitessePresent = 1;
        asnMsg.GetAsnMsg().vitesse          = (uint)MIL_Tools::ConvertSpeedSimToMos( rSpeed_ );
    }
    
    asnMsg.Send();

    if( asnMsg.GetAsnMsg().m.fluxPresent )
        NET_ASN_Tools::Delete( asnMsg.GetAsnMsg().flux );
    if( asnMsg.GetAsnMsg().m.itinerairePresent )
        NET_ASN_Tools::Delete( asnMsg.GetAsnMsg().itineraire );
}
