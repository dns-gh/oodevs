
// Created: JVT 02-06-28
//*****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Workspace.h"
#include "DEC_Model.h"
#include "Entities/Populations/DEC_PopulationDecision.h"
#include "Entities/Agents/Actions/Firing/DirectFiring/PHY_DirectFireData.h"
#include "Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h"
#include "Entities/Orders/MIL_PionMissionType.h"
#include "Entities/Orders/MIL_AutomateMissionType.h"
#include "Entities/Orders/MIL_AutomateMission.h"
#include "Entities/Orders/MIL_PopulationMissionType.h"
#include "Entities/Orders/MIL_PopulationMission.h"
#include "Entities/Orders/MIL_FragOrderType.h"
#include "Entities/Orders/MIL_FragOrder.h"
#include "Entities/Orders/MIL_ParameterType_ABC.h"
#include "Decision/DEC_Rep_PathPoint.h"
#include "Decision/DEC_Rep_PathPoint_Front.h"
#include "Decision/DEC_Rep_PathPoint_Special.h"
#include "Decision/DEC_Rep_PathPoint_Lima.h"
#include "Decision/DEC_GeometryFunctions.h"
#include "Decision/DEC_DIAFunctions.h"
#include "Decision/DEC_LogisticFunctions.h"
#include "Decision/DEC_ObjectFunctions.h"
#include "Decision/DEC_OrdersFunctions.h"
#include "Decision/DEC_PathFunctions.h"
#include "Decision/DEC_Tools.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/DEC_Knowledge_RapFor_ABC.h"
#include "Tools/InputBinaryStream.h"
#include "Tools/Loader_ABC.h"
#include "Tools/xmlcodecs.h"
#include "Tools/MIL_AffinitiesMap.h"
#include "MT_Tools/MT_ScipioException.h"
#include "MT_Tools/MT_FormatString.h"
#include <xeumeuleu/xml.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/bind.hpp>
#include <sys/stat.h>

namespace
{
    const std::string strUnits( "units" );
    const std::string strAutomats( "automats" );
    const std::string strPopulation( "populations" );
}

//-----------------------------------------------------------------------------
// Name: DEC_Workspace constructor
// Created: JVT 02-06-28
// Last modified: JVT 02-09-16
//-----------------------------------------------------------------------------
DEC_Workspace::DEC_Workspace( MIL_Config& config )
{
    MT_LOG_INFO_MSG( "Initializing decision" );
    modelTypes_[strUnits] = &pionModels_;
    modelTypes_[strAutomats] = &automateModels_;
    modelTypes_[strPopulation] = &populationModels_;
    InitializeConfig( config );
    InitializeObjectNames( config );
    InitializeDIA   ( config );
}

//-----------------------------------------------------------------------------
// Name: DEC_Workspace destructor
// Created: JVT 02-07-02
// Last modified: AGN 02-11-05
//-----------------------------------------------------------------------------
DEC_Workspace::~DEC_Workspace()
{
    for( CIT_ModelMap it = automateModels_.begin(); it != automateModels_.end(); ++it )
        delete it->second;
    for( CIT_ModelMap it = pionModels_.begin(); it != pionModels_.end(); ++it )
        delete it->second;
    for( CIT_ModelMap it = populationModels_.begin(); it != populationModels_.end(); ++it )
        delete it->second;
}

// -----------------------------------------------------------------------------
// Name: DEC_Workspace::InitializeConfig
// Created: NLD 2004-06-14
// -----------------------------------------------------------------------------
void DEC_Workspace::InitializeConfig( MIL_Config& config )
{
    const std::string fileLoaded = config.GetLoader().LoadPhysicalFile( "decisional", boost::bind( &DEC_Workspace::LoadDecisional, this, _1 ) );
    config.AddFileToCRC( fileLoaded );
}

