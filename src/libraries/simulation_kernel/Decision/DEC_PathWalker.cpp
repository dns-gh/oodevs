// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "protocol/protocol.h"
#include "DEC_PathWalker.h"
#include "Decision/DEC_PathPoint.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/Effects/MIL_EffectManager.h"
#include "Entities/Actions/PHY_MovingEntity_ABC.h"
#include "Entities/Orders/MIL_Report.h"
#include "UrbanModel.h"
#include "protocol/protocol.h"
#include <urban/Architecture.h>
#include <urban/Model.h>
#include <urban/TerrainObject_ABC.h>

//$$$ Refactorer gestion collisions objets

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
    , bForcePathCheck_   ( true )
    , bHasMoved_         ( false )
{

}

// -----------------------------------------------------------------------------
// Name: DEC_PathWalker destructor
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
DEC_PathWalker::~DEC_PathWalker()
{

}

// =============================================================================
// TOOLS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_PathWalker::ComputeFutureObjectCollision
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
bool DEC_PathWalker::ComputeFutureObjectCollision( const MT_Vector2D& vStartPos, const T_KnowledgeObjectVector& objectsToTest, MT_Float& rDistance, boost::shared_ptr< DEC_Knowledge_Object >& pObject ) const
{
    if( !pCurrentPath_.get() )
        return false;
    return pCurrentPath_->ComputeFutureObjectCollision( vStartPos, objectsToTest, rDistance, pObject );
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
MT_Vector2D DEC_PathWalker::ExtrapolatePosition( const MT_Vector2D& position, const MT_Float rSpeed, const MT_Float rTime, const bool bBoundOnPath ) const
{
    if( !pCurrentPath_.get() )
        return movingEntity_.GetPosition();
    return pCurrentPath_->GetFuturePosition( position, rTime * rSpeed, bBoundOnPath );
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_PathWalker::ComputeCurrentSpeed
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
inline
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
    rCurrentSpeed_ = movingEntity_.GetSpeedWithReinforcement( environment_ );
}

// -----------------------------------------------------------------------------
// Name: DEC_PathWalker::InitializeEnvironment
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
inline
void DEC_PathWalker::InitializeEnvironment( const DEC_PathResult& path )
{
    DEC_PathResult::CIT_PathPointList itPathPointTmp = itCurrentPathPoint_;
    while( itPathPointTmp != path.GetResult().end() && (*itPathPointTmp)->GetType() != DEC_PathPoint::eTypePointPath )
        ++itPathPointTmp;
    assert( itPathPointTmp != path.GetResult().end() );

    TerrainData tmpEnvironment = (*itPathPointTmp)->GetObjectTypesToNextPoint();
    if( !( environment_ == tmpEnvironment ) ) //$$$
    {
        movingEntity_.NotifyEnvironmentChanged();
        environment_ = tmpEnvironment;
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_PathWalker::SetCurrentPath
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
bool DEC_PathWalker::SetCurrentPath( boost::shared_ptr< DEC_PathResult > pPath )
{
    if( pCurrentPath_.get() && pPath == pCurrentPath_ && !bForcePathCheck_  /*&& !GetRole< PHY_RolePion_Location >().HasDoneMagicMove()*/ )
        return true;

    pCurrentPath_ = pPath;

    pPath->InsertDecPoints(); // $$$ HIDEUX
    movingEntity_.NotifyCurrentPathChanged();
    bForcePathCheck_ = false;

    assert( !pPath->GetResult().empty() );
    itCurrentPathPoint_ = pPath->GetCurrentKeyOnPath( movingEntity_.GetPosition() );
    if( itCurrentPathPoint_ == pPath->GetResult().end() )
        return false;

    if( pCurrentPath_->GetState() == DEC_PathResult::ePartial )
        movingEntity_.SendRC( MIL_Report::eReport_DifficultTerrain );

    itNextPathPoint_ = itCurrentPathPoint_;
    ++itNextPathPoint_;
    InitializeEnvironment( *pPath );
    return true;
}

//-----------------------------------------------------------------------------
// Name: DEC_PathWalker::GoToNextNavPoint
// Created: JVT 02-12-06
// Last modified: JVT 03-02-04
//-----------------------------------------------------------------------------
inline
bool DEC_PathWalker::GoToNextNavPoint( const DEC_PathResult& path )
{
    if( (*itNextPathPoint_)->GetType() == DEC_PathPoint::eTypePointPath )
    {
        movingEntity_.NotifyMovingOnPathPoint( **itNextPathPoint_ );
        itCurrentPathPoint_ = itNextPathPoint_;
        ++itNextPathPoint_;
        return false;
    }

    // points particuliers -> EVT vers DEC
    do
    {
        movingEntity_.NotifyMovingOnSpecialPoint( *itNextPathPoint_ );
        itCurrentPathPoint_ = itNextPathPoint_;
    }
    while      ( ++itNextPathPoint_ != path.GetResult().end()
            && ( *itNextPathPoint_ )->GetType() != DEC_PathPoint::eTypePointPath
            && ( *itNextPathPoint_ )->GetPos() == vNewPos_ );
    return true;
}

//-----------------------------------------------------------------------------
// Name: DEC_PathWalker::ComputeObjectsCollision
// Created: NLD 2002-12-17
//-----------------------------------------------------------------------------
void DEC_PathWalker::ComputeObjectsCollision( const MT_Vector2D& vStart, const MT_Vector2D& vEnd, T_MoveStepSet& moveStepSet )
{
    // R�cup�ration de la liste des objets dynamiques contenus dans le rayon vEnd - vStart
    TER_Object_ABC::T_ObjectVector objects;
    TER_World::GetWorld().GetObjectManager().GetListWithinCircle( vNewPos_, (vEnd - vStart).Magnitude(), objects );

    MT_Line lineTmp( vStart, vEnd );

    moveStepSet.insert( T_MoveStep( vStart ) );
    moveStepSet.insert( T_MoveStep( vEnd   ) );

    TER_DistanceLess colCmp( vStart );
    T_PointSet collisions( colCmp );

    for( TER_Object_ABC::CIT_ObjectVector itObject = objects.begin(); itObject != objects.end(); ++itObject )
    {
        MIL_Object_ABC& object = static_cast< MIL_Object_ABC& >( **itObject );

        // Ajout des points de collision dans moveStepSet
        if( object.Intersect2D( lineTmp, collisions ) )
        {
            for( IT_PointSet itPoint = collisions.begin(); itPoint != collisions.end(); ++itPoint )
            {
                MT_Vector2D& vPoint = *itPoint;
                assert( object.IsInside( vPoint ) );

                IT_MoveStepSet itMoveStep = moveStepSet.insert( T_MoveStep( vPoint ) ).first;
                itMoveStep->ponctualObjectsOnSet_.insert( &object );

                // A - C - B ( Le point C ajout� entre A et B contient les m�mes objets que de A -> B)
                if( itMoveStep != moveStepSet.begin() )
                {
                    IT_MoveStepSet itPrevMoveStep = itMoveStep;
                    itMoveStep->objectsToNextPointSet_ = (--itPrevMoveStep)->objectsToNextPointSet_;
                }
            }
            collisions.clear();
        }

        // D�termination si objet courant se trouve sur le trajet entre chaque point
        IT_MoveStepSet itPrevMoveStep = moveStepSet.begin();
        bool bInsideObjectOnPrevPoint = false;
        for( IT_MoveStepSet itMoveStep = ++(moveStepSet.begin()); itMoveStep != moveStepSet.end(); ++itMoveStep )
        {
            // Picking au milieu de la ligne reliant les 2 points
            MT_Vector2D vTmp = ( itMoveStep->vPos_ + itPrevMoveStep->vPos_ ) / 2;
            if( object.IsInside( vTmp ) )
            {
                itPrevMoveStep->objectsToNextPointSet_.insert( &object );
                bInsideObjectOnPrevPoint = true;
                itPrevMoveStep->ponctualObjectsOnSet_.erase( &object ); // This is not yet a ponctual object
            }
            else
            {
                // Stockage des objets desquels on sort
                if( bInsideObjectOnPrevPoint )
                {
                    itMoveStep->objectsOutSet_.insert( &object );
                    bInsideObjectOnPrevPoint = false;
                }
            }
            itPrevMoveStep = itMoveStep;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_PathWalker::TryToMoveToNextStep
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
bool DEC_PathWalker::TryToMoveToNextStep( CIT_MoveStepSet itCurMoveStep, CIT_MoveStepSet itNextMoveStep, MT_Float& rTimeRemaining, bool bFirstMove )
{
    static int nDistanceBeforeBlockingObject = -10;
    CIT_ObjectSet itObject;

    // Prise en compte des objets ponctuels se trouvant sur le 'move step'
    for( itObject = itCurMoveStep->ponctualObjectsOnSet_.begin(); itObject != itCurMoveStep->ponctualObjectsOnSet_.end(); ++itObject )
    {
        MIL_Object_ABC& object = const_cast< MIL_Object_ABC& >( **itObject );

        MT_Float rSpeedWithinObject = movingEntity_.GetSpeedWithReinforcement( environment_, object );

        if( !bFirstMove ) //// $$$$$ !bFirstMove A REVOIR - PERMET DE SORTIR D'UN OBSTACLE PONCTUEL
        {
            if( movingEntity_.CanObjectInteractWith( object ) )
                movingEntity_.NotifyMovingInsideObject( object );
            if( rSpeedWithinObject == 0. )
            {
                rCurrentSpeed_ = 0;
                vNewPos_           = ( itCurMoveStep->vPos_ + ( vNewDir_ * nDistanceBeforeBlockingObject ) );
                movingEntity_.NotifyMovingOutsideObject( object );  // $$$$ NLD 2007-05-07:
                return false;
            }
        }
        movingEntity_.NotifyMovingOutsideObject( object );
    }

    MT_Float rMaxSpeedForStep = std::numeric_limits< MT_Float >::max();
    for( itObject = itCurMoveStep->objectsToNextPointSet_.begin(); itObject != itCurMoveStep->objectsToNextPointSet_.end(); ++itObject )
    {
        MIL_Object_ABC& object = const_cast< MIL_Object_ABC& >( **itObject );
        if( movingEntity_.CanObjectInteractWith( object ) )
            movingEntity_.NotifyMovingInsideObject( object );

        rMaxSpeedForStep = std::min( rMaxSpeedForStep, movingEntity_.GetSpeedWithReinforcement( environment_, object) );
        if( rMaxSpeedForStep == 0. )
        {
            rCurrentSpeed_ = 0;
            vNewPos_       = ( itCurMoveStep->vPos_ + ( vNewDir_ * nDistanceBeforeBlockingObject ) );
            movingEntity_.NotifyMovingOutsideObject( object );  // $$$$ NLD 2007-05-07: FOIREUX
            return false;
        }
    }

    // itCurMoveStep a pu �tre d�pass� => notification des objets dont on sort
    for( itObject = itNextMoveStep->objectsOutSet_.begin(); itObject != itNextMoveStep->objectsOutSet_.end(); ++itObject )
        movingEntity_.NotifyMovingOutsideObject( const_cast< MIL_Object_ABC& >( **itObject ) );

    if( rMaxSpeedForStep != std::numeric_limits< MT_Float >::max() )
        rCurrentSpeed_ = rMaxSpeedForStep;
    const MT_Float rDistToWalk = rTimeRemaining * rCurrentSpeed_;
    const MT_Vector2D vNewPosTmp( vNewPos_ + ( vNewDir_ * rDistToWalk ) );

    if( vNewPos_.SquareDistance( vNewPosTmp ) >= vNewPos_.SquareDistance( itNextMoveStep->vPos_ )  )
    {
        rTimeRemaining -= ( itNextMoveStep->vPos_ - vNewPos_ ).Magnitude() / rCurrentSpeed_;
        vNewPos_        = itNextMoveStep->vPos_;
        return true;
    }
    else
    {
        rTimeRemaining -= ( vNewPosTmp - vNewPos_ ).Magnitude() / rCurrentSpeed_;
        vNewPos_        = vNewPosTmp;
        return false;
    }
}

//-----------------------------------------------------------------------------
// Name: DEC_PathWalker::TryToMoveTo
// Created: NLD 2002-12-17
//-----------------------------------------------------------------------------
bool DEC_PathWalker::TryToMoveTo( const DEC_PathResult& path, const MT_Vector2D& vNewPosTmp, MT_Float& rTimeRemaining )
{
    // Deplacement de vNewPos_ vers vNewPosTmp
    if( vNewPosTmp == vNewPos_ )
        return true;

    assert( rCurrentSpeed_ > 0. );
//    bool bFirstMove = ( vNewPos_.Distance( (*path.GetResult().begin())->GetPos() ) <= 10. );
    bool bFirstMove = ( (float)vNewPos_.rX_ == (float)(*path.GetResult().begin())->GetPos().rX_ && (float)vNewPos_.rY_ == (float)(*path.GetResult().begin())->GetPos().rY_ );

    sMoveStepCmp  cmp( vNewPos_ );
    T_MoveStepSet moveStepSet( cmp );

    ComputeObjectsCollision( vNewPos_, vNewPosTmp, moveStepSet );

    assert( moveStepSet.size() >= 2 );

    CIT_MoveStepSet itCurMoveStep  = moveStepSet.begin();
    CIT_MoveStepSet itNextMoveStep = moveStepSet.begin();
    ++itNextMoveStep;

    while( rTimeRemaining > 0. )
    {
        if( !TryToMoveToNextStep( itCurMoveStep, itNextMoveStep, rTimeRemaining, bFirstMove ) )
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

    if( !SetCurrentPath( pPath ) )
        return eItineraireMustBeJoined;

    bHasMoved_ = true;
    ComputeCurrentSpeed();
    vNewPos_ = movingEntity_.GetPosition ();
    vNewDir_ = movingEntity_.GetDirection();
    MIL_EffectManager::GetEffectManager().Register( effectMove_ );

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
        movingEntity_.SendRC( MIL_Report::eReport_OutOfGas );
        return eNotEnoughFuel;
    }

    MT_Float rTimeRemaining = 1.; // 1 dT
    if( (*itNextPathPoint_)->GetPos() != vNewPos_ )
        vNewDir_ = ( (*itNextPathPoint_)->GetPos() - vNewPos_ ).Normalize();

    rWalkedDistance_ = 0;
    while( rTimeRemaining > 0. )
    {
        const MT_Vector2D vPosBeforeMove( vNewPos_ );

        if( !TryToMoveTo( *pPath, (*itNextPathPoint_)->GetPos(), rTimeRemaining ) )
        {
            rWalkedDistance_ += vPosBeforeMove.Distance( vNewPos_ );
            return eRunning;
        }

        rWalkedDistance_ += vPosBeforeMove.Distance( vNewPos_ );

        bool bStopOnInterestingPoint = GoToNextNavPoint( *pPath );
        if( bStopOnInterestingPoint )
            return eRunning;

        if( itNextPathPoint_ == pPath->GetResult().end() )
        {
            rCurrentSpeed_ = 0;
            return eFinished;
        }

        ComputeCurrentSpeed();

        if( (*itNextPathPoint_)->GetPos() != vNewPos_ )
            vNewDir_ = ( (*itNextPathPoint_)->GetPos() - vNewPos_).Normalize();
    }
    return eRunning;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathWalker::MoveSuspended
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
void DEC_PathWalker::MoveSuspended( boost::shared_ptr< DEC_PathResult > pPath )
{
    assert( pCurrentPath_.get() || bForcePathCheck_ );

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

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_PathWalker::SerializeEnvironmentType
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
void DEC_PathWalker::SerializeEnvironmentType( MsgsSimToClient::MsgUnitEnvironmentType& msg ) const
{
    msg.set_area   ( environment_.Area() );
    msg.set_left   ( environment_.Left() );
    msg.set_right  ( environment_.Right() );
    msg.set_linear ( environment_.Linear() );
}

// -----------------------------------------------------------------------------
// Name: DEC_PathWalker::SerializeCurrentPath
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
bool DEC_PathWalker::SerializeCurrentPath( Common::MsgPath& asn ) const
{
    if( !pCurrentPath_.get() )
        return false;

    pCurrentPath_->Serialize( asn );
    return true;
}