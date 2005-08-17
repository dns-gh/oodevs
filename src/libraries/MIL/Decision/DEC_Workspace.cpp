//*****************************************************************************
// Created: JVT 02-06-28
//*****************************************************************************

#include "MIL_Pch.h"

#include "DEC_Workspace.h"

#include "DEC_ModelPion.h"
#include "DEC_ModelAutomate.h"

#include "Entities/Automates/DEC_AutomateDecision.h"
#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Orders/Pion/MIL_PionMissionType.h"
#include "Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "Entities/Orders/Conduite/MIL_OrderConduiteType.h"
#include "Decision/Knowledge/DEC_Rep_PathPoint.h"
#include "Decision/Knowledge/DEC_Rep_PathPoint_Front.h"
#include "Decision/Knowledge/DEC_Rep_PathPoint_Special.h"
#include "Decision/Knowledge/DEC_Rep_PathPoint_Lima.h"
#include "Decision/Genie/DEC_Gen_Object.h"
#include "Decision/Functions/DEC_GeometryFunctions.h"
#include "Decision/Functions/DEC_DIAFunctions.h"
#include "Decision/Functions/DEC_LogisticFunctions.h"
#include "Decision/DEC_Tools.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "DIA/DIA_Tool_Archive_lib.h"
#include "DIA/DIA_SDK_Manager.h"
#include "MT_Tools/MT_ArchiveDirectoryHelper.h"

#include <sys/stat.h>

//-----------------------------------------------------------------------------
// Name: DEC_Workspace constructor
// Created: JVT 02-06-28
// Last modified: JVT 02-09-16
//-----------------------------------------------------------------------------
DEC_Workspace::DEC_Workspace( MIL_InputArchive& archive )
    : pFuncTable_     ( 0 )
    , pFunctionCaller_( 0 )
{
    MT_LOG_INFO_MSG( "Initializing decision" );
    DIA_SDK_Manager::InitializeSDK();
   
    std::string strFile;
    archive.ReadField( "Decisionnel", strFile );

    MIL_InputArchive decArchive;
    decArchive.AddWarningStream( std::cout );
    decArchive.Open( strFile );

    MIL_AgentServer::GetWorkspace().GetConfig().AddFileToCRC( strFile );
    
    MT_ArchiveDirectoryHelper directoryChanger( strFile );

    decArchive.Section( "Decisionnel" );

    InitializeConfig( decArchive );
    InitializeDIA   ( decArchive );

    decArchive.EndSection(); // Decisionnel

    decArchive.Close();
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
}