// -----------------------------------------------------------------------------
// Name: DEC_Workspace::LoadDecisional
// Created: LDC 2010-12-01
// -----------------------------------------------------------------------------
void DEC_Workspace::LoadDecisional( xml::xistream& xisDecisional )
{
    unsigned int nTmp;

    xisDecisional >> xml::start( "decisional" )
                      >> xml::start( "dangerosity-modifiers" )
                          >> xml::attribute( "max-accuracy", nTmp );
    if( nTmp < 0 || nTmp > 100 )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "dangerosity-modifiers: max-accuracy not in [0..100]" );
    DEC_Knowledge_Agent::rMaxDangerosityDegradationByRelevance_ = nTmp / 100.;

    xisDecisional >> xml::attribute( "max-operational-state", nTmp );
    if( nTmp < 0 || nTmp > 100 )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "dangerosity-modifiers: max-operationnal-state not in [0..100]" );
    DEC_Knowledge_Agent     ::rMaxDangerosityDegradationByOpState_ = nTmp / 100.;
    PHY_RoleInterface_Composantes::rMaxDangerosityDegradationByOpState_ = nTmp / 100.;

    xisDecisional >> xml::attribute( "max-neutralized-state", nTmp );
    if( nTmp < 0 || nTmp > 100 )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "dangerousity-modifiers: max-neutralized-state not in [0..100]" );
    DEC_Knowledge_Agent     ::rMaxDangerosityDegradationByNeutralizedState_ = nTmp / 100.;
    PHY_RoleInterface_Composantes::rMaxDangerosityDegradationByNeutralizedState_ = nTmp / 100.;

    xisDecisional >> xml::end
                  >> xml::start( "operational-state-weights" )
                      >> xml::attribute( "component", PHY_RoleInterface_Composantes::rOpStateWeightNonMajorComposante_ )
                      >> xml::attribute( "major-component", PHY_RoleInterface_Composantes::rOpStateWeightMajorComposante_ )
                      >> xml::attribute( "crew", PHY_ComposantePion::rOpStateWeightHumans_ )
                  >> xml::end;

    xisDecisional >> xml::optional() >> xml::start( "urban-combat" )
                      >> xml::attribute( "hit-factor", firing::PHY_DirectFireData::nUrbanCoefficient_ )
                  >> xml::end;

    xisDecisional >> xml::start( "critical-intelligence-delay" );
    tools::ReadTimeAttribute( xisDecisional, "minimum-affinity", MIL_AffinitiesMap::interrogateDelayForMinimumAffinity_ );
    tools::ReadTimeAttribute( xisDecisional, "neutral-affinity", MIL_AffinitiesMap::interrogateDelayForNeutralAffinity_ );
    tools::ReadTimeAttribute( xisDecisional, "maximum-affinity", MIL_AffinitiesMap::interrogateDelayForMaximumAffinity_ );
    xisDecisional >> xml::end;

    if( PHY_RoleInterface_Composantes::rOpStateWeightNonMajorComposante_ < 0 || PHY_RoleInterface_Composantes::rOpStateWeightNonMajorComposante_ > 1 )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "operational-state-weights: component not in [0..1]" );
    if( PHY_RoleInterface_Composantes::rOpStateWeightMajorComposante_ < 0 || PHY_RoleInterface_Composantes::rOpStateWeightMajorComposante_ > 1 )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "operational-state-weights: major-component not in [0..1]" );
    if( PHY_ComposantePion::rOpStateWeightHumans_ < 0 || PHY_ComposantePion::rOpStateWeightHumans_ > 1 )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "operational-state-weights: crew not in [0..1]" );

    if( PHY_RoleInterface_Composantes::rOpStateWeightMajorComposante_ + PHY_RoleInterface_Composantes::rOpStateWeightNonMajorComposante_ != 1. )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Sum of 'Decisionnel::EtatOps::PoidsComposantesMajeures', 'PoidsComposantesMajeures' and 'PoidsPersonnel' != 1" ); // $$$$ ABL 2007-07-25: error context

    DEC_Knowledge_RapFor_ABC::Initialize( xisDecisional );

    xisDecisional >> xml::end;
}

//-----------------------------------------------------------------------------
// Name: DEC_Workspace::InitializeDIA
// Created: JVT 02-07-08
// Last modified: JVT 03-11-24
//-----------------------------------------------------------------------------
void DEC_Workspace::InitializeDIA( MIL_Config& config )
{
    //DIA4
    config.GetLoader().LoadFile( config.GetDecisionalFile(), boost::bind( &DEC_Workspace::LoadDIA, this, boost::ref( config ), _1 ) );
    config.AddFileToCRC( config.GetDecisionalFile() );
}

