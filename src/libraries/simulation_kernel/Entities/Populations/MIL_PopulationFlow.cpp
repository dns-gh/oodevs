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

#include "simulation_kernel_pch.h"
#include "MIL_PopulationFlow.h"

#include "MIL_Population.h"
#include "MIL_PopulationConcentration.h"
#include "MIL_PopulationAttitude.h"
#include "DEC_PopulationKnowledge.h"
#include "Entities/Orders/MIL_Report.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Objects/MIL_RealObject_ABC.h"
#include "Entities/Orders/MIL_Report.h"
#include "Decision/Path/Population/DEC_Population_Path.h"
#include "Decision/Path/DEC_PathFind_Manager.h"
#include "Decision/Path/DEC_PathPoint.h"
#include "Network/NET_ASN_Messages.h"
#include "Network/NET_ASN_Tools.h"
#include "Tools/MIL_IDManager.h"
#include "Tools/MIL_Tools.h"
#include "CheckPoints/MIL_CheckPointSerializationHelpers.h"

BOOST_CLASS_EXPORT_GUID( MIL_PopulationFlow, "MIL_PopulationFlow" )

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow constructor
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
MIL_PopulationFlow::MIL_PopulationFlow( MIL_Population& population, MIL_PopulationConcentration& sourceConcentration )
    : PHY_MovingEntity_ABC     ()
    , TER_PopulationFlow_ABC   ()
    , MIL_PopulationElement_ABC( population, MIL_IDManager::populationFlows_.GetFreeSimID() )
    , pSourceConcentration_    ( &sourceConcentration )
    , pDestConcentration_      ( 0 )
    , primaryDestination_      ()
    , alternateDestination_    ()
    , pCurrentPath_            ( 0 )
    , bHeadMoveFinished_       ( false )
    , flowShape_               ( 2, sourceConcentration.GetPosition() )
    , direction_               ( 0., 1. )
    , rSpeed_                  ( 0. )
    , bPathUpdated_            ( true )
    , bFlowShapeUpdated_       ( true ) 
    , bDirectionUpdated_       ( true )
    , bSpeedUpdated_           ( true )
    , pSplittingObject_        ( 0 )
{
    SetAttitude( sourceConcentration.GetAttitude() );
    UpdateLocation();
    SendCreation  ();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow constructor
// Created: SBO 2005-10-18
// -----------------------------------------------------------------------------
MIL_PopulationFlow::MIL_PopulationFlow()
    : PHY_MovingEntity_ABC     ()
    , TER_PopulationFlow_ABC   ()
    , MIL_PopulationElement_ABC()
    , pSourceConcentration_    ( 0 )
    , pDestConcentration_      ( 0 )
    , primaryDestination_      ()
    , alternateDestination_    ()
    , pCurrentPath_            ( 0 )
    , bHeadMoveFinished_       ( false )
    , flowShape_               ()
    , direction_               ( 0., 1. )
    , rSpeed_                  ( 0. )
    , bPathUpdated_            ( true )
    , bFlowShapeUpdated_       ( true ) 
    , bDirectionUpdated_       ( true )
    , bSpeedUpdated_           ( true )
    , pSplittingObject_        ( 0 )
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
    RemoveFromPatch();
}