//-----------------------------------------------------------------------------
// Name: DEC_Workspace::RegisterDIA_Functions
// Created: AGN 03-01-30
//-----------------------------------------------------------------------------
// static
void DEC_Workspace::RegisterDIA_Functions( DIA_FunctionTable< DEC_Workspace >* pFuncTable )
{
    // Geometry
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::SplitListPoints               , "DEC_Geometrie_DecouperListePoints"           );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::SplitPath                     , "DEC_Geometrie_DecouperItineraire"            );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::ComputeCoverPosition          , "DEC_Geometrie_CalculerPositionCouverture"    );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::ComputeAgentsBarycenter       , "DEC_Geometrie_CalculerBarycentreAgents"      );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::TranslatePosition             , "DEC_Geometrie_PositionTranslate"             );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::TranslatePositionInDirection  , "DEC_Geometrie_PositionTranslateDir"          );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::ComparePositions              , "DEC_Geometrie_PositionsEgales"               );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::CompareDirection              , "DEC_Geometrie_DirectionEgales"               );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::Distance                      , "DEC_Geometrie_Distance"                      );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::ConvertPointToLocalisation    , "DEC_Geometrie_ConvertirPointEnLocalisation"  );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::IsPointInsideLocalisation     , "DEC_Geometrie_EstPointDansLocalisation"      );    
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::CompareLocalisations          , "DEC_Geometrie_LocalisationsEgales"           );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::CreateLocalisation            , "DEC_Geometrie_CreerLocalisation"             );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::DeleteLocalisation            , "DEC_Geometrie_DetruireLocalisation"          );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::DeleteListLocalisation        , "DEC_Geometrie_DetruireListeLocalisations"    );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::CreateListPoint               , "DEC_Geometrie_CreerListePoints"              );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::DeleteListPoint               , "DEC_Geometrie_DetruireListePoints"           );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::CreatePoint                   , "DEC_Geometrie_CreerPoint"                    );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::DeletePoint                   , "DEC_Geometrie_DetruirePoint"                 );     
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::CreateDirection               , "DEC_Geometrie_CreerDirection"                );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::CreateOrthoDirection          , "DEC_Geometrie_CreerDirectionPerpendiculaire" );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::ReverseDirection              , "DEC_Geometrie_InverseDirection"              );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::RotateDirection               , "DEC_Geometrie_RotateDirection"               );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::CopyAndReverseDirection       , "DEC_Geometrie_CopieEtInverseDirection"       );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::CopyAndRotateDirection        , "DEC_Geometrie_CopieEtRotateDirection"        );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::DeleteDirection               , "DEC_Geometrie_DetruireDirection"             );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::ComputeDistanceFromMiddleLine , "DEC_Geometrie_CalculeDistanceLigneMoyenne"   );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::ComputeLocalisationBarycenter , "DEC_Geometrie_CalculeBarycentreLocalisation" );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::ComputeMeanDirection          , "DEC_Geometrie_DirectionMoyenne"              );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::ComputeRandomPointOnCircle    , "DEC_Geometrie_PositionAleatoireSurCercle"    );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::ComputeRandomPointInCircle    , "DEC_Geometrie_PositionAleatoireDansCercle"   );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::CreateCircleLocalisation      , "DEC_Geometrie_CreerLocalisationCercle"       );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::IsPionCoordinated             , "DEC_Geometrie_PionEstCoordonne"              );
    pFuncTable->RegisterFunction( DEC_GeometryFunctions::ComputeLocalisationArea       , "DEC_Geometrie_AireLocalisation"              );
    
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
    pFuncTable->RegisterFunction( DEC_DIAFunctions::UserTypeList_Size    , "DEC_UserTypeList_Size"      );
    pFuncTable->RegisterFunction( DEC_DIAFunctions::UserTypeList_Contains, "DEC_UserTypeList_Contient"  );
    pFuncTable->RegisterFunction( DEC_DIAFunctions::UserTypeList_PushBack, "DEC_UserTypeList_PushBack"  );
    pFuncTable->RegisterFunction( DEC_DIAFunctions::UserTypeList_Remove  , "DEC_UserTypeList_Remove"    );
    pFuncTable->RegisterFunction( DEC_DIAFunctions::UserTypeList_Delete  , "DEC_UserTypeList_DeleteAll" );
    pFuncTable->RegisterFunction( DEC_DIAFunctions::UserTypeList_Clear   , "DEC_UserTypeList_Clear"     );
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

    // Debug
    pFuncTable->RegisterFunction( DEC_DIAFunctions::PointToString    , "DEC_PointToString"      );
    pFuncTable->RegisterFunction( DEC_DIAFunctions::DirectionToString, "DEC_DirectionToString"  );
    pFuncTable->RegisterFunction( DEC_DIAFunctions::PathToString     , "DEC_ItineraireToString" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Workspace::InitializeConfig
// Created: NLD 2004-06-14
// -----------------------------------------------------------------------------
void DEC_Workspace::InitializeConfig( MIL_InputArchive& archive )
{
    // Dangerosite
    archive.Section( "Dangerosite" );

    uint nTmp;
    archive.ReadField( "DegradationMaxParPertinence", nTmp, CheckValueBound( 0, 100 ) );
    DEC_Knowledge_Agent::rMaxDangerosityDegradationByRelevance_ = nTmp / 100.;
    
    archive.ReadField( "DegradationMaxParEtatOps", nTmp, CheckValueBound( 0, 100 ) );
    DEC_Knowledge_Agent      ::rMaxDangerosityDegradationByEtatOps_ = nTmp / 100.;
    PHY_RolePion_Composantes::rMaxDangerosityDegradationByEtatOps_ = nTmp / 100.;

    archive.ReadField( "DegradationMaxParEtatNeutralise", nTmp, CheckValueBound( 0, 100 ) );
    DEC_Knowledge_Agent      ::rMaxDangerosityDegradationByNeutralizedState_ = nTmp / 100.;
    PHY_RolePion_Composantes::rMaxDangerosityDegradationByNeutralizedState_ = nTmp / 100.;

    archive.EndSection(); // Dangerosite

    // EtatOpsEtatOps
    archive.Section( "EtatOps" );

    archive.ReadField( "PoidsComposantesNonMajeures", PHY_RolePion_Composantes::rEtatOpsWeightNonMajorComposante_, CheckValueBound( 0., 1. ) );
    archive.ReadField( "PoidsComposantesMajeures"   , PHY_RolePion_Composantes::rEtatOpsWeightMajorComposante_   , CheckValueBound( 0., 1. ) );

    // $$$ Optionnel pour compat v0.110.x
    if( !archive.ReadField( "PoidsPersonnel", PHY_RolePion_Composantes::rEtatOpsWeightHumans_, CheckValueBound( 0., 1. ), MIL_InputArchive::eThrow, MIL_InputArchive::eNothing ) )
        PHY_RolePion_Composantes::rEtatOpsWeightHumans_ = 1. - ( PHY_RolePion_Composantes::rEtatOpsWeightMajorComposante_ + PHY_RolePion_Composantes::rEtatOpsWeightNonMajorComposante_ );

    if( PHY_RolePion_Composantes::rEtatOpsWeightHumans_ + PHY_RolePion_Composantes::rEtatOpsWeightMajorComposante_ + PHY_RolePion_Composantes::rEtatOpsWeightNonMajorComposante_ != 1. )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Sum of 'Decisionnel::EtatOps::PoidsComposantesMajeures', 'PoidsComposantesMajeures' and 'PoidsPersonnel' != 1", archive.GetContext() );
    archive.EndSection(); // EtatOps

    DEC_Knowledge_RapFor_ABC::Initialize( archive );
}


// -----------------------------------------------------------------------------
// Name: DEC_Workspace::InitializeDIATypes
// Created: NLD 2005-04-11
// -----------------------------------------------------------------------------
void DEC_Workspace::InitializeDIATypes( MIL_InputArchive& initArchive, bool& bNeedScriptParsing, const std::string& strBinaryPath, const std::string& /*strSourcePath*/ )
{
    MT_LOG_INFO_MSG( "\tReading DIA types" );

    std::string strScript;
    initArchive.ReadField( "DIATypes", strScript );

    // Check if the script files has been modified
    std::string strOpenedFileArchiveName = strBinaryPath + "/files/type_file.bin";
    bNeedScriptParsing = bNeedScriptParsing || CheckFilesDepencies( strOpenedFileArchiveName );

    std::string    strErrors;
    T_StringVector openedFiles;
    if( !DIA_ReadScript_Types( strScript, strErrors, openedFiles ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, std::string( "Error while parsing types files" ) + strScript, strErrors );

    // Updating opened files archive
    MT_FlatBinaryOutputArchive openedFilesArchive;
    openedFilesArchive << openedFiles.size();
    for( CIT_StringVector it = openedFiles.begin(); it != openedFiles.end(); ++it )
        openedFilesArchive << (*it);
    openedFilesArchive.WriteToFile( strOpenedFileArchiveName, true );
}

// -----------------------------------------------------------------------------
// Name: DEC_Workspace::InitializeDIAWorkspace
// Created: NLD 2005-04-11
// -----------------------------------------------------------------------------
void DEC_Workspace::InitializeDIAWorkspace( MIL_InputArchive& initArchive, bool& bNeedScriptParsing, const std::string& strBinaryPath, const std::string& /*strSourcePath*/ )
{ 
    MT_LOG_INFO_MSG( "\tReading DIA Workspace" );

    std::string strScript;
    initArchive.ReadField( "DIAWorkspace", strScript );

    std::string strOpenedFileArchiveName = strBinaryPath + "/files/workspace_file.bin";
    bNeedScriptParsing = bNeedScriptParsing || CheckFilesDepencies( strOpenedFileArchiveName );

    std::string    strErrors;
    T_StringVector openedFiles;
    if( !DIA_ReadScript_Workspace( strScript, strErrors, openedFiles ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, std::string( "Error while parsing workspace file " ) + strScript, strErrors );

    // Updating opened files archive
    MT_FlatBinaryOutputArchive openedFilesArchive;
    openedFilesArchive << openedFiles.size();
    for( CIT_StringVector it = openedFiles.begin(); it != openedFiles.end(); ++it )
        openedFilesArchive << (*it);
    openedFilesArchive.WriteToFile( strOpenedFileArchiveName, true );

    // Write binary archive
    MT_FlatBinaryOutputArchive archiveOut;
    archiveOut.EnableIntEncoding( true );
    DIA_Tool_Archive_Engine::WriteWorkspaceInArchive( archiveOut );
    archiveOut.WriteToFile( strBinaryPath + "/models/workspace.bin", true );
}

//-----------------------------------------------------------------------------
// Name: DEC_Workspace::InitializeDIA
// Created: JVT 02-07-08
// Last modified: JVT 03-11-24
//-----------------------------------------------------------------------------
void DEC_Workspace::InitializeDIA( MIL_InputArchive& initArchive )
{
    initArchive.Section( "DirectIA" );

    // Read the archive path output
    std::string strBinaryPath;
    std::string strSourcePath;
    initArchive.ReadField( "RepertoireBinaires", strBinaryPath );
    initArchive.ReadField( "RepertoireSources" , strSourcePath );
    MT_MakeDir( strBinaryPath             );
    MT_MakeDir( strBinaryPath + "/files"  );
    MT_MakeDir( strBinaryPath + "/models" );
    MT_MakeDir( strSourcePath + "/debug"  );
    DIA_Workspace::Instance().SetWorkingDirectory( strSourcePath );
    std::string strTmp;
    MT_FormatDirName( MT_GetCurrentDir() + "/" + strSourcePath, strTmp );
    MT_LOG_INFO_MSG( MT_FormatString( "DirectIA scripts sources base directory : %s", strTmp.c_str() ) );

    DIA_Workspace::Instance().RegisterDebugInfoGenerator( DIA_CreateDebugInfoGenerator( "/debug/workspace.ddi" ) );
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

    bool bNeedScriptParsing = !MIL_AgentServer::GetWorkspace().GetConfig().UseDIAArchive();

    InitializeDIATypes    ( initArchive, bNeedScriptParsing, strBinaryPath, strSourcePath );
    InitializeDIAWorkspace( initArchive, bNeedScriptParsing, strBinaryPath, strSourcePath );
    InitializeModels      ( initArchive, bNeedScriptParsing, strBinaryPath, strSourcePath );

    DEC_Tools                ::InitializeDIA();
    DEC_AutomateDecision     ::InitializeDIA();
    DEC_RolePion_Decision    ::InitializeDIA();   
    DEC_Rep_PathPoint        ::InitializeDIA();
    DEC_Rep_PathPoint_Front  ::InitializeDIA();
    DEC_Rep_PathPoint_Special::InitializeDIA();
    DEC_Rep_PathPoint_Lima   ::InitializeDIA();       
    DEC_Gen_Object           ::InitializeDIA(); 
    MIL_PionMissionType      ::InitializeDIA();
    MIL_AutomateMissionType  ::InitializeDIA();
    MIL_OrderConduiteType    ::InitializeDIA();   
            
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
    
    DIA_Workspace::Instance().WriteDebugFile( "." ); //$$$$$$$ NLD ??

    // Debugger
    uint nDebuggerPort;
    initArchive.ReadField( "PortDebogueur", nDebuggerPort );
    nDebuggerPort += MIL_AgentServer::GetWorkspace().GetExerciceID();

    if( MIL_AgentServer::GetWorkspace().GetConfig().UseDiaDebugServer() )
    {
        DIA_Workspace::Instance().RegisterDebuger( DIA_CreateDebugServer( (uint16)nDebuggerPort ) );
        MT_LOG_INFO_MSG( MT_FormatString( "Starting DirectIA debug server at port %d", nDebuggerPort ) );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Workspace::InitializeModels
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
void DEC_Workspace::InitializeModels( MIL_InputArchive& initArchive, bool bNeedScriptParsing, const std::string& strBinaryPath, const std::string& strSourcePath )
{
    std::string strModelsFile;
    initArchive.ReadField( "Modeles", strModelsFile );

    MIL_InputArchive modelArchive;
    modelArchive.AddWarningStream( std::cout );
    modelArchive.Open( strModelsFile );

    modelArchive.Section( "Modeles" );
    modelArchive.BeginList( "Pions" );
    while( modelArchive.NextListElement() )
    {
        modelArchive.Section( "Modele" );

        std::string strName;
        modelArchive.ReadAttribute( "nom", strName );

        const DEC_ModelPion*& pModel = pionModels_[ strName ];
        if( pModel )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown model name", modelArchive.GetContext() );   
        pModel = new DEC_ModelPion( *this, strName, modelArchive, bNeedScriptParsing, strBinaryPath, strSourcePath );
        static_cast< DIA_BehaviorPart& >( pModel->GetDIAModel().GetBehaviorTool() ).RegisterInstanceEndHandlerForAllActions( &debug_ );

        modelArchive.EndSection(); // Modele
    }
    modelArchive.EndList(); // Pions

    modelArchive.BeginList( "Automates" );

    while( modelArchive.NextListElement() )
    {
        modelArchive.Section( "Modele" );

        std::string strName;
        modelArchive.ReadAttribute( "nom", strName );

        const DEC_ModelAutomate*& pModel = automateModels_[ strName ];
        if( pModel )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknwon model name", modelArchive.GetContext() );   
        pModel = new DEC_ModelAutomate( *this, strName, modelArchive, bNeedScriptParsing, strBinaryPath, strSourcePath );
        static_cast< DIA_BehaviorPart& >( pModel->GetDIAModel().GetBehaviorTool() ).RegisterInstanceEndHandlerForAllActions( &debug_ );

        modelArchive.EndSection(); // Modele
    }
    modelArchive.EndList(); // Automates
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
// Name: DEC_Workspace::CheckFilesDepencies
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
        if( (_stat( strOpenFileName.c_str(), & scriptInfo ) == -1) || ( scriptInfo.st_mtime > binInfo.st_mtime ) )
            return true;;
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
    return 0;
}
