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
// Name: TER_World::GetMeterPerPixel
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
inline
MT_Float TER_World::GetMeterPerPixel() const
{
    return pCoordinateManager_->GetMeterPerPixel();
}

// -----------------------------------------------------------------------------
// Name: TER_World::GetMeterSquarePerPixelSquare
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
inline
MT_Float TER_World::GetMeterSquarePerPixelSquare() const
{
    return pCoordinateManager_->GetMeterSquarePerPixelSquare();
}

// -----------------------------------------------------------------------------
// Name: TER_World::GetWeldValue
// Created: AGE 2005-01-31
// -----------------------------------------------------------------------------
inline
MT_Float TER_World::GetWeldValue() const
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

// -----------------------------------------------------------------------------
// Name: TER_World::GetGraphFileName
// Created: JVT 2005-04-07
// -----------------------------------------------------------------------------
inline
const std::string& TER_World::GetGraphFileName() const
{
    return graphFileName_;
}

// -----------------------------------------------------------------------------
// Name: TER_World::GetNodeFileName
// Created: JVT 2005-04-07
// -----------------------------------------------------------------------------
inline
const std::string& TER_World::GetNodeFileName() const
{
    return nodeFileName_;
}

// -----------------------------------------------------------------------------
// Name: TER_World::GetLinkFileName
// Created: JVT 2005-04-07
// -----------------------------------------------------------------------------
inline
const std::string& TER_World::GetLinkFileName() const
{
    return linkFileName_;
}
