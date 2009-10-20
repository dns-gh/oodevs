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
#include "Knowledge/DEC_Knowledge_PopulationCollision.h"
#include "Network/NET_ASN_Messages.h"

#include "simulation_kernel/PostureComputer_ABC.h"
#include "simulation_kernel/SpeedComputer_ABC.h"
#include "simulation_kernel/MaxSlopeComputer_ABC.h"
#include "simulation_kernel/MoveComputer_ABC.h"
#include "simulation_kernel/MoveComputerFactory_ABC.h"
#include "simulation_kernel/ConsumptionComputerFactory_ABC.h"
#include "simulation_kernel/ConsumptionModeChangeRequest_ABC.h"
#include "simulation_kernel/ConsumptionChangeRequestHandler_ABC.h"
#include "simulation_kernel/ObjectCollisionNotificationHandler_ABC.h"
#include "AlgorithmsFactories.h"

BOOST_CLASS_EXPORT_GUID( moving::PHY_RoleAction_Moving, "PHY_RoleAction_Moving" )

namespace moving
{

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

double maxSpeedEnvObj(const PHY_ComposantePion& comp, const TerrainData& environment, const MIL_Object_ABC& object )
{
    return std::min( comp.GetMaxSpeed(environment), comp.GetMaxSpeed(object));
}

class SpeedComputerStrategy : public moving::SpeedStrategy_ABC
{
public:
    SpeedComputerStrategy( bool isMaxSpeed, bool withReinforcement, const MIL_Object_ABC& obj, const TerrainData* env=0 )
        : withReinforcement_( withReinforcement ), isMax_( isMaxSpeed )
    {
        if(env)
        {
            compFunctor_ = boost::bind( maxSpeedEnvObj, _1, boost::cref( *env ), boost::cref( obj ) );
            pionFunctor_ = isMaxSpeed ? 
                boost::mem_fn( &PHY_RoleAction_Moving::GetMaxSpeedWithReinforcement ) :
                ( boost::function< double( const PHY_RoleAction_Moving&)> )boost::bind( &PHY_MovingEntity_ABC::GetSpeedWithReinforcement, _1, boost::cref( *env ), boost::cref( obj ) );
        }
        else
        {
            compFunctor_ = boost::bind( boost::mem_fn<double, PHY_ComposantePion, const MIL_Object_ABC&>( &PHY_ComposantePion::GetMaxSpeed ), _1, boost::cref( obj ) );
            pionFunctor_ = boost::mem_fn( &PHY_RoleAction_Moving::GetMaxSpeedWithReinforcement );
        }
    }
    SpeedComputerStrategy( bool isMaxSpeed, bool withReinforcement, const TerrainData* env=0 ) :
            withReinforcement_( withReinforcement ), isMax_( isMaxSpeed )
    {
        if(env)
        {
            compFunctor_ = boost::bind( boost::mem_fn<double,PHY_ComposantePion,const TerrainData&>( &PHY_ComposantePion::GetMaxSpeed ), _1, boost::cref( *env ) );
            pionFunctor_ = isMaxSpeed ? 
                boost::mem_fn( &PHY_RoleAction_Moving::GetMaxSpeedWithReinforcement ):
                ( boost::function< double( const PHY_RoleAction_Moving& )> )boost::bind( &PHY_RoleAction_Moving::GetSpeedWithReinforcement, _1, boost::cref( *env ) );            
        }
        else
        {
            compFunctor_ = boost::mem_fn(&PHY_ComposantePion::GetMaxSpeed);
            pionFunctor_ = boost::mem_fn(&PHY_RoleAction_Moving::GetMaxSpeedWithReinforcement);
        }
    }
    double ApplyOnComponent(const PHY_ComposantePion& comp) const
    {
        return compFunctor_(comp);
    }
    double ApplyOnReinforcement( MIL_AgentPion& pion) const
    {
        return withReinforcement_ ?
                pionFunctor_( pion.GetRole<PHY_RoleAction_Moving>() ):
                std::numeric_limits<double>::max();
    }
    double ApplyOnPopulation(const DEC_Knowledge_PopulationCollision& population) const
    {
        return isMax_ ? population.GetPionMaxSpeed() : std::numeric_limits<double>::max();
    }
    double AddModifier( double ratio, bool isMax=true) const
    {
        return isMax == isMax_ ? ratio : 1.0;
    }
private:
    bool withReinforcement_;
    bool isMax_;
    boost::function< double(const PHY_ComposantePion&)> compFunctor_;
    boost::function< double(PHY_RoleAction_Moving&)> pionFunctor_;
};

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
    , bHasMove_              ( false )
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
    file & boost::serialization::base_object< tools::Role_ABC >( *this );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::ApplyMaxSpeedModificators
// Created: NLD 2004-11-04
// -----------------------------------------------------------------------------
MT_Float PHY_RoleAction_Moving::ApplyMaxSpeedModificators( double rSpeed ) const
{
    rSpeed *= rMaxSpeedModificator_;
    return rSpeed;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::ApplySpeedModificators
// Created: NLD 2004-11-04
// -----------------------------------------------------------------------------
double PHY_RoleAction_Moving::ApplySpeedModificators( double rSpeed ) const
{
    rSpeed *= rSpeedModificator_;
    return rSpeed;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::GetMaxSpeed
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
double PHY_RoleAction_Moving::GetMaxSpeed( const MIL_Object_ABC& object ) const
{
    SpeedComputerStrategy strategy(true,false,object);
    const moving::SpeedComputer_ABC& computer =
            pion_.Execute(
                    pion_.GetAlgorithms().moveComputerFactory_->CreateSpeedComputer( strategy ) );

    return computer.GetSpeed();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::GetMaxSpeed
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
double PHY_RoleAction_Moving::GetMaxSpeed( const TerrainData& environment ) const
{
    SpeedComputerStrategy strategy(true,false,&environment);
    const moving::SpeedComputer_ABC& computer =
            pion_.Execute(
                    pion_.GetAlgorithms().moveComputerFactory_->CreateSpeedComputer( strategy ) );

    return computer.GetSpeed();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::GetMaxSpeed
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
double PHY_RoleAction_Moving::GetMaxSpeed() const
{
    SpeedComputerStrategy strategy(true,false,0);
    const moving::SpeedComputer_ABC& computer =
            pion_.Execute(
                    pion_.GetAlgorithms().moveComputerFactory_->CreateSpeedComputer( strategy ) );

    return computer.GetSpeed();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::GetMaxSlope
// Created: AGE 2005-04-13
// -----------------------------------------------------------------------------
double PHY_RoleAction_Moving::GetMaxSlope() const
{
    moving::MaxSlopeComputer_ABC& computer =
            pion_.GetAlgorithms().moveComputerFactory_->CreateMaxSlopeComputer();
    pion_.Execute((OnComponentComputer_ABC&)computer);

    return computer.GetMaxSlope();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::GetMaxSpeedWithReinforcement
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
double PHY_RoleAction_Moving::GetMaxSpeedWithReinforcement() const
{
    SpeedComputerStrategy strategy(true,true,0);
    const moving::SpeedComputer_ABC& computer =
                pion_.Execute(
                        pion_.GetAlgorithms().moveComputerFactory_->CreateSpeedComputer( strategy ) );
    double rSpeed = computer.GetSpeed();
    return ApplyMaxSpeedModificators( rSpeed );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::GetSpeedWithReinforcement
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
double PHY_RoleAction_Moving::GetSpeedWithReinforcement( const TerrainData& environment ) const
{
    SpeedComputerStrategy strategy(false,true,&environment);
    const moving::SpeedComputer_ABC& computer =
                pion_.Execute(
                        pion_.GetAlgorithms().moveComputerFactory_->CreateSpeedComputer( strategy ) );
    double rSpeed = computer.GetSpeed();
    rSpeed = std::min( rSpeed, GetMaxSpeedWithReinforcement() );
    return rSpeed;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::GetSpeedWithReinforcement
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
double PHY_RoleAction_Moving::GetSpeedWithReinforcement( const TerrainData& environment, const MIL_Object_ABC& object ) const
{    
    if( !object().HasMobilityInfluence() )
        return std::numeric_limits< double >::max();

    SpeedComputerStrategy strategy(false,true,object);
    const moving::SpeedComputer_ABC& computer =
                    pion_.Execute(
                            pion_.GetAlgorithms().moveComputerFactory_->CreateSpeedComputer( strategy ) );
    double rObjectSpeed = computer.GetSpeed();

    const double rCurrentMaxSpeed = GetMaxSpeed();
    const double rCurrentEnvSpeed = GetSpeedWithReinforcement( environment );

    rObjectSpeed = std::min( rObjectSpeed, rCurrentMaxSpeed );
    rObjectSpeed = ApplySpeedModificators( rObjectSpeed );
    return object().ApplySpeedPolicy( rObjectSpeed, rCurrentEnvSpeed, rCurrentMaxSpeed );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::ExtrapolatePosition
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
MT_Vector2D PHY_RoleAction_Moving::ExtrapolatePosition( const double rTime, const bool bBoundOnPath ) const
{
    return PHY_MovingEntity_ABC::ExtrapolatePosition( pRoleLocation_->GetPosition(), pRoleLocation_->GetCurrentSpeed(), rTime, bBoundOnPath );
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
void PHY_RoleAction_Moving::SendFullState( NET_ASN_MsgUnitAttributes& asnMsg ) const
{
    UNREFERENCED_PARAMETER( asnMsg );

    SendCurrentPath    ();
    SendEnvironmentType();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::SendChangedState
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
void PHY_RoleAction_Moving::SendChangedState( NET_ASN_MsgUnitAttributes& asnMsg ) const
{
    UNREFERENCED_PARAMETER( asnMsg );

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
void PHY_RoleAction_Moving::ApplyMove( const MT_Vector2D& position, const MT_Vector2D& direction, double rSpeed, double /*rWalkedDistance*/ )
{
    rSpeed_ = rSpeed;
    bHasMove_ = ( rSpeed != 0. );
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
    pion_.Apply(&terrain::ObjectCollisionNotificationHandler_ABC::NotifyMovingInsideObject, object );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::NotifyMovingOutsideObject
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
void PHY_RoleAction_Moving::NotifyMovingOutsideObject( MIL_Object_ABC& object )
{
    pion_.Apply(&terrain::ObjectCollisionNotificationHandler_ABC::NotifyMovingInsideObject, object );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::CanMove
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
bool PHY_RoleAction_Moving::CanMove() const
{
    moving::MoveComputer_ABC& moveComputer = pion_.GetAlgorithms().moveComputerFactory_->CreateMoveComputer();
    pion_.Execute(moveComputer);

    return moveComputer.CanMoveOverride() || moveComputer.CanMove();
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
    moving::MoveComputer_ABC& moveComputer = pion_.GetAlgorithms().moveComputerFactory_->CreateMoveComputer();
        pion_.Execute(moveComputer);

    if (moveComputer.CanMoveOverride())
        return true;
    
    dotation::ConsumptionModeChangeRequest_ABC& request =
            pion_.GetAlgorithms().consumptionComputerFactory_->CreateConsumptionModeChangeRequest(PHY_ConsumptionType::moving_);
    pion_.Apply(&dotation::ConsumptionChangeRequestHandler_ABC::ChangeConsumptionMode,request); // automatic rollback

    return request.AllChanged();
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
    if (!bHasMove_)
        rSpeed_ = 0.;
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
    bHasMove_               = false;
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
void PHY_RoleAction_Moving::SetSpeedModificator( double rFactor )
{
    rSpeedModificator_ = rFactor;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::SetMaxSpeedModificator
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
void PHY_RoleAction_Moving::SetMaxSpeedModificator( double rFactor )
{
    rMaxSpeedModificator_ = rFactor;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::Execute
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

} // namespace moving
