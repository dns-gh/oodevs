// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
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

#include "simulation_kernel_pch.h"
#include "MIL_IDManager.h"

#include "MIL_AgentServer.h"
#include "xeumeuleu/xml.h"

using namespace xml;

MIL_MOSIDManager                 MIL_IDManager::units_;
MIL_MOSIDManager                 MIL_IDManager::fireResultsPion_;
MIL_MOSIDManager                 MIL_IDManager::fireResultsPopulation_;
MIL_MOSIDManager                 MIL_IDManager::limas_;
MIL_MOSIDManager                 MIL_IDManager::limits_;
MIL_MOSIDManager                 MIL_IDManager::knowledgesAgent_;
MIL_MOSIDManager                 MIL_IDManager::effectsWeather_;
MIL_MOSIDManager                 MIL_IDManager::maintenanceComposanteStates_;
MIL_MOSIDManager                 MIL_IDManager::medicalHumanStates_;
MIL_MOSIDManager                 MIL_IDManager::supplyStates_;
MIL_MOSIDManager                 MIL_IDManager::populations_;
MIL_MOSIDManager                 MIL_IDManager::populationConcentrations_;
MIL_MOSIDManager                 MIL_IDManager::populationFlows_;
MIL_MOSIDManager                 MIL_IDManager::knowledgePopulations_;
MIL_MOSIDManager                 MIL_IDManager::knowledgePopulationConcentrations_;
MIL_MOSIDManager                 MIL_IDManager::knowledgePopulationFlows_;
MIL_IDManager::T_ObjectIDManager MIL_IDManager::objectNameToType_;
MIL_IDManager::T_IDManagerMap    MIL_IDManager::objectClassIDToType_;

struct MIL_IDManager::LoadingWrapper
{
    void ReadClasse( xml::xistream& xis )
    {
        MIL_IDManager::ReadClasse( xis );
    }
};

//-----------------------------------------------------------------------------
// Name: MIL_IDManager::Initialize
// Created: JVT 03-02-27
// Last modified: JVT 04-03-29
//-----------------------------------------------------------------------------
void MIL_IDManager::Initialize( MIL_Config& config )
{
    MT_LOG_INFO_MSG( "Initializing classe ids" );

    xml::xifstream xisPhysical( config.GetPhysicalFile() );
    xisPhysical >> start( "physical" );

    std::string strName;
    xisPhysical >> start( "classeids" )
                    >> attribute( "file", strName )
                >> end();

    xml::xifstream  xisClasse( config.BuildPhysicalChildFile( strName ) );

    config.AddFileToCRC( strName );

    LoadingWrapper loader;

    xisClasse >> start( "Classes" )
                  >> list( "Classe", loader, &LoadingWrapper::ReadClasse )
              >> end();
}

// -----------------------------------------------------------------------------
// Name: MIL_IDManager::ReadClasse
// Created: ABL 2007-07-27
// -----------------------------------------------------------------------------
void MIL_IDManager::ReadClasse( xml::xistream& xis )
{
    uint nClassID;
    std::string strName;
    xis >> attribute( "id", nClassID )
        >> attribute( "nom", strName );

            if( sCaseInsensitiveEqual()( strName, "Unite"                                  ) )  units_                             .SetClassID( nClassID );
    else if( sCaseInsensitiveEqual()( strName, "TirPion"                                ) )  fireResultsPion_                   .SetClassID( nClassID );
    else if( sCaseInsensitiveEqual()( strName, "TirPopulation"                          ) )  fireResultsPopulation_             .SetClassID( nClassID );
    else if( sCaseInsensitiveEqual()( strName, "Lima"                                   ) )  limas_                             .SetClassID( nClassID );
    else if( sCaseInsensitiveEqual()( strName, "Limite"                                 ) )  limits_                            .SetClassID( nClassID );
    else if( sCaseInsensitiveEqual()( strName, "GhostAgent"                             ) )  knowledgesAgent_                   .SetClassID( nClassID );
    else if( sCaseInsensitiveEqual()( strName, "EffetsTirs"                             ) )  effectsWeather_                    .SetClassID( nClassID );
    else if( sCaseInsensitiveEqual()( strName, "TraitementLogMaintenance"               ) )  maintenanceComposanteStates_       .SetClassID( nClassID );
    else if( sCaseInsensitiveEqual()( strName, "TraitementLogSante"                     ) )  medicalHumanStates_                .SetClassID( nClassID );
    else if( sCaseInsensitiveEqual()( strName, "TraitementLogRavitaillement"            ) )  supplyStates_                      .SetClassID( nClassID );
    else if( sCaseInsensitiveEqual()( strName, "Population"                             ) )  populations_                       .SetClassID( nClassID );
    else if( sCaseInsensitiveEqual()( strName, "PopulationConcentration"                ) )  populationConcentrations_          .SetClassID( nClassID );
    else if( sCaseInsensitiveEqual()( strName, "PopulationFlux"                         ) )  populationFlows_                   .SetClassID( nClassID );
    else if( sCaseInsensitiveEqual()( strName, "ConnaissancePopulation"                 ) )  knowledgePopulations_              .SetClassID( nClassID );
    else if( sCaseInsensitiveEqual()( strName, "ConnaissancePopulationConcentration"    ) )  knowledgePopulationConcentrations_ .SetClassID( nClassID );
    else if( sCaseInsensitiveEqual()( strName, "ConnaissancePopulationFlux"             ) )  knowledgePopulationFlows_          .SetClassID( nClassID );
    else // Objects
    {
        MIL_MOSIDManager*& pIDManager = objectClassIDToType_[ nClassID ];
        if( !pIDManager )
        {
            pIDManager = new MIL_MOSIDManager();
            pIDManager->SetClassID( nClassID );
        }
        objectNameToType_[ strName ] = pIDManager;
    }
}

