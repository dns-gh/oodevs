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

#include "simulation_kernel_pch.h"

#include "PHY_RoleAction_Moving.h"

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
#include "Entities/Agents/Roles/Population/PHY_RolePion_Population.h"
#include "Entities/Agents/Actions/Flying/PHY_RoleAction_InterfaceFlying.h"
#include "Entities/Agents/Units/Dotations/PHY_ConsumptionType.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Objects/MIL_RealObject_ABC.h"
#include "Entities/RC/MIL_RC.h"
#include "Entities/MIL_Army.h"
#include "Decision/Path/DEC_PathPoint.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_Army.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Network/NET_ASN_Messages.h"

BOOST_CLASS_EXPORT_GUID( PHY_RoleAction_Moving, "PHY_RoleAction_Moving" )

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving constructor
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
PHY_RoleAction_Moving::PHY_RoleAction_Moving( MT_RoleContainer& role, MIL_AgentPion& pion )
    : MT_Role_ABC            ( role  )
    , pPion_                 ( &pion )
    , pRoleLocation_         ( &GetRole< PHY_RolePion_Location >() )
    , rSpeedModificator_     ( 1. )
    , rMaxSpeedModificator_  ( 1. )    
    , bCurrentPathHasChanged_( true )
    , bEnvironmentHasChanged_( true )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving constructor
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
PHY_RoleAction_Moving::PHY_RoleAction_Moving()
    : MT_Role_ABC            ()
    , pPion_                 ( 0 )
    , pRoleLocation_         ( 0 )
    , rSpeedModificator_     ( 1. )
    , rMaxSpeedModificator_  ( 1. )
    , bCurrentPathHasChanged_( true )
    , bEnvironmentHasChanged_( true )
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
    rSpeed  = GetRole< PHY_RolePion_Population   >().ModifyMaxSpeed( rSpeed );
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
    sMaxSpeedEnvCalculator( const TerrainData& environment ) 
        : rMaxSpeedEnv_        ( std::numeric_limits< MT_Float >::max() )
        , environment_         ( environment )
        , bHasUsableComposante_( false )
    {
    }

    void operator() ( const PHY_ComposantePion& composante )
    {
        if( composante.CanMove() )
        {
            bHasUsableComposante_ = true;
            rMaxSpeedEnv_ = std::min( rMaxSpeedEnv_, composante.GetMaxSpeed( environment_ ) );
        }
    }

    bool        bHasUsableComposante_;
    MT_Float    rMaxSpeedEnv_;
    TerrainData environment_;
};

