
// Created: JVT 02-06-28
//*****************************************************************************

#include "simulation_kernel_pch.h"

#include "DEC_Workspace.h"

#include "DEC_Model.h"

#include "Entities/Populations/DEC_PopulationDecision.h"
#include "Entities/Automates/DEC_AutomateDecision.h"
#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"
#include "Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h"
#include "Entities/Orders/MIL_PionMissionType.h"
#include "Entities/Orders/MIL_PionMission.h"
#include "Entities/Orders/MIL_AutomateMissionType.h"
#include "Entities/Orders/MIL_AutomateMission.h"
#include "Entities/Orders/MIL_PopulationMissionType.h"
#include "Entities/Orders/MIL_PopulationMission.h"
#include "Entities/Orders/MIL_FragOrderType.h"
#include "Entities/Orders/MIL_FragOrder.h"
#include "simulation_orders/MIL_ParameterType_ABC.h"
#include "Decision/Knowledge/DEC_Rep_PathPoint.h"
#include "Decision/Knowledge/DEC_Rep_PathPoint_Front.h"
#include "Decision/Knowledge/DEC_Rep_PathPoint_Special.h"
#include "Decision/Knowledge/DEC_Rep_PathPoint_Lima.h"
#include "Decision/Functions/DEC_GeometryFunctions.h"
#include "Decision/Functions/DEC_DIAFunctions.h"
#include "Decision/Functions/DEC_LogisticFunctions.h"
#include "Decision/Functions/DEC_ObjectFunctions.h"
#include "Decision/Functions/DEC_OrdersFunctions.h"
#include "Decision/Functions/DEC_PathFunctions.h"
#include "Decision/DEC_Tools.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/DEC_Knowledge_RapFor_ABC.h"
#include <xeumeuleu/xml.h>
#include "tools/InputBinaryStream.h"
#include <boost/filesystem/operations.hpp>


#include <sys/stat.h>


namespace
{
    static const std::string strUnits( "units" );
    static const std::string strAutomats( "automats" );
    static const std::string strPopulation( "populations" );
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
    // $$$$ NLD 2007-01-11: A DEPLACER
    xml::xifstream xis( config.GetPhysicalFile() );

    std::string strDecFile;
    xis >> xml::start( "physical" )
            >> xml::start( "decisional" )
                >> xml::attribute( "file", strDecFile )
            >> xml::end()
        >> xml::end();

    strDecFile = config.BuildPhysicalChildFile( strDecFile );

    xml::xifstream xisDecisional( strDecFile );
    config.AddFileToCRC( strDecFile );

    uint nTmp;

    xisDecisional >> xml::start( "decisonal" )
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

    xisDecisional     >> xml::end()
                          >> xml::start( "operational-state-weights" )
                              >> xml::attribute( "component", PHY_RoleInterface_Composantes::rOpStateWeightNonMajorComposante_ )
                              >> xml::attribute( "major-component", PHY_RoleInterface_Composantes::rOpStateWeightMajorComposante_ )
                              >> xml::attribute( "crew", PHY_ComposantePion::rOpStateWeightHumans_ )
                          >> xml::end();

    if( PHY_RoleInterface_Composantes::rOpStateWeightNonMajorComposante_ < 0 || PHY_RoleInterface_Composantes::rOpStateWeightNonMajorComposante_ > 1 )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "operational-state-weights: component not in [0..1]" );
    if( PHY_RoleInterface_Composantes::rOpStateWeightMajorComposante_ < 0 || PHY_RoleInterface_Composantes::rOpStateWeightMajorComposante_ > 1 )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "operational-state-weights: major-component not in [0..1]" );
    if( PHY_ComposantePion::rOpStateWeightHumans_ < 0 || PHY_ComposantePion::rOpStateWeightHumans_ > 1 )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "operational-state-weights: crew not in [0..1]" );

    if( PHY_RoleInterface_Composantes::rOpStateWeightMajorComposante_ + PHY_RoleInterface_Composantes::rOpStateWeightNonMajorComposante_ != 1. )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Sum of 'Decisionnel::EtatOps::PoidsComposantesMajeures', 'PoidsComposantesMajeures' and 'PoidsPersonnel' != 1" ); // $$$$ ABL 2007-07-25: error context

    DEC_Knowledge_RapFor_ABC::Initialize( xisDecisional );

    xisDecisional >> xml::end();
}

//-----------------------------------------------------------------------------
// Name: DEC_Workspace::InitializeDIA
// Created: JVT 02-07-08
// Last modified: JVT 03-11-24
//-----------------------------------------------------------------------------
void DEC_Workspace::InitializeDIA( MIL_Config& config )
{
    //DIA4
    const std::string strDecFile = config.GetDecisionalFile();

    xml::xifstream xis( strDecFile );
    config.AddFileToCRC( strDecFile );
    xis >> xml::start( "decisional" );
    std::string strSourcePath;
    xis >> xml::content( "RepertoireSources" , strSourcePath );
    strSourcePath = config.BuildDecisionalChildFile( strSourcePath );

    MIL_ParameterType_ABC    ::Initialize   ();

    InitializeMissions( config );
    InitializeModels  ( config, strSourcePath );

    // Debugger
    if( config.UseDiaDebugger() )
    {
        MT_LOG_INFO_MSG( MT_FormatString( "Starting DirectIA debug server on port %d", config.GetDiaDebuggerPort() ) );
    }

    //BM
    if( xis.has_child( "BMDatabase" ) )
    {
        xis >> xml::start( "BMDatabase" );
        dataBase_.reset( new DEC_DataBase( xis, strSourcePath ) );
        xis >> xml::end();
    }
    else
    {
        dataBase_.reset( new DEC_DataBase( std::vector< std::string >(), std::vector< const std::string >() ) );
    }

}

