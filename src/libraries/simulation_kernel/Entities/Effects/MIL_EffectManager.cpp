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
#include "MT_Tools/MT_Logger.h"

namespace
{
    MIL_EffectManager* pEffectManager_ = 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_EffectManager::EffectComparator::operator()
// Created: JSR 2013-02-04
// -----------------------------------------------------------------------------
bool MIL_EffectManager::EffectComparator::operator() (const MIL_Effect_ABC* lhs, const MIL_Effect_ABC* rhs) const
{
    if( !lhs )
        return false;
    if( !rhs )
        return true;
    return lhs->GetId() < rhs->GetId();
}

// -----------------------------------------------------------------------------
// Name: MIL_EffectManager::GetEffectManager
// Created: LDC 2010-01-04
// -----------------------------------------------------------------------------
MIL_EffectManager& MIL_EffectManager::GetEffectManager()
{
    if( !pEffectManager_ )
        throw MASA_EXCEPTION( "Effect manager unset" );
    return *pEffectManager_;
}

// -----------------------------------------------------------------------------
// Name: MIL_EffectManager constructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
MIL_EffectManager::MIL_EffectManager()
{
    if( pEffectManager_ )
        throw MASA_EXCEPTION( "Effect manager already created" );
    pEffectManager_ = this;
}

// -----------------------------------------------------------------------------
// Name: MIL_EffectManager destructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
MIL_EffectManager::~MIL_EffectManager()
{
    pEffectManager_ = 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_EffectManager::Register
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
void MIL_EffectManager::Register( MIL_Effect_ABC& effect )
{
    if( ! effects_.insert( &effect ).second )
        MT_LOG_ERROR_MSG( __FUNCTION__ << " : Insert failed" );
}

// -----------------------------------------------------------------------------
// Name: MIL_EffectManager::Update
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void MIL_EffectManager::Update()
{
    for( auto it = effects_.begin(); it != effects_.end(); )
    {
        MIL_Effect_ABC& effect = **it;
        bool executed = false;
        try
        {
            executed = effect.Execute();
        }
        catch( const std::exception& e )
        {
            MT_LOG_ERROR_MSG( "Effect error : " << tools::GetExceptionMsg( e ) );
        }
        if( executed )
            ++it;
        else
            it = effects_.erase( it );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_EffectManager::RegisterFlyingShell
// Created: NLD 2005-02-21
// -----------------------------------------------------------------------------
void MIL_EffectManager::RegisterFlyingShell( const MIL_Effect_IndirectFire& effect )
{
    if( ! flyingShells_.insert( &effect ).second )
        MT_LOG_ERROR_MSG( __FUNCTION__ << " : Insert failed" );
}

// -----------------------------------------------------------------------------
// Name: MIL_EffectManager::UnregisterFlyingShell
// Created: NLD 2005-02-21
// -----------------------------------------------------------------------------
void MIL_EffectManager::UnregisterFlyingShell( const MIL_Effect_IndirectFire& effect )
{
    if( flyingShells_.erase( &effect ) != 1 )
        MT_LOG_ERROR_MSG( __FUNCTION__ << " : Erase failed" );
}

// -----------------------------------------------------------------------------
// Name: MIL_EffectManager::GetFlyingShells
// Created: NLD 2005-02-21
// -----------------------------------------------------------------------------
const MIL_EffectManager::T_FlyingShellSet& MIL_EffectManager::GetFlyingShells() const
{
    return flyingShells_;
}
