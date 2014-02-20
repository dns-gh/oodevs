// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_PopulationFlow.h"
#include "MIL_PopulationConcentration.h"
#include "MIL_PopulationAttitude.h"
#include "MIL_PopulationType.h"
#include "MIL_FlowCollisionManager.h"
#include "Decision/DEC_Population_Path.h"
#include "Decision/DEC_PathFind_Manager.h"
#include "Decision/DEC_PathPoint.h"
#include "Decision/DEC_Population_PathClass.h"
#include "Decision/DEC_PathType.h"
#include "Decision/DEC_PathWalker.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Objects/AnimatorAttribute.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/Objects/MIL_ObjectManipulator_ABC.h"
#include "Entities/Objects/MIL_ObjectType_ABC.h"
#include "Entities/Objects/MIL_ObjectFactory.h"
#include "Entities/Objects/PopulationAttribute.h"
#include "Entities/Orders/MIL_Report.h"
#include "Entities/Populations/MIL_Population.h"
#include "MIL_AgentServer.h"
#include "Network/NET_ASN_Tools.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"
#include "simulation_kernel/PopulationCollisionNotificationHandler_ABC.h"
#include "simulation_terrain/TER_World.h"
#include "simulation_terrain/TER_PopulationManager.h"
#include "simulation_terrain/TER_PopulationFlowManager.h"
#include "Tools/MIL_Tools.h"
#include <boost/make_shared.hpp>

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

