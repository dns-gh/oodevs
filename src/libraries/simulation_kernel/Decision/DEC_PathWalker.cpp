// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_PathWalker.h"
#include "Decision/DEC_PathPoint.h"
#include "Entities/Effects/MIL_EffectManager.h"
#include "Entities/Actions/PHY_MovingEntity_ABC.h"
#include "Entities/Orders/MIL_Report.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/Objects/MIL_ObjectType_ABC.h"
#include "protocol/Protocol.h"
#include "simulation_terrain/TER_ObjectManager.h"
#include "simulation_terrain/TER_World.h"

namespace
{
    int pathSizeThreshold = 200;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathWalker constructor
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
DEC_PathWalker::DEC_PathWalker( PHY_MovingEntity_ABC& movingEntity )
    : movingEntity_      ( movingEntity )
    , itNextPathPoint_   ()
    , itCurrentPathPoint_()
    , effectMove_        ( *this )
    , vNewPos_           ( 0., 0. )
    , vNewDir_           ( 0., 0. )
    , rCurrentSpeed_     ( 0. )
    , rWalkedDistance_   ( 0. )
    , pointsPassed_      ( 0 )
    , bForcePathCheck_   ( true )
    , bHasMoved_         ( false )
    , bFuelReportSent_   ( false )
    , pathSet_           ( eFinished )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_PathWalker destructor
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
DEC_PathWalker::~DEC_PathWalker()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_PathWalker::ComputeFutureObjectCollision
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
bool DEC_PathWalker::ComputeFutureObjectCollision( const T_KnowledgeObjectVector& objectsToTest, double& rDistance, boost::shared_ptr< DEC_Knowledge_Object >& pObject, const MIL_Agent_ABC& agent, bool blockedByObject, bool applyScale ) const
{
    if( !pCurrentPath_.get() )
        return false;
    return pCurrentPath_->ComputeFutureObjectCollision( objectsToTest, rDistance, pObject, agent, blockedByObject, applyScale );
}

// -----------------------------------------------------------------------------
// Name: DEC_PathWalker::IsMovingOn
// Created: JVT 2004-11-30
// -----------------------------------------------------------------------------
bool DEC_PathWalker::IsMovingOn( const DEC_Path_ABC& path ) const
{
    return pCurrentPath_.get() ? path == *pCurrentPath_ : false;
}

//-----------------------------------------------------------------------------
// Name: DEC_PathWalker::ExtrapolatePosition
// Created: JVT 03-09-25
//-----------------------------------------------------------------------------
MT_Vector2D DEC_PathWalker::ExtrapolatePosition( const MT_Vector2D& position, const double rSpeed, const double rTime, const bool bBoundOnPath ) const
{
    if( !pCurrentPath_.get() )
        return movingEntity_.GetPosition();
    try
    {
        return pCurrentPath_->GetFuturePosition( position, rTime * rSpeed, bBoundOnPath );
    }
    catch( const std::exception& e )
    {
        MT_LOG_ERROR_MSG( tools::GetExceptionMsg( e ) );
        return movingEntity_.GetPosition();
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_PathWalker::ComputeCurrentSpeed
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
void DEC_PathWalker::ComputeCurrentSpeed()
{
    const DEC_PathPoint& curPathPoint = **itCurrentPathPoint_;
    if( curPathPoint.GetType() == DEC_PathPoint::eTypePointPath )
    {
        TerrainData tmpEnvironment = curPathPoint.GetObjectTypesToNextPoint();
        if( !( environment_ == tmpEnvironment ) ) //$$$
        {
            movingEntity_.NotifyEnvironmentChanged();
            environment_ = tmpEnvironment;
        }
    }
    rCurrentSpeed_ = movingEntity_.GetSpeed( environment_ );
    if( curPathPoint.IsSlopeValid() )
    {
        const double maxSlope = movingEntity_.GetMaxSlope();
        if( maxSlope <= 0 || curPathPoint.GetSlope() > maxSlope )
            rCurrentSpeed_ = 0;
        else
        {
            const double factor = 1 - curPathPoint.GetSlope() / maxSlope * movingEntity_.GetSlopeDeceleration();
            rCurrentSpeed_ = std::max( 0., rCurrentSpeed_ * factor );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_PathWalker::InitializeEnvironment
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
void DEC_PathWalker::InitializeEnvironment( const DEC_PathResult& path )
{
    DEC_PathResult::T_PathPoints::const_iterator itPathPointTmp = itCurrentPathPoint_;
    while( itPathPointTmp != path.GetResult().end() && ( *itPathPointTmp )->GetType() != DEC_PathPoint::eTypePointPath )
        ++itPathPointTmp;
    if( itPathPointTmp == path.GetResult().end() )
        MT_LOG_ERROR_MSG( "Path point is invalid" );

    TerrainData tmpEnvironment = ( *itPathPointTmp )->GetObjectTypesToNextPoint();
    if( !( environment_ == tmpEnvironment ) ) //$$$
    {
        movingEntity_.NotifyEnvironmentChanged();
        environment_ = tmpEnvironment;
    }
}

namespace
{
    bool IsDifficultToPassThrough( const MT_Vector2D& lastWaypoint, const MIL_Object_ABC& object, PHY_MovingEntity_ABC& movingEntity, const TerrainData& environment )
    {
        return object.IsInside( lastWaypoint )
            && movingEntity.GetSpeed( environment, object ) == 0
            && movingEntity.CanObjectInteractWith( object )
            && movingEntity.HasKnowledgeObject( object );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_PathWalker::SetCurrentPath
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
DEC_PathWalker::E_ReturnCode DEC_PathWalker::SetCurrentPath( boost::shared_ptr< DEC_PathResult > pPath )
{
    if( pCurrentPath_.get() && pPath == pCurrentPath_ && !bForcePathCheck_  /*&& !GetRole< PHY_RoleInterface_Location >().HasDoneMagicMove()*/ )
        return eRunning;

    pointsPassed_ = 0;
    DEC_PathWalker::E_ReturnCode rc = eRunning;
    bool bCanSendTerrainReport = pPath != pCurrentPath_;
    pCurrentPath_ = pPath;
    pPath->InsertDecPoints(); // $$$ HIDEUX
    movingEntity_.NotifyCurrentPathChanged();
    bForcePathCheck_ = false;
    if( pPath->GetResult().empty() )
        MT_LOG_ERROR_MSG( "List of path points resulting from pathfind is empty" );
    itCurrentPathPoint_ = pPath->GetCurrentKeyOnPath();
    if( itCurrentPathPoint_ == pPath->GetResult().end() )
        return eItineraireMustBeJoined;
    if( *itCurrentPathPoint_ )
        pPath->NotifyPointReached( itCurrentPathPoint_ );
    if( ( pPath->GetState() == DEC_PathResult::ePartial ) && bCanSendTerrainReport )
    {
        const MT_Vector2D& lastWaypoint = pPath->GetLastWaypoint();
        TER_Object_ABC::T_ObjectVector objects;
        TER_World::GetWorld().GetObjectManager().GetListWithinCircle( lastWaypoint, 100., objects );
        std::string objectName;
        double cost = 0;
        for( TER_Object_ABC::CIT_ObjectVector itObject = objects.begin(); itObject != objects.end(); ++itObject )
        {
            const MIL_Object_ABC& object = static_cast< MIL_Object_ABC& >( **itObject );
            if( IsDifficultToPassThrough( lastWaypoint, object, movingEntity_, environment_ ) )
            {
                const double objectCost = movingEntity_.GetObjectCost( object.GetType(), pPath->GetPathType() );
                if( objectCost > cost )
                {
                    cost = objectCost;
                    objectName = object.GetType().GetRealName();
                }
            }
        }

        if( objectName != "" )
            movingEntity_.SendRC( report::eRC_DifficultMovementProgression, objectName );
        else
            movingEntity_.SendRC( report::eRC_TerrainDifficile );
        rc = ePartialPath;
    }
    itNextPathPoint_ = itCurrentPathPoint_;
    ++itNextPathPoint_;
    InitializeEnvironment( *pPath );
    return rc;
}

//-----------------------------------------------------------------------------
// Name: DEC_PathWalker::GoToNextNavPoint
// Created: JVT 02-12-06
// Last modified: JVT 03-02-04
//-----------------------------------------------------------------------------
inline
bool DEC_PathWalker::GoToNextNavPoint( DEC_PathResult& path )
{
    if( ( *itNextPathPoint_ )->GetType() == DEC_PathPoint::eTypePointPath )
    {
        movingEntity_.NotifyMovingOnPathPoint( (*itNextPathPoint_)->GetPos() );
        SetCurrentPathPoint( path );
        ++itNextPathPoint_;
        CheckPathNotification();
        return false;
    }
    // points particuliers -> EVT vers DEC
    do
    {
        movingEntity_.NotifyMovingOnSpecialPoint( *itNextPathPoint_ );
        SetCurrentPathPoint( path );
        CheckPathNotification();
    }
    while( ++itNextPathPoint_ != path.GetResult().end() &&
         ( *itNextPathPoint_ )->GetType() != DEC_PathPoint::eTypePointPath && ( *itNextPathPoint_ )->GetPos() == vNewPos_ );
    return true;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathWalker::CheckPathNotification
// Created: LDC 2012-10-22
// -----------------------------------------------------------------------------
void DEC_PathWalker::CheckPathNotification()
{
    ++pointsPassed_;
    if( pointsPassed_ % pathSizeThreshold == 0 )
    {
        movingEntity_.NotifyCurrentPathChanged();
    }
}

//-----------------------------------------------------------------------------
// Name: DEC_PathWalker::SetCurrentPathPoint
// Created: LDC 2012-01-18
//-----------------------------------------------------------------------------
void DEC_PathWalker::SetCurrentPathPoint( DEC_PathResult& path )
{
    itCurrentPathPoint_ = itNextPathPoint_;
    if( itCurrentPathPoint_ != path.GetResult().end() && *itCurrentPathPoint_ )
        path.NotifyPointReached( itCurrentPathPoint_ );
}

//-----------------------------------------------------------------------------
// Name: DEC_PathWalker::ComputeObjectsCollision
// Created: NLD 2002-12-17
//-----------------------------------------------------------------------------
void DEC_PathWalker::ComputeObjectsCollision( const MT_Vector2D& vStart, const MT_Vector2D& vEnd, T_MoveStepSet& moveStepSet )
{
    // R�cup�ration de la liste des objets dynamiques contenus dans le rayon vEnd - vStart
    TER_Object_ABC::T_ObjectVector objects;
    TER_World::GetWorld().GetObjectManager().GetListWithinCircle( vStart, ( vEnd - vStart ).Magnitude(), objects );
    MT_Line lineTmp( vStart, vEnd );
    moveStepSet.insert( T_MoveStep( vStart ) );
    moveStepSet.insert( T_MoveStep( vEnd   ) );
    TER_DistanceLess colCmp( vStart );
    T_PointSet collisions( colCmp );
    for( TER_Object_ABC::CIT_ObjectVector itObject = objects.begin(); itObject != objects.end(); ++itObject )
    {
        const MIL_Object_ABC& object = static_cast< MIL_Object_ABC& >( **itObject );
        // Ajout des points de collision dans moveStepSet
        if( object.Intersect2D( lineTmp, collisions ) )
        {
            for( auto itPoint = collisions.begin(); itPoint != collisions.end(); ++itPoint )
            {
                if( !object.IsInside( *itPoint ) )
                    continue;
                IT_MoveStepSet itMoveStep = moveStepSet.insert( T_MoveStep( *itPoint ) ).first;
                const_cast< T_ObjectSet& >( itMoveStep->ponctualObjectsOnSet_ ).insert( &object );
                // A - C - B ( Le point C ajout� entre A et B contient les m�mes objets que de A -> B)
                if( itMoveStep != moveStepSet.begin() )
                {
                    IT_MoveStepSet itPrevMoveStep = itMoveStep;
                    const_cast< T_ObjectSet& >( itMoveStep->objectsToNextPointSet_ ) = ( --itPrevMoveStep )->objectsToNextPointSet_;
                }
            }
            collisions.clear();
        }

        // D�termination si objet courant se trouve sur le trajet entre chaque point
        IT_MoveStepSet itPrevMoveStep = moveStepSet.begin();
        bool bInsideObjectOnPrevPoint = false;
        for( auto itMoveStep = ++( moveStepSet.begin() ); itMoveStep != moveStepSet.end(); ++itMoveStep )
        {
            // Picking au milieu de la ligne reliant les 2 points
            MT_Vector2D vTmp = ( itMoveStep->vPos_ + itPrevMoveStep->vPos_ ) / 2;
            if( object.IsInside( vTmp ) )
            {
                const_cast< T_ObjectSet& >( itPrevMoveStep->objectsToNextPointSet_ ).insert( &object );
                bInsideObjectOnPrevPoint = true;
                const_cast< T_ObjectSet& >( itPrevMoveStep->ponctualObjectsOnSet_ ).erase( &object ); // This is not yet a ponctual object
            }
            else
            {
                // Stockage des objets desquels on sort
                if( bInsideObjectOnPrevPoint )
                {
                    const_cast< T_ObjectSet& >( itMoveStep->objectsOutSet_ ).insert( &object );
                    bInsideObjectOnPrevPoint = false;
                }
            }
            itPrevMoveStep = itMoveStep;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_PathWalker::SetBlockedByObject
// Created: LDC 2013-06-19
// -----------------------------------------------------------------------------
void DEC_PathWalker::SetBlockedByObject( const MT_Vector2D& startPosition, MIL_Object_ABC& object, CIT_MoveStepSet itCurMoveStep )
{
    static const double rDistanceBeforeBlockingObject = TER_World::GetWorld().GetWeldValue();

    rCurrentSpeed_ = 0;
    MT_Vector2D oldPosition = vNewPos_;
    vNewPos_ = itCurMoveStep->vPos_;
    do
    {
        vNewPos_ -= vNewDir_ * rDistanceBeforeBlockingObject;
    }
    while( object.IsInside( vNewPos_ ) || object.IsOnBorder( vNewPos_ ) );
    if( itCurMoveStep->vPos_.Distance( startPosition ) < itCurMoveStep->vPos_.Distance( vNewPos_ ) )
        vNewPos_ = startPosition;

    movingEntity_.NotifyMovingOutsideObject( object );  // $$$$ NLD 2007-05-07: FOIREUX
    pathSet_ = eBlockedByObject;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathWalker::TryToMoveToNextStep
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
bool DEC_PathWalker::TryToMoveToNextStep( const MT_Vector2D& startPosition, CIT_MoveStepSet itCurMoveStep, CIT_MoveStepSet itNextMoveStep, double& rTimeRemaining )
{
    // Prise en compte des objets ponctuels se trouvant sur le 'move step'
    for( auto itObject = itCurMoveStep->ponctualObjectsOnSet_.begin(); itObject != itCurMoveStep->ponctualObjectsOnSet_.end(); ++itObject )
    {
        MIL_Object_ABC& object = const_cast< MIL_Object_ABC& >( **itObject );
        double rSpeedWithinObject = movingEntity_.GetSpeed( environment_, object );
        if( movingEntity_.CanObjectInteractWith( object ) )
        {
            movingEntity_.NotifyMovingInsideObject( object );
            if( rSpeedWithinObject == 0. )
            {
                if( object.IsOnBorder( vNewPos_ ) || !object.IsInside( vNewPos_ ) )
                {
                    SetBlockedByObject( startPosition, object, itCurMoveStep );
                    return false;
                }
            }
            movingEntity_.NotifyMovingOutsideObject( object );
        }
    }

    double rMaxSpeedForStep = std::numeric_limits< double >::max();
    for( auto itObject = itCurMoveStep->objectsToNextPointSet_.begin(); itObject != itCurMoveStep->objectsToNextPointSet_.end(); ++itObject )
    {
        MIL_Object_ABC& object = const_cast< MIL_Object_ABC& >( **itObject );
        if( movingEntity_.CanObjectInteractWith( object ) )
        {
            movingEntity_.NotifyMovingInsideObject( object );
            double rSpeedWithinObject = movingEntity_.GetSpeed( environment_, object );
            if( rSpeedWithinObject == 0. )
            {
                if( object.IsOnBorder( vNewPos_ ) || !object.IsInside( vNewPos_ ) )
                {
                    SetBlockedByObject( startPosition, object, itCurMoveStep );
                    return false;
                }
            }
            else
                rMaxSpeedForStep = std::min( rMaxSpeedForStep, rSpeedWithinObject );
        }
    }

    // itCurMoveStep a pu �tre d�pass� => notification des objets dont on sort
    for( auto itObject = itNextMoveStep->objectsOutSet_.begin(); itObject != itNextMoveStep->objectsOutSet_.end(); ++itObject )
    {
        MIL_Object_ABC& object = const_cast< MIL_Object_ABC& >( **itObject );
        if( movingEntity_.CanObjectInteractWith( object ) )
            movingEntity_.NotifyMovingOutsideObject( object );
    }

    if( rMaxSpeedForStep != std::numeric_limits< double >::max() )
        rCurrentSpeed_ = rMaxSpeedForStep;
    const double rDistToWalk = rTimeRemaining * rCurrentSpeed_;
    const MT_Vector2D vNewPosTmp( vNewPos_ + ( vNewDir_ * rDistToWalk ) );

    if( vNewPos_.SquareDistance( vNewPosTmp ) >= vNewPos_.SquareDistance( itNextMoveStep->vPos_ )  )
    {
        rTimeRemaining -= ( itNextMoveStep->vPos_ - vNewPos_ ).Magnitude() / rCurrentSpeed_;
        vNewPos_ = itNextMoveStep->vPos_;
        return true;
    }
    else
    {
        rTimeRemaining -= ( vNewPosTmp - vNewPos_ ).Magnitude() / rCurrentSpeed_;
        vNewPos_ = vNewPosTmp;
        return false;
    }
}

//-----------------------------------------------------------------------------
// Name: DEC_PathWalker::TryToMoveTo
// Created: NLD 2002-12-17
//-----------------------------------------------------------------------------
bool DEC_PathWalker::TryToMoveTo( const MT_Vector2D& vNewPosTmp, double& rTimeRemaining )
{
    // Deplacement de vNewPos_ vers vNewPosTmp
    if( vNewPosTmp == vNewPos_ )
        return true;

    if( rCurrentSpeed_ <= 0. )
    {
        MT_LOG_ERROR_MSG( "Current speed is not positive" );
        return false;
    }

    MT_Vector2D vNewPos( vNewPos_ );

    sMoveStepCmp cmp( vNewPos );
    T_MoveStepSet moveStepSet( cmp );
    ComputeObjectsCollision( vNewPos, vNewPosTmp, moveStepSet );
    if( moveStepSet.size() < 2 )
    {
        MT_LOG_ERROR_MSG( "Move step set has not at least 2 elements" ); // $$$$ LDC RC Can never happen.
        return false;
    }
    CIT_MoveStepSet itCurMoveStep  = moveStepSet.begin();
    CIT_MoveStepSet itNextMoveStep = moveStepSet.begin();
    ++itNextMoveStep;
    while( rTimeRemaining > 0. )
    {
        if( !TryToMoveToNextStep( vNewPos, itCurMoveStep, itNextMoveStep, rTimeRemaining ) )
            return false;
        itCurMoveStep = itNextMoveStep;
        ++itNextMoveStep;
        if( itNextMoveStep == moveStepSet.end() )
            return true;
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathWalker::Move
// Created: NLD 2004-09-22
// Modified: MGD 2010-03-12
// -----------------------------------------------------------------------------
int DEC_PathWalker::Move( boost::shared_ptr< DEC_PathResult > pPath )
{
    if( bHasMoved_ )
        return eAlreadyMoving;

    DEC_PathResult::E_State nPathState = pPath->GetState();
    if( nPathState == DEC_Path_ABC::eInvalid || nPathState == DEC_Path_ABC::eImpossible || nPathState == DEC_Path_ABC::eCanceled )
        return eNotAllowed;

    if( nPathState == DEC_Path_ABC::eComputing )
    {
        bHasMoved_ = true;
        return eRunning;
    }

    pathSet_ = SetCurrentPath( pPath );
    if( pathSet_ == eItineraireMustBeJoined )
        return eItineraireMustBeJoined;

    bHasMoved_ = true;
    ComputeCurrentSpeed();
    vNewPos_ = movingEntity_.GetPosition ();
    vNewDir_ = movingEntity_.GetDirection();
    MIL_EffectManager::GetEffectManager().Register( effectMove_ );

    if( !movingEntity_.IsReady() )
    {
        rCurrentSpeed_ = 0.;
        return eRunning;
    }

    if( rCurrentSpeed_ == 0. || !movingEntity_.CanMove() )
    {
        rCurrentSpeed_ = 0.;
        return eNotAllowed;
    }

    if( itNextPathPoint_ == pPath->GetResult().end() )
    {
        rCurrentSpeed_ = 0.;
        return eFinished;
    }

    if( !movingEntity_.HasResources() )
    {
        rCurrentSpeed_ = 0.;
        if( !bFuelReportSent_ )
        {
            movingEntity_.SendRC( report::eRC_PlusDeCarburant );
            bFuelReportSent_ = true;
        }
        return eNotEnoughFuel;
    }
    bFuelReportSent_ = false;

    double rTimeRemaining = 1.; // 1 dT
    if( ( *itNextPathPoint_ )->GetPos() != vNewPos_ )
        vNewDir_ = ( ( *itNextPathPoint_ )->GetPos() - vNewPos_ ).Normalize();

    rWalkedDistance_ = 0;
    while( rTimeRemaining > 0. )
    {
        const MT_Vector2D vPosBeforeMove( vNewPos_ );
        if( !TryToMoveTo( ( *itNextPathPoint_ )->GetPos(), rTimeRemaining ) )
        {
            rWalkedDistance_ += vPosBeforeMove.Distance( vNewPos_ );
            return pathSet_;
        }

        rWalkedDistance_ += vPosBeforeMove.Distance( vNewPos_ );

        bool bStopOnInterestingPoint = GoToNextNavPoint( *pPath );
        if( bStopOnInterestingPoint )
            return pathSet_;

        if( itNextPathPoint_ == pPath->GetResult().end() )
        {
            rCurrentSpeed_ = 0;
            return ( pathSet_ != ePartialPath ) ? eFinished : pathSet_;
        }

        ComputeCurrentSpeed();

        if( ( *itNextPathPoint_ )->GetPos() != vNewPos_ )
            vNewDir_ = ( ( *itNextPathPoint_ )->GetPos() - vNewPos_ ).Normalize();
    }
    return pathSet_;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathWalker::MoveSuspended
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
void DEC_PathWalker::MoveSuspended( boost::shared_ptr< DEC_PathResult > pPath )
{
    if( !pCurrentPath_ && ( pPath->GetState() == DEC_Path_ABC::eValid || pPath->GetState() == DEC_Path_ABC::ePartial ) )
        SetCurrentPath(pPath );
    if( !pCurrentPath_.get() && !bForcePathCheck_ )
        MT_LOG_ERROR_MSG( "Move cannot be suspended" );
    if( pCurrentPath_.get() && pCurrentPath_ == pPath )
        bForcePathCheck_ = true;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathWalker::MoveCanceled
// Created: NLD 2005-03-16
// -----------------------------------------------------------------------------
void DEC_PathWalker::MoveCanceled( boost::shared_ptr< DEC_PathResult > pPath )
{
    if( pCurrentPath_ == pPath )
    {
        pCurrentPath_.reset();
        bForcePathCheck_ = true;
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_PathWalker::Apply
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
void DEC_PathWalker::Apply()
{
    if( movingEntity_.CanMove() )
        movingEntity_.ApplyMove( vNewPos_, vNewDir_, rCurrentSpeed_, rWalkedDistance_ );
}

// -----------------------------------------------------------------------------
// Name: DEC_PathWalker::SerializeEnvironmentType
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
void DEC_PathWalker::SerializeEnvironmentType( sword::UnitEnvironmentType& msg ) const
{
    msg.set_area( environment_.Area() );
    msg.set_left( environment_.Left() );
    msg.set_right( environment_.Right() );
    msg.set_linear( environment_.Linear() );
}

// -----------------------------------------------------------------------------
// Name: DEC_PathWalker::SerializeCurrentPath
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
void DEC_PathWalker::SerializeCurrentPath( sword::Path& asn ) const
{
    if( pCurrentPath_.get() )
        pCurrentPath_->Serialize( asn, pointsPassed_, 2*pathSizeThreshold );
}

// -----------------------------------------------------------------------------
// Name: DEC_PathWalker::HasCurrentPath
// Created: LGY 2013-07-18
// -----------------------------------------------------------------------------
bool DEC_PathWalker::HasCurrentPath() const
{
    return pCurrentPath_.get() ? true : false;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathWalker::Clean
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
void DEC_PathWalker::Clean()
{
    if( !bHasMoved_ )
        bForcePathCheck_ = true;
    bHasMoved_ = false;
}
