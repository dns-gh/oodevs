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
#include "Entities/RC/MIL_RC.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Decision/Path/Population/DEC_Population_Path.h"
#include "Decision/Path/DEC_PathFind_Manager.h"
#include "Decision/Path/DEC_PathPoint.h"
#include "Network/NET_ASN_Messages.h"
#include "Network/NET_ASN_Tools.h"
#include "Tools/MIL_Tools.h"
#include "CheckPoints/MIL_CheckPointSerializationHelpers.h"

MIL_MOSIDManager MIL_PopulationFlow::idManager_;

BOOST_CLASS_EXPORT_GUID( MIL_PopulationFlow, "MIL_PopulationFlow" )

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow constructor
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
MIL_PopulationFlow::MIL_PopulationFlow( MIL_Population& population, MIL_PopulationConcentration& sourceConcentration )
    : PHY_MovingEntity_ABC  ()
    , TER_PopulationFlow_ABC()
    , pPopulation_          ( &population )
    , nID_                 ( idManager_.GetFreeSimID() )
    , pSourceConcentration_( &sourceConcentration )
    , pDestConcentration_  ( 0 )
    , pAttitude_           ( &sourceConcentration.GetAttitude() )
    , destination_         ( )
    , pCurrentPath_        ( 0 )
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
    , rDensity_            ( 0. )
{
    assert( pAttitude_ );
    SendCreation();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow constructor
// Created: SBO 2005-10-18
// -----------------------------------------------------------------------------
MIL_PopulationFlow::MIL_PopulationFlow()
    : PHY_MovingEntity_ABC  ()
    , TER_PopulationFlow_ABC()
    , pPopulation_          ( 0 )
    , nID_                  ( 0 )
    , pSourceConcentration_ ( 0 )
    , pDestConcentration_   ( 0 )
    , pAttitude_            ( 0 )
    , destination_          ()
    , pCurrentPath_         ( 0 )
    , bHeadMoveFinished_    ( false )
    , flowShape_            ()
    , direction_            ( 0., 1. )
    , rSpeed_               ( 0. )
    , rNbrAliveHumans_      ( 0. )
    , rNbrDeadHumans_       ( 0. )
    , bPathUpdated_         ( true )
    , bFlowShapeUpdated_    ( true ) 
    , bDirectionUpdated_    ( true )
    , bSpeedUpdated_        ( true )
    , bHumansUpdated_       ( true )
    , bAttitudeUpdated_     ( true )
    , rDensity_             ( 0. )
{
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
// Name: MIL_PopulationFlow::ApplyMove
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
void MIL_PopulationFlow::ApplyMove( const MT_Vector2D& position, const MT_Vector2D& direction, MT_Float rSpeed, MT_Float /*rWalkedDistance*/ )
{
    SetDirection( direction );
    SetSpeed    ( rSpeed    );
    const MT_Float rWalkedDistance = pPopulation_->GetMaxSpeed() /* * 1.*/; // vitesse en pixel/deltaT = metre/deltaT
    const MT_Float rNbrHumans      = rWalkedDistance * pPopulation_->GetDefaultFlowDensity(); // Nombre d'humains deplacés

    // Head management
    SetHeadPosition( position );
    if( bHeadMoveFinished_ && !pDestConcentration_ )
    {
        pDestConcentration_ = &pPopulation_->GetConcentration( GetHeadPosition() );
        pDestConcentration_->RegisterPushingFlow( *this );
    }
    if( pDestConcentration_ )
    {
        rNbrAliveHumans_ -= pDestConcentration_->PushHumans( std::min( rNbrHumans, rNbrAliveHumans_ ) );
        bHumansUpdated_ = true;
    }

    // Tail management
    if( pSourceConcentration_ )
    {
        rNbrAliveHumans_ += pSourceConcentration_->PullHumans( rNbrHumans );
        bHumansUpdated_ = true;
    }
    else
        UpdateTailPosition( rWalkedDistance );

    if( bFlowShapeUpdated_ )
        UpdateLocation();

    if( bFlowShapeUpdated_ || bHumansUpdated_ )
        UpdateDensity();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::Update
// Created: NLD 2005-10-04
// -----------------------------------------------------------------------------
bool MIL_PopulationFlow::Update()
{
    // Destruction
    if( !IsValid() )
    {
        pDestConcentration_->UnregisterPushingFlow( *this );
        pDestConcentration_ = 0;
        RemoveFromPatch();
        return false; // Must be destroyed
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
// Name: MIL_PopulationFlow::UpdateLocation
// Created: NLD 2005-10-07
// -----------------------------------------------------------------------------
void MIL_PopulationFlow::UpdateLocation()
{
    location_.Reset( flowShape_ );
    UpdatePatch();
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
    assert( pPopulation_ );
    return pPopulation_->GetMaxSpeed();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::SendRC
// Created: NLD 2005-10-05
// -----------------------------------------------------------------------------
void MIL_PopulationFlow::SendRC( const MIL_RC& rc ) const
{
    assert( pPopulation_ );
    rc.Send( *pPopulation_, MIL_RC::eRcTypeWarning );
}

// =============================================================================
// PION EFFECTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::GetPionMaxSpeed
// Created: NLD 2005-10-21
// -----------------------------------------------------------------------------
MT_Float MIL_PopulationFlow::GetPionMaxSpeed( const PHY_Volume& pionVolume ) const
{
    assert( pAttitude_ );
    return pPopulation_->GetPionMaxSpeed( *pAttitude_, rDensity_, pionVolume );
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
    assert( pPopulation_ );

    NET_ASN_MsgPopulationFluxCreation asnMsg;
    asnMsg.GetAsnMsg().oid_flux       = nID_;
    asnMsg.GetAsnMsg().oid_population = pPopulation_->GetID();
    asnMsg.Send();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::SendDestruction
// Created: NLD 2005-10-05
// -----------------------------------------------------------------------------
void MIL_PopulationFlow::SendDestruction() const
{
    assert( pPopulation_ );

    NET_ASN_MsgPopulationFluxDestruction asnMsg;
    asnMsg.GetAsnMsg().oid_flux       = nID_;
    asnMsg.GetAsnMsg().oid_population = pPopulation_->GetID();
    asnMsg.Send();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::SendFullState
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
void MIL_PopulationFlow::SendFullState() const
{
    assert( pAttitude_ );
    assert( pPopulation_ );

    NET_ASN_MsgPopulationFluxUpdate asnMsg;
    
    asnMsg.GetAsnMsg().oid_flux       = nID_;
    asnMsg.GetAsnMsg().oid_population = pPopulation_->GetID();

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
void MIL_PopulationFlow::SendChangedState() const
{
    assert( pPopulation_ );
    if( !HasChanged() )
        return;

    NET_ASN_MsgPopulationFluxUpdate asnMsg;
    
    asnMsg.GetAsnMsg().oid_flux       = nID_;
    asnMsg.GetAsnMsg().oid_population = pPopulation_->GetID();

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

// =============================================================================
// CHECKPOINT
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::load
// Created: SBO 2005-10-18
// -----------------------------------------------------------------------------
void MIL_PopulationFlow::load( MIL_CheckPointInArchive& file, const uint )
{
    file >> boost::serialization::base_object< PHY_MovingEntity_ABC   >( *this );
    file >> boost::serialization::base_object< TER_PopulationFlow_ABC >( *this );

    file >> pPopulation_
         >> const_cast< uint& >( nID_ )
         >> pSourceConcentration_
         >> pDestConcentration_;

    if ( !idManager_.IsMosIDValid( nID_ ) )
        idManager_.LockSimID( nID_ );

    uint nAttitudeID;
    file >> nAttitudeID;
    pAttitude_ = MIL_PopulationAttitude::Find( nAttitudeID );
    assert( pAttitude_ );

    file >> flowShape_
         >> direction_
         >> rNbrAliveHumans_
         >> rNbrDeadHumans_
         >> rDensity_;
}
    
// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::save
// Created: SBO 2005-10-18
// -----------------------------------------------------------------------------
void MIL_PopulationFlow::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    file << boost::serialization::base_object< PHY_MovingEntity_ABC   >( *this );
    file << boost::serialization::base_object< TER_PopulationFlow_ABC >( *this );

    file << pPopulation_
         << nID_
         << pSourceConcentration_
         << pDestConcentration_
         << pAttitude_->GetID()
         << flowShape_
         << direction_
         << rNbrAliveHumans_
         << rNbrDeadHumans_
         << rDensity_;
}


