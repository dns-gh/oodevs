// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: NLD 2005-09-30 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#include "MIL_pch.h"
#include "DEC_PathWalker.h"

#include "Decision/Path/DEC_PathPoint.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/Objects/MIL_RealObject_ABC.h"
#include "Entities/Effects/MIL_EffectManager.h"
#include "Entities/Actions/PHY_MovingEntity_ABC.h"
#include "Entities/RC/MIL_RC.h"
#include "MIL_AgentServer.h"

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
    , pCurrentPath_      ( 0 )
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
bool DEC_PathWalker::ComputeFutureObjectCollision( const MT_Vector2D& vStartPos, const T_KnowledgeObjectVector& objectsToTest, MT_Float& rDistance, const DEC_Knowledge_Object** pObject ) const
{
    if( !pCurrentPath_ )
        return false;
    return pCurrentPath_->ComputeFutureObjectCollision( vStartPos, objectsToTest, rDistance, pObject );
}

// -----------------------------------------------------------------------------
// Name: DEC_PathWalker::IsMovingOn
// Created: JVT 2004-11-30
// -----------------------------------------------------------------------------
bool DEC_PathWalker::IsMovingOn( const DEC_Path_ABC& path ) const
{
    return pCurrentPath_ ? path == *pCurrentPath_ : false;
}

