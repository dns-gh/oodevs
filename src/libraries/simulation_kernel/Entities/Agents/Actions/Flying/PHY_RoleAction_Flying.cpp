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
#include "MIL_AgentServer.h"
#include "PHY_ActionFly.h"
#include "Entities/MIL_Entity_ABC.h"
#include "Entities/Agents/Roles/Dotations/PHY_RoleInterface_Dotations.h"
#include "Entities/Agents/Units/Dotations/PHY_ConsumptionType.h"
#include "Entities/Effects/MIL_EffectManager.h"
#include "Entities/MIL_EntityManager.h"

#include "simulation_kernel/LocationComputer_ABC.h"
#include "simulation_kernel/PostureComputer_ABC.h"

BOOST_CLASS_EXPORT_GUID( PHY_RoleAction_Flying, "PHY_RoleAction_Flying" )

template< typename Archive >
void save_construct_data( Archive& archive, const PHY_RoleAction_Flying* role, const unsigned int /*version*/ )
{
    MIL_Entity_ABC* const entity = &role->entity_;
    archive << entity;
}

template< typename Archive >
void load_construct_data( Archive& archive, PHY_RoleAction_Flying* role, const unsigned int /*version*/ )
{
    MIL_Entity_ABC* entity;
    archive >> entity;
    ::new( role )PHY_RoleAction_Flying( *entity );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Flying constructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RoleAction_Flying::PHY_RoleAction_Flying( MIL_Entity_ABC& entity )
    : entity_                        ( entity )
    , effectFly_                    ( *this )
    , pActionFly_                   ( 0 )
    , bForceLanding_                ( false )
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
void PHY_RoleAction_Flying::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< PHY_RoleAction_InterfaceFlying >( *this )
		 & entity_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Flying::TakeOff
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
bool PHY_RoleAction_Flying::TakeOff()
{
    if( pActionFly_ )
        return false;

    pActionFly_ = new PHY_ActionFly( entity_ );
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

    rHeight_ = 0.;
    delete pActionFly_;
    pActionFly_ = 0;
    return true;
}


// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Flying::SetFlyingHeight
// Created: JVT 2004-11-02
// -----------------------------------------------------------------------------
void PHY_RoleAction_Flying::SetFlyingHeight( MT_Float rHeight )
{
    effectFly_.SetFlyingHeight( std::max( 0., rHeight ) );

    if ( rHeight > 0. )
        TakeOff();
    else
        Land();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Flying::Fly
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
void PHY_RoleAction_Flying::Fly()
{

    bForceLanding_ = !entity_.GetRole< PHY_RoleInterface_Dotations >().SetConsumptionMode( PHY_ConsumptionType::moving_ );
    MIL_AgentServer::GetWorkspace().GetEntityManager().GetEffectManager().Register( effectFly_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Flying::Apply
// Created: NLD 2004-10-04
// Modified: JVT 2004-11-02
// -----------------------------------------------------------------------------
void PHY_RoleAction_Flying::Apply( MT_Float rHeight )
{
    if ( bForceLanding_ || rHeight <= 0. )
    {
        Land();
        bForceLanding_ = false;
    }
    else
        rHeight_ = rHeight;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Flying::Update
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
void PHY_RoleAction_Flying::Update( bool /*bIsDead*/ )
{
    // NOTHING
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
    algorithm.SetHeight( rHeight_ );
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

