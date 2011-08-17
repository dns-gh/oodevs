//*****************************************************************************
//
// $Created: NLD 2003-07-22 $
// $Archive: /MVW_v10/Build/SDK/ter/src/TER_Localisation.inl $
// $Author: Age $
// $Modtime: 24/02/05 11:16 $
// $Revision: 1 $
// $Workfile: TER_Localisation.inl $
//
//*****************************************************************************

// -----------------------------------------------------------------------------
// Name: TER_World::GetWorld
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
inline
TER_World& TER_World::GetWorld()
{
    assert( pInstance_ );
    return *pInstance_;
}

// -----------------------------------------------------------------------------
// Name: TER_World::GetWeldValue
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
inline
double TER_World::GetWeldValue() const
{
    return pCoordinateManager_->GetWeldValue();
}

// -----------------------------------------------------------------------------
// Name: TER_World::GetPathFindManager
// Created: AGE 2005-02-01
// -----------------------------------------------------------------------------
inline
TER_PathFindManager& TER_World::GetPathFindManager() const
{
    assert( pPathfindManager_ );
    return *pPathfindManager_;
}

// -----------------------------------------------------------------------------
// Name: TER_World::GetAnalyzerManager
// Created: CMA 2011-08-16
// -----------------------------------------------------------------------------
inline
TER_AnalyzerManager& TER_World::GetAnalyzerManager() const
{
    assert( pAnalyzerManager_ );
    return *pAnalyzerManager_;
}

// -----------------------------------------------------------------------------
// Name: TER_World::GetAgentManager
// Created: NLD 2005-10-07
// -----------------------------------------------------------------------------
inline
TER_AgentManager& TER_World::GetAgentManager() const
{
    assert( pAgentManager_ );
    return *pAgentManager_;
}

// -----------------------------------------------------------------------------
// Name: TER_World::GetObjectManager
// Created: NLD 2005-10-07
// -----------------------------------------------------------------------------
inline
TER_ObjectManager& TER_World::GetObjectManager() const
{
    assert( pObjectManager_ );
    return *pObjectManager_;
}

// -----------------------------------------------------------------------------
// Name: TER_World::GetPopulationManager
// Created: NLD 2005-10-07
// -----------------------------------------------------------------------------
inline
TER_PopulationManager& TER_World::GetPopulationManager() const
{
    assert( pPopulationManager_ );
    return *pPopulationManager_;
}