namespace
{
    MIL_FlowCollisionManager& GetFlowCollisionManager()
    {
        return MIL_AgentServer::GetWorkspace().GetEntityManager().GetFlowCollisionManager();
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow constructor
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
MIL_PopulationFlow::MIL_PopulationFlow( MIL_Population& population, MIL_PopulationConcentration& sourceConcentration )
    : MIL_PopulationElement_ABC( population, idManager_.GetId() )
    , pSourceConcentration_       ( &sourceConcentration )
    , pDestConcentration_         ( 0 )
    , flowShape_                  ( 2, std::make_pair( sourceConcentration.GetPosition(), 0 ) )
    , direction_                  ( 0., 1. )
    , rSpeed_                     ( 0. )
    , rWalkedDistance_            ( 0 )
    , bHeadMoveFinished_          ( false )
    , bPathUpdated_               ( true )
    , bFlowShapeUpdated_          ( true )
    , bDirectionUpdated_          ( true )
    , bSpeedUpdated_              ( true )
    , bBlocked_                   ( false )
    , pSplittingObject_           ( 0 )
    , pBlockingObject_            ( 0 )
    , pFirstSplittingObject_      ( sourceConcentration.GetSplittingObject() )
    , personsPassedThroughObject_ ( 0 )
    , armedIndividualsBeforeSplit_( 0 )
    , objectDensity_              ( 1. )
    , canCollideWithFlow_( population.GetType().CanCollideWithFlow() )
    , hasDoneMagicMove_( false )
    , speedLimit_( std::numeric_limits< double >::max() )
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
    : MIL_PopulationElement_ABC( population, idManager_.GetId() )
    , pSourceConcentration_       ( 0 )
    , pDestConcentration_         ( 0 )
    , primaryDestination_         ( source.primaryDestination_ )
    , alternateDestination_       ( source.alternateDestination_ )
    , pHeadPath_                  ( source.pHeadPath_ ) //$$$$ Degueu : faire une copie
    , flowShape_                  ( source.flowShape_ )
    , direction_                  ( 0., 1. )
    , rSpeed_                     ( 0. )
    , rWalkedDistance_            ( 0 )
    , bHeadMoveFinished_          ( false )
    , bPathUpdated_               ( true )
    , bFlowShapeUpdated_          ( true )
    , bDirectionUpdated_          ( true )
    , bSpeedUpdated_              ( true )
    , bBlocked_                   ( false )
    , pSplittingObject_           ( 0 )
    , pBlockingObject_            ( 0 )
    , pFirstSplittingObject_      ( 0 )
    , personsPassedThroughObject_ ( 0 )
    , armedIndividualsBeforeSplit_( 0 )
    , objectDensity_              ( 1. )
    , canCollideWithFlow_( population.GetType().CanCollideWithFlow() )
    , hasDoneMagicMove_( false )
    , speedLimit_( source.speedLimit_ )
    , moveAlongPath_( source.moveAlongPath_ )
{
    auto itSplit = FindPointInShape( splitPoint );
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
    , direction_                  ( 0., 1. )
    , rSpeed_                     ( 0. )
    , rWalkedDistance_            ( 0 )
    , bHeadMoveFinished_          ( false )
    , bPathUpdated_               ( true )
    , bFlowShapeUpdated_          ( true )
    , bDirectionUpdated_          ( true )
    , bSpeedUpdated_              ( true )
    , bBlocked_                   ( false )
    , pSplittingObject_           ( 0 )
    , pBlockingObject_            ( 0 )
    , pFirstSplittingObject_      ( 0 )
    , personsPassedThroughObject_ ( 0 )
    , armedIndividualsBeforeSplit_( 0 )
    , objectDensity_              ( 1. )
    , canCollideWithFlow_( population.GetType().CanCollideWithFlow() )
    , hasDoneMagicMove_( false )
    , speedLimit_( std::numeric_limits< double >::max() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow destructor
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
MIL_PopulationFlow::~MIL_PopulationFlow()
{
    if( MIL_AgentServer::IsInitialized() )
        GetFlowCollisionManager().NotifyFlowDestruction( this );
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
    auto pDestination = boost::make_shared< MT_Vector2D >( destination );
    std::vector< boost::shared_ptr< MT_Vector2D > > vDestination;
    vDestination.push_back( pDestination );
    ComputePathAlong( vDestination, vDestination );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::ComputePathAlong
// Created: JSR 2014-01-16
// -----------------------------------------------------------------------------
void MIL_PopulationFlow::ComputePathAlong( const std::vector< boost::shared_ptr< MT_Vector2D > >& headDestination, const std::vector< boost::shared_ptr< MT_Vector2D > >& tailDestination )
{
    CancelMove();
    if( pTailPath_ )
    {
        pTailPath_->Cancel();
        pTailPath_->DecRef();
        pTailPath_.reset();
    }
    pHeadPath_.reset( new DEC_Population_Path( GetPopulation(), GetHeadPosition(), headDestination ) );
    pHeadPath_->AddRef();
    MIL_AgentServer::GetWorkspace().GetPathFindManager().StartCompute( pHeadPath_ );
    pTailPath_.reset( new DEC_Population_Path( GetPopulation(), GetTailPosition(), tailDestination ) );
    pTailPath_->AddRef();
    MIL_AgentServer::GetWorkspace().GetPathFindManager().StartCompute( pTailPath_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::MagicMove
// Created: NLD 2005-12-06
// -----------------------------------------------------------------------------
void MIL_PopulationFlow::MagicMove( const MT_Vector2D& destination )
{
    hasDoneMagicMove_ = true;
    MIL_PopulationConcentration& newConcentration = GetPopulation().GetConcentration( destination );
    newConcentration.PushHumans( PullHumans( GetAllHumans() ) );
    newConcentration.SetAttitude( GetAttitude() );
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
    if( nOut == DEC_PathWalker::eItineraireMustBeJoined )
        ApplyMove( GetHeadPosition(), GetDirection(), 0, 0 );
    if( nOut == DEC_PathWalker::eFinished )
        bHeadMoveFinished_ = true;
    if( nOut == DEC_PathWalker::eBlockedByObject )
    {
        if( !bBlocked_ )
        {
            if( pBlockingObject_ )
            {
                const std::string name = MIL_AgentServer::GetWorkspace().GetObjectFactory().FindType( pBlockingObject_->GetType().GetName() ).GetRealName();
                SendRC( report::eRC_DifficultMovementProgression, name );
            }
        }
        bBlocked_ = true;
        pBlockingObject_ = 0;
    }
    else
        bBlocked_ = false;
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
        pFirstSplittingObject_ = &object;
    if( pFirstSplittingObject_ )
        return;
    objectDensity_ = 1.;
    const PopulationAttribute* attr = object.RetrieveAttribute< PopulationAttribute >();
    if( !attr )
        return;
    objectDensity_ = attr->GetDensity();
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
    pBlockingObject_ = &object;
    if( objectDensity_ != 0 )
    {
        pSplittingObject_ = 0;
        if( &object == pFirstSplittingObject_ )
            pFirstSplittingObject_ = 0;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::GetSpeed
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
double MIL_PopulationFlow::GetSpeed( const TerrainData& environment, const MIL_Object_ABC& object ) const
{
    double result = GetPopulation().GetType().GetMaxSpeed( environment );
    if( CanObjectInteractWith( object ) )
    {
        result = object().ApplySpeedPolicy( object().GetMaxSpeed(), result, result, GetPopulation() );
        const PopulationAttribute* populationAttribute = object.RetrieveAttribute< PopulationAttribute >();
        if( populationAttribute && populationAttribute->GetDensity() == 0)
            result = 0.;
    }
    return std::min( speedLimit_, result );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::CanObjectInteractWith
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
bool MIL_PopulationFlow::CanObjectInteractWith( const MIL_Object_ABC& object ) const
{
    return object.CanInteractWithEntity();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::CanBeBlock
// Created: LGY 2013-01-21
// -----------------------------------------------------------------------------
bool MIL_PopulationFlow::HasKnowledgeObject( const MIL_Object_ABC& /*object*/ ) const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::GetObjectCost
// Created: LGY 2013-01-21
// -----------------------------------------------------------------------------
double MIL_PopulationFlow::GetObjectCost( const MIL_ObjectType_ABC& objectType,
                                          const DEC_PathType& pathType ) const
{
    return DEC_Population_PathClass::GetPathClass( pathType.GetName() ).GetObjectCost( objectType );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::NotifyMovingOnPathPoint
// Created: NLD 2005-10-04
// -----------------------------------------------------------------------------
void MIL_PopulationFlow::NotifyMovingOnPathPoint( const MT_Vector2D& point )
{
    pointsToInsert_.push_back( point );
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
    auto itNext = std::next( flowShape_.begin() );
    MT_Vector2D vNext = itNext->first;
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
            flowShape_.erase( std::next( flowShape_.begin() ), itNext );
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
                flowShape_.erase( std::next( flowShape_.begin() ), std::prev( flowShape_.end() ) );
                SetTailPosition( GetHeadPosition() );
                break;
            }
            vNext = itNext->first;
            vDir = vNext - vCur;
            rDirLength = vDir.Magnitude();
            if( rDirLength )
                vDir /= rDirLength;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::Split
// Created: JSR 2014-01-08
// -----------------------------------------------------------------------------
MIL_PopulationFlow* MIL_PopulationFlow::Split( const MT_Vector2D& splittingPoint, std::size_t segmentIndex )
{
    assert( canCollideWithFlow_ );
    const double rDensityBeforeSplit = GetDensity();
    auto insertIt = FindPointInShape( splittingPoint );
    if( insertIt == std::prev( flowShape_.end() ) )
        return 0;
    if( insertIt == flowShape_.end() )
    {
        if( segmentIndex < flowShape_.size() )
        {
            insertIt = flowShape_.begin();
            std::advance( insertIt, segmentIndex + 1 );
        }
        auto itPoint = std::prev( insertIt );
        insertIt = flowShape_.insert( insertIt, std::make_pair( splittingPoint, itPoint->second ) );
    }
    else if( flowShape_.size() == 2 )
        return 0;
    MIL_PopulationFlow& newFlow = GetPopulation().CreateFlow( *this, splittingPoint );
    flowShape_.erase( std::next( insertIt ), flowShape_.end() );
    flowShape_.push_back( std::make_pair( splittingPoint, flowShape_.rbegin()->second ) );
    pHeadPath_.reset();
    assert( flowShape_.size() >= 2 );

    DetachFromDestConcentration();
    bFlowShapeUpdated_ = true;
    UpdateLocation();
    const int nNbrHumans = static_cast< unsigned int >( GetLocation().GetArea() * rDensityBeforeSplit );
    newFlow.PushHumans( PullHumans( GetAllHumans() - nNbrHumans ) );
    UpdateDensity();
    return &newFlow;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::ManageSplit
// Created: NLD 2007-03-02
// -----------------------------------------------------------------------------
bool MIL_PopulationFlow::ManageSplit()
{
    if( !pTailPath_ )
        return false;
    pointsToInsert_.clear();
    bool bSplit = false;
    auto itSplit = flowShape_.begin();
    for( auto it = flowShape_.begin(); it != flowShape_.end(); ++it )
    {
        if( !pTailPath_->IsOnPath( it->first ) )
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
    MIL_PopulationFlow& newFlow = GetPopulation().CreateFlow( *this, itSplit->first );
    flowShape_.erase( std::next( itSplit ), flowShape_.end() );
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
    if( pDestConcentration_ || !pSplittingObject_ || pFirstSplittingObject_ )
        return false;
    MIL_PopulationConcentration* target = &GetPopulation().GetConcentration( GetHeadPosition() );
    if( objectDensity_ == 1 )
        return false;
    if( target == pSourceConcentration_ )
        return true;
    pointsToInsert_.clear();
    MoveToAlternateDestination( GetHeadPosition() );
    pDestConcentration_ = target;
    pDestConcentration_->SetPullingFlowsDensity( *pSplittingObject_ );
    pDestConcentration_->RegisterPushingFlow( *this );
    //pDestConcentration_->Move( destination_ ); $$ Auto next tick
    //$$$ TMP CRs - a changer apres refactor objets
    // $$$ TODO
    const AnimatorAttribute* animatorAttribute = pSplittingObject_->RetrieveAttribute<AnimatorAttribute>();
    if( animatorAttribute )
    {
        for( auto it = animatorAttribute->GetAnimators().begin(); it != animatorAttribute->GetAnimators().end(); ++it )
            MIL_Report::PostEvent( **it, report::eRC_DebutInterventionFaceAPopulation, GetAttitude().GetID() );
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::ApplyMove
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
void MIL_PopulationFlow::ApplyMove( const MT_Vector2D& position, const MT_Vector2D& direction, double rSpeed, double /*rWalkedDistance*/ )
{
    if( canCollideWithFlow_ && !GetFlowCollisionManager().CanMove( this ) )
        return; 
    if( ! CanMove() )
        return;
    if( ManageSplit() )
        return;
    if( ManageObjectSplit() )
        return;
    if( bBlocked_ && canCollideWithFlow_ )
        return;
    if( !bHeadMoveFinished_ )
        rWalkedDistance_ = rSpeed/* * 1.*/; // vitesse en pixel/deltaT = metre/deltaT
    //$$ TMP
    unsigned int nNbrHumans = 0;
    if( pSourceConcentration_ )
        nNbrHumans = static_cast< unsigned int >( rWalkedDistance_ * pSourceConcentration_->GetPullingFlowsDensity() + 0.5f );
    else
    {
        const double rArea = GetLocation().GetArea();
        if( rArea )
            nNbrHumans = static_cast< unsigned int >( rWalkedDistance_ * ( GetAllHumans() / rArea ) + 0.5f );
        if( nNbrHumans == 0 ) // $$$$ ABR 2011-05-20: to prevent ghost flow
            nNbrHumans = GetAllHumans();
    }
    if( nNbrHumans == 0 )
        return;
    SetDirection( direction );
    SetSpeed( rWalkedDistance_ );
    if( pSourceConcentration_ )
        nNbrHumans = std::min( nNbrHumans, pSourceConcentration_->GetAllHumans() );
    // Head management
    if( !pDestConcentration_ )
    {
        SetHeadPosition( position );
        if( ( bHeadMoveFinished_ || ( !canCollideWithFlow_ && rSpeed == 0 ) ) && !pDestConcentration_ &&
            ( !pSourceConcentration_ || !pSourceConcentration_->IsNearPosition( GetHeadPosition() ) ) )
        {
            MIL_PopulationConcentration* target = &GetPopulation().GetConcentration( GetHeadPosition() );
            if( target != pSourceConcentration_ )
            {
                pDestConcentration_ = target;
                pDestConcentration_->RegisterPushingFlow( *this );
            }
        }
    }
    if( pDestConcentration_ )
    {
        if( pDestConcentration_->GetSplittingObject() )
        {
            personsPassedThroughObject_ += nNbrHumans;
            double proportion = std::min( 1., static_cast< double >( personsPassedThroughObject_ ) / GetPopulation().GetAllHumans() );
            double newArmed = armedIndividualsBeforeSplit_ * ( 1 - proportion ) +  GetPopulation().GetNewArmedIndividuals() * proportion;
            const AnimatorAttribute* animatorAttribute = pDestConcentration_->GetSplittingObject()->RetrieveAttribute<AnimatorAttribute>();
            if( animatorAttribute && animatorAttribute->GetAnimators().size() > 0 )
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
        UpdateTailPosition( rWalkedDistance_ );
    if( bFlowShapeUpdated_ )
        UpdateLocation();
    if( bFlowShapeUpdated_ || HasHumansChanged() )
        UpdateDensity();
}

namespace
{
    bool AreReverse( const MT_Line& l1, const MT_Line& l2 )
    {
        if( MT_IsZero( l1.Magnitude() ) || MT_IsZero( l2.Magnitude() ) )
            return false;
        const MT_Vector2D nV1 = ( l1.GetPosEnd() - l1.GetPosStart() ).Normalized();
        const MT_Vector2D nV2 = ( l2.GetPosEnd() - l2.GetPosStart() ).Normalized();
        return std::abs( CrossProduct( nV1, nV2 ) ) < 0.1 && DotProduct( nV1, nV2 ) < 0;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::UpdateSpeedLimit
// Created: JSR 2014-01-31
// -----------------------------------------------------------------------------
void MIL_PopulationFlow::UpdateSpeedLimit()
{
    speedLimit_ = std::numeric_limits< double >::max();
    if( canCollideWithFlow_ )
    {
        TER_PopulationFlowManager::T_PopulationFlowVector flows;
        const double radius = GetPopulation().GetMaxSpeed();
        TER_World::GetWorld().GetPopulationManager().GetFlowManager().GetListWithinCircle( GetHeadPosition(), radius, flows );
        for( auto it = flows.begin(); it != flows.end(); ++it )
        {
            const MIL_PopulationFlow* flow = static_cast< MIL_PopulationFlow* >( *it );
            if( flow == this )
                continue;
            const double squareDistance = GetHeadPosition().SquareDistance( flow->GetTailPosition() );
            if( squareDistance > radius * radius )
                continue;

            // If the two crowds are reverse, the speed is not limited
            // We skip the too small segments which are not relevant (can happen at tight turns)
            bool reverse = false;
            auto itEnd1 = GetFlowShape().end();
            auto itStart1 = std::prev( itEnd1 );
            bool l1Relevant = false;
            while( !reverse && !l1Relevant && itStart1 != GetFlowShape().begin() )
            {
                itEnd1 = itStart1;
                itStart1 = std::prev( itEnd1 );
                MT_Line l1( *itStart1, *itEnd1 );
                l1Relevant = ( l1.Magnitude() >= radius );

                auto itStart2 = flow->GetFlowShape().begin();
                auto itEnd2 = std::next( itStart2 );

                bool l2Relevant = false;
                while( !reverse && !l2Relevant && itEnd2 != flow->GetFlowShape().end() )
                {
                    MT_Line l2( *itStart2, *itEnd2 );
                    l2Relevant = ( l2.Magnitude() >= radius );
                    if( AreReverse( l1, l2 ) && l1.Intersect2D( l2 ) != eDontIntersect )
                        reverse = true;
                    else if( !l2Relevant )
                    {
                        itStart2 = itEnd2;
                        itEnd2 = std::next( itStart2 );
                    }
                }
            }
            if( !reverse )
                speedLimit_ = std::min( speedLimit_, sqrt( squareDistance ) / 2 );
        }
    }
}

namespace
{
    bool AreNear( const MT_Line& line, const MT_Line& flowSegment, MT_Vector2D& intersection )
    {
        static const double nearSquareDistance = 10;
        if( line.GetPosStart().SquareDistance( flowSegment.GetPosStart() ) < nearSquareDistance ||
            line.GetPosEnd().SquareDistance( flowSegment.GetPosStart() ) < nearSquareDistance )
        {
            intersection = flowSegment.GetPosStart();
            return true;
        }
        if( line.GetPosStart().SquareDistance( flowSegment.GetPosEnd() ) < nearSquareDistance ||
            line.GetPosEnd().SquareDistance( flowSegment.GetPosEnd() ) < nearSquareDistance )
        {
            intersection = flowSegment.GetPosEnd();
            return true;
        }
        return false;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::AddFlowCollision
// Created: JSR 2014-01-15
// -----------------------------------------------------------------------------
bool MIL_PopulationFlow::AddFlowCollision( const MT_Line& line, const MT_Line& flowSegment, T_FlowCollisions& collisions )
{
    MT_Vector2D intersection;
    if( AreNear( line, flowSegment, intersection ) || flowSegment.Intersect2D( line, intersection ) == eDoIntersect )
    {
        if( AreReverse( line, flowSegment ) )
            return false;

        if( intersection.SquareDistance( flowSegment.GetPosEnd() ) < 100 )
        {
            auto it = FindPointInShape( flowSegment.GetPosEnd() );
            bool relevantSegment = false;
            while( !relevantSegment && it != flowShape_.end() && std::next( it ) != flowShape_.end() )
            {
                const MT_Line localLine( it->first, std::next( it )->first );
                if( line.Intersect2D( localLine ) != eDoIntersect && AreReverse( line, localLine ) )
                    return false;
                relevantSegment = ( localLine.Magnitude() > GetPopulation().GetMaxSpeed() );
                ++it;
            }
        }

        if( intersection.SquareDistance( flowSegment.GetPosStart() ) < 100 )
        {
            auto it = FindPointInShape( flowSegment.GetPosStart() );
            if( it != flowShape_.end() )
            {
                bool relevantSegment = false;
                while( !relevantSegment )
                {
                    if( it == flowShape_.begin() )
                        return false;
                    const MT_Line localLine( std::prev( it )->first, it->first );
                    if( line.Intersect2D( localLine ) != eDoIntersect && AreReverse( line, localLine ) )
                        return false;
                    relevantSegment = ( localLine.Magnitude() > GetPopulation().GetMaxSpeed() );
                    --it;
                }
            }
        }
        collisions.push_back( std::make_pair( this, intersection ) );
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::CanCollideWithFlow
// Created: JSR 2014-01-16
// -----------------------------------------------------------------------------
bool MIL_PopulationFlow::CanCollideWithFlow() const
{
    return canCollideWithFlow_ && GetDensity() > 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::CanCollideWith
// Created: JSR 2014-01-16
// -----------------------------------------------------------------------------
bool MIL_PopulationFlow::CanCollideWith( MIL_PopulationFlow* flow ) const
{
    if( !CanCollideWithFlow() || !flow->CanCollideWithFlow() )
        return false;
    if( &flow->GetPopulation() == &GetPopulation() )
        return false;
    const MT_Vector2D& head1 = GetHeadPosition();
    const MT_Vector2D& tail1 = GetTailPosition();
    const MT_Vector2D& head2 = flow->GetHeadPosition();
    const MT_Vector2D& tail2 = flow->GetTailPosition();
    return head1 != head2 && head1 != tail2 && tail1 != head2 && tail1 != tail2;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::ComputeFlowCollisions
// Created: JSR 2014-01-15
// -----------------------------------------------------------------------------
bool MIL_PopulationFlow::ComputeFlowCollisions( const MT_Line& line, T_FlowCollisions& collisions )
{
    if( line.Magnitude() < 1 )
        return false;
    TER_PopulationFlowManager::T_PopulationFlowVector flows;
    TER_World::GetWorld().GetPopulationManager().GetFlowManager().GetListIntersectingLine( line.GetPosStart(), line.GetPosEnd(), flows );
    for( auto it = flows.begin(); it != flows.end(); ++it )
    {
        MIL_PopulationFlow* flow = static_cast< MIL_PopulationFlow* >( *it );
        if( CanCollideWith( flow ) )
            flow->ApplyOnShape( boost::bind( &MIL_PopulationFlow::AddFlowCollision, flow, boost::cref( line ), _1, boost::ref( collisions ) ) );
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::UpdateCrowdCollisions
// Created: JSR 2014-01-14
// -----------------------------------------------------------------------------
void MIL_PopulationFlow::UpdateCrowdCollisions()
{
    if( canCollideWithFlow_ && GetDensity() > 0 )
    {
        // todo : extract in method, refactor, simplify
        T_FlowCollisions collisions;
        ApplyOnShape( boost::bind( &MIL_PopulationFlow::ComputeFlowCollisions, this, _1, boost::ref( collisions ) ) );
        GetFlowCollisionManager().SetCollisions( this, collisions );
        UpdateSpeedLimit();
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::Update
// Created: NLD 2005-10-04
// -----------------------------------------------------------------------------
bool MIL_PopulationFlow::Update()
{
    if( !IsValid() )
    {
        DetachFromDestConcentration();
        RemoveFromPatch();
        return false; // Must be destroyed
    }
    
    // Collisions
    UpdateCollisions();
    UpdateCrowdCollisions();
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::UpdateLocation
// Created: NLD 2005-10-07
// -----------------------------------------------------------------------------
void MIL_PopulationFlow::UpdateLocation()
{
    location_.Reset( TER_Localisation::eLine, GetFlowShape() );
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
    if( canCollideWithFlow_ && !GetFlowCollisionManager().CanMove( this ) )
        return 0;
    return std::min( speedLimit_, GetPopulation().GetMaxSpeed() );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::GetSlopeDeceleration
// Created: JSR 2013-11-12
// -----------------------------------------------------------------------------
double MIL_PopulationFlow::GetSlopeDeceleration() const
{
    return 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::GetMaxSlope
// Created: JSR 2014-01-20
// -----------------------------------------------------------------------------
double MIL_PopulationFlow::GetMaxSlope() const
{
    return std::numeric_limits< double >::max();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::SendRC
// Created: NLD 2005-10-05
// -----------------------------------------------------------------------------
void MIL_PopulationFlow::SendRC( const MIL_DecisionalReport& reportId ) const
{
    MIL_Report::PostEvent( GetPopulation(), reportId ); //$$$
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::SendRC
// Created: CMA 2012-02-02
// -----------------------------------------------------------------------------
void MIL_PopulationFlow::SendRC( const MIL_DecisionalReport& reportId, const std::string& name ) const
{
    MIL_Report::PostEvent( GetPopulation(), reportId, name );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::GetSafetyPosition
// Created: SBO 2005-12-16
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > MIL_PopulationFlow::GetSafetyPosition( const MIL_AgentPion& agent, double rMinDistance, double /*rSeed*/ ) const
{
    const MT_Vector2D& agentPosition = agent.GetRole< PHY_RoleInterface_Location >().GetPosition();
    MT_Vector2D nearestPointOnFlow;
    GetLocation().ComputeNearestPoint( agentPosition, nearestPointOnFlow );
    // find flow segment containing nearestPointOnFlow
    auto itStart = flowShape_.begin();
    auto itEnd = std::next( itStart );
    for( ; itEnd != flowShape_.end(); ++itStart, ++itEnd )
        if( MT_Line( itStart->first, itEnd->first ).IsInside( nearestPointOnFlow, 10.0f ) ) // $$$$ SBO 2006-02-22: epsilon should be 0
            break;
    if( itEnd == flowShape_.end() ) // $$$$ SBO 2006-02-22: should not happen
        return boost::shared_ptr< MT_Vector2D >();
    MT_Vector2D evadeDirection = ( itEnd->first - itStart->first ).Normalize().Rotate( MT_PI / 2 );
    if( evadeDirection.IsZero() )
        evadeDirection = -agent.GetOrderManager().GetDirDanger();
    auto safetyPos = boost::make_shared< MT_Vector2D >( nearestPointOnFlow + evadeDirection * rMinDistance );
    TER_World::GetWorld().ClipPointInsideWorld( *safetyPos );
    return safetyPos;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::SendCreation
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
void MIL_PopulationFlow::SendCreation( unsigned int context ) const
{
    client::CrowdFlowCreation asnMsg;
    asnMsg().mutable_flow()->set_id( GetID() );
    asnMsg().mutable_crowd()->set_id( GetPopulation().GetID() );
    asnMsg.Send( NET_Publisher_ABC::Publisher(), context );
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
    sword::Path& path = *asnMsg().mutable_path();
    path.mutable_location()->set_type( sword::Location::line );
    SerializeCurrentPath( path );
    NET_ASN_Tools::WritePath( GetFlowShape(), *asnMsg().mutable_parts() );
    NET_ASN_Tools::WriteDirection( direction_, *asnMsg().mutable_direction() );
    asnMsg().set_speed( MIL_Tools::ConvertSpeedSimToMos( rSpeed_ ) );
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
    sword::Path& path = *asnMsg().mutable_path();
    path.mutable_location()->set_type( sword::Location::line );
    SerializeCurrentPath( *asnMsg().mutable_path() );
    if( bFlowShapeUpdated_ )
        NET_ASN_Tools::WritePath( GetFlowShape(), *asnMsg().mutable_parts() );
    if( bDirectionUpdated_ )
        NET_ASN_Tools::WriteDirection( direction_, *asnMsg().mutable_direction() );
    if( bSpeedUpdated_ )
        asnMsg().set_speed( MIL_Tools::ConvertSpeedSimToMos( rSpeed_ ) );
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
    idManager_.GetId( MIL_PopulationElement_ABC::GetID(), true );
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
// Name: MIL_PopulationFlow::GetSpeed
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
double MIL_PopulationFlow::GetSpeed( const TerrainData& environment ) const
{
    return std::min( speedLimit_, GetPopulation().GetType().GetMaxSpeed( environment ) );
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
    pBlockingObject_ = 0;
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
void MIL_PopulationFlow::UnregisterSourceConcentration()
{
    pSourceConcentration_ = 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::GetHeadPosition
// Created: NLD 2005-10-05
// -----------------------------------------------------------------------------
const MT_Vector2D& MIL_PopulationFlow::GetHeadPosition() const
{
    assert( flowShape_.size() >= 2 );
    return flowShape_.back().first;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::GetTailPosition
// Created: NLD 2005-10-05
// -----------------------------------------------------------------------------
const MT_Vector2D& MIL_PopulationFlow::GetTailPosition() const
{
    assert( flowShape_.size() >= 2 );
    return flowShape_.front().first;
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
    if( GetHeadPosition() == position )
        return;

    bFlowShapeUpdated_ = true;
    flowShape_.back().first = position;
    
    for( auto it = pointsToInsert_.begin(); it != pointsToInsert_.end(); ++it )
    {
        if( FindPointInShape( *it ) == flowShape_.end() )
        {
            auto itTmp = std::prev( flowShape_.end() );
            flowShape_.insert( itTmp, std::make_pair( *it, flowShape_.back().second ) );
        }
    }
    pointsToInsert_.clear();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::SetTailPosition
// Created: NLD 2005-10-05
// -----------------------------------------------------------------------------
void MIL_PopulationFlow::SetTailPosition( const MT_Vector2D& position )
{
    assert( flowShape_.size() >= 2 );
    if( flowShape_.front().first == position )
        return;
    bFlowShapeUpdated_ = true;
    flowShape_.front().first = position;
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
    return !hasDoneMagicMove_ && ( GetAllHumans() > 0. || pSourceConcentration_ );
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

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::Intersect2DWithCircle
// Created: SLI 2012-09-26
// -----------------------------------------------------------------------------
bool MIL_PopulationFlow::Intersect2DWithCircle( const MT_Vector2D& vCircleCenter, double rRadius, std::vector< MT_Vector2D >& shape ) const
{
    return TER_PopulationFlow_ABC::Intersect2DWithCircle( vCircleCenter, rRadius, shape );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::MoveAlong
// Created: LDC 2013-03-29
// -----------------------------------------------------------------------------
void MIL_PopulationFlow::MoveAlong( const std::vector< boost::shared_ptr< MT_Vector2D > >& destination )
{
    if( !pHeadPath_ )
    {
        if( moveAlongPath_ != destination )
        {
            moveAlongPath_ = destination;
            for( auto it = flowShape_.begin(); it != flowShape_.end(); ++it )
                it->second = 0;
        }

        const std::size_t waypointIndex = flowShape_.back().second;
        if( waypointIndex == 0 || waypointIndex >= destination.size() )
            ComputePathAlong( destination, destination );
        else
        {
            const std::vector< boost::shared_ptr< MT_Vector2D > > subvector( destination.begin() + waypointIndex, destination.end() );
            ComputePathAlong( subvector, destination );
        }
    }

    std::size_t& waypointIndex = flowShape_.back().second;
    if( waypointIndex < destination.size() - 1 )
    {
        const double weldValue = TER_World::GetWorld().GetWeldValue() * TER_World::GetWorld().GetWeldValue() / 10;
        if( destination[ waypointIndex ]->SquareDistance( GetPosition() ) <= weldValue )
            ++waypointIndex;
    }

    primaryDestination_ = *destination.back();
    Move( primaryDestination_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::CancelMove
// Created: LDC 2013-03-29
// -----------------------------------------------------------------------------
void MIL_PopulationFlow::CancelMove()
{
    if( pHeadPath_ )
    {
        pHeadPath_->Cancel();
        pHeadPath_->DecRef();
        pHeadPath_.reset();
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::IsReady
// Created: SLI 2013-04-23
// -----------------------------------------------------------------------------
bool MIL_PopulationFlow::IsReady() const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::GetFlowShape
// Created: LDC 2013-10-04
// -----------------------------------------------------------------------------
const T_PointList& MIL_PopulationFlow::GetFlowShape() const
{
    if( bFlowShapeUpdated_ || computedFlowShape_.empty() )
    {
        computedFlowShape_.resize( flowShape_.size() );
        auto itComputed = computedFlowShape_.begin();
        for( auto it = flowShape_.begin(); it != flowShape_.end(); ++it, ++itComputed )
            *itComputed = it->first;
    }
    return computedFlowShape_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::ApplyOnShape
// Created: JSR 2014-01-15
// -----------------------------------------------------------------------------
void MIL_PopulationFlow::ApplyOnShape( const boost::function< bool( const MT_Line& ) >& f ) const
{
    auto itStart = flowShape_.begin();
    auto itEnd = std::next( itStart );
    for( ; itEnd != flowShape_.end(); ++itStart, ++itEnd )
    {
        MT_Line line( itStart->first, itEnd->first );
        if( f( line ) )
            return;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::FindPointInShape
// Created: JSR 2014-02-04
// -----------------------------------------------------------------------------
MIL_PopulationFlow::T_FlowShape::const_iterator MIL_PopulationFlow::FindPointInShape( const MT_Vector2D& v ) const
{
    return std::find_if( flowShape_.begin(), flowShape_.end(),
        [ & ]( std::pair< MT_Vector2D, std::size_t > value )->bool {
            return v == value.first; } );
}
