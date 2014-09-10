// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Flying/PHY_RoleAction_Flying.cpp $
// $Author: Jvt $
// $Modtime: 14/04/05 12:35 $
// $Revision: 7 $
// $Workfile: PHY_RoleAction_Flying.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_RoleAction_Flying.h"
#include "FlyListener_ABC.h"
#include "LocationComputer_ABC.h"
#include "PostureComputer_ABC.h"
#include "MoveComputer_ABC.h"
#include "DefaultConsumptionModeChangeRequest.h"
#include "ConsumptionChangeRequestHandler_ABC.h"
#include "PHY_ActionFly.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Effects/MIL_EffectManager.h"
#include "Entities/Agents/Units/Dotations/PHY_ConsumptionType.h"

BOOST_CLASS_EXPORT_IMPLEMENT( PHY_RoleAction_Flying )

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Flying constructor
// Created: LDC 2013-01-09
// -----------------------------------------------------------------------------
PHY_RoleAction_Flying::PHY_RoleAction_Flying()
    : owner_    ( 0 )
    , effectFly_( *this )
    , rHeight_  ( 0. )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Flying constructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RoleAction_Flying::PHY_RoleAction_Flying( MIL_Agent_ABC& entity )
    : owner_    ( &entity )
    , effectFly_( *this )
    , rHeight_  ( 0. )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Flying destructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RoleAction_Flying::~PHY_RoleAction_Flying()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Flying::serialize
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_RoleAction_Flying::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< PHY_RoleAction_InterfaceFlying >( *this );
    file & owner_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Flying::TakeOff
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
bool PHY_RoleAction_Flying::TakeOff()
{
    if( pActionFly_ )
        return false;

    pActionFly_.reset( new PHY_ActionFly( *owner_ ) );
    owner_->RegisterAction( pActionFly_ );
    owner_->Apply( &FlyListener_ABC::TakeOff );
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Flying::Land
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
bool PHY_RoleAction_Flying::Land()
{
    if( !pActionFly_ )
        return false;

    owner_->UnregisterAction( pActionFly_->GetId() );
    NotifyStopFlying();
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Flying::NotifyStopFlying
// Created: LDC 2012-10-24
// -----------------------------------------------------------------------------
void PHY_RoleAction_Flying::NotifyStopFlying()
{
    if( !pActionFly_ )
        return;
    rHeight_ = 0.;
    pActionFly_.reset();
    owner_->Apply( &FlyListener_ABC::Land );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Flying::SetFlyingHeight
// Created: JVT 2004-11-02
// -----------------------------------------------------------------------------
void PHY_RoleAction_Flying::SetFlyingHeight( double rHeight )
{
    effectFly_.SetFlyingHeight( std::max( 0., rHeight ) );

    if( rHeight > 0. )
        TakeOff();
    else
        Land();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Flying::Fly
// Created: NLD 2004-10-04
// Modified: MGD 2010-03-12
// -----------------------------------------------------------------------------
void PHY_RoleAction_Flying::Fly()
{
    MIL_EffectManager::GetEffectManager().Register( effectFly_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Flying::Apply
// Created: NLD 2004-10-04
// Modified: JVT 2004-11-02
// -----------------------------------------------------------------------------
void PHY_RoleAction_Flying::Apply( double rHeight )
{
    dotation::DefaultConsumptionModeChangeRequest request( PHY_ConsumptionType::moving_ );
    owner_->Apply( &dotation::ConsumptionChangeRequestHandler_ABC::ChangeConsumptionMode, request ); // automatic rollback

    if( !request.AllChanged() || rHeight <= 0. )
        Land();
    else
        rHeight_ = rHeight;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Flying::Update
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
void PHY_RoleAction_Flying::Update( bool bIsDead )
{
    if( bIsDead  )
        SetFlyingHeight( 0 );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Flying::Clean
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
void PHY_RoleAction_Flying::Clean()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Flying::CanMove
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
bool PHY_RoleAction_Flying::CanMove() const
{
    return pActionFly_ != 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Flying::IsFlying
// Created: JVT 2005-02-11
// -----------------------------------------------------------------------------
bool PHY_RoleAction_Flying::IsFlying() const
{
    return pActionFly_ != 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::Execute
// Created: MGD 2009-09-21
// -----------------------------------------------------------------------------
void PHY_RoleAction_Flying::Execute( location::LocationComputer_ABC& algorithm ) const
{
    algorithm.IncreaseHeight( rHeight_ );
    algorithm.SetFlying();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::Execute
// Created: MGD 2009-09-21
// -----------------------------------------------------------------------------
void PHY_RoleAction_Flying::Execute( posture::PostureComputer_ABC& algorithm ) const
{
    if( rHeight_ == 0. )
        algorithm.UnsetPostureMovement();
    else
        algorithm.SetPostureMovement();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::Execute
// Created: AHC 2009-10-05
// -----------------------------------------------------------------------------
void PHY_RoleAction_Flying::Execute( moving::MoveComputer_ABC& algorithm ) const
{
    if( pActionFly_ == 0 )
        algorithm.NotifyCannotFly();
}
