
// Created: JVT 02-06-28
//*****************************************************************************

#include "simulation_kernel_pch.h"

#include "DEC_Workspace.h"

#include "DEC_ModelPion.h"
#include "DEC_ModelAutomate.h"
#include "DEC_ModelPopulation.h"

#include "Entities/Populations/DEC_PopulationDecision.h"
#include "Entities/Automates/DEC_AutomateDecision.h"
#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Orders/MIL_PionMissionType.h"
#include "Entities/Orders/MIL_PionMission.h"
#include "Entities/Orders/MIL_AutomateMissionType.h"
#include "Entities/Orders/MIL_AutomateMission.h"
#include "Entities/Orders/MIL_PopulationMissionType.h"
#include "Entities/Orders/MIL_PopulationMission.h"
#include "Entities/Orders/MIL_FragOrderType.h"
#include "Entities/Orders/MIL_FragOrder.h"
#include "Entities/Orders/MIL_ParameterType_ABC.h"
#include "Decision/Knowledge/DEC_Rep_PathPoint.h"
#include "Decision/Knowledge/DEC_Rep_PathPoint_Front.h"
#include "Decision/Knowledge/DEC_Rep_PathPoint_Special.h"
#include "Decision/Knowledge/DEC_Rep_PathPoint_Lima.h"
#include "Decision/Functions/DEC_GeometryFunctions.h"
#include "Decision/Functions/DEC_DIAFunctions.h"
#include "Decision/Functions/DEC_LogisticFunctions.h"
#include "Decision/Functions/DEC_ObjectFunctions.h"
#include "Decision/DEC_Tools.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/DEC_Knowledge_RapFor_ABC.h"
#include "DIA/DIA_Tool_Archive_lib.h"
#include "DIA/DIA_SDK_Manager.h"

#include <sys/stat.h>

//-----------------------------------------------------------------------------
// Name: DEC_Workspace constructor
// Created: JVT 02-06-28
// Last modified: JVT 02-09-16
//-----------------------------------------------------------------------------
DEC_Workspace::DEC_Workspace( MIL_Config& config )
    : pFuncTable_     ( 0 )
    , pFunctionCaller_( 0 )
{
    MT_LOG_INFO_MSG( "Initializing decision" );
    DIA_SDK_Manager::InitializeSDK();  

    InitializeConfig( config );
    InitializeDIA   ( config );
}

//-----------------------------------------------------------------------------
// Name: DEC_Workspace destructor
// Created: JVT 02-07-02
// Last modified: AGN 02-11-05
//-----------------------------------------------------------------------------
DEC_Workspace::~DEC_Workspace()
{
    DIA_SDK_Manager::TerminateSDK();

    delete pFuncTable_;
    delete pFunctionCaller_;
    for( CIT_ModelPionMap itModelPion = pionModels_.begin(); itModelPion != pionModels_.end(); ++itModelPion )
        delete itModelPion->second;
    for( CIT_ModelAutomateMap itModelAutomate = automateModels_.begin(); itModelAutomate != automateModels_.end(); ++itModelAutomate )
        delete itModelAutomate->second;
    for( CIT_ModelPopulationMap itModelPopulation = populationModels_.begin(); itModelPopulation != populationModels_.end(); ++itModelPopulation )
        delete itModelPopulation->second;
}