// =============================================================================
// ACTIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::ComputePath
// Created: NLD 2005-12-11
// -----------------------------------------------------------------------------
void MIL_PopulationFlow::ComputePath( const MT_Vector2D& destination )
{
    if( pCurrentPath_ )
    {
        pCurrentPath_->Cancel();
        pCurrentPath_->DecRef();
        pCurrentPath_ = 0;
    }

    pCurrentPath_ = new DEC_Population_Path( *this, destination );
    pCurrentPath_->IncRef();
    MIL_AgentServer::GetWorkspace().GetPathFindManager().StartCompute( *pCurrentPath_ );

    if( pDestConcentration_ )
    {
        pDestConcentration_->UnregisterPushingFlow( *this );
        pDestConcentration_ = 0;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::MagicMove
// Created: NLD 2005-12-06
// -----------------------------------------------------------------------------
void MIL_PopulationFlow::MagicMove( const MT_Vector2D& destination )
{
    MIL_PopulationConcentration& newConcentration = GetPopulation().GetConcentration( destination );
    newConcentration.PushHumans( PullHumans( GetNbrHumans() ) );

    if( pDestConcentration_ )
    {
        pDestConcentration_->UnregisterPushingFlow( *this );
        pDestConcentration_ = 0;
    }

    if( pSourceConcentration_ )
    {
        pSourceConcentration_->UnregisterPushingFlow( *this );
        pSourceConcentration_ = 0;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::MoveToAlternateDestination
// Created: NLD 2006-01-02
// -----------------------------------------------------------------------------
void MIL_PopulationFlow::MoveToAlternateDestination( const MT_Vector2D& destination )
{
    if( destination != alternateDestination_ )
    {
        alternateDestination_ = destination;
        ComputePath( alternateDestination_ );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::Move
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
void MIL_PopulationFlow::Move( const MT_Vector2D& destination )
{
    if(  destination != primaryDestination_ 
      || GetPopulation().GetKnowledge().HasChannelingChanged() )
    {
        primaryDestination_   = destination;
        alternateDestination_ = destination;
        ComputePath( primaryDestination_ );
    }

    assert( pCurrentPath_ );
    int nOut = PHY_MovingEntity_ABC::Move( *pCurrentPath_ );
    if( nOut == DEC_PathWalker::eFinished )
        bHeadMoveFinished_ = true;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::NotifyMovingInsideObject
// Created: NLD 2005-12-10
// -----------------------------------------------------------------------------
void MIL_PopulationFlow::NotifyMovingInsideObject( MIL_Object_ABC& object )
{
    if( !object.IsReal() )
        return;

    MIL_RealObject_ABC& realObject = static_cast< MIL_RealObject_ABC& >( object );
    // $$$ POURRI
    realObject.ApplyAttrition( *this );


    //$$$ DEUGUEU Cf. refactor gestion objets<->population
    if( pSourceConcentration_ && pSourceConcentration_->GetSplittingObject() == &realObject )
        return;

    if( realObject.GetExitingPopulationDensity() == std::numeric_limits< MT_Float >::max() )
        return;

    if( !pSplittingObject_ || realObject.GetExitingPopulationDensity() < pSplittingObject_->GetExitingPopulationDensity() )
        pSplittingObject_ = &realObject;
}
    
// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::GetSpeedWithReinforcement
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
MT_Float MIL_PopulationFlow::GetSpeedWithReinforcement( const TerrainData& /*environment*/, const MIL_Object_ABC& object ) const
{
    if( !object.IsReal() )
        return GetMaxSpeed();

    const MIL_RealObject_ABC& realObject = static_cast< const MIL_RealObject_ABC& >( object );
    if( realObject.GetExitingPopulationDensity() == std::numeric_limits< MT_Float >::max() )
        return GetMaxSpeed();

    if( pSourceConcentration_ && pSourceConcentration_->GetSplittingObject() == &realObject )
        return GetMaxSpeed();
    return 0.; // First collision with 'splitting' object => stop the move (concentration will be created on collision point ...)
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::CanObjectInteractWith
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
bool MIL_PopulationFlow::CanObjectInteractWith( const MIL_Object_ABC& object ) const
{
    return object.CanInteractWith( GetPopulation() );
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
// Name: MIL_PopulationFlow::ManageSplit
// Created: NLD 2006-03-08
// -----------------------------------------------------------------------------
void MIL_PopulationFlow::ManageSplit()
{
    if( pDestConcentration_ || !pSplittingObject_ )
        return;

    MoveToAlternateDestination( GetHeadPosition() );

    pDestConcentration_ = &GetPopulation().GetConcentration( GetHeadPosition() );
    pDestConcentration_->SetPullingFlowsDensity( *pSplittingObject_ );
    pDestConcentration_->RegisterPushingFlow( *this );
    //pDestConcentration_->Move( destination_ ); $$ Auto next tick

    //$$$ TMP CRs - a changer apres refactor objets
    const MIL_RealObject_ABC::T_AgentSet& animators = pSplittingObject_->GetAnimators();
    for( MIL_RealObject_ABC::CIT_AgentSet it = animators.begin(); it != animators.end(); ++it )
        MIL_Report::PostEvent( **it, MIL_Report::eReport_InterventionAgainstPopulationStarted, GetAttitude().GetID() );            

/*
    //$$$$$$$$$$$$$$$$$$$$$
    // $$$ TEST
    TER_ObjectManager::T_ObjectVector objects; 
    TER_World::GetWorld().GetObjectManager().GetListWithinLocalisation( GetLocation(), objects );
    printf( "%d objects colliding\n", objects.size() );

    if( new object collision && object has effect on population flow )
    {
        ptCollision = GetCollisionPoint()
        pNewConcentration = GetConcentration( ptCollision );
        pNewFlow = CopyFlow( *this );
        pNewFlow->ChangeTail( ptCollision ); // reduction nombre humains
        ChangeHead( pNewConcentration ); /// reduction nombre humains
    }


    $$ TEST 2

    A chaque tick, check collisions objets<->flux
        => pour toutes nouvelles collisions
            => application effet (attrition ...)
            => si effet split sur popu
                => ptCollision = GetCollisionPoint()
                => si ptCollision = tête
                        
                => sinon
                    


*/
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::ApplyMove
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
void MIL_PopulationFlow::ApplyMove( const MT_Vector2D& position, const MT_Vector2D& direction, MT_Float /*rSpeed*/, MT_Float /*rWalkedDistance*/ )
{
    const MT_Float rWalkedDistance = GetPopulation().GetMaxSpeed() /* * 1.*/; // vitesse en pixel/deltaT = metre/deltaT

    //$$ TMP
    MT_Float rNbrHumans = 0.;
    if( pSourceConcentration_ )
        rNbrHumans = rWalkedDistance * pSourceConcentration_->GetPullingFlowsDensity();
    else
    {
        const MT_Float rArea = GetLocation().GetArea();
        if( rArea )
            rNbrHumans = rWalkedDistance * ( GetNbrHumans() / rArea );
        else
            rNbrHumans = GetNbrHumans();
    }

    if( rNbrHumans == 0. )
        return;

    SetDirection( direction );
    SetSpeed    ( rWalkedDistance ); // $$$$ SBO 2006-04-03: euh... non rien

    // Head management
    SetHeadPosition( position );
    if( bHeadMoveFinished_ && !pDestConcentration_ )
    {
        pDestConcentration_ = &GetPopulation().GetConcentration( GetHeadPosition() );
        pDestConcentration_->RegisterPushingFlow( *this );
    }
    if( pDestConcentration_ )
        pDestConcentration_->PushHumans( PullHumans( rNbrHumans ) );

    // Tail management
    if( pSourceConcentration_ )
        PushHumans( pSourceConcentration_->PullHumans( rNbrHumans ) );
    else
        UpdateTailPosition( rWalkedDistance );

    if( bFlowShapeUpdated_ )
        UpdateLocation();

    if( bFlowShapeUpdated_ || HasHumansChanged() )
        UpdateDensity();

    ManageSplit();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::Update
// Created: NLD 2005-10-04
// -----------------------------------------------------------------------------
bool MIL_PopulationFlow::Update()
{
    ClearCollisions();

    if( !IsValid() )
    {
        if( pDestConcentration_ )
        {
            pDestConcentration_->UnregisterPushingFlow( *this );
            pDestConcentration_ = 0;
        }
        RemoveFromPatch();
        return false; // Must be destroyed
    }

    // Collisions
    UpdateCollisions();
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

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::NotifyCollision
// Created: NLD 2005-11-03
// -----------------------------------------------------------------------------
void MIL_PopulationFlow::NotifyCollision( MIL_Agent_ABC& agent )
{
    agent.GetRole< PHY_RoleInterface_Location >().NotifyPopulationCollision( *this );
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
    return GetPopulation().GetMaxSpeed();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::SendRC
// Created: NLD 2005-10-05
// -----------------------------------------------------------------------------
void MIL_PopulationFlow::SendRC( int nReportID ) const
{
    MIL_Report::PostEvent( GetPopulation(), (MIL_Report::E_EngineReport)nReportID ); //$$$
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::GetSafetyPosition
// Created: SBO 2005-12-16
// -----------------------------------------------------------------------------
MT_Vector2D MIL_PopulationFlow::GetSafetyPosition( const MIL_AgentPion& agent, MT_Float rMinDistance, MT_Float /*rSeed*/ ) const
{
    const MT_Vector2D& agentPosition = agent.GetRole< PHY_RolePion_Location >().GetPosition();
    MT_Vector2D nearestPointOnFlow;
    GetLocation().ComputeNearestPoint( agentPosition, nearestPointOnFlow );

    // find flow segment containing nearestPointOnFlow
    CIT_PointList itStart = flowShape_.begin();
    CIT_PointList itEnd   = itStart;
    ++itEnd;
    for( ; itEnd != flowShape_.end(); ++itStart, ++itEnd )
        if( MT_Line( *itStart, *itEnd ).IsInside( nearestPointOnFlow, 10.0f ) ) // $$$$ SBO 2006-02-22: epsilon should be 0
            break;
    if( itEnd == flowShape_.end() ) // $$$$ SBO 2006-02-22: should not happen
        return MT_Vector2D();

    MT_Vector2D evadeDirection = ( *itEnd - *itStart ).Normalize().Rotate( MT_PI / 2 );

    if( evadeDirection.IsZero() )
        evadeDirection = -agent.GetDirDanger();

    MT_Vector2D safetyPos = nearestPointOnFlow + evadeDirection * rMinDistance;
    TER_World::GetWorld().ClipPointInsideWorld( safetyPos );
    return safetyPos;
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
    asnMsg().oid_flux       = GetID();
    asnMsg().oid_population = GetPopulation().GetID();
    asnMsg.Send();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::SendDestruction
// Created: NLD 2005-10-05
// -----------------------------------------------------------------------------
void MIL_PopulationFlow::SendDestruction() const
{
    NET_ASN_MsgPopulationFluxDestruction asnMsg;
    asnMsg().oid_flux       = GetID();
    asnMsg().oid_population = GetPopulation().GetID();
    asnMsg.Send();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::SendFullState
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
void MIL_PopulationFlow::SendFullState() const
{
    NET_ASN_MsgPopulationFluxUpdate asnMsg;
    
    asnMsg().oid_flux       = GetID();
    asnMsg().oid_population = GetPopulation().GetID();

    if( SerializeCurrentPath( asnMsg().itineraire ) )
        asnMsg().m.itinerairePresent = 1;

    asnMsg().m.fluxPresent               = 1;
    asnMsg().m.attitudePresent           = 1;
    asnMsg().m.directionPresent          = 1;
    asnMsg().m.nb_humains_vivantsPresent = 1;
    asnMsg().m.nb_humains_mortsPresent   = 1;
    asnMsg().m.vitessePresent            = 1;

    NET_ASN_Tools::WritePath     ( flowShape_, asnMsg().flux      );
    NET_ASN_Tools::WriteDirection( direction_, asnMsg().direction );
    asnMsg().attitude           = GetAttitude().GetAsnID();
    asnMsg().vitesse            = (uint)MIL_Tools::ConvertSpeedSimToMos( rSpeed_ );
    asnMsg().nb_humains_vivants = GetPopulation().GetBoundedPeople( GetNbrAliveHumans() );
    asnMsg().nb_humains_morts   = GetPopulation().GetBoundedPeople( GetNbrDeadHumans () );

    asnMsg.Send();

    NET_ASN_Tools::Delete( asnMsg().flux );
    if( asnMsg().m.itinerairePresent )
        NET_ASN_Tools::Delete( asnMsg().itineraire );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::SendChangedState
// Created: NLD 2005-10-04
// -----------------------------------------------------------------------------
void MIL_PopulationFlow::SendChangedState() const
{
    if( !HasChanged() )
        return;

    NET_ASN_MsgPopulationFluxUpdate asnMsg;
    
    asnMsg().oid_flux       = GetID();
    asnMsg().oid_population = GetPopulation().GetID();

    if( bPathUpdated_ && SerializeCurrentPath( asnMsg().itineraire ) )
        asnMsg().m.itinerairePresent = 1;

    if( bFlowShapeUpdated_ )
    {
        asnMsg().m.fluxPresent = 1;
        NET_ASN_Tools::WritePath( flowShape_, asnMsg().flux );
    }

    if( HasAttitudeChanged() )
    {
        asnMsg().m.attitudePresent = 1;
        asnMsg().attitude          = GetAttitude().GetAsnID();
    }

    if( bDirectionUpdated_ )
    {
        asnMsg().m.directionPresent = 1;
        NET_ASN_Tools::WriteDirection( direction_, asnMsg().direction );
    }

    if( HasHumansChanged() )
    {
        asnMsg().m.nb_humains_vivantsPresent = 1; 
        asnMsg().m.nb_humains_mortsPresent   = 1;
        asnMsg().nb_humains_vivants          = GetPopulation().GetBoundedPeople( GetNbrAliveHumans() );
        asnMsg().nb_humains_morts            = GetPopulation().GetBoundedPeople( GetNbrDeadHumans () );
    }

    if( bSpeedUpdated_ )
    {
        asnMsg().m.vitessePresent = 1;
        asnMsg().vitesse          = (uint)MIL_Tools::ConvertSpeedSimToMos( rSpeed_ );
    }
    
    asnMsg.Send();

    if( asnMsg().m.fluxPresent )
        NET_ASN_Tools::Delete( asnMsg().flux );
    if( asnMsg().m.itinerairePresent )
        NET_ASN_Tools::Delete( asnMsg().itineraire );
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
    file >> boost::serialization::base_object< PHY_MovingEntity_ABC      >( *this );
    file >> boost::serialization::base_object< TER_PopulationFlow_ABC    >( *this );
    file >> boost::serialization::base_object< MIL_PopulationElement_ABC >( *this );

    file >> pSourceConcentration_
         >> pDestConcentration_
         >> flowShape_
         >> direction_
         >> rSpeed_;

    UpdateLocation();
}
    
// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::save
// Created: SBO 2005-10-18
// -----------------------------------------------------------------------------
void MIL_PopulationFlow::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    file << boost::serialization::base_object< PHY_MovingEntity_ABC      >( *this );
    file << boost::serialization::base_object< TER_PopulationFlow_ABC    >( *this );
    file << boost::serialization::base_object< MIL_PopulationElement_ABC >( *this );

    file << pSourceConcentration_
         << pDestConcentration_
         << flowShape_
         << direction_
         << rSpeed_;
}


