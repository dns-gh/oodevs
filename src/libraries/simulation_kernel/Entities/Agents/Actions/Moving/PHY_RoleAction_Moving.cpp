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

#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Reinforcement/PHY_RoleInterface_Reinforcement.h"
#include "Entities/Agents/Roles/Dotations/PHY_RoleInterface_Dotations.h"
#include "Entities/Agents/Roles/Transported/PHY_RoleInterface_Transported.h"
#include "Entities/Agents/Roles/NBC/PHY_RoleInterface_NBC.h"
#include "Entities/Agents/Roles/HumanFactors/PHY_RoleInterface_HumanFactors.h"
#include "Entities/Agents/Roles/Logistic/Supply/PHY_RoleInterface_Supply.h"
#include "Entities/Agents/Roles/Communications/PHY_RoleInterface_Communications.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/Roles/Surrender/PHY_RoleInterface_Surrender.h"
#include "Entities/Agents/Roles/Population/PHY_RoleInterface_Population.h"
#include "Entities/Agents/Actions/Flying/PHY_RoleAction_InterfaceFlying.h"
#include "Entities/Agents/Units/Dotations/PHY_ConsumptionType.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/MIL_AgentPion.h"

#include "Entities/Objects/MIL_ObjectManipulator_ABC.h"
#include "Entities/Objects/MIL_Object_ABC.h"

#include "Entities/Orders/MIL_Report.h"
#include "Entities/MIL_Army.h"
#include "Decision/DEC_Representations.h"
#include "Decision/Path/DEC_PathPoint.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_Army.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Network/NET_ASN_Messages.h"

#include "simulation_kernel/PostureComputer_ABC.h"

BOOST_CLASS_EXPORT_GUID( PHY_RoleAction_Moving, "PHY_RoleAction_Moving" )

template< typename Archive >
void save_construct_data( Archive& archive, const PHY_RoleAction_Moving* role, const unsigned int /*version*/ )
{
    MIL_AgentPion* const pion = &role->pion_;
    archive << pion;
}