//-----------------------------------------------------------------------------
// Name: DEC_Workspace::RegisterDIA_Functions
// Created: AGN 03-01-30
//-----------------------------------------------------------------------------
// static
void DEC_Workspace::RegisterDIA_Functions( DIA_FunctionTable< DEC_Workspace >* pFuncTable )
{
    // Geometry
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::SplitListPoints               , "DEC_Geometrie_DecouperListePoints"            );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::SplitPath                     , "DEC_Geometrie_DecouperItineraire"             );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::ComputeCoverPosition          , "DEC_Geometrie_CalculerPositionCouverture"     );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::ComputeAgentsBarycenter       , "DEC_Geometrie_CalculerBarycentreAgents"       );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::TranslatePosition             , "DEC_Geometrie_PositionTranslate"              );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::TranslatePositionInDirection  , "DEC_Geometrie_PositionTranslateDir"           );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::ComparePositions              , "DEC_Geometrie_PositionsEgales"                );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::CompareDirection              , "DEC_Geometrie_DirectionEgales"                );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::Distance                      , "DEC_Geometrie_Distance"                       );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::ConvertPointToLocalisation    , "DEC_Geometrie_ConvertirPointEnLocalisation"   );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::IsPointInsideLocalisation     , "DEC_Geometrie_EstPointDansLocalisation"       );    
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::CompareLocalisations          , "DEC_Geometrie_LocalisationsEgales"            );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::CreateLocalisation            , "DEC_Geometrie_CreerLocalisation"              );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::CreateListPoint               , "DEC_Geometrie_CreerListePoints"               );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::CreatePoint                   , "DEC_Geometrie_CreerPoint"                     );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::CreateDirection               , "DEC_Geometrie_CreerDirection"                 );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::CreateOrthoDirection          , "DEC_Geometrie_CreerDirectionPerpendiculaire"  );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::ReverseDirection              , "DEC_Geometrie_InverseDirection"               );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::RotateDirection               , "DEC_Geometrie_RotateDirection"                );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::CopyAndReverseDirection       , "DEC_Geometrie_CopieEtInverseDirection"        );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::CopyAndRotateDirection        , "DEC_Geometrie_CopieEtRotateDirection"         );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::ComputeDistanceFromMiddleLine , "DEC_Geometrie_CalculerDistanceLigneMoyenne"   );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::ComputeLocalisationBarycenter , "DEC_Geometrie_CalculerBarycentreLocalisation" );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::ComputeMeanDirection          , "DEC_Geometrie_DirectionMoyenne"               );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::ComputeRandomPointOnCircle    , "DEC_Geometrie_PositionAleatoireSurCercle"     );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::ComputeRandomPointInCircle    , "DEC_Geometrie_PositionAleatoireDansCercle"    );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::CreateCircleLocalisation      , "DEC_Geometrie_CreerLocalisationCercle"        );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::IsPionCoordinated             , "DEC_Geometrie_PionEstCoordonne"               );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::ComputeLocalisationArea       , "DEC_Geometrie_AireLocalisation"               );
    
    // Time management
    pFuncTable->RegisterFunction( DEC_DIAFunctions::GetSimTime         , "DEC_TempsSim"        );
    pFuncTable->RegisterFunction( DEC_DIAFunctions::GetRealTime        , "DEC_TempsReel"       );
    pFuncTable->RegisterFunction( DEC_DIAFunctions::GetTimeStepDuration, "DEC_DureePasDeTemps" );
    pFuncTable->RegisterFunction( DEC_DIAFunctions::IsNight            , "DEC_Nuit"            );

    // Parameters copy
    pFuncTable->RegisterFunction( DEC_DIAFunctions::CopyDirection                     , "DEC_Copie_Direction"                          );
    pFuncTable->RegisterFunction( DEC_DIAFunctions::CopyPoint                         , "DEC_Copie_Point"                              );
    pFuncTable->RegisterFunction( DEC_DIAFunctions::CopyListPoint                     , "DEC_Copie_ListePoints"                        );
    pFuncTable->RegisterFunction( DEC_DIAFunctions::CopyPointToListPoint              , "DEC_Copie_PointDansListePoints"               );
    pFuncTable->RegisterFunction( DEC_DIAFunctions::CopyLocalisation                  , "DEC_Copie_Localisation"                       );
    pFuncTable->RegisterFunction( DEC_DIAFunctions::CopyPointToLocalisation           , "DEC_Copie_PointDansLocalisation"              );
    pFuncTable->RegisterFunction( DEC_DIAFunctions::CopyListLocalisation              , "DEC_Copie_ListeLocalisations"                 );
    pFuncTable->RegisterFunction( DEC_DIAFunctions::CopyLocalisationToListLocalisation, "DEC_Copie_LocalisationDansListeLocalisations" );

    // User type lists manipulation
    pFuncTable->RegisterFunction( DEC_DIAFunctions::UserTypeList_GetAt   , "DEC_UserTypeList_GetAt"     );
    pFuncTable->RegisterFunction( DEC_DIAFunctions::UserTypeList_Contains, "DEC_UserTypeList_Contient"  );
    pFuncTable->RegisterFunction( DEC_DIAFunctions::UserTypeList_PushBack, "DEC_UserTypeList_PushBack"  );
    pFuncTable->RegisterFunction( DEC_DIAFunctions::UserTypeList_Remove  , "DEC_UserTypeList_Remove"    );
        
    pFuncTable->RegisterFunction( DEC_DIAFunctions::ListPoint_GetAt      , "DEC_ListePoints_GetAt"      );
    pFuncTable->RegisterFunction( DEC_DIAFunctions::ListPoint_Size       , "DEC_ListePoints_Size"       );
    pFuncTable->RegisterFunction( DEC_DIAFunctions::ListPoint_PushBack   , "DEC_ListePoints_PushBack"   );
    pFuncTable->RegisterFunction( DEC_DIAFunctions::ListPoint_Remove     , "DEC_ListePoints_Remove"     );
    pFuncTable->RegisterFunction( DEC_DIAFunctions::ListPoint_Clear      , "DEC_ListePoints_Clear"      );
	// DIA Thing management
	pFuncTable->RegisterFunction( DEC_DIAFunctions::CreateDIAThing		 , "DEC_CreerDIAThing"    );
	pFuncTable->RegisterFunction( DEC_DIAFunctions::DestroyDIAThing		 , "DEC_DetruireDIAThing" );

    // Logistic
    pFuncTable->RegisterFunction( DEC_LogisticFunctions::HasWoundedHumansToEvacuate       , "DEC_NecessiteEvacuationBlesses"            );
    pFuncTable->RegisterFunction( DEC_LogisticFunctions::EvacuateWoundedHumansToTC2       , "DEC_EvacuerBlessesVersTC2"                 );
    pFuncTable->RegisterFunction( DEC_LogisticFunctions::ForbidWoundedHumansAutoEvacuation, "DEC_InterdireEvacuationAutomatiqueBlesses" );
    pFuncTable->RegisterFunction( DEC_LogisticFunctions::AllowWoundedHumansAutoEvacuation , "DEC_AutoriserEvacuationAutomatiqueBlesses" );

    // Objets
    pFuncTable->RegisterFunction( DEC_ObjectFunctions::CanObjectTypeBeBypassed, "DEC_TypeObjet_PeutEtreContourne" );
    pFuncTable->RegisterFunction( DEC_ObjectFunctions::CanObjectTypeBeMined   , "DEC_TypeObjet_PeutEtreValorise"  );
    pFuncTable->RegisterFunction( DEC_ObjectFunctions::CanObjectTypeBePrepared, "DEC_TypeObjet_PeutEtrePrepare"   );

    // Gen objects
    pFuncTable->RegisterFunction( DEC_ObjectFunctions::GetGenObjectType             , "DEC_GenObject_Type"               );
    pFuncTable->RegisterFunction( DEC_ObjectFunctions::GetGenObjectLocalisation     , "DEC_GenObject_Localisation"       );
    pFuncTable->RegisterFunction( DEC_ObjectFunctions::GetGenObjectDensity          , "DEC_GenObject_Densite"            );
    pFuncTable->RegisterFunction( DEC_ObjectFunctions::GetGenObjectPreliminaire     , "DEC_GenObject_Preliminaire"       );
    pFuncTable->RegisterFunction( DEC_ObjectFunctions::GetGenObjectTC2              , "DEC_GenObject_TC2"                );
    pFuncTable->RegisterFunction( DEC_ObjectFunctions::GetGenObjectMinesActivityTime, "DEC_GenObject_DelaiActiviteMines" );

    // Debug
    pFuncTable->RegisterFunction( DEC_DIAFunctions::PointToString    , "DEC_PointToString"      );
    pFuncTable->RegisterFunction( DEC_DIAFunctions::DirectionToString, "DEC_DirectionToString"  );
    pFuncTable->RegisterFunction( DEC_DIAFunctions::PathToString     , "DEC_ItineraireToString" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Workspace::InitializeConfig
// Created: NLD 2004-06-14
// -----------------------------------------------------------------------------
void DEC_Workspace::InitializeConfig( MIL_Config& config )
{
    // $$$$ NLD 2007-01-11: A DEPLACER
    MIL_InputArchive phyArchive;
    phyArchive.Open( config.GetPhysicalFile() );

    std::string strDecFile;
    phyArchive.Section( "physical" );
    phyArchive.ReadField( "Decisionnel", strDecFile );
    phyArchive.EndSection(); // physical

    strDecFile = config.BuildPhysicalChildFile( strDecFile );

    MIL_InputArchive decArchive;
    decArchive.Open( strDecFile );
    config.AddFileToCRC( strDecFile );

    decArchive.Section( "Decisionnel" );

    // Dangerosite
    decArchive.Section( "Dangerosite" );

    uint nTmp;
    decArchive.ReadField( "DegradationMaxParPertinence", nTmp, CheckValueBound( 0, 100 ) );
    DEC_Knowledge_Agent::rMaxDangerosityDegradationByRelevance_ = nTmp / 100.;
    
    decArchive.ReadField( "DegradationMaxParEtatOps", nTmp, CheckValueBound( 0, 100 ) );
    DEC_Knowledge_Agent     ::rMaxDangerosityDegradationByOpState_ = nTmp / 100.;
    PHY_RolePion_Composantes::rMaxDangerosityDegradationByOpState_ = nTmp / 100.;

    decArchive.ReadField( "DegradationMaxParEtatNeutralise", nTmp, CheckValueBound( 0, 100 ) );
    DEC_Knowledge_Agent     ::rMaxDangerosityDegradationByNeutralizedState_ = nTmp / 100.;
    PHY_RolePion_Composantes::rMaxDangerosityDegradationByNeutralizedState_ = nTmp / 100.;

    decArchive.EndSection(); // Dangerosite

    // EtatOpsEtatOps
    decArchive.Section( "EtatOps" );

    decArchive.ReadField( "PoidsComposantesNonMajeures", PHY_RolePion_Composantes::rOpStateWeightNonMajorComposante_, CheckValueBound( 0., 1. ) );
    decArchive.ReadField( "PoidsComposantesMajeures"   , PHY_RolePion_Composantes::rOpStateWeightMajorComposante_   , CheckValueBound( 0., 1. ) );
    decArchive.ReadField( "PoidsPersonnel"             , PHY_ComposantePion      ::rOpStateWeightHumans_            , CheckValueBound( 0., 1. ) );

    if( PHY_RolePion_Composantes::rOpStateWeightMajorComposante_ + PHY_RolePion_Composantes::rOpStateWeightNonMajorComposante_ != 1. )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Sum of 'Decisionnel::EtatOps::PoidsComposantesMajeures', 'PoidsComposantesMajeures' and 'PoidsPersonnel' != 1", decArchive.GetContext() );
    decArchive.EndSection(); // EtatOps

    DEC_Knowledge_RapFor_ABC::Initialize( decArchive );

    decArchive.EndSection(); // Decisionnel
}

// -----------------------------------------------------------------------------
// Name: DEC_Workspace::InitializeDIATypes
// Created: NLD 2005-04-11
// -----------------------------------------------------------------------------
void DEC_Workspace::InitializeDIATypes( MIL_InputArchive& initArchive, bool& bNeedScriptParsing, const std::string& strBinaryPath )
{
    MT_LOG_INFO_MSG( "\tReading DIA types" );

    std::string    strErrors;
    T_StringVector openedFiles;
    std::string strScript;
    initArchive.ReadField( "DIATypes", strScript );
    if( MIL_AgentServer::GetWorkspace().GetConfig().UseOnlyDIAArchive() )
    {
        if( !DIA_ReadScript_TypesBin( strScript, strBinaryPath + "/type.model", strErrors, openedFiles ) )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Error while reading type file", strErrors );
    }
    else
    {
        // Check if the script files has been modified
        std::string strOpenedFileArchiveName = strBinaryPath + "/type.files";
        bNeedScriptParsing = bNeedScriptParsing || CheckFilesDepencies( strOpenedFileArchiveName );

        if( !DIA_ReadScript_Types( strScript, strBinaryPath + "/type.model", strErrors, openedFiles ) )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, std::string( "Error while parsing types files" ) + strScript, strErrors );
        
        // Updating opened files archive
        MT_FlatBinaryOutputArchive openedFilesArchive;
        openedFilesArchive << openedFiles.size();
        for( IT_StringVector it = openedFiles.begin(); it != openedFiles.end(); ++it )
            openedFilesArchive << it->erase( 0, DIA_Workspace::Instance().GetWorkingDirectory().size() );
        openedFilesArchive.WriteToFile( strOpenedFileArchiveName, true );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Workspace::InitializeDIAWorkspace
// Created: NLD 2005-04-11
// -----------------------------------------------------------------------------
void DEC_Workspace::InitializeDIAWorkspace( MIL_InputArchive& initArchive, bool& bNeedScriptParsing, const std::string& strBinaryPath )
{ 
    MT_LOG_INFO_MSG( "\tReading DIA Workspace" );

    std::string    strErrors;
    T_StringVector openedFiles;
    std::string strScript;
    initArchive.ReadField( "DIAWorkspace", strScript );
    if( MIL_AgentServer::GetWorkspace().GetConfig().UseOnlyDIAArchive() )
    {
        if( !DIA_ReadScript_WorkspaceBin( strScript, strBinaryPath + "/workspace.model", strErrors, openedFiles ) )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Error while reading workspace file", strErrors );
    }
    else
    {
        std::string strOpenedFileArchiveName = strBinaryPath + "/workspace.files";
        bNeedScriptParsing = bNeedScriptParsing || CheckFilesDepencies( strOpenedFileArchiveName );

        if( !DIA_ReadScript_Workspace( strScript, strBinaryPath + "/workspace.model", strErrors, openedFiles ) )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, std::string( "Error while parsing workspace file " ) + strScript, strErrors );

        // Updating opened files archive
        MT_FlatBinaryOutputArchive openedFilesArchive;
        openedFilesArchive << openedFiles.size();
        for( IT_StringVector it = openedFiles.begin(); it != openedFiles.end(); ++it )
            openedFilesArchive << it->erase( 0, DIA_Workspace::Instance().GetWorkingDirectory().size() );
        openedFilesArchive.WriteToFile( strOpenedFileArchiveName, true );
    }
}

//-----------------------------------------------------------------------------
// Name: DEC_Workspace::InitializeDIA
// Created: JVT 02-07-08
// Last modified: JVT 03-11-24
//-----------------------------------------------------------------------------
void DEC_Workspace::InitializeDIA( MIL_Config& config )
{
    const std::string strDecFile = config.GetDecisionalFile();
    
    MIL_InputArchive decArchive;
    decArchive.Open( strDecFile );
    config.AddFileToCRC( strDecFile );

    decArchive.Section( "decisional" );

    // Read the archive path output
    std::string strBinaryPath;
    std::string strSourcePath;
    decArchive.ReadField( "RepertoireBinaires", strBinaryPath );
    decArchive.ReadField( "RepertoireSources" , strSourcePath );
    strBinaryPath = config.BuildDecisionalChildFile( strBinaryPath );
    strSourcePath = config.BuildDecisionalChildFile( strSourcePath );
    MT_MakeDir( strBinaryPath                  );
    MT_MakeDir( strBinaryPath + "/automats"   );
    MT_MakeDir( strBinaryPath + "/units"       );
    MT_MakeDir( strBinaryPath + "/populations" );
    MT_MakeDir( strSourcePath + "/debug"  );
    MT_MakeDir( strSourcePath + "/debug/automats"  );
    MT_MakeDir( strSourcePath + "/debug/units"       );
    MT_MakeDir( strSourcePath + "/debug/populations" );
    DIA_Workspace::Instance().SetWorkingDirectory( strSourcePath );
    MT_LOG_INFO_MSG( MT_FormatString( "DirectIA scripts sources base directory : %s", strSourcePath.c_str() ) );

    DIA_Workspace::Instance().RegisterDebugInfoGenerator( DIA_CreateDebugInfoGenerator( "/debug/workspace.ddi" ) );
    DIA_Workspace::Instance().RegisterGarbageCollector  ( DEC_Tools::ManageDeletion );
    DIA_SetParsingOptions( eParsingOption_Default );

    //$$$$$$$ NLD ??
	// test if an older workspace debug file exist
	MT_File workspaceDebugFile;
	if( workspaceDebugFile.Open( strSourcePath + "/debug/workspace.ddi", "rb" ) )
	{
		workspaceDebugFile.Close();
		DIA_Workspace::Instance().ReadDebugFile();
	}
    //$$$$$$$ NLD ??

    bool bNeedScriptParsing = false;//!MIL_AgentServer::GetWorkspace().GetConfig().UseDIAArchive();

    MT_LOG_INFO_MSG( "Initializing DIA" );
    InitializeDIATypes    ( decArchive, bNeedScriptParsing, strBinaryPath );
    InitializeDIAWorkspace( decArchive, bNeedScriptParsing, strBinaryPath );

    DEC_Tools                ::InitializeDIA();
    DEC_PopulationDecision   ::InitializeDIA();
    DEC_AutomateDecision     ::InitializeDIA();
    DEC_RolePion_Decision    ::InitializeDIA();   
    DEC_Rep_PathPoint        ::InitializeDIA();
    DEC_Rep_PathPoint_Front  ::InitializeDIA();
    DEC_Rep_PathPoint_Special::InitializeDIA();
    DEC_Rep_PathPoint_Lima   ::InitializeDIA();       
    MIL_PionMission          ::InitializeDIA();
    MIL_AutomateMission      ::InitializeDIA();
    MIL_PopulationMission    ::InitializeDIA();
    MIL_FragOrder            ::InitializeDIA();
    MIL_ParameterType_ABC    ::Initialize   ();

    InitializeMissions( config );
    InitializeModels  ( config, bNeedScriptParsing, strBinaryPath );
            
    // Finish the initialiazation of the Workspace by linking function calls
    pFuncTable_ = new DIA_FunctionTable< DEC_Workspace >();
    RegisterDIA_Functions( pFuncTable_ );
    pFunctionCaller_ = new DIA_FunctionCaller< DEC_Workspace >( *this, *pFuncTable_ );

    try
    {
        DIA_Workspace::Instance().LinkToFunctionCaller( *pFunctionCaller_ );
    }
    catch( DIA_Internal_Exception& e )
    {
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, e.GetExceptionMessage() );
    }
    
    try
    {
        DIA_Workspace::Instance().WriteDebugFile( "." ); //$$$$$$$ NLD ??
    }
    catch( std::exception& )
    {
        MT_LOG_ERROR_MSG( "Error when writing DIA debug files : DIA debugging won't work" );
    }

    // Debugger
    if( config.UseDiaDebugger() )
    {
        DIA_Workspace::Instance().RegisterDebuger( DIA_CreateDebugServer( config.GetDiaDebuggerPort(), 10 ) );
        MT_LOG_INFO_MSG( MT_FormatString( "Starting DirectIA debug server on port %d", config.GetDiaDebuggerPort() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Workspace::InitializeMissions
// Created: NLD 2006-11-24
// -----------------------------------------------------------------------------
void DEC_Workspace::InitializeMissions( MIL_Config& config )
{
    MIL_InputArchive phyArchive;
    phyArchive.Open( config.GetPhysicalFile() );

    std::string strMissionsFile;
    phyArchive.Section( "physical" );
    phyArchive.ReadField( "Missions", strMissionsFile );
    phyArchive.EndSection(); // physical

    strMissionsFile = config.BuildPhysicalChildFile( strMissionsFile );

    MIL_InputArchive missionsArchive;
    missionsArchive.Open( strMissionsFile );
    config.AddFileToCRC( strMissionsFile );

    MIL_PionMissionType      ::Initialize( missionsArchive );
    MIL_AutomateMissionType  ::Initialize( missionsArchive );
    MIL_PopulationMissionType::Initialize( missionsArchive );
    MIL_FragOrderType        ::Initialize( missionsArchive );

    missionsArchive.Close();
}

// -----------------------------------------------------------------------------
// Name: DEC_Workspace::InitializeModels
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
void DEC_Workspace::InitializeModels( MIL_Config& config, bool bNeedScriptParsing, const std::string& strBinaryPath )
{
    MIL_InputArchive phyArchive;
    phyArchive.Open( config.GetPhysicalFile() );

    std::string strModelsFile;
    phyArchive.Section( "physical" );
    phyArchive.ReadField( "Modeles", strModelsFile );
    phyArchive.EndSection(); // physical

    strModelsFile = config.BuildPhysicalChildFile( strModelsFile );

    MIL_InputArchive modelArchive;
    modelArchive.Open( strModelsFile );
    config.AddFileToCRC( strModelsFile );

    modelArchive.Section( "Modeles" );

    // Pions
    MT_LOG_INFO_MSG( "Initializing unit DIA models" );
    modelArchive.BeginList( "Pions" );
    while( modelArchive.NextListElement() )
    {
        modelArchive.Section( "Modele" );

        std::string strName;
        modelArchive.ReadAttribute( "nom", strName );

        const DEC_ModelPion*& pModel = pionModels_[ strName ];
        if( pModel )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown model name", modelArchive.GetContext() );   
        pModel = new DEC_ModelPion( *this, strName, modelArchive, bNeedScriptParsing, config.UseOnlyDIAArchive(), strBinaryPath );
        static_cast< DIA_BehaviorPart& >( pModel->GetDIAModel().GetBehaviorTool() ).RegisterInstanceEndHandlerForAllActions( &debug_ );

        modelArchive.EndSection(); // Modele
    }
    modelArchive.EndList(); // Pions

    // Automates
    MT_LOG_INFO_MSG( "Initializing automat DIA models" );
    modelArchive.BeginList( "Automates" );
    while( modelArchive.NextListElement() )
    {
        modelArchive.Section( "Modele" );

        std::string strName;
        modelArchive.ReadAttribute( "nom", strName );

        const DEC_ModelAutomate*& pModel = automateModels_[ strName ];
        if( pModel )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknwon model name", modelArchive.GetContext() );   
        pModel = new DEC_ModelAutomate( *this, strName, modelArchive, bNeedScriptParsing, config.UseOnlyDIAArchive(), strBinaryPath );
        static_cast< DIA_BehaviorPart& >( pModel->GetDIAModel().GetBehaviorTool() ).RegisterInstanceEndHandlerForAllActions( &debug_ );

        modelArchive.EndSection(); // Modele
    }
    modelArchive.EndList(); // Automates

    // Populations
    MT_LOG_INFO_MSG( "Initializing population DIA models" );
    modelArchive.BeginList( "Populations" );
    while( modelArchive.NextListElement() )
    {
        modelArchive.Section( "Modele" );

        std::string strName;
        modelArchive.ReadAttribute( "nom", strName );

        const DEC_ModelPopulation*& pModel = populationModels_[ strName ];
        if( pModel )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknwon model name", modelArchive.GetContext() );   
        pModel = new DEC_ModelPopulation( *this, strName, modelArchive, bNeedScriptParsing, config.UseOnlyDIAArchive(), strBinaryPath );
        static_cast< DIA_BehaviorPart& >( pModel->GetDIAModel().GetBehaviorTool() ).RegisterInstanceEndHandlerForAllActions( &debug_ );

        modelArchive.EndSection(); // Modele
    }
    modelArchive.EndList(); // Populations
    
    modelArchive.EndSection(); // Modeles
    modelArchive.Close();
}


//-----------------------------------------------------------------------------
// Name: DEC_Workspace::GetTime
// Created: JVT 02-06-28
//-----------------------------------------------------------------------------
float DEC_Workspace::GetTime() const
{
    return (float)MIL_AgentServer::GetWorkspace().GetSimTime();
}

//-----------------------------------------------------------------------------
// Name: DEC_Workspace::k
// Created: AGN 03-06-16
//-----------------------------------------------------------------------------
// static
bool DEC_Workspace::CheckFilesDepencies( const std::string& strArchiveFile )
{
    struct _stat binInfo;
    if( _stat( strArchiveFile.c_str(), & binInfo ) == -1 )
        return true;

    MT_FlatBinaryInputArchive needFilesArchive;
    if( ! needFilesArchive.Open( strArchiveFile ) )
        return true;

    T_StringVector::size_type nSize;
    needFilesArchive >> nSize;
    for( uint n = 0; n < nSize; ++n )
    {
        std::string strOpenFileName;
        needFilesArchive >> strOpenFileName;
        struct _stat scriptInfo;
        // We need to read script model if the types file have been has been modified
        if( (_stat( ( DIA_Workspace::Instance().GetWorkingDirectory() + strOpenFileName ).c_str(), & scriptInfo ) == -1) || ( scriptInfo.st_mtime > binInfo.st_mtime ) )
            return true;
    }
    return false;
}

//-----------------------------------------------------------------------------
// Name: GetDIAType
// Created: AGN 03-07-17
//-----------------------------------------------------------------------------
//extern
const DIA_TypeDef& GetDIAType( const std::string& strTypeName )
{
    const DIA_TypeDef* pType = static_cast< const DIA_TypeDef* >( DIA_TypeManager::Instance().GetType( strTypeName ) );
    if( pType == 0 )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, std::string( "Type '" ) + strTypeName + "' is not an existing DirectIA type." );

    return *pType;
}

//-----------------------------------------------------------------------------
// Name: InitializeDIAField
// Created: AGN 03-07-17
//-----------------------------------------------------------------------------
//extern
int InitializeDIAField( const std::string& strFieldName, const DIA_TypeDef& diaType )
{
    int nResult = DIA_TypeManager::Instance().GetFieldIdx( strFieldName, diaType );
    if( nResult == -1 )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, std::string( "Field '" ) + strFieldName + "' is not member of DirectIA type " + diaType.GetName() );

    return nResult;
}

// -----------------------------------------------------------------------------
// Name: DEC_Workspace::FindDIAModelFromScript
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
DIA_Model* DEC_Workspace::FindDIAModelFromScript( const std::string& strScriptName ) const
{
    for( CIT_ModelAutomateMap itModelAutomate = automateModels_.begin(); itModelAutomate != automateModels_.end(); ++itModelAutomate )
    {
        if( itModelAutomate->second && itModelAutomate->second->GetScriptName() == strScriptName )
            return &itModelAutomate->second->GetDIAModel();
    }
    for( CIT_ModelPionMap itModelPion = pionModels_.begin(); itModelPion != pionModels_.end(); ++itModelPion )
    {
        if( itModelPion->second && itModelPion->second->GetScriptName() == strScriptName )
            return &itModelPion->second->GetDIAModel();
    }
    for( CIT_ModelPopulationMap itModelPopulation = populationModels_.begin(); itModelPopulation != populationModels_.end(); ++itModelPopulation )
    {
        if( itModelPopulation->second && itModelPopulation->second->GetScriptName() == strScriptName )
            return &itModelPopulation->second->GetDIAModel();
    }
    return 0;
}