//-----------------------------------------------------------------------------
// Name: DEC_PathWalker::ExtrapolatePosition
// Created: JVT 03-09-25
//-----------------------------------------------------------------------------
MT_Vector2D DEC_PathWalker::ExtrapolatePosition( const MT_Vector2D& position, const MT_Float rSpeed, const MT_Float rTime, const bool bBoundOnPath ) const
{
    if( !pCurrentPath_ )
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
bool DEC_PathWalker::SetCurrentPath( DEC_PathResult& path )
{
    if( pCurrentPath_ && path == *pCurrentPath_ && !bForcePathCheck_  /*&& !GetRole< PHY_RolePion_Location >().HasDoneMagicMove()*/ )
        return true;

    if( pCurrentPath_ )
        pCurrentPath_->DecRef();
    pCurrentPath_ = &path;
    pCurrentPath_->IncRef();

    path.InsertDecPoints(); // $$$ HIDEUX
    movingEntity_.NotifyCurrentPathChanged();
    bForcePathCheck_ = false;

    assert( !path.GetResult().empty() );
    itCurrentPathPoint_ = path.GetCurrentKeyOnPath( movingEntity_.GetPosition() );
    if( itCurrentPathPoint_ == path.GetResult().end() )
        return false;

    if( pCurrentPath_->GetState() == DEC_PathResult::ePartial )
        movingEntity_.SendRC( *MIL_RC::pRcTerrainDifficile_ );

    itNextPathPoint_ = itCurrentPathPoint_;   
    ++itNextPathPoint_;
    InitializeEnvironment( path );
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
    if ( (*itNextPathPoint_)->GetType() == DEC_PathPoint::eTypePointPath )
    {
        movingEntity_.NotifyMovingOnPathPoint( **itNextPathPoint_ );
        itCurrentPathPoint_ = itNextPathPoint_;
        ++itNextPathPoint_;
        return false;
    }

    // points particuliers -> EVT vers DEC
    do 
    {
        movingEntity_.NotifyMovingOnSpecialPoint( **itNextPathPoint_ );
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
    // Récupération de la liste des objets dynamiques contenus dans le rayon vEnd - vStart
    TER_Object_ABC::T_ObjectVector objects;
    TER_World::GetWorld().GetObjectManager().GetListWithinCircle( vNewPos_, (vEnd - vStart).Magnitude(), objects ); 

    moveStepSet.clear();

    MT_Line lineTmp( vStart, vEnd );

    moveStepSet.insert( T_MoveStep( vStart ) );
    moveStepSet.insert( T_MoveStep( vEnd   ) );

    TER_DistanceLess colCmp( vStart );
    T_PointSet collisions( colCmp );

    for( TER_Object_ABC::CIT_ObjectVector itObject = objects.begin(); itObject != objects.end(); ++itObject )
    {
        MIL_Object_ABC& object = static_cast< MIL_Object_ABC& >( **itObject );

        if( !movingEntity_.CanObjectInteractWith( object ) )
            continue;

        // Ajout des points de collision dans moveStepSet
        if( object.Intersect2D( lineTmp, collisions ) )
        {           
            for( IT_PointSet itPoint = collisions.begin(); itPoint != collisions.end(); ++itPoint )
            {
                MT_Vector2D& vPoint = *itPoint;
                assert( object.IsInside( vPoint ) );

                IT_MoveStepSet itMoveStep = moveStepSet.insert( T_MoveStep( vPoint ) ).first;
                itMoveStep->ponctualObjectsOnSet_.insert( &object );

                // A - C - B ( Le point C ajouté entre A et B contient les mêmes objets que de A -> B)
                if( itMoveStep != moveStepSet.begin() )
                {
				    IT_MoveStepSet itPrevMoveStep = itMoveStep;
                    itMoveStep->objectsToNextPointSet_ = (--itPrevMoveStep)->objectsToNextPointSet_;
                }
            }
            collisions.clear();
        }

        // Détermination si objet courant se trouve sur le trajet entre chaque point 
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
    CIT_ObjectSet itObject;

    // Prise en compte des objets ponctuels se trouvant sur le 'move step'
    for( itObject = itCurMoveStep->ponctualObjectsOnSet_.begin(); itObject != itCurMoveStep->ponctualObjectsOnSet_.end(); ++itObject )
    {
		MIL_Object_ABC& object = **itObject;

        MT_Float rSpeedWithinObject = movingEntity_.GetSpeedWithReinforcement( environment_, object );
   
        if( !bFirstMove ) //// $$$$$ !bFirstMove A REVOIR - PERMET DE SORTIR D'UN OBSTACLE PONCTUEL
        {
            movingEntity_.NotifyMovingInsideObject( object );
            
            if( rSpeedWithinObject == 0. ) 
            {
                rSpeedWithinObject = 0;
                vNewPos_           = itCurMoveStep->vPos_;
                return false;
            }
        }
        movingEntity_.NotifyMovingOutsideObject( object );
    }
    
    MT_Float rMaxSpeedForStep = std::numeric_limits< MT_Float >::max();
    for( itObject = itCurMoveStep->objectsToNextPointSet_.begin(); itObject != itCurMoveStep->objectsToNextPointSet_.end(); ++itObject )
    {
        MIL_Object_ABC& object = **itObject;
        movingEntity_.NotifyMovingInsideObject( object );

        rMaxSpeedForStep = std::min( rMaxSpeedForStep, movingEntity_.GetSpeedWithReinforcement( environment_, object) );        
        if( rMaxSpeedForStep == 0. )
        {
            rCurrentSpeed_ = 0;
            vNewPos_       = itCurMoveStep->vPos_;
            return false;
        }    
    }

    // itCurMoveStep a pu être dépassé => notification des objets dont on sort
    for( itObject = itNextMoveStep->objectsOutSet_.begin(); itObject != itNextMoveStep->objectsOutSet_.end(); ++itObject )
        movingEntity_.NotifyMovingOutsideObject( **itObject );

    if( rMaxSpeedForStep != std::numeric_limits< MT_Float >::max() )
        rCurrentSpeed_ = rMaxSpeedForStep;
    const MT_Float rDistToWalk = rTimeRemaining * rCurrentSpeed_;
    const MT_Vector2D vNewPosTmp( vNewPos_ + ( vNewDir_ * rDistToWalk ) );

    if ( vNewPos_.SquareDistance( vNewPosTmp ) >= vNewPos_.SquareDistance( itNextMoveStep->vPos_ )  )
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
bool DEC_PathWalker::TryToMoveTo( const DEC_PathResult& path, const MT_Vector2D& vNewPosTmp, MT_Float& rTimeRemaining )
{
    // Deplacement de vNewPos_ vers vNewPosTmp
    if( vNewPosTmp == vNewPos_ )
        return true;

    assert( rCurrentSpeed_ > 0. );
    bool bFirstMove = ( vNewPos_ == (*path.GetResult().begin())->GetPos() );

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
// Modified: JVT 2004-10-20
// -----------------------------------------------------------------------------
int DEC_PathWalker::Move( DEC_PathResult& path )
{
    if( bHasMoved_ ) 
        return eAlreadyMoving;

    DEC_PathResult::E_State nPathState = path.GetState();
    if( nPathState == DEC_Path_ABC::eInvalid || nPathState == DEC_Path_ABC::eImpossible || nPathState == DEC_Path_ABC::eCanceled )
        return eNotAllowed;
    
    if( nPathState == DEC_Path_ABC::eComputing )
    {
        bHasMoved_ = true;
        return eRunning;
    }
    
    if( !SetCurrentPath( path ) )
        return eItineraireMustBeJoined;

    bHasMoved_ = true;
    ComputeCurrentSpeed();
    vNewPos_ = movingEntity_.GetPosition ();
    vNewDir_ = movingEntity_.GetDirection();
    MIL_AgentServer::GetWorkspace().GetEntityManager().GetEffectManager().Register( effectMove_ );

    if( rCurrentSpeed_ == 0. || !movingEntity_.CanMove() )
    {
        rCurrentSpeed_ = 0.;
        return eNotAllowed;
    }

    if( itNextPathPoint_ == path.GetResult().end() )
    {
        rCurrentSpeed_ = 0.;
        return eFinished;
    }

    if( !movingEntity_.HasResources() )
    {
        rCurrentSpeed_ = 0.;
        movingEntity_.SendRC( *MIL_RC::pRcPlusDeCarburant_ );
        return eNotEnoughFuel;
    }

    MT_Float rTimeRemaining = 1.; // 1 dT
    if( (*itNextPathPoint_)->GetPos() != vNewPos_ )
        vNewDir_ = ( (*itNextPathPoint_)->GetPos() - vNewPos_ ).Normalize();

    rWalkedDistance_ = 0;
    while( rTimeRemaining > 0. )
    {
        const MT_Vector2D vPosBeforeMove( vNewPos_ );
        
        if( !TryToMoveTo( path, (*itNextPathPoint_)->GetPos(), rTimeRemaining ) )
        {
            rWalkedDistance_ += vPosBeforeMove.Distance( vNewPos_ );
            return eRunning;
        }

        rWalkedDistance_ += vPosBeforeMove.Distance( vNewPos_ );

        bool bStopOnInterestingPoint = GoToNextNavPoint( path );
        if( bStopOnInterestingPoint )
            return eRunning;

        if( itNextPathPoint_ == path.GetResult().end() )
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
void DEC_PathWalker::MoveSuspended( DEC_PathResult& path )
{
    assert( pCurrentPath_ || bForcePathCheck_ );

    if( pCurrentPath_ && *pCurrentPath_ == path ) 
        bForcePathCheck_ = true;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathWalker::MoveCanceled
// Created: NLD 2005-03-16
// -----------------------------------------------------------------------------
void DEC_PathWalker::MoveCanceled( DEC_PathResult& path )
{
    if( pCurrentPath_ == &path )
    {
        pCurrentPath_->DecRef();
        pCurrentPath_ = 0;
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
void DEC_PathWalker::SerializeEnvironmentType( DIN::DIN_BufferedMessage& dinMsg ) const
{
    dinMsg << environment_.Area();
    dinMsg << environment_.Left();
    dinMsg << environment_.Right();
    dinMsg << environment_.Linear();
}

// -----------------------------------------------------------------------------
// Name: DEC_PathWalker::SerializeCurrentPath
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
bool DEC_PathWalker::SerializeCurrentPath( ASN1T_Itineraire& asn ) const
{
    if( !pCurrentPath_ )
        return false;

    pCurrentPath_->Serialize( asn );
    return true;
}

