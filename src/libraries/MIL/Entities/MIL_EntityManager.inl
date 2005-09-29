// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/MIL_EntityManager.inl $
// $Author: Jvt $
// $Modtime: 23/03/05 11:24 $
// $Revision: 7 $
// $Workfile: MIL_EntityManager.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::FindArmy
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
inline
MIL_Army* MIL_EntityManager::FindArmy( const std::string& strName ) const
{
    CIT_ArmyMap it = armies_.find( strName );
    if( it == armies_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::FindPopulation
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
inline
MIL_Population* MIL_EntityManager::FindPopulation( uint nID ) const
{
    CIT_PopulationMap it = populations_.find( nID );
    if( it == populations_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::FindAutomate
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
inline
MIL_Automate* MIL_EntityManager::FindAutomate( uint nID ) const
{
    CIT_AutomateMap it = automates_.find( nID );
    if( it == automates_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::FindAgentPion
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
inline
MIL_AgentPion* MIL_EntityManager::FindAgentPion( uint nID ) const
{
    CIT_PionMap itPion = pions_.find( nID );
    if( itPion != pions_.end() )
        return itPion->second;
    return 0;
}


// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::GetArmies
// Created: NLD 2004-09-01
// -----------------------------------------------------------------------------
inline
const MIL_EntityManager::T_ArmyMap& MIL_EntityManager::GetArmies() const
{
    return armies_;
}

// =============================================================================
// STATS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::GetKnowledgesTime
// Created: NLD 2004-10-08
// -----------------------------------------------------------------------------
inline
MT_Float MIL_EntityManager::GetKnowledgesTime() const
{
    return rKnowledgesTime_;
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::GetDecisionsTime
// Created: NLD 2004-10-08
// -----------------------------------------------------------------------------
inline
MT_Float MIL_EntityManager::GetDecisionsTime() const
{
    return rDecisionsTime_;
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::GetActionsTime
// Created: NLD 2004-10-08
// -----------------------------------------------------------------------------
inline
MT_Float MIL_EntityManager::GetActionsTime() const
{
    return rActionsTime_;
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::GetEffectsTime
// Created: NLD 2004-10-08
// -----------------------------------------------------------------------------
inline
MT_Float MIL_EntityManager::GetEffectsTime() const
{
    return rEffectsTime_;
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::GetStatesTime
// Created: NLD 2004-10-08
// -----------------------------------------------------------------------------
inline
MT_Float MIL_EntityManager::GetStatesTime() const
{
    return rStatesTime_;
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::GetEffectManager
// Created: NLD 2005-02-21
// -----------------------------------------------------------------------------
inline
MIL_EffectManager& MIL_EntityManager::GetEffectManager() const
{
    return effectManager_;
}