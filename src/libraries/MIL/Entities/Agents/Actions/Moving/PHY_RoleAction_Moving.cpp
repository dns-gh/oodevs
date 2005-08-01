// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Moving/PHY_RoleAction_Moving.cpp $
// $Author: Age $
// $Modtime: 15/06/05 18:46 $
// $Revision: 28 $
// $Workfile: PHY_RoleAction_Moving.cpp $
//
// *****************************************************************************

#include "MIL_Pch.h"

#include "PHY_RoleAction_Moving.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/Effects/MIL_EffectManager.h"
#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"
#include "Entities/Agents/Roles/Reinforcement/PHY_RolePion_Reinforcement.h"
#include "Entities/Agents/Roles/Dotations/PHY_RolePion_Dotations.h"
#include "Entities/Agents/Roles/Transported/PHY_RolePion_Transported.h"
#include "Entities/Agents/Roles/NBC/PHY_RolePion_NBC.h"
#include "Entities/Agents/Roles/HumanFactors/PHY_RolePion_HumanFactors.h"
#include "Entities/Agents/Roles/Communications/PHY_RolePion_Communications.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/Roles/Surrender/PHY_RolePion_Surrender.h"
#include "Entities/Agents/Actions/Flying/PHY_RoleAction_InterfaceFlying.h"
#include "Entities/Agents/Units/Dotations/PHY_ConsumptionType.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Objects/MIL_RealObject_ABC.h"
#include "Entities/RC/MIL_RC.h"
#include "Decision/Path/DEC_PathPoint.h"
#include "Knowledge/DEC_KS_AgentQuerier.h"
#include "Network/NET_ASN_Messages.h"
#include "MIL_AgentServer.h"
#include "TER/TER_DynaObject_ABC.h"
#include "TER/TER_World.h"

