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
#include <boost/range/algorithm_ext/erase.hpp>

namespace
{
    MIL_EffectManager* manager = 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_EffectManager::GetEffectManager
// Created: LDC 2010-01-04
// -----------------------------------------------------------------------------
MIL_EffectManager& MIL_EffectManager::GetEffectManager()
{
    if( !manager )
        throw MASA_EXCEPTION( "Effect manager unset" );
    return *manager;
}

// -----------------------------------------------------------------------------
// Name: MIL_EffectManager constructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
MIL_EffectManager::MIL_EffectManager()
{
    if( manager )
        throw MASA_EXCEPTION( "Effect manager already created" );
    manager = this;
}

// -----------------------------------------------------------------------------
// Name: MIL_EffectManager destructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
MIL_EffectManager::~MIL_EffectManager()
{
    manager = 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_EffectManager::Register
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
void MIL_EffectManager::Register( MIL_Effect_ABC& effect )
{
    effects_.push_back( &effect );
}

// -----------------------------------------------------------------------------
// Name: MIL_EffectManager::Update
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void MIL_EffectManager::Update()
{
    T_Effects effects;
    effects.swap( effects_ );
    for( auto it = effects.begin(); it != effects.end(); ++it )
    {
        try
        {
            if( (*it)->Execute() )
                effects_.push_back( *it );
        }
        catch( const std::exception& e )
        {
            delete *it;
            MT_LOG_ERROR_MSG( "Effect error : " << tools::GetExceptionMsg( e ) );
        }
        catch( ... )
        {
            delete *it;
            MT_LOG_ERROR_MSG( "Effect unknown error" );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_EffectManager::RegisterFlyingShell
// Created: NLD 2005-02-21
// -----------------------------------------------------------------------------
void MIL_EffectManager::RegisterFlyingShell( const MIL_Effect_IndirectFire& effect )
{
    flyingShells_.push_back( &effect );
}

// -----------------------------------------------------------------------------
// Name: MIL_EffectManager::UnregisterFlyingShell
// Created: NLD 2005-02-21
// -----------------------------------------------------------------------------
void MIL_EffectManager::UnregisterFlyingShell( const MIL_Effect_IndirectFire& effect )
{
    boost::remove_erase( flyingShells_, &effect );
}

// -----------------------------------------------------------------------------
// Name: MIL_EffectManager::GetFlyingShells
// Created: NLD 2005-02-21
// -----------------------------------------------------------------------------
const MIL_EffectManager::T_FlyingShells& MIL_EffectManager::GetFlyingShells() const
{
    return flyingShells_;
}
