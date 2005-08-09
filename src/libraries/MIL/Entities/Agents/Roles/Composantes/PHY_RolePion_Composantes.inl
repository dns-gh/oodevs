// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.inl $
// $Author: Jvt $
// $Modtime: 31/03/05 18:28 $
// $Revision: 5 $
// $Workfile: PHY_RolePion_Composantes.inl $
//
// *****************************************************************************

#include "MIL_AgentServer.h"

// -----------------------------------------------------------------------------
// Name: template< typename T > void PHY_RolePion_Composantes::Apply
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
template< typename T > inline void PHY_RolePion_Composantes::Apply( T& functor ) const
{
    for( CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
        functor( **it );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::HasChanged
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
inline
bool PHY_RolePion_Composantes::HasChanged() const
{
    return nNbrComposanteChanged_ != 0 || bLendsChanged_ || bEtatOpsChanged_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::IsUsable
// Created: NLD 2005-08-09
// -----------------------------------------------------------------------------
inline
bool PHY_RolePion_Composantes::IsUsable() const
{
    return nNbrUsableComposantes_ > 0; 
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GetMajorComposante
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
inline
const PHY_ComposantePion* PHY_RolePion_Composantes::GetMajorComposante() const
{
    return pMajorComposante_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GetEtatOps
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
inline
MT_Float PHY_RolePion_Composantes::GetEtatOps() const
{
    return rEtatOps_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GetPion
// Created: NLD 2005-01-04
// -----------------------------------------------------------------------------
inline
const MIL_AgentPion& PHY_RolePion_Composantes::GetPion() const
{
    assert( pPion_ );
    return *pPion_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::RepairAllComposantes
// Created: NLD 2004-09-21
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_Composantes::RepairAllComposantes()
{
    for( CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
        (**it).Repair();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::HealAllHumans
// Created: NLD 2004-09-21
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_Composantes::HealAllHumans()
{
    for( CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
        (**it).HealAllHumans();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::DestroyRandomComposante
// Created: NLD 2004-09-21
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_Composantes::DestroyRandomComposante()
{
    for( CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
    {
        PHY_ComposantePion& composante = **it;
        if( composante.GetState().IsUsable() )
        {
            composante.ReinitializeState( PHY_ComposanteState::dead_ );
            return;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::DestroyAllComposantes
// Created: NLD 2004-12-07
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_Composantes::DestroyAllComposantes()
{
    for( CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
        (**it).ReinitializeState( PHY_ComposanteState::dead_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::IsNeutralized
// Created: NLD 2004-10-12
// -----------------------------------------------------------------------------
inline
bool PHY_RolePion_Composantes::IsNeutralized() const
{
    return nNeutralizationEndTimeStep_ >= MIL_AgentServer::GetWorkspace().GetCurrentTimeStep();
}


// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::CanMove
// Created: NLD 2005-06-28
// -----------------------------------------------------------------------------
inline
bool PHY_RolePion_Composantes::CanMove() const
{
    return nNbrUndamagedMajorComposantes_ > 0 || nNbrUndamagedNonMajorComposantes_ > 0;
}
