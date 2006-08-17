// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Effects/MIL_EffectManager.cpp $
// $Author: Nld $
// $Modtime: 21/02/05 11:19 $
// $Revision: 3 $
// $Workfile: MIL_EffectManager.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_EffectManager.h"

#include "MIL_Effect_ABC.h"

// -----------------------------------------------------------------------------
// Name: MIL_EffectManager constructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
MIL_EffectManager::MIL_EffectManager()
{

}

// -----------------------------------------------------------------------------
// Name: MIL_EffectManager destructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
MIL_EffectManager::~MIL_EffectManager()
{

}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_EffectManager::Register
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
void MIL_EffectManager::Register( MIL_Effect_ABC& effect )
{
    bool bOut = effects_.insert( &effect ).second;
    assert( bOut );
}

// -----------------------------------------------------------------------------
// Name: MIL_EffectManager::Update
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void MIL_EffectManager::Update()
{
    for( IT_EffectSet it = effects_.begin(); it != effects_.end(); )
    {
        MIL_Effect_ABC& effect = **it;
        bool bContinueEffect = effect.Execute();
        if( bContinueEffect )
            ++it;
        else
            it = effects_.erase( it );
    }
}

// =============================================================================
// FLYING SHELLS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_EffectManager::RegisterFlyingShell
// Created: NLD 2005-02-21
// -----------------------------------------------------------------------------
void MIL_EffectManager::RegisterFlyingShell( const MIL_Effect_IndirectFire& effect )
{
    bool bOut = flyingShells_.insert( &effect ).second;
    assert( bOut );
}

// -----------------------------------------------------------------------------
// Name: MIL_EffectManager::UnregisterFlyingShell
// Created: NLD 2005-02-21
// -----------------------------------------------------------------------------
void MIL_EffectManager::UnregisterFlyingShell( const MIL_Effect_IndirectFire& effect )
{
    int nOut = flyingShells_.erase( &effect );
    assert( nOut == 1 );
}

