// *****************************************************************************
//
// $Created: NLD 2003-11-24 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/DEC_Model_ABC.cpp $
// $Author: Nld $
// $Modtime: 11/04/05 14:55 $
// $Revision: 9 $
// $Workfile: DEC_Model_ABC.cpp $
//
// *****************************************************************************


#include "MIL_pch.h"
#include "DEC_Model_ABC.h"

#include "MIL_AgentServer.h"

#include "Decision/DEC_Workspace.h"

#include "DIA/DIA_Tool_Archive_lib.h"
#include "DIA/DIA_Archive_Exception.h"

#include <direct.h>
#include <sys/types.h>
#include <sys/stat.h>


// -----------------------------------------------------------------------------
// Name: DEC_Model_ABC constructor
// Created: NLD 2003-11-24
// -----------------------------------------------------------------------------
DEC_Model_ABC::DEC_Model_ABC( const DEC_Workspace& decWorkspace, const std::string& strModel, MIL_InputArchive& input, bool bNeedParsing, const std::string& strBinaryPath, const std::string& strSourcePath )
    : strModel_ ( strModel )
    , pDIAModel_( 0 )
    , pDIAType_ ( 0 )
{
    std::string strDIAType;
    input.ReadField( "DIAType", strDIAType );
    input.ReadField( "File"   , strScript_ );

    pDIAType_ = DIA_TypeManager::Instance().GetType( strDIAType );
    if( !pDIAType_ )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, std::string( "Unknown DIA Type : " ) + strDIAType );

    pDIAModel_ = decWorkspace.FindDIAModelFromScript( strScript_ );
    if( pDIAModel_ )
    {
        MT_LOG_INFO_MSG( MT_FormatString( "\tReading model %s already parsed", strModel_.c_str() ).c_str() );
        return;
    }

    pDIAModel_ = new DIA_Model( *pDIAType_ );
    DIA_Workspace::Instance().RegisterModel( *pDIAModel_, strModel_ );

    // tests if we need to update binary archive
    std::string strArchiveName           = strBinaryPath + "/models/" + strModel_ + ".bin";
    std::string strOpenedFileArchiveName = strBinaryPath + "/files/"  + strModel_ + ".bin";

    if(    !MIL_AgentServer::GetWorkspace().GetConfig().UseOnlyDIAArchive() 
        && NeedScriptParsing( bNeedParsing, strArchiveName, strSourcePath + "/" + strScript_ /*$$$ n'importe quoi */, strOpenedFileArchiveName ) )
    {
        std::string    strReport;
        T_StringVector openedFiles;

        if( !DIA_ReadScript_Engine( *pDIAModel_, strScript_.c_str(), strReport, openedFiles ) )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, std::string( "Errors while parsing model file " ) + strScript_, strReport );
        MT_LOG_INFO_MSG( MT_FormatString( "\tReading model %s FROM SCRIPT", strModel_.c_str() ).c_str() );

        // Updating opened files archive
        MT_FlatBinaryOutputArchive openedFilesArchive;
        openedFilesArchive << openedFiles.size();
        for( CIT_StringVector it = openedFiles.begin(); it != openedFiles.end(); ++it )
            openedFilesArchive << (*it);
        openedFilesArchive.WriteToFile( strOpenedFileArchiveName, true );

        // Write binary archive
        MT_FlatBinaryOutputArchive archiveOut;
        archiveOut.EnableIntEncoding( true );
        DIA_Tool_Archive_Engine::WriteArchive( *pDIAModel_, archiveOut );
        archiveOut.WriteToFile( strArchiveName, true );
        DIA_Workspace::Instance().WriteModelDebugFile( *pDIAModel_, std::string( "/debug/" ) + strModel_ + ".ddi" );        
    }
    else
    {
        // Init the model from the archive
        MT_FlatBinaryInputArchive archiveIn;
        archiveIn.EnableIntEncoding( true );
        if( ! archiveIn.Open( strArchiveName ) )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, std::string( "Errors while reading archive file " ) + strArchiveName );
        try
        {
            DIA_Tool_Archive_Engine::ReadArchive( *pDIAModel_, archiveIn );
        }
        catch ( DIA_Archive_Exception& e )
        {
            throw MT_ScipioException( "Error during model initialization.", strModel_, 0, e.GetErrorMessage() );
        }
        try
        {
            DIA_Workspace::Instance().ReadModelDebugFile( *pDIAModel_, std::string( "/debug/" ) + strModel_ + ".ddi" );
        }
        catch ( MT_Exception& e )
        {
            // NOTHING
        }
        MT_LOG_INFO_MSG( MT_FormatString( "\tReading model %s FROM ARCHIVE", strModel_.c_str() ).c_str() );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Model_ABC destructor
// Created: NLD 2003-11-24
// -----------------------------------------------------------------------------
DEC_Model_ABC::~DEC_Model_ABC()
{
    // NB : The DIA function tables are deleted in DEC_AgentXXX::TerminateDIA()
    DIA_Workspace::Instance().UnregisterModel( *pDIAModel_ );
    delete pDIAModel_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Model_ABC::NeedScriptParsing
// Created: NLD 2003-11-24
// -----------------------------------------------------------------------------
bool DEC_Model_ABC::NeedScriptParsing( bool bNeedParsing, const std::string& strArchiveName, const std::string& strFileName, const std::string& strOpenedFileArchiveName ) const
{
    if( bNeedParsing )
        return true;

    struct _stat binInfo;
    if( ::_stat( strArchiveName.c_str(), &binInfo ) == -1 )
        return true;

    // retrieve the list of the files needed by the model
    MT_FlatBinaryInputArchive needFilesArchive;
    if( !needFilesArchive.Open( strOpenedFileArchiveName ) )
        return true;

    std::string strFormatedFileName;
    MT_FormatFileName( strFileName, strFormatedFileName ); //$$$NLDJVT$$ DEGUEU ... 

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
// Name: DEC_Model_ABC::FileChanged
// Created: AGE 2005-03-22
// -----------------------------------------------------------------------------
bool DEC_Model_ABC::FileChanged( const std::string& strFileName, time_t since )
{
    static std::map< std::string, time_t > changedFiles; // $$$$ AGE 2005-03-22: ram ?

    std::map< std::string, time_t >::iterator itFile = changedFiles.find( strFileName );
    if( itFile == changedFiles.end() )
    {
        struct _stat scriptInfo;
        if( _stat( strFileName.c_str(), & scriptInfo ) == -1 )
            return true;
        itFile = changedFiles.insert( std::make_pair( strFileName, scriptInfo.st_mtime ) ).first;
    }
    return itFile->second > since;
}