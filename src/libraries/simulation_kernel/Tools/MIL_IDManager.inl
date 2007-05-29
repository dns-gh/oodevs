// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: template< typename Archive > void MIL_AgentServer::SerializeStatics
// Created: NLD 2005-11-04
// -----------------------------------------------------------------------------
template< typename Archive >
void MIL_IDManager::serialize( Archive& file, uint /*version*/ )
{
    file & units_
         & fireResultsPion_
         & fireResultsPopulation_
         & limas_
         & limits_
         & knowledgesAgent_
         & effectsWeather_
         & maintenanceComposanteStates_
         & medicalHumanStates_
         & supplyStates_
         & populations_
         & populationConcentrations_
         & populationFlows_
         & knowledgePopulations_
         & knowledgePopulationConcentrations_
         & knowledgePopulationFlows_
         & objectClassIDToType_;
}

// -----------------------------------------------------------------------------
// Name: MIL_IDManager::FindObjectIDManager
// Created: NLD 2005-11-04
// -----------------------------------------------------------------------------
inline
MIL_MOSIDManager* MIL_IDManager::FindObjectIDManager( const std::string& strName )
{
    CIT_ObjectIDManager it = objectNameToType_.find( strName );
    if( it == objectNameToType_.end() )
        return 0;
    return it->second;
}
