
// Created: JVT 02-06-28
//*****************************************************************************

#include "simulation_kernel_pch.h"

#include "DEC_Workspace.h"

#include "DEC_Model.h"

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
#include "Decision/Functions/DEC_OrdersFunctions.h"
#include "Decision/Functions/DEC_PathFunctions.h"
#include "Decision/DEC_Tools.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/DEC_Knowledge_RapFor_ABC.h"
#include "DIA/DIA_Tool_Archive_lib.h"
#include "DIA/DIA_SDK_Manager.h"
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
    : pFuncTable_     ( 0 )
    , pFunctionCaller_( 0 )
{
    MT_LOG_INFO_MSG( "Initializing decision" );
    DIA_SDK_Manager::InitializeSDK();
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
    DIA_SDK_Manager::TerminateSDK();

    delete pFuncTable_;
    delete pFunctionCaller_;
    for( CIT_ModelMap it = automateModels_.begin(); it != automateModels_.end(); ++it )
        delete it->second;
    for( CIT_ModelMap it = pionModels_.begin(); it != pionModels_.end(); ++it )
        delete it->second;
    for( CIT_ModelMap it = populationModels_.begin(); it != populationModels_.end(); ++it )
        delete it->second;
}

//-----------------------------------------------------------------------------
// Name: DEC_Workspace::RegisterDIA_Functions
// Created: AGN 03-01-30
//-----------------------------------------------------------------------------
// static
void DEC_Workspace::RegisterDIA_Functions( DIA_FunctionTable< DEC_Workspace >* pFuncTable )
{
    // Geometry
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::SplitListPoints                     , "DEC_Geometrie_DecouperListePoints"               );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::SplitPath                           , "DEC_Geometrie_DecouperItineraire"                );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::ComputeCoverPosition                , "DEC_Geometrie_CalculerPositionCouverture"        );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::ComputeAgentsBarycenter             , "DEC_Geometrie_CalculerBarycentreAgents"          );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::TranslatePosition                   , "DEC_Geometrie_PositionTranslate"                 );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::TranslatePositionInDirection        , "DEC_Geometrie_PositionTranslateDir"              );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::ComparePositions                    , "DEC_Geometrie_PositionsEgales"                   );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::CompareDirection                    , "DEC_Geometrie_DirectionEgales"                   );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::Distance                            , "DEC_Geometrie_Distance"                          );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::ConvertPointToLocalisation          , "DEC_Geometrie_ConvertirPointEnLocalisation"      );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::IsPointInsideLocalisation           , "DEC_Geometrie_EstPointDansLocalisation"          );    
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::CompareLocalisations                , "DEC_Geometrie_LocalisationsEgales"               );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::CreateLocalisation                  , "DEC_Geometrie_CreerLocalisation"                 );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::CreateListPoint                     , "DEC_Geometrie_CreerListePoints"                  );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::CreatePoint                         , "DEC_Geometrie_CreerPoint"                        );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::CreateDirection                     , "DEC_Geometrie_CreerDirection"                    );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::CreateOrthoDirection                , "DEC_Geometrie_CreerDirectionPerpendiculaire"     );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::ReverseDirection                    , "DEC_Geometrie_InverseDirection"                  );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::RotateDirection                     , "DEC_Geometrie_RotateDirection"                   );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::CopyAndReverseDirection             , "DEC_Geometrie_CopieEtInverseDirection"           );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::CopyAndRotateDirection              , "DEC_Geometrie_CopieEtRotateDirection"            );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::ComputeDistanceFromMiddleLine       , "DEC_Geometrie_CalculerDistanceLigneMoyenne"      );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::ComputeLocalisationBarycenter       , "DEC_Geometrie_CalculerBarycentreLocalisation"    );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::ComputeMeanDirection                , "DEC_Geometrie_DirectionMoyenne"                  );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::ComputeRandomPointOnCircle          , "DEC_Geometrie_PositionAleatoireSurCercle"        );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::ComputeRandomPointInCircle          , "DEC_Geometrie_PositionAleatoireDansCercle"       );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::CreateCircleLocalisation            , "DEC_Geometrie_CreerLocalisationCercle"           );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::IsPionCoordinated                   , "DEC_Geometrie_PionEstCoordonne"                  );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::ComputeLocalisationArea             , "DEC_Geometrie_AireLocalisation"                  );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::ComputeClosedTerrainRatioInZone     , "DEC_Geometrie_PourcentageTerrainCompartimente"   );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::ComputeOpenTerrainRatioInZone       , "DEC_Geometrie_PourcentageTerrainOuvert"          );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::SortZonesAccordingToTerrainOpening  , "DEC_Geometrie_TrierZonesSelonOuvertureTerrain"   );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::IsLocalisationInFuseau              , "DEC_Geometrie_EstLocalisationDansFuseau"         );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::ConvertFuseauToLocalisation         , "DEC_Geometrie_ConvertirFuseauEnLocalisation"     );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::ComputeAutomatesBarycenter          , "DEC_Geometrie_CalculerBarycentreAutomates"       );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::GetNextObjectiveInFuseau            , "DEC_Geometrie_ProchainObjectifDansFuseau"        );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::ComputeAreaInZone                   , "DEC_Geometrie_CalculerZoneAutourPointDansFuseau" );

    // Time management
    // $$$$ AGE 2007-10-11: Un seul temps
    pFuncTable->RegisterFunction( DEC_DIAFunctions::GetSimTime         , "DEC_TempsSim"        );
    pFuncTable->RegisterFunction( DEC_DIAFunctions::GetRealTime        , "DEC_TempsReel"       );
    pFuncTable->RegisterFunction( DEC_DIAFunctions::GetTimeStepDuration, "DEC_DureePasDeTemps" );
    pFuncTable->RegisterFunction( DEC_DIAFunctions::IsNight            , "DEC_Nuit"            );

    // Parameters copy
    pFuncTable->RegisterFunction( DEC_DIAFunctions::CopyDirection                 , "DEC_Copie_Direction"                          );
    pFuncTable->RegisterFunction( DEC_DIAFunctions::CopyPoint                     , "DEC_Copie_Point"                              );
    pFuncTable->RegisterFunction( DEC_DIAFunctions::CopyListPoint                 , "DEC_Copie_ListePoints"                        );
    pFuncTable->RegisterFunction( DEC_DIAFunctions::CopyPointToListPoint          , "DEC_Copie_PointDansListePoints"               );
    pFuncTable->RegisterFunction( DEC_DIAFunctions::CopyLocalisation              , "DEC_Copie_Localisation"                       );
    pFuncTable->RegisterFunction( DEC_DIAFunctions::CopyPointToLocalisation       , "DEC_Copie_PointDansLocalisation"              );
    pFuncTable->RegisterFunction( DEC_DIAFunctions::CopyLocationList              , "DEC_Copie_ListeLocalisations"                 );
    pFuncTable->RegisterFunction( DEC_DIAFunctions::CopyLocalisationToLocationList, "DEC_Copie_LocalisationDansListeLocalisations" );

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
    pFuncTable->RegisterFunction( DEC_DIAFunctions::CreateDIAThing         , "DEC_CreerDIAThing"    );
    pFuncTable->RegisterFunction( DEC_DIAFunctions::DestroyDIAThing         , "DEC_DetruireDIAThing" );

    // Logistic
    pFuncTable->RegisterFunction( DEC_LogisticFunctions::HasWoundedHumansToEvacuate       , "DEC_NecessiteEvacuationBlesses"            );
    pFuncTable->RegisterFunction( DEC_LogisticFunctions::EvacuateWoundedHumansToTC2       , "DEC_EvacuerBlessesVersTC2"                 );
    pFuncTable->RegisterFunction( DEC_LogisticFunctions::ForbidWoundedHumansAutoEvacuation, "DEC_InterdireEvacuationAutomatiqueBlesses" );
    pFuncTable->RegisterFunction( DEC_LogisticFunctions::AllowWoundedHumansAutoEvacuation , "DEC_AutoriserEvacuationAutomatiqueBlesses" );

    // Gen objects
    pFuncTable->RegisterFunction( DEC_ObjectFunctions::GetGenObjectType             , "DEC_GenObject_Type"               );
    pFuncTable->RegisterFunction( DEC_ObjectFunctions::GetGenObjectLocalisation     , "DEC_GenObject_Localisation"       );
    pFuncTable->RegisterFunction( DEC_ObjectFunctions::GetGenObjectDensity          , "DEC_GenObject_Densite"            );
    pFuncTable->RegisterFunction( DEC_ObjectFunctions::GetGenObjectReservedObstacle , "DEC_GenObject_TypeObstacleManoeuvre" );
    pFuncTable->RegisterFunction( DEC_ObjectFunctions::GetGenObjectTC2              , "DEC_GenObject_TC2"                );
    pFuncTable->RegisterFunction( DEC_ObjectFunctions::GetGenObjectMinesActivityTime, "DEC_GenObject_DelaiActiviteMines" );

    // Objectives
    pFuncTable->RegisterFunction( DEC_ObjectFunctions::GetObjectiveLocalisation     , "DEC_Objectif_Localisation" );
    pFuncTable->RegisterFunction( DEC_ObjectFunctions::IsObjectiveFlagged           , "DEC_Objectif_EstFlage"     );
    pFuncTable->RegisterFunction( DEC_ObjectFunctions::SetObjectiveFlag             , "DEC_Objectif_Flag"         );
    
    // Objects
    pFuncTable->RegisterFunction( DEC_ObjectFunctions::ConvertTypeObjectToString    , "S_TypeObject_ToString" );

    //Rep_Points
    pFuncTable->RegisterFunction( DEC_PathFunctions::GetRepPoint     , "DEC_GetRepPoint" ); //point_
    pFuncTable->RegisterFunction( DEC_PathFunctions::IsAvantPoint    , "DEC_IsAvantPoint" ); //cls_
    pFuncTable->RegisterFunction( DEC_PathFunctions::IsPoint         , "DEC_IsPoint" ); //cls_
    pFuncTable->RegisterFunction( DEC_PathFunctions::GetTypePoint    , "DEC_GetTypePoint" ); //eType_
    pFuncTable->RegisterFunction( DEC_PathFunctions::GetDestPoint    , "DEC_GetDestPoint" ); //dest_
    pFuncTable->RegisterFunction( DEC_PathFunctions::GetTypeLimaPoint, "DEC_GetTypeLimaPoint" ); //nTypeLima_
    pFuncTable->RegisterFunction( DEC_PathFunctions::GetLimaPoint    , "DEC_GetLimaPoint" ); //lima_

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
    PHY_RolePion_Composantes::rMaxDangerosityDegradationByOpState_ = nTmp / 100.;

    xisDecisional >> xml::attribute( "max-neutralized-state", nTmp );
    if( nTmp < 0 || nTmp > 100 )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "dangerousity-modifiers: max-neutralized-state not in [0..100]" );
    DEC_Knowledge_Agent     ::rMaxDangerosityDegradationByNeutralizedState_ = nTmp / 100.;
    PHY_RolePion_Composantes::rMaxDangerosityDegradationByNeutralizedState_ = nTmp / 100.;

    xisDecisional     >> xml::end()
                          >> xml::start( "operational-state-weights" )
                              >> xml::attribute( "component", PHY_RolePion_Composantes::rOpStateWeightNonMajorComposante_ )
                              >> xml::attribute( "major-component", PHY_RolePion_Composantes::rOpStateWeightMajorComposante_ )
                              >> xml::attribute( "crew", PHY_ComposantePion::rOpStateWeightHumans_ )
                          >> xml::end();

    if( PHY_RolePion_Composantes::rOpStateWeightNonMajorComposante_ < 0 || PHY_RolePion_Composantes::rOpStateWeightNonMajorComposante_ > 1 )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "operational-state-weights: component not in [0..1]" );
    if( PHY_RolePion_Composantes::rOpStateWeightMajorComposante_ < 0 || PHY_RolePion_Composantes::rOpStateWeightMajorComposante_ > 1 )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "operational-state-weights: major-component not in [0..1]" );
    if( PHY_ComposantePion::rOpStateWeightHumans_ < 0 || PHY_ComposantePion::rOpStateWeightHumans_ > 1 )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "operational-state-weights: crew not in [0..1]" );

    if( PHY_RolePion_Composantes::rOpStateWeightMajorComposante_ + PHY_RolePion_Composantes::rOpStateWeightNonMajorComposante_ != 1. )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Sum of 'Decisionnel::EtatOps::PoidsComposantesMajeures', 'PoidsComposantesMajeures' and 'PoidsPersonnel' != 1" ); // $$$$ ABL 2007-07-25: error context

    DEC_Knowledge_RapFor_ABC::Initialize( xisDecisional );

    xisDecisional >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: DEC_Workspace::InitializeDIATypes