MT_Float PHY_RoleAction_Moving::GetMaxSpeed( const TerrainData& environment ) const
{
    sMaxSpeedEnvCalculator functor( environment );
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
MT_Float PHY_RoleAction_Moving::GetSpeedWithReinforcement( const TerrainData& environment ) const
{
    MT_Float rSpeed = GetMaxSpeed( environment );
    const PHY_RolePion_Reinforcement::T_PionSet& reinforcements = GetRole< PHY_RolePion_Reinforcement >().GetReinforcements();
    for( PHY_RolePion_Reinforcement::CIT_PionSet itReinforcement = reinforcements.begin(); itReinforcement != reinforcements.end(); ++itReinforcement )
        rSpeed = std::min( rSpeed, (**itReinforcement).GetRole< PHY_RoleAction_Moving >().GetSpeedWithReinforcement( environment ) );

    rSpeed = std::min( rSpeed, GetMaxSpeedWithReinforcement() );
    return ApplySpeedModificators( rSpeed );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::GetSpeedWithReinforcement
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
inline
MT_Float PHY_RoleAction_Moving::GetSpeedWithReinforcement( const TerrainData& environment, const MIL_Object_ABC& object ) const
{
    if( !object.IsReal() )
        return std::numeric_limits< MT_Float >::max();
        
    const MIL_RealObject_ABC& realObject = static_cast< const MIL_RealObject_ABC& >( object );

    MT_Float rObjectSpeed = GetMaxSpeed( realObject );
    const PHY_RolePion_Reinforcement::T_PionSet& reinforcements = GetRole< PHY_RolePion_Reinforcement >().GetReinforcements();
    for( PHY_RolePion_Reinforcement::CIT_PionSet itReinforcement = reinforcements.begin(); itReinforcement != reinforcements.end(); ++itReinforcement )
        rObjectSpeed = std::min( rObjectSpeed, (**itReinforcement).GetRole< PHY_RoleAction_Moving >().GetSpeedWithReinforcement( environment, object ) );

    const MT_Float rCurrentMaxSpeed = GetMaxSpeed();
    const MT_Float rCurrentEnvSpeed = GetSpeedWithReinforcement( environment );

    rObjectSpeed = std::min( rObjectSpeed, rCurrentMaxSpeed );
    rObjectSpeed = ApplySpeedModificators( rObjectSpeed );
    return realObject.ApplySpeedPolicy( rObjectSpeed, rCurrentEnvSpeed, rCurrentMaxSpeed );
}

// =============================================================================
// PATH OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::ExtrapolatePosition
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
MT_Vector2D PHY_RoleAction_Moving::ExtrapolatePosition( const MT_Float rTime, const bool bBoundOnPath ) const
{
    assert( pPion_ );
    const PHY_RolePion_Location& roleLocation = pPion_->GetRole< PHY_RolePion_Location >();
    return PHY_MovingEntity_ABC::ExtrapolatePosition( roleLocation.GetPosition(), roleLocation.GetCurrentSpeed(), rTime, bBoundOnPath );
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
    assert( pPion_ );

    NET_AS_MOSServerMsgMgr& msgMgr = MIL_AgentServer::GetWorkspace().GetAgentServer().GetMessageMgr();
    DIN::DIN_BufferedMessage dinMsg = msgMgr.BuildMessage();

    dinMsg << ( uint32 )pPion_->GetID();
    SerializeEnvironmentType( dinMsg );
    msgMgr.SendMsgEnvironmentType( dinMsg );    
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::SendCurrentPath
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
void PHY_RoleAction_Moving::SendCurrentPath() const
{  
    NET_ASN_MsgUnitPathFind asnMsg;
    
    assert( pPion_ );
    asnMsg.GetAsnMsg().oid_pion = pPion_->GetID();   
    if( !SerializeCurrentPath( asnMsg.GetAsnMsg().itineraire ) )
        return;

    asnMsg.Send();
    delete [] asnMsg.GetAsnMsg().itineraire.vecteur_point.elem;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::SendFullState
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
void PHY_RoleAction_Moving::SendFullState() const
{
    SendCurrentPath    ();
    SendEnvironmentType();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::SendChangedState
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
void PHY_RoleAction_Moving::SendChangedState() const
{
    if( bCurrentPathHasChanged_  )
        SendCurrentPath();

    if( bEnvironmentHasChanged_ )
        SendEnvironmentType();
}



///////////

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::GetPosition
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
const MT_Vector2D& PHY_RoleAction_Moving::GetPosition() const
{
    assert( pRoleLocation_ );
    return pRoleLocation_->GetPosition();
}
    
// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::GetDirection
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
const MT_Vector2D& PHY_RoleAction_Moving::GetDirection() const
{
    assert( pRoleLocation_ );
    return pRoleLocation_->GetDirection();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::ApplyMove
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
void PHY_RoleAction_Moving::ApplyMove( const MT_Vector2D& position, const MT_Vector2D& direction, MT_Float rSpeed, MT_Float /*rWalkedDistance*/ )
{
    assert( pRoleLocation_ );
    return pRoleLocation_->Move( position, direction, rSpeed );
}


// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::NotifyMovingOnPathPoint
// Created: NLD 2005-10-04
// -----------------------------------------------------------------------------
void PHY_RoleAction_Moving::NotifyMovingOnPathPoint( const DEC_PathPoint& /*point*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::NotifyMovingOnSpecialPoint
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
void PHY_RoleAction_Moving::NotifyMovingOnSpecialPoint( const DEC_PathPoint& point )
{
    point.SendToDIA( GetRole< DEC_RolePion_Decision >() );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::NotifyMovingInsideObject
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
void PHY_RoleAction_Moving::NotifyMovingInsideObject( MIL_Object_ABC& object )
{
    assert( pRoleLocation_ );
    pRoleLocation_->NotifyMovingInsideObject( object );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::NotifyMovingOutsideObject
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
void PHY_RoleAction_Moving::NotifyMovingOutsideObject( MIL_Object_ABC& object )
{
    assert( pRoleLocation_ );
    pRoleLocation_->NotifyMovingOutsideObject( object );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::CanMove
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
bool PHY_RoleAction_Moving::CanMove() const
{
    if( GetRole< PHY_RolePion_Surrender >().IsSurrendered() )
        return true;

    return      GetRole< PHY_RoleAction_InterfaceFlying >().CanMove() 
            && !GetRole< PHY_RolePion_Transported       >().IsTransported()
            && !GetRole< PHY_RolePion_Reinforcement     >().IsReinforcing();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::CanObjectInteractWith
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
bool PHY_RoleAction_Moving::CanObjectInteractWith( const MIL_Object_ABC& object ) const
{
    return object.CanInteractWith( *pPion_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::HasResources
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
bool PHY_RoleAction_Moving::HasResources()
{
    if( GetRole< PHY_RolePion_Surrender >().IsSurrendered() )
        return true;
    
    // Reservation des consommations du pion + renforts
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

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::SendRC
// Created: NLD 2005-10-05
// -----------------------------------------------------------------------------
void PHY_RoleAction_Moving::SendRC( const MIL_RC& rc ) const
{
    assert( pPion_ );
    rc.Send( *pPion_, MIL_RC::eRcTypeWarning );
}
