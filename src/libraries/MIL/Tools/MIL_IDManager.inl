// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: NLD 2005-11-04 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************



// =============================================================================
// CHECKPOINTS
// =============================================================================
namespace boost
{
    namespace serialization
    {
        template< typename Archive >
        inline
        void serialize( Archive& file, MIL_IDManager::T_IDManagerMap& map, const uint nVersion )
        {
            split_free( file, map, nVersion );
        }
        
        template< typename Archive >
        void save( Archive& file, const MIL_IDManager::T_IDManagerMap& map, const uint )
        {
            file << map.size();
            for ( MIL_IDManager::CIT_IDManagerMap it = map.begin(); it != map.end(); ++it )
            {
                file << it->first;
                it->second->serialize( file );
            }
        }
        
        template< typename Archive >
        void load( Archive& file, MIL_IDManager::T_IDManagerMap& map, const uint )
        {
            uint nNbr;
            file >> nNbr;
            while ( nNbr-- )
            {
                uint nID;
                file >> nID;

                MIL_MOSIDManager* pIDManager = map[ nID ];
                pIDManager->serialize( file );
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: template< typename Archive > void MIL_AgentServer::SerializeStatics
// Created: NLD 2005-11-04
// -----------------------------------------------------------------------------
template< typename Archive > 
void MIL_IDManager::serialize( Archive& file )
{
    units_                              .serialize( file );
    fireResultsPion_                    .serialize( file );
    fireResultsPopulation_              .serialize( file );
    limas_                              .serialize( file );
    limits_                             .serialize( file );
    orders_                             .serialize( file );
    knowledgesAgent_                    .serialize( file );
    effectsWeather_                     .serialize( file );
    maintenanceComposanteStates_        .serialize( file );
    medicalHumanStates_                 .serialize( file );
    supplyStates_                       .serialize( file );
    populations_                        .serialize( file );
    populationConcentrations_           .serialize( file );
    populationFlows_                    .serialize( file );
    knowledgePopulations_               .serialize( file );
    knowledgePopulationConcentrations_  .serialize( file );
    knowledgePopulationFlows_           .serialize( file );

    file & objectClassIDToType_;
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