//-----------------------------------------------------------------------------
// Name: DEC_Workspace::InitializeDIA
// Created: JVT 02-07-08
// Last modified: JVT 03-11-24
//-----------------------------------------------------------------------------
void DEC_Workspace::LoadDIA( MIL_Config& config, xml::xistream& xis )
{
    xis >> xml::start( "decisional" );
    std::map< std::string, std::string > strSourcePaths;
    xis >> xml::start( "RepertoiresSources" )
            >> xml::list( "RepertoireSources" , *this, &DEC_Workspace::RegisterSourcePath, config, strSourcePaths )
        >> xml::end;

    MIL_ParameterType_ABC::Initialize();

    InitializeMissions( config );
    InitializeModels  ( config, strSourcePaths );

    // Debugger
    if( config.UseDiaDebugger() )
        MT_LOG_INFO_MSG( MT_FormatString( "Starting DirectIA debug server on port %d", config.GetDiaDebuggerPort() ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_Workspace::RegisterSourcePath
// Created: MGD 2010-03-17
// -----------------------------------------------------------------------------
void DEC_Workspace::RegisterSourcePath( xml::xistream& xis, MIL_Config& config, std::map< std::string, std::string >& paths )
{
    std::string nameSpace, dir;
    xis >> xml::attribute( "namespace", nameSpace )
        >> xml::attribute( "directory", dir );

    paths.insert( std::pair< std::string, std::string >( nameSpace, config.BuildDecisionalChildFile( dir ) ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_Workspace::InitializeMissions
// Created: NLD 2006-11-24
// -----------------------------------------------------------------------------
void DEC_Workspace::InitializeMissions( MIL_Config& config )
{
    const std::string fileLoaded = config.GetLoader().LoadPhysicalFile( "missions", boost::bind( &DEC_Workspace::LoadMissions, this, _1 ) );
    config.AddFileToCRC( fileLoaded );
}

// -----------------------------------------------------------------------------
// Name: DEC_Workspace::LoadMissions
// Created: LDC 2010-12-02
// -----------------------------------------------------------------------------
void DEC_Workspace::LoadMissions( xml::xistream& xisMission )
{
    MIL_PionMissionType      ::Initialize( xisMission );
    MIL_AutomateMissionType  ::Initialize( xisMission );
    MIL_PopulationMissionType::Initialize( xisMission );
    MIL_FragOrderType        ::Initialize( xisMission );
}

// -----------------------------------------------------------------------------
// Name: DEC_Workspace::InitializeModels
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
void DEC_Workspace::InitializeModels( MIL_Config& config, const std::map< std::string, std::string >& strSourcePaths )
{
    const std::string fileLoaded = config.GetLoader().LoadPhysicalFile( "models", boost::bind( &DEC_Workspace::LoadModels, this, _1, boost::cref( strSourcePaths ) ) );
    config.AddFileToCRC( fileLoaded );

}

// -----------------------------------------------------------------------------
// Name: DEC_Workspace::LoadModels
// Created: LDC 2010-12-02
// -----------------------------------------------------------------------------
void DEC_Workspace::LoadModels( xml::xistream& xisModels, const std::map< std::string, std::string >& strSourcePaths )
{
    xisModels >> xml::start( "models" );

    // Pions
    MT_LOG_INFO_MSG( "Initializing unit DIA models" );
    xisModels >> xml::start( "units" )
                >> xml::list( "unit", *this, &DEC_Workspace::ReadModel, strSourcePaths, strUnits, MIL_PionMissionType::MissionNames() )
              >> xml::end;

    // Automates
    MT_LOG_INFO_MSG( "Initializing automat DIA models" );
    xisModels >> xml::start( "automats" )
                  >> xml::list( "automat", *this, &DEC_Workspace::ReadModel, strSourcePaths, strAutomats, MIL_AutomateMissionType::MissionNames() )
              >> xml::end;

    // Populations
    MT_LOG_INFO_MSG( "Initializing population DIA models" );
    xisModels >> xml::start( "crowd" )
                  >> xml::list( "crowd", *this, &DEC_Workspace::ReadModel, strSourcePaths, strPopulation, MIL_PopulationMissionType::MissionNames() )
              >> xml::end;

    xisModels >> xml::end; // models
}

// -----------------------------------------------------------------------------
// Name: DEC_Workspace::ReadModel
// Created: RDS 2008-05-21
// -----------------------------------------------------------------------------
void DEC_Workspace::ReadModel( xml::xistream& xis, const std::map< std::string, std::string >& strSourcePaths, const std::string& strEntityType, const T_MissionTypeNameMap& missionTypes )
{
    std::string strName;
    xis >> xml::attribute( "name", strName );

    T_ModelMap* pModels = modelTypes_[strEntityType];
    if( !pModels )
        xis.error( "Unknown model type" );
    const DEC_Model_ABC*& pModel = (*pModels)[ strName ];
    if( pModel )
        xis.error( "Duplicate model name" );

    bool isMasalife = false;
    xis >> xml::attribute( "masalife", isMasalife );
    //Key is net.masagroup for masalife, none for others.
    std::string key = isMasalife ? "net.masagroup" : "none";

    const std::map< std::string, std::string >::const_iterator itFind = strSourcePaths.find( key );
    if( itFind == strSourcePaths.end() )
        xis.error( "Model corresponds to an unknown namespace" );

    pModel = new DEC_Model( strName, xis, itFind->second, missionTypes, isMasalife );
}

//-----------------------------------------------------------------------------
// Name: DEC_Workspace::GetTime
// Created: JVT 02-06-28
//-----------------------------------------------------------------------------
float DEC_Workspace::GetTime() const
{
    return (float)MIL_AgentServer::GetWorkspace().GetSimTime();
}

// -----------------------------------------------------------------------------
// Name: DEC_Workspace::InitializeObjectNames
// Created: LDC 2009-05-25
// -----------------------------------------------------------------------------
void DEC_Workspace::InitializeObjectNames( MIL_Config& config )
{
    const std::string fileLoaded = config.GetLoader().LoadPhysicalFile( "object-names", &DEC_ObjectFunctions::RegisterObjectNames );
    config.AddFileToCRC( fileLoaded );
}