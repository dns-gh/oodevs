//*****************************************************************************
//
// $Created: AGN 03-06-16 $
// $Archive: /MVW_v10/Build/SDK/SSC/src/SSC_Workspace.cpp $
// $Author: Age $
// $Modtime: 22/03/05 15:10 $
// $Revision: 18 $
// $Workfile: SSC_Workspace.cpp $
//
//*****************************************************************************

#include "SSC_Workspace.h"

#include "DIA/DIA_Tool_Script_lib.h"
#include "DIA/DIA_Tool_Archive_lib.h"
#include "DIA/DIA_DebugInfo_Generator_lib.h"

#include "MT_Tools/MT_ScipioException.h"

#include "MT_Profiler.h"
#include "MT/MT_XmlTools/MT_XmlTools_lib.h"

#include <direct.h>

#include <sys/types.h>
#include <sys/stat.h>

//-----------------------------------------------------------------------------
// Name: SSC_Workspace constructor
// Created: AGN 03-06-16
//-----------------------------------------------------------------------------
SSC_Workspace::SSC_Workspace( int argc, char** argv )
    : bGeneratesPreprocessedFiles_( false )
{
    MT_Profiler::Initialize();
    DIA_Workspace::InitializeSDK();

    ParseArguments( argc, argv );

    std::string strDecFile = GetDecFile();

    MT_XXmlInputArchive decArchive;
    if( !decArchive.Open( strDecFile ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );

    std::string strModelsDir;
    MT_ExtractFilePath( strDecFile, strModelsDir );
    std::string strInitialDir =  MT_GetCurrentDir();
    MT_ChangeDir( strModelsDir );

    Initialize( decArchive);

    decArchive.EndSection(); // Decisionnel
    MT_ChangeDir( strInitialDir );
    decArchive.Close();
}

//-----------------------------------------------------------------------------
// Name: SSC_Workspace destructor
// Created: AGN 03-06-16
//-----------------------------------------------------------------------------
SSC_Workspace::~SSC_Workspace()
{
    DIA_Workspace::TerminateSDK();
}

// -----------------------------------------------------------------------------
// Name: SSC_Workspace::ParseArguments
// Created: NLD 2004-10-01
// -----------------------------------------------------------------------------
void SSC_Workspace::ParseArguments( int argc, char** argv )
{
    for( int nArgNbr = 1; nArgNbr < argc && *argv[nArgNbr] == '-'; ++nArgNbr )
    {
        if( stricmp( argv[nArgNbr], "-outputpreprocessresult" ) == 0 )
            bGeneratesPreprocessedFiles_ = true;
    }
}

// -----------------------------------------------------------------------------
// Name: SSC_Workspace::GetDecFile
// Created: NLD 2004-10-01
// -----------------------------------------------------------------------------
std::string SSC_Workspace::GetDecFile() const
{
    MT_XXmlInputArchive scipioArchive;
    if( !scipioArchive.Open( "scipio.xml" ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, std::string( "Unable to open file 'scipio.xml'" ) );    

    if( !scipioArchive.Section( "Scipio" ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );

    if( !scipioArchive.Section( "Donnees" ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );

    std::string strFile;
    if( !scipioArchive.ReadField( "Decisionnel", strFile ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );

    scipioArchive.EndSection(); // Donnees
    scipioArchive.EndSection(); // Scipio
    scipioArchive.Close();

    return strFile;
}

//-----------------------------------------------------------------------------
// Name: SSC_Workspace::CheckFilesDepencies
// Created: AGN 03-06-16
//-----------------------------------------------------------------------------
// static
bool SSC_Workspace::CheckFilesDepencies( const std::string& strArchiveFile )
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
        if( FileChanged( strOpenFileName, binInfo.st_mtime ) )
            return true;
    }

    return false;
}

// -----------------------------------------------------------------------------
// Name: SSC_Workspace::FileChanged
// Created: AGE 2005-03-22
// -----------------------------------------------------------------------------
bool SSC_Workspace::FileChanged( const std::string& strFileName, time_t since )
{
    IT_ChangeMap itFile = changedFiles_.find( strFileName );
    if( itFile == changedFiles_.end() )
    {
        struct _stat scriptInfo;
        if( _stat( strFileName.c_str(), & scriptInfo ) == -1 )
            return true;
        itFile = changedFiles_.insert( std::make_pair( strFileName, scriptInfo.st_mtime ) ).first;
    }
    return itFile->second > since;
}

// -----------------------------------------------------------------------------
// Name: SSC_Workspace::NeedScriptParsing
// Created: NLD 2003-11-24
// -----------------------------------------------------------------------------
bool SSC_Workspace::NeedScriptParsing( bool bNeedParsing, const std::string& strArchiveName, const std::string& strFileName, const std::string& strOpenedFileArchiveName )
{
    if( bNeedParsing )
        return true;

    // Does the archive exist ?
    struct _stat binInfo;
    if( ::_stat( strArchiveName.c_str(), &binInfo ) == -1 )
        return true;

    // Does the dependency file exist ?
    MT_FlatBinaryInputArchive needFilesArchive;
    if( !needFilesArchive.Open( strOpenedFileArchiveName ) )
        return true;

    std::string strFormatedFileName;
    MT_FormatFileName( strFileName, strFormatedFileName ); //$$$NLDJVT$$ DEGUEU ... 

    // Does any of the dependent file changed ?
    T_StringVector::size_type nSize;
    needFilesArchive >> nSize;
    for( uint n = 0; n < nSize; ++n )
    {
        std::string strOpenFileName;
        needFilesArchive >> strOpenFileName;

        // The first name is the file associated to the model. if this one has changed,
        // we need to read the file again
        if( n == 0 && strOpenFileName != strFormatedFileName )
            return true;

        if( FileChanged( strOpenFileName, binInfo.st_mtime ) )
            return true;
    }

    return false;
}

// -----------------------------------------------------------------------------
// Name: SSC_Workspace::Initialize
// Created: NLD 2004-10-01
// -----------------------------------------------------------------------------
void SSC_Workspace::Initialize( MT_InputArchive_ABC& initArchive )
{
    if( !initArchive.Section( "Decisionnel" ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );

    if( !initArchive.Section( "DirectIA" ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );

    // Read the archive path output
    std::string strArchivePath;
    if( !initArchive.ReadField( "RepertoireBinaires", strArchivePath ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );
    MT_MakeDir( strArchivePath             );
    MT_MakeDir( strArchivePath + "/files"  );
    MT_MakeDir( strArchivePath + "/models" );

    std::string strSourcePath;
    if( !initArchive.ReadField( "RepertoireSources", strSourcePath ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "" );
    MT_MakeDir( strSourcePath + "/debug"  );
    DIA_Workspace::Instance().SetWorkingDirectory( strSourcePath );

    // Init DirectIA types
    std::string strDIATypesFile;
    if( !initArchive.ReadField( "DIATypes", strDIATypesFile ) ) 
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Field 'DIA::Types' doesn't exist", initArchive.RetrieveLastError()->GetInfo() );
    DIA_Workspace::Instance().RegisterDebugInfoGenerator( DIA_CreateDebugInfoGenerator( "/debug/workspace.ddi" ) );

    int nOption = eParsingOption_Default;
    if( bGeneratesPreprocessedFiles_ )
        nOption |= eParsingOption_DumpPreprocessFiles;
    DIA_SetParsingOptions( nOption );

    // Check if the script files has been modified
    std::string strArchiveTypeName = strArchivePath + "/files/type_file.bin";
    bool bNeedScriptParsing = CheckFilesDepencies( strArchiveTypeName );

    std::string errors;
    T_StringVector openedFiles;

    if( !DIA_ReadScript_Types( strDIATypesFile, errors, openedFiles ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, std::string( "Error while parsing types files" ) + strDIATypesFile, errors );

    // Updating opened files archive
    MT_FlatBinaryOutputArchive archiveFilesOut;
    archiveFilesOut << openedFiles.size();
    IT_StringVector it;
    for( it = openedFiles.begin(); it != openedFiles.end(); ++it )
        archiveFilesOut << (*it);

    archiveFilesOut.WriteToFile( strArchiveTypeName, true );

    // Init DirectIA workspace's variables
    std::string strDIAWorkspaceFile;
    if( !initArchive.ReadField( "DIAWorkspace", strDIAWorkspaceFile ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Field 'DIA::Worskpace' doesn't exist", initArchive.RetrieveLastError()->GetInfo() );

    errors = "";
    openedFiles.clear();

    std::string strArchiveWorkspaceName = strArchivePath + "/files/workspace_file.bin";

	// test if an older workspace debug file exist
	MT_File workspaceDebugFile;
	if( workspaceDebugFile.Open( strSourcePath + "/debug/workspace.ddi", "rb" ) )
	{
		workspaceDebugFile.Close();
		DIA_Workspace::Instance().ReadDebugFile();
	}

    bNeedScriptParsing = bNeedScriptParsing || CheckFilesDepencies( strArchiveWorkspaceName );

    if( !DIA_ReadScript_Workspace( strDIAWorkspaceFile, errors, openedFiles ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, std::string( "Error while parsing workspace file " ) + strDIAWorkspaceFile, errors );

    // Updating opened files archive
    MT_FlatBinaryOutputArchive archiveFilesWorkspaceOut;
    archiveFilesWorkspaceOut << openedFiles.size();
    for( it = openedFiles.begin(); it != openedFiles.end(); ++it )
        archiveFilesWorkspaceOut << (*it);

    archiveFilesWorkspaceOut.WriteToFile( strArchiveWorkspaceName, true );

    
    // Init DirectIA behavioral models
    std::string strModelsFile;
    if( ! initArchive.ReadField( "Modeles", strModelsFile ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Field 'DIA::Models' doesn't exist", initArchive.RetrieveLastError()->GetInfo() );
    InitializeModels( strModelsFile, bNeedScriptParsing, strArchivePath, strSourcePath );
    
    try
    {
        DIA_Workspace::Instance().WriteDebugFile( "." ); //$$$$$$$ NLD ??
    }
    catch( std::exception& e )
    {
        std::cerr << "Error when writing DIA debug files : DIA debugging won't work" << std::endl;
    }
    initArchive.EndSection(); // Decisionnel
}


// -----------------------------------------------------------------------------
// Name: SSC_Workspace::InitializeModels
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
void SSC_Workspace::InitializeModels( const std::string& strFile, bool bNeedScriptParsing, const std::string& strArchivePath, const std::string& strSourcePath )
{
    MT_XXmlInputArchive modelArchive;
    if( !modelArchive.Open( strFile.c_str() ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, std::string( "Unable to open file ") + strFile, modelArchive.RetrieveLastError()->GetInfo() );

    if( ! modelArchive.Section( "Modeles" ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "List 'Models' doesn't exist", modelArchive.RetrieveLastError()->GetInfo() );

    if( ! modelArchive.BeginList( "Pions" ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "List 'Models' doesn't exist", modelArchive.RetrieveLastError()->GetInfo() );   
    while( modelArchive.NextListElement() )
    {
        if( !modelArchive.Section( "Modele" ) )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "List 'Models' doesn't exist", modelArchive.RetrieveLastError()->GetInfo() );   

        std::string strName;
        if( !modelArchive.ReadAttribute( "nom", strName ))
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "List 'Models' doesn't exist", modelArchive.RetrieveLastError()->GetInfo() );   

        std::string& strScriptName = models_[ strName ];
        if( strScriptName != "" )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Model '%s' already exists", strName.c_str() ) );
        strScriptName = InitializeModel( strName, modelArchive, bNeedScriptParsing, strArchivePath, strSourcePath );
        modelArchive.EndSection(); // Modele
    }
    modelArchive.EndList(); // Pions

    if( ! modelArchive.BeginList( "Automates" ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "List 'Models' doesn't exist", modelArchive.RetrieveLastError()->GetInfo() );   
    while( modelArchive.NextListElement() )
    {
        if( !modelArchive.Section( "Modele" ) )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "List 'Models' doesn't exist", modelArchive.RetrieveLastError()->GetInfo() );   

        std::string strName;
        if( !modelArchive.ReadAttribute( "nom", strName ))
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "List 'Models' doesn't exist", modelArchive.RetrieveLastError()->GetInfo() );   
    
        std::string& strScriptName = models_[ strName ];
        if( strScriptName != "" )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Model '%s' already exists", strName.c_str() ) );
        strScriptName = InitializeModel( strName, modelArchive, bNeedScriptParsing, strArchivePath, strSourcePath );
        modelArchive.EndSection(); // Modele
    }
    modelArchive.EndList(); // Automates

    modelArchive.EndSection(); // Modeles

    modelArchive.Close();
}

// -----------------------------------------------------------------------------
// Name: SSC_Workspace::FindModelFromScript
// Created: NLD 2004-10-01
// -----------------------------------------------------------------------------
bool SSC_Workspace::FindModelFromScript( const std::string strFileName ) const
{
    for( CIT_ModelMap it = models_.begin(); it != models_.end(); ++it )
    {
        if( it->second == strFileName )
            return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: SSC_Workspace::InitializeModel
// Created: NLD 2004-10-01
// -----------------------------------------------------------------------------
std::string SSC_Workspace::InitializeModel( const std::string& strModelName, MT_InputArchive_ABC& input, bool bNeedParsing, const std::string& strArchivePath, const std::string& strSourcePath )
{
    std::cout << "\tReading model " << strModelName << " ";
    std::string strDIAType;
    if( ! input.ReadField( "DIAType", strDIAType ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Field 'Models::Model::DIAType' doesn't exist", input.RetrieveLastError()->GetInfo() );

    std::string strFileName;
    if( ! input.ReadField( "File", strFileName ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Field 'Models::Model::File' doesn't exist", input.RetrieveLastError()->GetInfo() );

    const DIA_Type* pDIAType_ = DIA_TypeManager::Instance().GetType( strDIAType );
    if( !pDIAType_ )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, std::string( "Unknown DIA Type : " ) + strDIAType );

    if( FindModelFromScript( strFileName ) )
    {
        std::cout << " already parsed" << std::endl;
        return strFileName;
    }
    DIA_Model* pDIAModel_ = new DIA_Model( *pDIAType_ );
    DIA_Workspace::Instance().RegisterModel( *pDIAModel_, strModelName );

    // tests if we need to update binary archive
    std::string strArchiveName = strArchivePath + "\\models\\";

    std::string strOpenedFileArchiveName;
    strOpenedFileArchiveName = strArchivePath + "\\files\\" + strModelName + ".bin";

    strArchiveName += strModelName + ".bin";

    if( NeedScriptParsing( bNeedParsing, strArchiveName, strSourcePath + "/" + strFileName /*$$$ n'importe quoi */, strOpenedFileArchiveName ) )
    {
        std::string strReport;
        T_StringVector openedFiles;
        openedFiles.clear();
        
        MT_Profiler profiler;
        profiler.Start();
        bool bNoError = DIA_ReadScript_Engine( *pDIAModel_, strFileName.c_str(), strReport, openedFiles );

        if( !bNoError )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, std::string( "Errors while parsing model file " ) + strFileName, strReport );

        std::cout << " FROM SCRIPT DONE in " << MT_FormatString(" %g ms", profiler.Stop() ) << std::endl;

            // Updating opened files archive
        MT_FlatBinaryOutputArchive archiveFilesOut;
        archiveFilesOut << openedFiles.size();
        for( IT_StringVector it = openedFiles.begin(); it != openedFiles.end(); ++it )
            archiveFilesOut << (*it);

        archiveFilesOut.WriteToFile( strOpenedFileArchiveName, true );

        // Updating binary archive
        MT_FlatBinaryOutputArchive archiveOut;
        archiveOut.EnableIntEncoding( true );
        DIA_Tool_Archive_Engine::WriteArchive( *pDIAModel_, archiveOut );
        DIA_Workspace::Instance().WriteModelDebugFile( *pDIAModel_, std::string( "/debug/" ) + strModelName + ".ddi" );
        archiveOut.WriteToFile( strArchiveName, true );
    }
    else
    {
        DIA_Workspace::Instance().ReadModelDebugFile( *pDIAModel_, std::string( "/debug/" ) + strModelName + ".ddi" );
        std::cout << " NOTHING TO DO" << std::endl;
    }
    return strFileName;
}
