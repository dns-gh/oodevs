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

#include "MIL_pch.h"

#include "PHY_RoleAction_Flying.h"
#include "PHY_ActionFly.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "Entities/Agents/Roles/Dotations/PHY_RolePion_Dotations.h"
#include "Entities/Agents/Units/Dotations/PHY_ConsumptionType.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/Effects/MIL_EffectManager.h"
#include "MIL_AgentServer.h"

BOOST_CLASS_EXPORT_GUID( PHY_RoleAction_Flying, "PHY_RoleAction_Flying" )

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Flying constructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RoleAction_Flying::PHY_RoleAction_Flying( MT_RoleContainer& role, MIL_AgentPion& pion )
    : PHY_RoleAction_InterfaceFlying( role )
    , pPion_                        ( &pion )
    , effectFly_                    ( *this )
    , pActionFly_                   ( 0 )
    , bForceLanding_                ( false )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Flying constructor
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
PHY_RoleAction_Flying::PHY_RoleAction_Flying()
    : PHY_RoleAction_InterfaceFlying()
    , pPion_                        ()
    , effectFly_                    ( *this )
    , pActionFly_                   ( 0 )
    , bForceLanding_                ( false )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Flying destructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RoleAction_Flying::~PHY_RoleAction_Flying()
{
}

// =============================================================================
// CHECKPOINTS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Flying::serialize
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
template< typename Archive > 
void PHY_RoleAction_Flying::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< PHY_RoleAction_InterfaceFlying >( *this )
         & pPion_;
}


// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Flying::TakeOff
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
inline
bool PHY_RoleAction_Flying::TakeOff()
{
    if( pActionFly_ )
        return false;

    assert( pPion_ );
    pActionFly_ = new PHY_ActionFly( *pPion_ );
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Flying::Land
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
inline
bool PHY_RoleAction_Flying::Land()
{
    if( !pActionFly_ ) 
        return false;

    GetRole< PHY_RolePion_Location >().Fly( 0. );
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
    bForceLanding_ = !GetRole< PHY_RolePion_Dotations >().SetConsumptionMode( PHY_ConsumptionType::moving_ );
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
        GetRole< PHY_RolePion_Location  >().Fly( rHeight );
}