// Created: NLD 2005-04-11
// -----------------------------------------------------------------------------
void DEC_Workspace::InitializeDIATypes( xml::xistream& xis, bool& bNeedScriptParsing, bool bUseOnlyDIAArchive, const std::string& strBinaryPath )
{
    MT_LOG_INFO_MSG( "\tReading DIA types" );

    std::string    strErrors;
    T_StringVector openedFiles;
    std::string strScript;
    xis >> xml::content( "DIATypes", strScript );

    if( bUseOnlyDIAArchive )
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
void DEC_Workspace::InitializeDIAWorkspace( xml::xistream& xis, bool& bNeedScriptParsing, bool bUseOnlyDIAArchive, const std::string& strBinaryPath )
{
    MT_LOG_INFO_MSG( "\tReading DIA Workspace" );

    std::string    strErrors;
    T_StringVector openedFiles;
    std::string strScript;
    xis >> xml::content( "DIAWorkspace", strScript );
    if( bUseOnlyDIAArchive )
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

    xml::xifstream xis( strDecFile );
    config.AddFileToCRC( strDecFile );
    xis >> xml::start( "decisional" );
    std::string strBinaryPath, strSourcePath;
    xis >> xml::content( "RepertoireBinaires", strBinaryPath )
        >> xml::content( "RepertoireSources" , strSourcePath );
    strBinaryPath = config.BuildDecisionalChildFile( strBinaryPath );
    strSourcePath = config.BuildDecisionalChildFile( strSourcePath );

    // Force "use binary only" if the sources directory doesn't exist
    bool bUseOnlyDIAArchive = config.UseOnlyDIAArchive();
    if( !bUseOnlyDIAArchive && !boost::filesystem::exists( strSourcePath ) )
    {
        MT_LOG_INFO_MSG( "DirectIA scripts sources directory not present - trying to load binary models" );
        bUseOnlyDIAArchive = true;
    }

    DIA_Workspace::Instance().SetWorkingDirectory( strSourcePath );
    MT_LOG_INFO_MSG( MT_FormatString( "DirectIA scripts sources base directory : %s", strSourcePath.c_str() ) );
    MT_MakeDir( strBinaryPath                  );
    MT_MakeDir( strBinaryPath + "/automats"    );
    MT_MakeDir( strBinaryPath + "/units"       );
    MT_MakeDir( strBinaryPath + "/populations" );
    MT_MakeDir( strSourcePath + "/debug"       );
    MT_MakeDir( strSourcePath + "/debug/automats"    );
    MT_MakeDir( strSourcePath + "/debug/units"       );
    MT_MakeDir( strSourcePath + "/debug/populations" );    

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
    InitializeDIATypes    ( xis, bNeedScriptParsing, bUseOnlyDIAArchive, strBinaryPath );
    InitializeDIAWorkspace( xis, bNeedScriptParsing, bUseOnlyDIAArchive, strBinaryPath );

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
    InitializeModels  ( config, bNeedScriptParsing, bUseOnlyDIAArchive, strBinaryPath );

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
void DEC_Workspace::InitializeModels( MIL_Config& config, bool bNeedScriptParsing, bool bUseOnlyDIAArchive, const std::string& strBinaryPath )
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
                >> xml::list( "unit", *this, &DEC_Workspace::ReadModel, bNeedScriptParsing, bUseOnlyDIAArchive, strBinaryPath, strUnits, MIL_PionMissionType::MissionNames() )
              >> xml::end();

    // Automates
    MT_LOG_INFO_MSG( "Initializing automat DIA models" );
    xisModels >> xml::start( "automats" )
                  >> xml::list( "automat", *this, &DEC_Workspace::ReadModel, bNeedScriptParsing, bUseOnlyDIAArchive, strBinaryPath, strAutomats, MIL_AutomateMissionType::MissionNames() )
              >> xml::end();

    // Populations
    MT_LOG_INFO_MSG( "Initializing population DIA models" );
    xisModels >> xml::start( "populations" )
                  >> xml::list( "population", *this, &DEC_Workspace::ReadModel, bNeedScriptParsing, bUseOnlyDIAArchive, strBinaryPath, strPopulation, MIL_PopulationMissionType::MissionNames() )
              >> xml::end();

    xisModels >> xml::end(); // models
}

// -----------------------------------------------------------------------------
// Name: DEC_Workspace::ReadModel
// Created: RDS 2008-05-21
// -----------------------------------------------------------------------------
void DEC_Workspace::ReadModel( xml::xistream& xis, bool bNeedScriptParsing, bool bUseOnlyDIAArchive, const std::string& strBinaryPath, const std::string& strEntityType, const T_MissionTypeNameMap& missionTypes )
{
    std::string strName;
    xis >> xml::attribute( "name", strName );
    
    T_ModelMap* pModels = modelTypes_[strEntityType];
    if( !pModels )
        xis.error( "Unknown model type" );
    const DEC_Model_ABC*& pModel = (*pModels)[ strName ];
    if( pModel )
        xis.error( "Duplicate model name" );
    pModel = new DEC_Model( *this, strName, xis, bNeedScriptParsing, bUseOnlyDIAArchive, strBinaryPath, strEntityType, missionTypes );
    static_cast< DIA_BehaviorPart& >( pModel->GetDIAModel().GetBehaviorTool() ).RegisterInstanceEndHandlerForAllActions( &debug_ );
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

    tools::InputBinaryStream needFilesArchive( strArchiveFile );
    if( ! needFilesArchive )
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

// -----------------------------------------------------------------------------
// Name: DEC_Workspace::FindDIAModelFromScript
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
DIA_Model* DEC_Workspace::FindDIAModelFromScript( const std::string& strScriptName ) const
{
    for( CIT_ModelMap itModelAutomate = automateModels_.begin(); itModelAutomate != automateModels_.end(); ++itModelAutomate )
    {
        if( itModelAutomate->second && itModelAutomate->second->GetScriptName() == strScriptName )
            return &itModelAutomate->second->GetDIAModel();
    }
    for( CIT_ModelMap itModelPion = pionModels_.begin(); itModelPion != pionModels_.end(); ++itModelPion )
    {
        if( itModelPion->second && itModelPion->second->GetScriptName() == strScriptName )
            return &itModelPion->second->GetDIAModel();
    }
    for( CIT_ModelMap itModelPopulation = populationModels_.begin(); itModelPopulation != populationModels_.end(); ++itModelPopulation )
    {
        if( itModelPopulation->second && itModelPopulation->second->GetScriptName() == strScriptName )
            return &itModelPopulation->second->GetDIAModel();
    }
    return 0;
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
