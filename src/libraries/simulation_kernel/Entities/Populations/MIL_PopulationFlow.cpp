// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_PopulationFlow.h"
#include "MIL_PopulationConcentration.h"
#include "MIL_PopulationAttitude.h"
#include "MIL_PopulationType.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Objects/AnimatorAttribute.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/Objects/MIL_ObjectManipulator_ABC.h"
#include "Entities/Objects/PopulationAttribute.h"
#include "Entities/Orders/MIL_Report.h"
#include "Decision/DEC_Population_Path.h"
#include "Decision/DEC_PathFind_Manager.h"
#include "Decision/DEC_PathPoint.h"
#include "Network/NET_ASN_Tools.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"
#include "simulation_kernel/PopulationCollisionNotificationHandler_ABC.h"
#include "simulation_terrain/TER_World.h"
#include "Tools/MIL_Tools.h"

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_PopulationFlow )

MIL_IDManager MIL_PopulationFlow::idManager_;

template< typename Archive >
void save_construct_data( Archive& archive, const MIL_PopulationFlow* flow, const unsigned int /*version*/ )
{
    MIL_Population* const pPopulation = &flow->GetPopulation();
    unsigned int nID = flow->GetID();
    archive << pPopulation << nID;
}

template< typename Archive >
void load_construct_data( Archive& archive, MIL_PopulationFlow* flow, const unsigned int /*version*/ )
{
    MIL_Population* pPopulation;
    unsigned int nID;
    archive >> pPopulation >> nID;
    ::new( flow )MIL_PopulationFlow( *pPopulation, nID);
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow constructor
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
MIL_PopulationFlow::MIL_PopulationFlow( MIL_Population& population, MIL_PopulationConcentration& sourceConcentration )
    : MIL_PopulationElement_ABC( population, idManager_.GetFreeId() )
    , pSourceConcentration_       ( &sourceConcentration )
    , pDestConcentration_         ( 0 )
    , bHeadMoveFinished_          ( false )
    , flowShape_                  ( 2, sourceConcentration.GetPosition() )
    , direction_                  ( 0., 1. )
    , rSpeed_                     ( 0. )
    , bPathUpdated_               ( true )
    , bFlowShapeUpdated_          ( true )
    , bDirectionUpdated_          ( true )
    , bSpeedUpdated_              ( true )
    , pSplittingObject_           ( 0 )
    , personsPassedThroughObject_ ( 0 )
    , armedIndividualsBeforeSplit_( 0 )
{
    SetAttitude( sourceConcentration.GetAttitude() );
    UpdateLocation();
    SendCreation();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow constructor
// Created: NLD 2007-03-01
// -----------------------------------------------------------------------------
MIL_PopulationFlow::MIL_PopulationFlow( MIL_Population& population, const MIL_PopulationFlow& source, const MT_Vector2D& splitPoint )
    : MIL_PopulationElement_ABC( population, idManager_.GetFreeId() )
    , pSourceConcentration_       ( 0 )
    , pDestConcentration_         ( 0 )
    , primaryDestination_         ( source.primaryDestination_ )
    , alternateDestination_       ( source.alternateDestination_ )
    , pHeadPath_                  ( source.pHeadPath_ ) //$$$$ Degueu : faire une copie
    , bHeadMoveFinished_          ( false )
    , flowShape_                  ( source.flowShape_ )
    , direction_                  ( 0., 1. )
    , rSpeed_                     ( 0. )
    , bPathUpdated_               ( true )
    , bFlowShapeUpdated_          ( true )
    , bDirectionUpdated_          ( true )
    , bSpeedUpdated_              ( true )
    , pSplittingObject_           ( 0 )
    , personsPassedThroughObject_ ( 0 )
    , armedIndividualsBeforeSplit_( 0 )
{
    IT_PointList itSplit = std::find( flowShape_.begin(), flowShape_.end(), splitPoint );
    if( itSplit != flowShape_.end() )
        flowShape_.erase( flowShape_.begin(), itSplit );
    assert( flowShape_.size() >= 2 );
    SetAttitude( source.GetAttitude() );
    UpdateLocation();
    SendCreation();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow constructor
// Created: SBO 2005-10-18
// -----------------------------------------------------------------------------
MIL_PopulationFlow::MIL_PopulationFlow( MIL_Population& population, unsigned int nID )
    : MIL_PopulationElement_ABC( population, nID )
    , pSourceConcentration_       ( 0 )
    , pDestConcentration_         ( 0 )
    , bHeadMoveFinished_          ( false )
    , direction_                  ( 0., 1. )
    , rSpeed_                     ( 0. )
    , bPathUpdated_               ( true )
    , bFlowShapeUpdated_          ( true )
    , bDirectionUpdated_          ( true )
    , bSpeedUpdated_              ( true )
    , pSplittingObject_           ( 0 )
    , personsPassedThroughObject_ ( 0 )
    , armedIndividualsBeforeSplit_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow destructor
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
MIL_PopulationFlow::~MIL_PopulationFlow()
{
    assert( !pSourceConcentration_ );
    assert( !pDestConcentration_ );
    SendDestruction();
    RemoveFromPatch();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::DetachFromDestConcentration
// Created: NLD 2007-03-07
// -----------------------------------------------------------------------------
void MIL_PopulationFlow::DetachFromDestConcentration()
{
    if( pDestConcentration_ )
    {
        pDestConcentration_->UnregisterPushingFlow( *this );
        pDestConcentration_ = 0;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::ComputePath
// Created: NLD 2005-12-11
// -----------------------------------------------------------------------------
void MIL_PopulationFlow::ComputePath( const MT_Vector2D& destination )
{
    if( pHeadPath_ )
    {
        pHeadPath_->Cancel();
        pHeadPath_.reset();
    }
    if( pTailPath_ )
    {
        pTailPath_->Cancel();
        pTailPath_.reset();
    }
    pHeadPath_.reset( new DEC_Population_Path( GetPopulation(), GetHeadPosition(), destination ) );
    MIL_AgentServer::GetWorkspace().GetPathFindManager().StartCompute( pHeadPath_ );
    pTailPath_.reset( new DEC_Population_Path( GetPopulation(), GetTailPosition(), destination ) );
    MIL_AgentServer::GetWorkspace().GetPathFindManager().StartCompute( pTailPath_ );
    DetachFromDestConcentration();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::MagicMove
// Created: NLD 2005-12-06
// -----------------------------------------------------------------------------
void MIL_PopulationFlow::MagicMove( const MT_Vector2D& destination )
{
    MIL_PopulationConcentration& newConcentration = GetPopulation().GetConcentration( destination );
    newConcentration.PushHumans( PullHumans( GetAllHumans() ) );
    DetachFromDestConcentration();
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
    if( !pHeadPath_ || destination != primaryDestination_ || GetPopulation().GetKnowledge().HasChannelingChanged() )
    {
        primaryDestination_ = destination;
        alternateDestination_ = destination;
        ComputePath( primaryDestination_ );
    }
    // Split management : $$ bof
    if( pTailPath_ && pTailPath_->GetState() != DEC_Path_ABC::ePartial && pTailPath_->GetState() != DEC_Path_ABC::eValid )
        return;
    assert( pHeadPath_ );
    boost::shared_ptr< DEC_PathResult > pHeadPath = boost::dynamic_pointer_cast< DEC_PathResult >( pHeadPath_ );
    int nOut = PHY_MovingEntity_ABC::Move( pHeadPath );
    if( nOut == DEC_PathWalker::eFinished )
        bHeadMoveFinished_ = true;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::NotifyMovingInsideObject
// Created: NLD 2005-12-10
// -----------------------------------------------------------------------------
void MIL_PopulationFlow::NotifyMovingInsideObject( MIL_Object_ABC& object )
{
    object.NotifyPopulationMovingInside( *this );
    //$$$ DEUGUEU Cf. refactor gestion objets <-> population
    if( pSourceConcentration_ && pSourceConcentration_->GetSplittingObject() == &object )
        return;
    const PopulationAttribute* attr = object.RetrieveAttribute< PopulationAttribute >();
    if( !attr )
        return;
    if( !pSplittingObject_ || attr->GetDensity() < pSplittingObject_->GetAttribute< PopulationAttribute >().GetDensity() )
    {
        personsPassedThroughObject_ = 0;
        armedIndividualsBeforeSplit_ = GetPopulation().GetArmedIndividuals();
        pSplittingObject_ = &object;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::NotifyMovingOutsideObject
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
void MIL_PopulationFlow::NotifyMovingOutsideObject( MIL_Object_ABC& object )
{
    object.NotifyPopulationMovingOutside( *this );
    pSplittingObject_ = 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::GetSpeedWithReinforcement
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
double MIL_PopulationFlow::GetSpeedWithReinforcement( const TerrainData& /*environment*/, const MIL_Object_ABC& object ) const
{
    double result = GetMaxSpeed();
    if( CanObjectInteractWith( object ) )
    {
        result = object().ApplySpeedPolicy( object().GetMaxSpeed(), result, result, GetPopulation() );
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::CanObjectInteractWith
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
bool MIL_PopulationFlow::CanObjectInteractWith( const MIL_Object_ABC& object ) const
{
    return object.CanInteractWith( *this );
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
    --itTmp;
    flowShape_.insert( itTmp, point.GetPos() );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::UpdateTailPosition
// Created: NLD 2005-10-04
// -----------------------------------------------------------------------------
void MIL_PopulationFlow::UpdateTailPosition( const double rWalkedDistance )
{
    bFlowShapeUpdated_ = true;
    // $$$$ A NETTOYER
    MT_Vector2D vCur = GetTailPosition();
    IT_PointList itNext = flowShape_.begin();
    ++itNext;
    MT_Vector2D vNext = *itNext;
    MT_Vector2D vDir  = vNext - vCur;
    double rDist = rWalkedDistance;
    double rDirLength = vDir.Magnitude();
    if( rDirLength )
        vDir /= rDirLength;
#pragma warning( push, 0 )
    while( 1 )
#pragma warning( pop )
    {
        if( rDist < rDirLength )
        {
            vCur = vCur + ( vDir * rDist );
            IT_PointList itStart = flowShape_.begin();
            ++ itStart;
            flowShape_.erase( itStart, itNext );
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
// Created: NLD 2007-03-02
// -----------------------------------------------------------------------------
bool MIL_PopulationFlow::ManageSplit()
{
    if( !pTailPath_ )
        return false;
    bool bSplit = false;
    IT_PointList itSplit = flowShape_.begin();
    for( IT_PointList it = flowShape_.begin(); it != flowShape_.end(); ++it )
    {
        if( pTailPath_->GetCurrentKeyOnPath( *it ) == pTailPath_->GetResult().end() )
        {
            bSplit = true;
            break;
        }
        itSplit = it;
    }
    if( !bSplit || itSplit == flowShape_.begin() )
    {
        pTailPath_->Cancel();
        pTailPath_.reset();
        return false;
    }
    // this flow => from tail position to split position
    // new  flow => from split position to head position
    const double rDensityBeforeSplit = GetDensity();
    MIL_PopulationFlow& newFlow = GetPopulation().CreateFlow( *this, *itSplit );
    flowShape_.erase( ++itSplit, flowShape_.end() );
    flowShape_.insert( flowShape_.end(), flowShape_.back() ); // split position is a way point
    assert( flowShape_.size() >= 2 );
    bFlowShapeUpdated_ = true;
    UpdateLocation();
    DetachFromDestConcentration();
    pHeadPath_ = pTailPath_; //$$$ Degueu : destruction de pHeadPath ... (newFlow.pHeadPath_ = pHeadPath_)
    pTailPath_.reset();
    const int nNbrHumans = static_cast< unsigned int >( GetLocation().GetArea() * rDensityBeforeSplit );
    newFlow.PushHumans( PullHumans( GetAllHumans() - nNbrHumans ) );
    UpdateDensity();
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::ManageObjectSplit
// Created: NLD 2006-03-08
// -----------------------------------------------------------------------------
bool MIL_PopulationFlow::ManageObjectSplit()
{
    if( pDestConcentration_ || !pSplittingObject_ )
        return false;
    MoveToAlternateDestination( GetHeadPosition() );
    pDestConcentration_ = &GetPopulation().GetConcentration( GetHeadPosition() );
    pDestConcentration_->SetPullingFlowsDensity( *pSplittingObject_ );
    pDestConcentration_->RegisterPushingFlow( *this );
    //pDestConcentration_->Move( destination_ ); $$ Auto next tick
    //$$$ TMP CRs - a changer apres refactor objets
    // $$$ TODO
    const AnimatorAttribute* animatorAttribute = pSplittingObject_->RetrieveAttribute<AnimatorAttribute>();
    if( animatorAttribute )
    {
        for( AnimatorAttribute::CIT_AgentSet it = animatorAttribute->GetAnimators().begin(); it != animatorAttribute->GetAnimators().end(); ++it )
            MIL_Report::PostEvent( **it, MIL_Report::eReport_InterventionAgainstPopulationStarted, GetAttitude().GetID() );
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::ApplyMove
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
void MIL_PopulationFlow::ApplyMove( const MT_Vector2D& position, const MT_Vector2D& direction, double rSpeed, double /*rWalkedDistance*/ )
{
    if( ManageSplit() )
        return;
    if( ManageObjectSplit() )
        return;
    const double rWalkedDistance = GetPopulation().GetMaxSpeed() /* * 1.*/; // vitesse en pixel/deltaT = metre/deltaT
    //$$ TMP
    unsigned int nNbrHumans = 0;
    if( pSourceConcentration_ )
        nNbrHumans = static_cast< unsigned int >( rWalkedDistance * pSourceConcentration_->GetPullingFlowsDensity() + 0.5f );
    else
    {
        const double rArea = GetLocation().GetArea();
        if( rArea )
            nNbrHumans = static_cast< unsigned int >( rWalkedDistance * ( GetAllHumans() / rArea ) + 0.5f );
        if( nNbrHumans == 0 ) // $$$$ ABR 2011-05-20: to prevent ghost flow
            nNbrHumans = GetAllHumans();
    }
    if( nNbrHumans == 0 )
        return;
    SetDirection( direction );
    SetSpeed( rWalkedDistance );
    if( pSourceConcentration_ )
        nNbrHumans = std::min( nNbrHumans, pSourceConcentration_->GetAllHumans() );
    // Head management
    SetHeadPosition( position );
    if( ( bHeadMoveFinished_ || rSpeed == 0 ) && !pDestConcentration_ &&
        ( !pSourceConcentration_ || !pSourceConcentration_->IsNearPosition( GetHeadPosition() ) ) )
    {
        pDestConcentration_ = &GetPopulation().GetConcentration( GetHeadPosition() );
        pDestConcentration_->RegisterPushingFlow( *this );
    }
    if( pDestConcentration_ )
    {
        if( pDestConcentration_->GetSplittingObject() )
        {
            personsPassedThroughObject_ += nNbrHumans;
            double proportion = std::min( 1., static_cast< double >( personsPassedThroughObject_ ) / GetPopulation().GetAllHumans() + 0.5f );
            double newArmed = armedIndividualsBeforeSplit_ * ( 1 - proportion ) +  GetPopulation().GetNewArmedIndividuals() * proportion;
            GetPopulation().SetArmedIndividuals( newArmed );
        }
        pDestConcentration_->PushHumans( PullHumans( nNbrHumans ) );
    }
    // Tail management
    if( pSourceConcentration_ )
    {
        if( rSpeed != 0 || pDestConcentration_ )
            PushHumans( pSourceConcentration_->PullHumans( nNbrHumans ) );
    }
    else
        UpdateTailPosition( rWalkedDistance );
    if( bFlowShapeUpdated_ )
        UpdateLocation();
    if( bFlowShapeUpdated_ || HasHumansChanged() )
        UpdateDensity();
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
        DetachFromDestConcentration();
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
    agent.Apply( &PopulationCollisionNotificationHandler_ABC::NotifyFlowCollision, *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::GetMaxSpeed
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
double MIL_PopulationFlow::GetMaxSpeed() const
{
    return GetPopulation().GetMaxSpeed();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::SendRC
// Created: NLD 2005-10-05
// -----------------------------------------------------------------------------
void MIL_PopulationFlow::SendRC( int nReportID ) const
{
    MIL_Report::PostEvent( GetPopulation(), static_cast< MIL_Report::E_EngineReport >( nReportID ) ); //$$$
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::GetSafetyPosition
// Created: SBO 2005-12-16
// -----------------------------------------------------------------------------
MT_Vector2D MIL_PopulationFlow::GetSafetyPosition( const MIL_AgentPion& agent, double rMinDistance, double /*rSeed*/ ) const
{
    const MT_Vector2D& agentPosition = agent.GetRole< PHY_RoleInterface_Location >().GetPosition();
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
        evadeDirection = -agent.GetOrderManager().GetDirDanger();
    MT_Vector2D safetyPos = nearestPointOnFlow + evadeDirection * rMinDistance;
    TER_World::GetWorld().ClipPointInsideWorld( safetyPos );
    return safetyPos;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::SendCreation
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
void MIL_PopulationFlow::SendCreation() const
{
    client::CrowdFlowCreation asnMsg;
    asnMsg().mutable_flow()->set_id( GetID() );
    asnMsg().mutable_crowd()->set_id( GetPopulation().GetID() );
    asnMsg.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::SendDestruction
// Created: NLD 2005-10-05
// -----------------------------------------------------------------------------
void MIL_PopulationFlow::SendDestruction() const
{
    client::CrowdFlowDestruction asnMsg;
    asnMsg().mutable_flow()->set_id( GetID() );
    asnMsg().mutable_crowd()->set_id( GetPopulation().GetID() );
    asnMsg.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::SendFullState
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
void MIL_PopulationFlow::SendFullState() const
{
    client::CrowdFlowUpdate asnMsg;
    asnMsg().mutable_flow()->set_id( GetID() );
    asnMsg().mutable_crowd()->set_id( GetPopulation().GetID() );
    if( !SerializeCurrentPath( *asnMsg().mutable_path() ) )
        asnMsg().clear_path();
    NET_ASN_Tools::WritePath( flowShape_, *asnMsg().mutable_parts() );
    NET_ASN_Tools::WriteDirection( direction_, *asnMsg().mutable_direction() );
    asnMsg().set_speed( static_cast< unsigned int >( MIL_Tools::ConvertSpeedSimToMos( rSpeed_ ) ) );
    asnMsg().set_healthy( GetHealthyHumans() );
    asnMsg().set_wounded( GetWoundedHumans() );
    asnMsg().set_contaminated( GetContaminatedHumans() );
    asnMsg().set_dead( GetDeadHumans() );
    asnMsg().set_attitude( GetAttitude().GetAsnID() );
    asnMsg.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::SendChangedState
// Created: NLD 2005-10-04
// -----------------------------------------------------------------------------
void MIL_PopulationFlow::SendChangedState() const
{
    if( !HasChanged() )
        return;
    client::CrowdFlowUpdate asnMsg;
    asnMsg().mutable_flow()->set_id( GetID() );
    asnMsg().mutable_crowd()->set_id( GetPopulation().GetID() );
    if( bPathUpdated_ && !SerializeCurrentPath( *asnMsg().mutable_path() ) )
        asnMsg().clear_path();
    if( bFlowShapeUpdated_ )
        NET_ASN_Tools::WritePath( flowShape_, *asnMsg().mutable_parts() );
    if( bDirectionUpdated_ )
        NET_ASN_Tools::WriteDirection( direction_, *asnMsg().mutable_direction() );
    if( bSpeedUpdated_ )
        asnMsg().set_speed( static_cast< unsigned int >( MIL_Tools::ConvertSpeedSimToMos( rSpeed_ ) ) );
    if( HasHumansChanged() )
    {
        asnMsg().set_healthy( GetHealthyHumans() );
        asnMsg().set_wounded( GetWoundedHumans() );
        asnMsg().set_contaminated( GetContaminatedHumans() );
        asnMsg().set_dead( GetDeadHumans() );
    }
    if( HasAttitudeChanged() )
        asnMsg().set_attitude( GetAttitude().GetAsnID() );
    asnMsg.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::load
// Created: SBO 2005-10-18
// -----------------------------------------------------------------------------
void MIL_PopulationFlow::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< PHY_MovingEntity_ABC      >( *this );
    file >> boost::serialization::base_object< TER_PopulationFlow_ABC    >( *this );
    file >> boost::serialization::base_object< MIL_PopulationElement_ABC >( *this );
    file >> pSourceConcentration_
         >> pDestConcentration_
         >> flowShape_
         >> direction_
         >> rSpeed_;
    idManager_.Lock( MIL_PopulationElement_ABC::GetID() );
    UpdateLocation();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::save
// Created: SBO 2005-10-18
// -----------------------------------------------------------------------------
void MIL_PopulationFlow::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
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

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::GetSpeedWithReinforcement
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
double MIL_PopulationFlow::GetSpeedWithReinforcement( const TerrainData& /*environment*/ ) const
{
    return GetMaxSpeed();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::NotifyMovingOnSpecialPoint
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
void MIL_PopulationFlow::NotifyMovingOnSpecialPoint( boost::shared_ptr< DEC_PathPoint > point /*point*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::NotifyEnvironmentChanged
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
void MIL_PopulationFlow::NotifyEnvironmentChanged()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::NotifyCurrentPathChanged
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
void MIL_PopulationFlow::NotifyCurrentPathChanged()
{
    bPathUpdated_ = true;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::CanMove
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
bool MIL_PopulationFlow::CanMove() const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::HasResources
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
bool MIL_PopulationFlow::HasResources()
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::Clean
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
void MIL_PopulationFlow::Clean()
{
    PHY_MovingEntity_ABC::Clean();
    MIL_PopulationElement_ABC::Clean();
    bPathUpdated_ = false;
    bFlowShapeUpdated_ = false;
    bDirectionUpdated_ = false;
    bSpeedUpdated_ = false;
    bHeadMoveFinished_ = false;
    pSplittingObject_ = 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::HasChanged
// Created: NLD 2005-10-04
// -----------------------------------------------------------------------------
bool MIL_PopulationFlow::HasChanged() const
{
    return HasHumansChanged()
        || HasAttitudeChanged()
        || bFlowShapeUpdated_
        || bDirectionUpdated_
        || bSpeedUpdated_
        || bPathUpdated_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::UnregisterSourceConcentration
// Created: NLD 2005-10-04
// -----------------------------------------------------------------------------
void MIL_PopulationFlow::UnregisterSourceConcentration( MIL_PopulationConcentration& concentration )
{
    assert( pSourceConcentration_ == &concentration );
    ( void )concentration; // remove warning in release
    pSourceConcentration_ = 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::GetHeadPosition
// Created: NLD 2005-10-05
// -----------------------------------------------------------------------------
const MT_Vector2D& MIL_PopulationFlow::GetHeadPosition() const
{
    assert( flowShape_.size() >= 2 );
    return flowShape_.back();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::GetTailPosition
// Created: NLD 2005-10-05
// -----------------------------------------------------------------------------
const MT_Vector2D& MIL_PopulationFlow::GetTailPosition() const
{
    assert( flowShape_.size() >= 2 );
    return flowShape_.front();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::GetPosition
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
const MT_Vector2D& MIL_PopulationFlow::GetPosition() const
{
    return GetHeadPosition();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::GetDirection
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
const MT_Vector2D& MIL_PopulationFlow::GetDirection() const
{
    return direction_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::SetDirection
// Created: NLD 2005-10-20
// -----------------------------------------------------------------------------
void MIL_PopulationFlow::SetDirection( const MT_Vector2D& direction )
{
    if( direction_ == direction )
        return;
    direction_ = direction;
    bDirectionUpdated_ = true;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::SetSpeed
// Created: NLD 2005-10-20
// -----------------------------------------------------------------------------
void MIL_PopulationFlow::SetSpeed( const double rSpeed )
{
    if( rSpeed_ == rSpeed )
        return;
    rSpeed_ = rSpeed;
    bSpeedUpdated_ = true;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::SetHeadPosition
// Created: NLD 2005-10-05
// -----------------------------------------------------------------------------
void MIL_PopulationFlow::SetHeadPosition( const MT_Vector2D& position )
{
    assert( flowShape_.size() >= 2 );
    if( flowShape_.back() == position )
        return;
    bFlowShapeUpdated_ = true;
    flowShape_.back() = position;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::SetTailPosition
// Created: NLD 2005-10-05
// -----------------------------------------------------------------------------
void MIL_PopulationFlow::SetTailPosition( const MT_Vector2D& position )
{
    assert( flowShape_.size() >= 2 );
    if( flowShape_.front() == position )
        return;
    bFlowShapeUpdated_ = true;
    flowShape_.front() = position;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::GetLocation
// Created: NLD 2005-10-07
// -----------------------------------------------------------------------------
const TER_Localisation& MIL_PopulationFlow::GetLocation() const
{
    return location_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::GetSpeed
// Created: NLD 2005-10-14
// -----------------------------------------------------------------------------
double MIL_PopulationFlow::GetSpeed() const
{
    return rSpeed_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::IsValid
// Created: NLD 2005-10-13
// -----------------------------------------------------------------------------
bool MIL_PopulationFlow::IsValid() const
{
    return GetAllHumans() > 0. || pSourceConcentration_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::CanBePerceived
// Created: NLD 2005-12-07
// -----------------------------------------------------------------------------
bool MIL_PopulationFlow::CanBePerceived() const
{
    return IsValid();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::GetDefaultDensity
// Created: SLG 2011-02-11
// -----------------------------------------------------------------------------
double MIL_PopulationFlow::GetDefaultDensity( const MIL_PopulationType& type ) const
{
    return type.GetDefaultFlowDensity();
}