template< typename Archive >
void load_construct_data( Archive& archive, PHY_RoleAction_Moving* role, const unsigned int /*version*/ )
{
    MIL_AgentPion* pion;
    archive >> pion;
    ::new( role )PHY_RoleAction_Moving( *pion );
}
// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving constructor
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
PHY_RoleAction_Moving::PHY_RoleAction_Moving( MIL_AgentPion& pion )
    : pion_                 ( pion )
    , pRoleLocation_         ( &pion_.GetRole< PHY_RoleInterface_Location >() )
    , rSpeed_                ( 0.)
    , rSpeedModificator_     ( 1. )
    , rMaxSpeedModificator_  ( 1. )    
    , bCurrentPathHasChanged_( true )
    , bEnvironmentHasChanged_( true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving destructor
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
PHY_RoleAction_Moving::~PHY_RoleAction_Moving()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::serialize
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_RoleAction_Moving::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< tools::Role_ABC >( *this )
		 & pion_
         & pRoleLocation_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::ApplyMaxSpeedModificators
// Created: NLD 2004-11-04
// -----------------------------------------------------------------------------
MT_Float PHY_RoleAction_Moving::ApplyMaxSpeedModificators( MT_Float rSpeed ) const
{

    rSpeed  = pion_.GetRole< nbc::PHY_RoleInterface_NBC     >().ModifyMaxSpeed( rSpeed );
    rSpeed  = pion_.GetRole< PHY_RoleInterface_HumanFactors >().ModifyMaxSpeed( rSpeed );
    rSpeed  = pion_.GetRole< PHY_RoleInterface_Population   >().ModifyMaxSpeed( rSpeed );
    rSpeed *= rMaxSpeedModificator_;
    return rSpeed;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::ApplySpeedModificators
// Created: NLD 2004-11-04
// -----------------------------------------------------------------------------
MT_Float PHY_RoleAction_Moving::ApplySpeedModificators( MT_Float rSpeed ) const
{

    rSpeed = pion_.GetRole< PHY_RoleInterface_Communications >().ModifySpeed( rSpeed );
    PHY_RoleInterface_Supply* role = pion_.RetrieveRole< PHY_RoleInterface_Supply >();
    if( role )
        rSpeed = role->ModifySpeed( rSpeed );
    rSpeed *= rSpeedModificator_;
    return rSpeed;
}

namespace 
{
    struct sMaxSpeedObjectCalculator : private boost::noncopyable
    {
    public:
        sMaxSpeedObjectCalculator( const MIL_Object_ABC& object ) 
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
              bool              bHasUsableComposante_;
              MT_Float          rMaxSpeedObject_;
        const MIL_Object_ABC&   object_;
    };
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::GetMaxSpeed
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
MT_Float PHY_RoleAction_Moving::GetMaxSpeed( const MIL_Object_ABC& object ) const
{

    sMaxSpeedObjectCalculator functor( object );
    pion_.GetRole< PHY_RolePion_Composantes >().Apply( functor );

    if( !functor.bHasUsableComposante_ )
        return 0.;
    return functor.rMaxSpeedObject_;
}

namespace
{
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
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::GetMaxSpeed
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
MT_Float PHY_RoleAction_Moving::GetMaxSpeed( const TerrainData& environment ) const
{

    sMaxSpeedEnvCalculator functor( environment );
    pion_.GetRole< PHY_RolePion_Composantes >().Apply( functor );

    if( !functor.bHasUsableComposante_  )
        return 0.;
    assert( functor.rMaxSpeedEnv_ != std::numeric_limits< MT_Float >::max() );
    return functor.rMaxSpeedEnv_;
}

namespace 
{
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
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::GetMaxSpeed
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
MT_Float PHY_RoleAction_Moving::GetMaxSpeed() const
{

    sMaxSpeedCalculator functor;
    pion_.GetRole< PHY_RolePion_Composantes >().Apply( functor );

    if( !functor.bHasUsableComposante_  )
        return 0.;
    assert( functor.rMaxSpeed_ != std::numeric_limits< MT_Float >::max() );
    return functor.rMaxSpeed_;
}

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

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::GetMaxSlope
// Created: AGE 2005-04-13
// -----------------------------------------------------------------------------
MT_Float PHY_RoleAction_Moving::GetMaxSlope() const
{

    sMaxSlopeCalculator calculator;
    pion_.GetRole< PHY_RolePion_Composantes >().Apply( calculator );
    return calculator.MaxSlope();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::GetMaxSpeedWithReinforcement
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
MT_Float PHY_RoleAction_Moving::GetMaxSpeedWithReinforcement() const
{

    MT_Float rSpeed = GetMaxSpeed();
    const PHY_RoleInterface_Reinforcement::T_PionSet& reinforcements = pion_.GetRole< PHY_RoleInterface_Reinforcement >().GetReinforcements();
    for( PHY_RoleInterface_Reinforcement::CIT_PionSet itReinforcement = reinforcements.begin(); itReinforcement != reinforcements.end(); ++itReinforcement )
        rSpeed = std::min( rSpeed, (**itReinforcement).GetRole< PHY_RoleAction_Moving >().GetMaxSpeedWithReinforcement() );
    return ApplyMaxSpeedModificators( rSpeed );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::GetSpeedWithReinforcement
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
MT_Float PHY_RoleAction_Moving::GetSpeedWithReinforcement( const TerrainData& environment ) const
{

    MT_Float rSpeed = GetMaxSpeed( environment );
    const PHY_RoleInterface_Reinforcement::T_PionSet& reinforcements = pion_.GetRole< PHY_RoleInterface_Reinforcement >().GetReinforcements();
    for( PHY_RoleInterface_Reinforcement::CIT_PionSet itReinforcement = reinforcements.begin(); itReinforcement != reinforcements.end(); ++itReinforcement )
        rSpeed = std::min( rSpeed, (**itReinforcement).GetRole< PHY_RoleAction_Moving >().GetSpeedWithReinforcement( environment ) );

    rSpeed = std::min( rSpeed, GetMaxSpeedWithReinforcement() );
    return ApplySpeedModificators( rSpeed );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::GetSpeedWithReinforcement
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
MT_Float PHY_RoleAction_Moving::GetSpeedWithReinforcement( const TerrainData& environment, const MIL_Object_ABC& object ) const
{    
    if( !object().HasMobilityInfluence() )
        return std::numeric_limits< MT_Float >::max();


    MT_Float rObjectSpeed = GetMaxSpeed( object );
    const PHY_RoleInterface_Reinforcement::T_PionSet& reinforcements = pion_.GetRole< PHY_RoleInterface_Reinforcement >().GetReinforcements();
    for( PHY_RoleInterface_Reinforcement::CIT_PionSet itReinforcement = reinforcements.begin(); itReinforcement != reinforcements.end(); ++itReinforcement )
        rObjectSpeed = std::min( rObjectSpeed, (**itReinforcement).GetRole< PHY_RoleAction_Moving >().GetSpeedWithReinforcement( environment, object ) );

    const MT_Float rCurrentMaxSpeed = GetMaxSpeed();
    const MT_Float rCurrentEnvSpeed = GetSpeedWithReinforcement( environment );

    rObjectSpeed = std::min( rObjectSpeed, rCurrentMaxSpeed );
    rObjectSpeed = ApplySpeedModificators( rObjectSpeed );
    return object().ApplySpeedPolicy( rObjectSpeed, rCurrentEnvSpeed, rCurrentMaxSpeed );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::ExtrapolatePosition
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
MT_Vector2D PHY_RoleAction_Moving::ExtrapolatePosition( const MT_Float rTime, const bool bBoundOnPath ) const
{

    const PHY_RoleInterface_Location& roleLocation = pion_.GetRole< PHY_RoleInterface_Location >();
    return PHY_MovingEntity_ABC::ExtrapolatePosition( roleLocation.GetPosition(), roleLocation.GetCurrentSpeed(), rTime, bBoundOnPath );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::SendEnvironmentType
// Created: SBO 2005-06-15
// -----------------------------------------------------------------------------
void PHY_RoleAction_Moving::SendEnvironmentType() const
{

    NET_ASN_MsgUnitEnvironmentType asn;
    asn().oid = pion_.GetID();
    SerializeEnvironmentType( asn() );
    asn.Send();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::SendCurrentPath
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
void PHY_RoleAction_Moving::SendCurrentPath() const
{  
    NET_ASN_MsgUnitPathFind asnMsg;
    

    asnMsg().oid = pion_.GetID();   
    if( !SerializeCurrentPath( asnMsg().itineraire ) )
        return;

    asnMsg.Send();
    delete [] asnMsg().itineraire.coordinates.elem;
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
    rSpeed_ = rSpeed;
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

    point.SendToDIA( pion_.GetRole< DEC_Representations >() );
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

    if( pion_.GetRole< PHY_RoleInterface_Surrender >().IsSurrendered() )
        return true;

    return      pion_.GetRole< PHY_RoleAction_InterfaceFlying >().CanMove() 
            && !pion_.GetRole< PHY_RoleInterface_Transported  >().IsTransported()
            && !pion_.GetRole< PHY_RoleInterface_Reinforcement     >().IsReinforcing();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::CanObjectInteractWith
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
bool PHY_RoleAction_Moving::CanObjectInteractWith( const MIL_Object_ABC& object ) const
{
    return object.CanInteractWith( pion_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::HasResources
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
bool PHY_RoleAction_Moving::HasResources()
{

    if( pion_.GetRole< PHY_RoleInterface_Surrender >().IsSurrendered() )
        return true;
    
    // Reservation des consommations du pion + renforts
    uint nNbrPionToRollback = 1;
    if( !pion_.GetRole< PHY_RoleInterface_Dotations >().SetConsumptionMode( PHY_ConsumptionType::moving_ ) )  
        return false;

    const PHY_RoleInterface_Reinforcement::T_PionSet& reinforcements = pion_.GetRole< PHY_RoleInterface_Reinforcement >().GetReinforcements();
    PHY_RoleInterface_Reinforcement::CIT_PionSet itReinforcement;
    for( itReinforcement = reinforcements.begin(); itReinforcement != reinforcements.end(); ++itReinforcement )
    {
        MIL_AgentPion& reinforcement = **itReinforcement;
        ++nNbrPionToRollback;
        if( !reinforcement.GetRole< PHY_RoleInterface_Dotations >().SetConsumptionMode( PHY_ConsumptionType::moving_ ) )
            break;            
    }

    if( itReinforcement == reinforcements.end() )
        return true;
    
    // Rollback
    pion_.GetRole< PHY_RoleInterface_Dotations >().RollbackConsumptionMode();
    itReinforcement = reinforcements.begin();
    while( --nNbrPionToRollback )
        (**itReinforcement++).GetRole< PHY_RoleInterface_Dotations >().RollbackConsumptionMode();
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::SendRC
// Created: NLD 2005-10-05
// -----------------------------------------------------------------------------
void PHY_RoleAction_Moving::SendRC( int nReportID ) const
{

    MIL_Report::PostEvent( pion_, (MIL_Report::E_EngineReport)nReportID );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::Update
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
void PHY_RoleAction_Moving::Update( bool /*bIsDead*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::Clean
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
void PHY_RoleAction_Moving::Clean()
{
    PHY_MovingEntity_ABC::Clean();

    bCurrentPathHasChanged_ = false;
    bEnvironmentHasChanged_ = false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::HasChanged
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
bool PHY_RoleAction_Moving::HasChanged() const
{
    return bCurrentPathHasChanged_ || bEnvironmentHasChanged_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::NotifyEnvironmentChanged
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
void PHY_RoleAction_Moving::NotifyEnvironmentChanged()
{
    bEnvironmentHasChanged_ = true;
}
    
// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::NotifyCurrentPathChanged
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
void PHY_RoleAction_Moving::NotifyCurrentPathChanged()
{
    bCurrentPathHasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::SetSpeedModificator
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
void PHY_RoleAction_Moving::SetSpeedModificator( MT_Float rFactor )
{
    rSpeedModificator_ = rFactor;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::SetMaxSpeedModificator
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
void PHY_RoleAction_Moving::SetMaxSpeedModificator( MT_Float rFactor )
{
    rMaxSpeedModificator_ = rFactor;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::Execute
// Created: MGD 2009-09-21
// -----------------------------------------------------------------------------
void PHY_RoleAction_Moving::Execute( posture::PostureComputer_ABC& algorithm ) const
{
    if( rSpeed_ == 0. )
        algorithm.UnsetPostureMovement();
    else
    {
        algorithm.SetPostureMovement();
    }
}

