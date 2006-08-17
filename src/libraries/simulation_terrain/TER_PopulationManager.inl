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
// Name: TER_PopulationManager::GetConcentrationManager
// Created: NLD 2005-10-07
// -----------------------------------------------------------------------------
inline
TER_PopulationConcentrationManager& TER_PopulationManager::GetConcentrationManager()
{
    return concentrationManager_;
}

// -----------------------------------------------------------------------------
// Name: TER_PopulationManager::GetFlowManager
// Created: NLD 2005-10-07
// -----------------------------------------------------------------------------
inline
TER_PopulationFlowManager& TER_PopulationManager::GetFlowManager()
{
    return flowManager_;
}