// -----------------------------------------------------------------------------
// Name: DEC_Workspace::InitializeMissions
// Created: NLD 2006-11-24
// -----------------------------------------------------------------------------
void DEC_Workspace::InitializeMissions( MIL_Config& config )
{
    xml::xifstream xis( config.GetPhysicalFile() );

    std::string strMissionsFile;
    xis >> xml::start( "physical" )
            >> xml::start( "missions" )
               >> xml::attribute( "file", strMissionsFile )
            >> xml::end();

    strMissionsFile = config.BuildPhysicalChildFile( strMissionsFile );

    xml::xifstream xisMission( strMissionsFile );

    config.AddFileToCRC( strMissionsFile );

    MIL_PionMissionType      ::Initialize( xisMission );
    MIL_AutomateMissionType  ::Initialize( xisMission );
    MIL_PopulationMissionType::Initialize( xisMission );
    MIL_FragOrderType        ::Initialize( xisMission );
}

// -----------------------------------------------------------------------------
// Name: DEC_Workspace::InitializeModels
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
void DEC_Workspace::InitializeModels( MIL_Config& config, const std::string& strSourcePath )
{
    xml::xifstream xis( config.GetPhysicalFile() );

    std::string strModelsFile;
    xis >> xml::start( "physical" )
            >> xml::start( "models" )
                >> xml::attribute( "file", strModelsFile )
            >> xml::end()
        >> xml::end();

    strModelsFile = config.BuildPhysicalChildFile( strModelsFile );

    xml::xifstream xisModels( strModelsFile );

    config.AddFileToCRC( strModelsFile );

    xisModels >> xml::start( "models" );

    // Pions
    MT_LOG_INFO_MSG( "Initializing unit DIA models" );
    xisModels >> xml::start( "units" )
                >> xml::list( "unit", *this, &DEC_Workspace::ReadModel, strSourcePath, strUnits, MIL_PionMissionType::MissionNames() )
              >> xml::end();

    // Automates
    MT_LOG_INFO_MSG( "Initializing automat DIA models" );
    xisModels >> xml::start( "automats" )
                  >> xml::list( "automat", *this, &DEC_Workspace::ReadModel, strSourcePath, strAutomats, MIL_AutomateMissionType::MissionNames() )
              >> xml::end();

    // Populations
    MT_LOG_INFO_MSG( "Initializing population DIA models" );
    xisModels >> xml::start( "populations" )
                  >> xml::list( "population", *this, &DEC_Workspace::ReadModel, strSourcePath, strPopulation, MIL_PopulationMissionType::MissionNames() )
              >> xml::end();

    xisModels >> xml::end(); // models
}

// -----------------------------------------------------------------------------
// Name: DEC_Workspace::ReadModel
// Created: RDS 2008-05-21
// -----------------------------------------------------------------------------
void DEC_Workspace::ReadModel( xml::xistream& xis, const std::string& strSourcePath, const std::string& strEntityType, const T_MissionTypeNameMap& missionTypes )
{
    std::string strName;
    xis >> xml::attribute( "name", strName );
    
    T_ModelMap* pModels = modelTypes_[strEntityType];
    if( !pModels )
        xis.error( "Unknown model type" );
    const DEC_Model_ABC*& pModel = (*pModels)[ strName ];
    if( pModel )
        xis.error( "Duplicate model name" );
    pModel = new DEC_Model( strName, xis, strSourcePath, strEntityType, missionTypes );
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
    xml::xifstream xis( config.GetPhysicalFile() );
    std::string strFile;
    xis >> xml::start( "physical" )
            >> xml::start( "object-names" )
                >> xml::attribute( "file", strFile )
            >> xml::end()
        >> xml::end();

    strFile = config.BuildPhysicalChildFile( strFile );

    xml::xifstream xisObjectNames( strFile );
    DEC_ObjectFunctions::RegisterObjectNames( xisObjectNames );
}

// -----------------------------------------------------------------------------
// Name: DEC_Workspace::InitKnowledges
// Created: MGD 2009-10-23
// -----------------------------------------------------------------------------
void DEC_Workspace::InitKnowledges( directia::Brain& brain ) const
{
    if( dataBase_.get() )
        dataBase_->InitKnowledges( brain );
}

// -----------------------------------------------------------------------------
// Name: DEC_Workspace::GetBMDatabase
// Created: MGD 2009-10-27
// -----------------------------------------------------------------------------
DEC_DataBase& DEC_Workspace::GetDatabase() const
{
    return *dataBase_;
}