BOOST_CLASS_EXPORT_GUID( PHY_RoleAction_Moving, "PHY_RoleAction_Moving" )

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving constructor
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
PHY_RoleAction_Moving::PHY_RoleAction_Moving( MT_RoleContainer& role, MIL_AgentPion& pion )
    : MT_Role_ABC          ( role  )
    , pPion_               ( &pion )
    , pRoleLocation_       ( &GetRole< PHY_RolePion_Location >() )
    , rSpeedModificator_   ( 1. )
    , rMaxSpeedModificator_( 1. )
    , itNextPathPoint_     ()
    , itCurrentPathPoint_  ()
    , effectMove_          ( *this )
    , vNewPos_             ( 0., 0. )
    , vNewDir_             ( 0., 0. )
    , rCurrentSpeed_       ( 0. )
    , bForcePathCheck_     ( true )
    , bHasChanged_         ( true )
    , bHasMoved_           ( false )  
    , bEnvironmentHasChanged_( true )
    , pCurrentPath_        ( 0 )
    , rCurrentMaxSpeed_    ( 0. ) // Cached data
    , rCurrentEnvSpeed_    ( 0. ) // Cached data
{
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving constructor
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
PHY_RoleAction_Moving::PHY_RoleAction_Moving()
    : MT_Role_ABC          ()
    , pPion_               ( 0 )
    , pRoleLocation_       ( 0 )
    , effectMove_          ( *this )
    , rSpeedModificator_   ( 1. )
    , rMaxSpeedModificator_( 1. )
    , itNextPathPoint_     ()
    , itCurrentPathPoint_  ()
    , vNewPos_             ( 0., 0. )
    , vNewDir_             ( 0., 0. )
    , rCurrentSpeed_       ( 0. )
    , bForcePathCheck_     ( true )
    , bHasChanged_         ( true )
    , bHasMoved_           ( false )  
    , bEnvironmentHasChanged_( true )
    , pCurrentPath_        ( 0 )
    , rCurrentMaxSpeed_    ( 0. ) // Cached data
    , rCurrentEnvSpeed_    ( 0. ) // Cached data
{
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving destructor
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
PHY_RoleAction_Moving::~PHY_RoleAction_Moving()
{

}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::serialize
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_RoleAction_Moving::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< MT_Role_ABC >( *this )
         & pPion_
         & pRoleLocation_;
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::CanMove
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
inline
bool PHY_RoleAction_Moving::CanMove() const
{
    if( GetRole< PHY_RolePion_Surrender >().IsSurrendered() )
        return true;

    return      GetRole< PHY_RoleAction_InterfaceFlying >().CanMove() 
            && !GetRole< PHY_RolePion_Transported       >().IsTransported()
            && !GetRole< PHY_RolePion_Reinforcement     >().IsReinforcing()
            &&  GetRole< PHY_RolePion_Composantes       >().CanMove();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::InitializeEnvironment
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
inline
void PHY_RoleAction_Moving::InitializeEnvironment( const DEC_Path& path )
{
    DEC_Path::CIT_PathPointList itPathPointTmp = itCurrentPathPoint_;
    while( itPathPointTmp != path.GetResult().end() && (*itPathPointTmp)->GetType() != DEC_PathPoint::eTypePointPath )
        ++itPathPointTmp;
    assert( itPathPointTmp != path.GetResult().end() );

    TerrainData tmpEnvironment = (*itPathPointTmp)->GetObjectTypesToNextPoint();
    if( !( environment_ == tmpEnvironment ) ) //$$$
    {
        bEnvironmentHasChanged_ = true;
        environment_            = tmpEnvironment;
    }    
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::ReserveConsumptionWithReinforcement
// Created: NLD 2004-09-30
// -----------------------------------------------------------------------------
inline
bool PHY_RoleAction_Moving::ReserveConsumptionWithReinforcement()
{
    uint nNbrPionToRollback = 1;
    if( !GetRole< PHY_RolePion_Dotations >().SetConsumptionMode( PHY_ConsumptionType::moving_ ) )  
        return false;

    const PHY_RolePion_Reinforcement::T_PionSet& reinforcements = GetRole< PHY_RolePion_Reinforcement >().GetReinforcements();
    PHY_RolePion_Reinforcement::CIT_PionSet itReinforcement;
    for( itReinforcement = reinforcements.begin(); itReinforcement != reinforcements.end(); ++itReinforcement )
    {
        MIL_AgentPion& reinforcement = **itReinforcement;
        ++nNbrPionToRollback;
        if( !reinforcement.GetRole< PHY_RolePion_Dotations >().SetConsumptionMode( PHY_ConsumptionType::moving_ ) )
            break;            
    }

    if( itReinforcement == reinforcements.end() )
        return true;
    
    // Rollback
    GetRole< PHY_RolePion_Dotations >().RollbackConsumptionMode();
    itReinforcement = reinforcements.begin();
    while( --nNbrPionToRollback )
        (**itReinforcement++).GetRole< PHY_RolePion_Dotations >().RollbackConsumptionMode();
    return false;
}

// =============================================================================
// SPEED CALCULATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::ApplyMaxSpeedModificators
// Created: NLD 2004-11-04
// -----------------------------------------------------------------------------
inline
MT_Float PHY_RoleAction_Moving::ApplyMaxSpeedModificators( MT_Float rSpeed ) const
{
    rSpeed  = GetRole< PHY_RolePion_NBC          >().ModifyMaxSpeed( rSpeed );
    rSpeed  = GetRole< PHY_RolePion_HumanFactors >().ModifyMaxSpeed( rSpeed );
    rSpeed *= rMaxSpeedModificator_;
    return rSpeed;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::ApplySpeedModificators
// Created: NLD 2004-11-04
// -----------------------------------------------------------------------------
inline
MT_Float PHY_RoleAction_Moving::ApplySpeedModificators( MT_Float rSpeed ) const
{
    rSpeed = GetRole< PHY_RolePion_Communications >().ModifySpeed( rSpeed );
    rSpeed *= rSpeedModificator_;
    return rSpeed;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::GetMaxSpeed
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
struct sMaxSpeedObjectCalculator
{
    MT_COPYNOTALLOWED( sMaxSpeedObjectCalculator );

public:
    sMaxSpeedObjectCalculator( const MIL_RealObject_ABC& object ) 
        : rMaxSpeedObject_     ( std::numeric_limits< MT_Float >::max() )
        , object_              ( object )
        , bHasUsableComposante_( false )
    {
    }

    void operator() ( const PHY_ComposantePion& composante )
    {
        if( composante.CanMove() )
        {
            bHasUsableComposante_ = true;
            rMaxSpeedObject_ = std::min( rMaxSpeedObject_, composante.GetMaxSpeed( object_ ) );
        }
    }
          bool                bHasUsableComposante_;
          MT_Float            rMaxSpeedObject_;
    const MIL_RealObject_ABC& object_;
};

MT_Float PHY_RoleAction_Moving::GetMaxSpeed( const MIL_RealObject_ABC& object ) const
{
    sMaxSpeedObjectCalculator functor( object );
    GetRole< PHY_RolePion_Composantes >().Apply( functor );

    if( !functor.bHasUsableComposante_ )
        return 0.;
    return functor.rMaxSpeedObject_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::GetMaxSpeed
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
struct sMaxSpeedEnvCalculator
{
public:
    sMaxSpeedEnvCalculator( const TerrainData& nEnv ) 
        : rMaxSpeedEnv_        ( std::numeric_limits< MT_Float >::max() )
        , nEnv_                ( nEnv )
        , bHasUsableComposante_( false )
    {
    }

    void operator() ( const PHY_ComposantePion& composante )
    {
        if( composante.CanMove() )
        {
            bHasUsableComposante_ = true;
            rMaxSpeedEnv_ = std::min( rMaxSpeedEnv_, composante.GetMaxSpeed( nEnv_ ) );
        }
    }

    bool        bHasUsableComposante_;
    MT_Float    rMaxSpeedEnv_;
    TerrainData nEnv_;
};

MT_Float PHY_RoleAction_Moving::GetMaxSpeed( const TerrainData& nEnv ) const
{
    sMaxSpeedEnvCalculator functor( nEnv );
    GetRole< PHY_RolePion_Composantes >().Apply( functor );

    if( !functor.bHasUsableComposante_  )
        return 0.;
    assert( functor.rMaxSpeedEnv_ != std::numeric_limits< MT_Float >::max() );
    return functor.rMaxSpeedEnv_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::GetMaxSpeed
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
struct sMaxSpeedCalculator
{
public:
    sMaxSpeedCalculator() 
        : rMaxSpeed_           ( std::numeric_limits< MT_Float >::max() )
        , bHasUsableComposante_( false )
    {
    }

    void operator() ( const PHY_ComposantePion& composante )
    {
        if( composante.CanMove() )
        {
            bHasUsableComposante_ = true;
            rMaxSpeed_ = std::min( rMaxSpeed_, composante.GetMaxSpeed() );
        }
    }

    bool     bHasUsableComposante_;
    MT_Float rMaxSpeed_;
};

MT_Float PHY_RoleAction_Moving::GetMaxSpeed() const
{
    sMaxSpeedCalculator functor;
    GetRole< PHY_RolePion_Composantes >().Apply( functor );

    if( !functor.bHasUsableComposante_  )
        return 0.;
    assert( functor.rMaxSpeed_ != std::numeric_limits< MT_Float >::max() );
    return functor.rMaxSpeed_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::GetMaxSlope
// Created: AGE 2005-04-13
// -----------------------------------------------------------------------------
namespace
{
    struct sMaxSlopeCalculator
    {
        sMaxSlopeCalculator() : rMaxSlope_( std::numeric_limits< MT_Float >::max() ), bHasUsableComposante_( false ) {}
        void operator() ( const PHY_ComposantePion& composante )
        {
            if( composante.CanMove() )
            {
                bHasUsableComposante_ = true;
                rMaxSlope_ = std::min( rMaxSlope_, composante.GetType().GetMaxSlope() );
            }
        }
        MT_Float MaxSlope() const { return bHasUsableComposante_ ? rMaxSlope_ : 0; };
    private:
        bool bHasUsableComposante_;
        MT_Float rMaxSlope_;
    };
};

MT_Float PHY_RoleAction_Moving::GetMaxSlope() const
{
    sMaxSlopeCalculator calculator;
    GetRole< PHY_RolePion_Composantes >().Apply( calculator );
    return calculator.MaxSlope();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::GetMaxSpeedWithReinforcement
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
inline
MT_Float PHY_RoleAction_Moving::GetMaxSpeedWithReinforcement() const
{
    MT_Float rSpeed = GetMaxSpeed();
    const PHY_RolePion_Reinforcement::T_PionSet& reinforcements = GetRole< PHY_RolePion_Reinforcement >().GetReinforcements();
    for( PHY_RolePion_Reinforcement::CIT_PionSet itReinforcement = reinforcements.begin(); itReinforcement != reinforcements.end(); ++itReinforcement )
        rSpeed = std::min( rSpeed, (**itReinforcement).GetRole< PHY_RoleAction_Moving >().GetMaxSpeedWithReinforcement() );
    return ApplyMaxSpeedModificators( rSpeed );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::GetSpeedWithReinforcement
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
inline
MT_Float PHY_RoleAction_Moving::GetSpeedWithReinforcement( const TerrainData& nEnv ) const
{
    MT_Float rSpeed = GetMaxSpeed( nEnv );
    const PHY_RolePion_Reinforcement::T_PionSet& reinforcements = GetRole< PHY_RolePion_Reinforcement >().GetReinforcements();
    for( PHY_RolePion_Reinforcement::CIT_PionSet itReinforcement = reinforcements.begin(); itReinforcement != reinforcements.end(); ++itReinforcement )
        rSpeed = std::min( rSpeed, (**itReinforcement).GetRole< PHY_RoleAction_Moving >().GetSpeedWithReinforcement( nEnv ) );

    rSpeed = std::min( rSpeed, GetMaxSpeedWithReinforcement() );
    return ApplySpeedModificators( rSpeed );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::GetSpeedWithReinforcement
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
inline
MT_Float PHY_RoleAction_Moving::GetSpeedWithReinforcement( const MIL_Object_ABC& object ) const
{
    if( !object.IsReal() )
        return std::numeric_limits< MT_Float >::max();
        
    const MIL_RealObject_ABC& realObject = static_cast< const MIL_RealObject_ABC& >( object );

    MT_Float rObjectSpeed = GetMaxSpeed( realObject );
    const PHY_RolePion_Reinforcement::T_PionSet& reinforcements = GetRole< PHY_RolePion_Reinforcement >().GetReinforcements();
    for( PHY_RolePion_Reinforcement::CIT_PionSet itReinforcement = reinforcements.begin(); itReinforcement != reinforcements.end(); ++itReinforcement )
        rObjectSpeed = std::min( rObjectSpeed, (**itReinforcement).GetRole< PHY_RoleAction_Moving >().GetSpeedWithReinforcement( object ) );

    rObjectSpeed = std::min( rObjectSpeed, rCurrentMaxSpeed_ );
    rObjectSpeed = ApplySpeedModificators( rObjectSpeed );
    return realObject.ApplySpeedPolicy( rObjectSpeed, rCurrentEnvSpeed_, rCurrentMaxSpeed_ );
}


// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::ComputeCurrentSpeed
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
inline
void PHY_RoleAction_Moving::ComputeCurrentSpeed()
{
    const DEC_PathPoint& curPathPoint = **itCurrentPathPoint_;
    if( curPathPoint.GetType() == DEC_PathPoint::eTypePointPath )
    {
        TerrainData tmpEnvironment = curPathPoint.GetObjectTypesToNextPoint();       
        if( !( environment_ == tmpEnvironment ) ) //$$$
        {
            bEnvironmentHasChanged_ = true;
            environment_            = tmpEnvironment;
        }    
    }

    rCurrentMaxSpeed_ = GetMaxSpeedWithReinforcement();
    rCurrentEnvSpeed_ = GetSpeedWithReinforcement( environment_ );    
    rCurrentSpeed_    = rCurrentEnvSpeed_;
}

//-----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::GoToNextNavPoint
// Created: JVT 02-12-06
// Last modified: JVT 03-02-04
//-----------------------------------------------------------------------------
inline
bool PHY_RoleAction_Moving::GoToNextNavPoint( const DEC_Path& path )
{
    if ( (*itNextPathPoint_)->GetType() == DEC_PathPoint::eTypePointPath )
    {
        itCurrentPathPoint_ = itNextPathPoint_;
        ++itNextPathPoint_;
        return false;
    }

    // on a rencontré des points particuliers -> EVT vers DEC
    DEC_RolePion_Decision& roleDecision = GetRole< DEC_RolePion_Decision >();
    do 
    {
        ( *( itCurrentPathPoint_ = itNextPathPoint_ ) )->SendToDIA( roleDecision );
    }
    while      ( ++itNextPathPoint_ != path.GetResult().end()
            && ( *itNextPathPoint_ )->GetType() != DEC_PathPoint::eTypePointPath
            && ( *itNextPathPoint_ )->GetPos() == vNewPos_ );
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::SetCurrentPath
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
bool PHY_RoleAction_Moving::SetCurrentPath( DEC_Path& path )
{
    if( pCurrentPath_ && path == *pCurrentPath_ && !bForcePathCheck_  /*&& !GetRole< PHY_RolePion_Location >().HasDoneMagicMove()*/ )
        return true;

    if( pCurrentPath_ )
        pCurrentPath_->DecRef();
    pCurrentPath_ = &path;
    pCurrentPath_->IncRef();

    path.InsertDecPoints(); // $$$ HIDEUX
    bHasChanged_     = true;
    bForcePathCheck_ = false;

    assert( !path.GetResult().empty() );
    itCurrentPathPoint_ = path.GetCurrentKeyOnPath( GetRole< PHY_RolePion_Location >().GetPosition() );
    if( itCurrentPathPoint_ == path.GetResult().end() )
        return false;

    if( pCurrentPath_->GetState() == DEC_Path::ePartial )
        MIL_RC::pRcTerrainDifficile_->Send( *pPion_, MIL_RC::eRcTypeWarning );

    itNextPathPoint_ = itCurrentPathPoint_;   
    ++itNextPathPoint_;
    InitializeEnvironment( path );
    return true;
}

// =============================================================================
// COMPUTE
// =============================================================================

//-----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::ComputeObjectsCollision
// Created: NLD 2002-12-17
//-----------------------------------------------------------------------------
void PHY_RoleAction_Moving::ComputeObjectsCollision( const MT_Vector2D& vStart, const MT_Vector2D& vEnd, T_MoveStepSet& moveStepSet )
{
    assert( pPion_ );
    
    // Récupération de la liste des objets dynamiques contenus dans le rayon vEnd - vStart
    TER_DynaObject_ABC::T_DynaObjectVector dynaObjectSet;
    TER_World::GetWorld().GetListDynaObjectWithinCircle( vNewPos_, (vEnd - vStart).Magnitude(), dynaObjectSet ); 

    moveStepSet.clear();

    MT_Line lineTmp( vStart, vEnd );

    moveStepSet.insert( T_MoveStep( vStart ) );
    moveStepSet.insert( T_MoveStep( vEnd   ) );

    TER_DistanceLess colCmp( vStart );
    T_PointSet collisions( colCmp );

    for( TER_DynaObject_ABC::CIT_DynaObjectVector itObject = dynaObjectSet.begin(); itObject != dynaObjectSet.end(); ++itObject )
    {
        MIL_Object_ABC& object = static_cast< MIL_Object_ABC& >( **itObject );

        if( !object.CanCollideWithAgent( *pPion_ ) )
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
// Name: PHY_RoleAction_Moving::TryToMoveToNextStep
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
bool PHY_RoleAction_Moving::TryToMoveToNextStep( CIT_MoveStepSet itCurMoveStep, CIT_MoveStepSet itNextMoveStep, MT_Float& rTimeRemaining, bool bFirstMove )
{
    assert( pRoleLocation_ );
    
    CIT_ObjectSet itObject;

    // Prise en compte des objets ponctuels se trouvant sur le 'move step'
    for( itObject = itCurMoveStep->ponctualObjectsOnSet_.begin(); itObject != itCurMoveStep->ponctualObjectsOnSet_.end(); ++itObject )
    {
		MIL_Object_ABC& object = **itObject;

        MT_Float rSpeedWithinObject = GetSpeedWithReinforcement( object );
   
        if( !bFirstMove ) //// $$$$$ !bFirstMove A REVOIR - PERMET DE SORTIR D'UN OBSTACLE PONCTUEL
        {
            pRoleLocation_->NotifyMovingInsideObject( object );
            
            if( rSpeedWithinObject == 0. ) 
            {
                rSpeedWithinObject = 0;
                vNewPos_           = itCurMoveStep->vPos_;
                return false;
            }
        }
        pRoleLocation_->NotifyMovingOutsideObject( object );
    }
    
    MT_Float rMaxSpeedForStep = std::numeric_limits< MT_Float >::max();
    for( itObject = itCurMoveStep->objectsToNextPointSet_.begin(); itObject != itCurMoveStep->objectsToNextPointSet_.end(); ++itObject )
    {
        MIL_Object_ABC& object = **itObject;
        pRoleLocation_->NotifyMovingInsideObject( object );

        rMaxSpeedForStep = std::min( rMaxSpeedForStep, GetSpeedWithReinforcement( object ) );        
        if( rMaxSpeedForStep == 0. )
        {
            rCurrentSpeed_ = 0;
            vNewPos_       = itCurMoveStep->vPos_;
            return false;
        }    
    }

    // itCurMoveStep a pu être dépassé => notification des objets dont on sort
    for( itObject = itNextMoveStep->objectsOutSet_.begin(); itObject != itNextMoveStep->objectsOutSet_.end(); ++itObject )
        pRoleLocation_->NotifyMovingOutsideObject( **itObject );

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
// Name: PHY_RoleAction_Moving::TryToMoveTo
// Created: NLD 2002-12-17
//-----------------------------------------------------------------------------
bool PHY_RoleAction_Moving::TryToMoveTo( const DEC_Path& path, const MT_Vector2D& vNewPosTmp, MT_Float& rTimeRemaining )
{
    assert( rCurrentSpeed_ > 0. );

    // Deplacement de vNewPos_ vers vNewPosTmp
    if( vNewPosTmp == vNewPos_ )
        return true;

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
// Name: PHY_RoleAction_Moving::Move
// Created: NLD 2004-09-22
// Modified: JVT 2004-10-20
// -----------------------------------------------------------------------------
int PHY_RoleAction_Moving::Move( DEC_Path& path )
{
    if( bHasMoved_ ) 
        return eAlreadyMoving;

    DEC_Path::E_State nPathState = path.GetState();
    if( nPathState == DEC_Path::eInvalid || nPathState == DEC_Path::eImpossible || nPathState == DEC_Path::eCanceled )
        return eNotAllowed;
    
    if( nPathState == DEC_Path::eComputing )
    {
        bHasMoved_ = true;
        return eRunning;
    }
    
    if( !SetCurrentPath( path ) )
        return eItineraireMustBeJoined;

    assert( pRoleLocation_ );

    bHasMoved_ = true;
    ComputeCurrentSpeed();
    vNewPos_ = pRoleLocation_->GetPosition ();
    vNewDir_ = pRoleLocation_->GetDirection();
    MIL_AgentServer::GetWorkspace().GetEntityManager().GetEffectManager().Register( effectMove_ );

    if( rCurrentSpeed_ == 0. || !CanMove() )
    {
        rCurrentSpeed_ = 0.;
        return eNotAllowed;
    }

    if( itNextPathPoint_ == path.GetResult().end() )
    {
        rCurrentSpeed_ = 0.;
        return eFinished;
    }

    if( !ReserveConsumptionWithReinforcement() && !GetRole< PHY_RolePion_Surrender >().IsSurrendered() )
    {
        rCurrentSpeed_ = 0.;
        MIL_RC::pRcPlusDeCarburant_->Send( *pPion_, MIL_RC::eRcTypeWarning );
        return eNotEnoughFuel;
    }

    MT_Float rTimeRemaining = 1.; // 1 dT
    if( (*itNextPathPoint_)->GetPos() != vNewPos_ )
        vNewDir_ = ( (*itNextPathPoint_)->GetPos() - vNewPos_ ).Normalize();

    while( rTimeRemaining > 0. )
    {
        if( !TryToMoveTo( path, (*itNextPathPoint_)->GetPos(), rTimeRemaining ) )
            return eRunning;

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
// Name: PHY_RoleAction_Moving::MoveSuspended
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
void PHY_RoleAction_Moving::MoveSuspended( DEC_Path& path )
{
    assert( pCurrentPath_ || bForcePathCheck_ );

    if( pCurrentPath_ && *pCurrentPath_ == path ) 
        bForcePathCheck_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::MoveCanceled
// Created: NLD 2005-03-16
// -----------------------------------------------------------------------------
void PHY_RoleAction_Moving::MoveCanceled( DEC_Path& path )
{
    if( pCurrentPath_ == &path )
    {
        pCurrentPath_->DecRef();
        pCurrentPath_ = 0;
        bForcePathCheck_ = true;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::Apply
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
void PHY_RoleAction_Moving::Apply()
{
    if( CanMove() )
    {
        assert( pRoleLocation_ );
        pRoleLocation_->Move( vNewPos_, vNewDir_, rCurrentSpeed_ );
    }
}

// =============================================================================
// PATH OPERATIONS
// =============================================================================

//-----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::ExtrapolatePosition
// Created: JVT 03-09-25
//-----------------------------------------------------------------------------
MT_Vector2D PHY_RoleAction_Moving::ExtrapolatePosition( MT_Float rTime, bool bBoundOnPath ) const
{
    assert( pRoleLocation_ );
    
    if ( !pCurrentPath_ )
        return pRoleLocation_->GetPosition();

    return pCurrentPath_->GetFuturePosition( pRoleLocation_->GetPosition(), rTime * pRoleLocation_->GetCurrentSpeed(), bBoundOnPath );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::ComputeFutureObjectCollisions
// Created: NLD 2004-10-18
// -----------------------------------------------------------------------------
void PHY_RoleAction_Moving::ComputeFutureObjectCollisions( const MIL_RealObjectTypeFilter& objectsToAvoid_, DEC_Path::T_KnowledgeObjectMultimap& objectsOnPath ) const
{
    objectsOnPath.clear();
    if( !pCurrentPath_ )
        return;

    assert( pPion_ );
    assert( pRoleLocation_ );

    T_KnowledgeObjectVector knowledges;
    pPion_->GetKSQuerier().GetObjects( knowledges, objectsToAvoid_ );
    pCurrentPath_->ComputeFutureObjectCollisions( pRoleLocation_->GetPosition(), knowledges, objectsOnPath );
}   

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::IsMovingOn
// Created: JVT 2004-11-30
// -----------------------------------------------------------------------------
bool PHY_RoleAction_Moving::IsMovingOn( const DEC_Path& path ) const
{
    return pCurrentPath_ ? path == *pCurrentPath_ : false;
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::SendEnvironmentType
// Created: SBO 2005-06-15
// -----------------------------------------------------------------------------
void PHY_RoleAction_Moving::SendEnvironmentType() const
{
    if( !bEnvironmentHasChanged_ )
        return;

    assert( pPion_ );

    NET_AS_MOSServerMsgMgr& msgMgr = MIL_AgentServer::GetWorkspace().GetAgentServer().GetMessageMgr();
    DIN::DIN_BufferedMessage dinMsg = msgMgr.BuildMessage();
    
    dinMsg << ( uint32 )pPion_->GetID();
    dinMsg << environment_.Area();
    dinMsg << environment_.Left();
    dinMsg << environment_.Right();
    dinMsg << environment_.Linear();
    msgMgr.SendMsgEnvironmentType( dinMsg );    
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::SendFullState
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
void PHY_RoleAction_Moving::SendFullState()
{
    if( !pCurrentPath_ )
        return;
    
    NET_ASN_MsgUnitPathFind asnMsg;
    
    assert( pPion_ );
    asnMsg.GetAsnMsg().oid_pion = pPion_->GetID();
    asnMsg.GetAsnMsg().order_id = pPion_->GetOrderManager().GetCurrentOrderID();
    pCurrentPath_->Serialize( asnMsg.GetAsnMsg().itineraire );
    asnMsg.Send();
    delete [] asnMsg.GetAsnMsg().itineraire.vecteur_point.elem;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::SendChangedState
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
void PHY_RoleAction_Moving::SendChangedState()
{
    if( HasChanged() )
        SendFullState();

    SendEnvironmentType();
}