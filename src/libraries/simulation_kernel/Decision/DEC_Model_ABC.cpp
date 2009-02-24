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


#include "simulation_kernel_pch.h"
#include "DEC_Model_ABC.h"
#include "Decision/DEC_Workspace.h"
#include "Entities/Orders/MIL_FragOrderType.h"
#include "DIA/DIA_Tool_Archive_lib.h"
#include "DIA/DIA_Archive_Exception.h"

#include <direct.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <xeumeuleu/xml.h>

// -----------------------------------------------------------------------------
// Name: DEC_Model_ABC constructor
// Created: NLD 2003-11-24
// -----------------------------------------------------------------------------
DEC_Model_ABC::DEC_Model_ABC( const DEC_Workspace& decWorkspace, const std::string& strModel, xml::xistream& xis, bool bNeedParsing, bool bUseOnlyArchive, const std::string& strBinaryPath, const std::string& strPrefix, const std::map< std::string, const MIL_MissionType_ABC*, sCaseInsensitiveLess >& missionTypes )
    : strModel_            ( strModel )
    , pDIAModel_           ( 0 )
    , pDIAType_            ( 0 )
{
    std::string strDIAType;
    xis >> xml::attribute( "dia-type", strDIAType )
        >> xml::attribute( "file", strScript_ );

    pDIAType_ = DIA_TypeManager::Instance().GetType( strDIAType );
    if( !pDIAType_ )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, std::string( "Unknown DIA Type : " ) + strDIAType );

    InitializeModel   ( decWorkspace, bNeedParsing, bUseOnlyArchive, strBinaryPath, strPrefix );
    InitializeMissions( xis, missionTypes );
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

        if( FileChanged( ( DIA_Workspace::Instance().GetWorkingDirectory() + strOpenFileName ).c_str(), binInfo.st_mtime ) )
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

// =============================================================================
// 
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Model_ABC::InitializeModel
// Created: NLD 2006-12-13
// -----------------------------------------------------------------------------
void DEC_Model_ABC::InitializeModel( const DEC_Workspace& decWorkspace, bool bNeedParsing, bool bUseOnlyArchive, const std::string& strBinaryPath, const std::string& strPrefix )
{
    pDIAModel_ = decWorkspace.FindDIAModelFromScript( strScript_ );
    if( pDIAModel_ )
    {
        MT_LOG_INFO_MSG( MT_FormatString( "\tReading model %s already parsed", strModel_.c_str() ).c_str() );
        return;
    }
    pDIAModel_ = new DIA_Model( *pDIAType_ );
    DIA_Workspace::Instance().RegisterModel( *pDIAModel_, strModel_ );

    // tests if we need to update binary archive
    std::string strArchiveName           = strBinaryPath + "/" + strPrefix + "/" + strModel_ + ".model";
    std::string strOpenedFileArchiveName = strBinaryPath + "/" + strPrefix + "/" + strModel_ + ".files";

    if( !bUseOnlyArchive && NeedScriptParsing( bNeedParsing, strArchiveName, strScript_, strOpenedFileArchiveName ) )
    {
        std::string    strReport;
        T_StringVector openedFiles;

        if( !DIA_ReadScript_Engine( *pDIAModel_, strScript_.c_str(), strReport, openedFiles ) )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, std::string( "Errors while parsing model file " ) + strScript_, strReport );
        MT_LOG_INFO_MSG( MT_FormatString( "\tReading model %s FROM SCRIPT", strModel_.c_str() ).c_str() );

        // Updating opened files archive
        MT_FlatBinaryOutputArchive openedFilesArchive;
        openedFilesArchive << openedFiles.size();
        for( IT_StringVector it = openedFiles.begin(); it != openedFiles.end(); ++it )
            openedFilesArchive << it->erase( 0, DIA_Workspace::Instance().GetWorkingDirectory().size() );
        openedFilesArchive.WriteToFile( strOpenedFileArchiveName, true );

        // Write binary archive
        MT_FlatBinaryOutputArchive archiveOut;
        archiveOut.EnableIntEncoding( true );
        DIA_Tool_Archive_Engine::WriteArchive( *pDIAModel_, archiveOut );
        archiveOut.WriteToFile( strArchiveName, true );
        DIA_Workspace::Instance().WriteModelDebugFile( *pDIAModel_, std::string( "/debug/" ) + strPrefix + "/" + strModel_ + ".ddi" );        
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
            DIA_Workspace::Instance().ReadModelDebugFile( *pDIAModel_, std::string( "/debug/" ) + strPrefix + "/" + strModel_ + ".ddi" );
        }
        catch ( MT_Exception& /*e*/ )
        {
            // NOTHING
        }
        MT_LOG_INFO_MSG( MT_FormatString( "\tReading model %s FROM ARCHIVE", strModel_.c_str() ).c_str() );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Model_ABC::InitializeMissions
// Created: NLD 2003-11-24
// -----------------------------------------------------------------------------
void DEC_Model_ABC::InitializeMissions( xml::xistream& xis, const std::map< std::string, const MIL_MissionType_ABC*, sCaseInsensitiveLess >& missionTypes )
{
    availableMissions_.clear();
    availableFragOrdersPerMission_.clear();

    xis >> xml::optional() >> xml::start( "missions" )
                           >> xml::list( "mission", *this, &DEC_Model_ABC::ReadMission, missionTypes )
                      >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: DEC_Model_ABC::ReadMission
// Created: ABL 2007-07-26
// -----------------------------------------------------------------------------
void DEC_Model_ABC::ReadMission( xml::xistream& xis, const std::map< std::string, const MIL_MissionType_ABC*, sCaseInsensitiveLess >& missionTypes )
{
    std::string strMission;
    xis >> xml::attribute( "name", strMission );

    std::map< std::string, const MIL_MissionType_ABC*, sCaseInsensitiveLess >::const_iterator it = missionTypes.find( strMission );
    if ( it == missionTypes.end() )
        xis.error( "Invalid mission name '" + strMission + "' for model '" + GetName() + "'" );
    const MIL_MissionType_ABC* pType = it->second;
    if( !pType )
        xis.error( "Invalid mission name '" + strMission + "' for model '" + GetName() + "'" );

	availableMissions_.insert( pType ) ;

    // Check if the DIA behavior is present
    if( !IsMissionAvailable( *pType ) )
        xis.error( "Missing behaviors for mission" );

    InitializeMissionFragOrders( xis, *pType );
}

// -----------------------------------------------------------------------------
// Name: DEC_Model_ABC::InitializeMissionFragOrders
// Created: NLD 2003-11-24
// -----------------------------------------------------------------------------
void DEC_Model_ABC::InitializeMissionFragOrders( xml::xistream& xis, const MIL_MissionType_ABC& missionType )
{
    xis >> xml::list( "fragorder", *this, &DEC_Model_ABC::ReadFragOrder, missionType );
}
// -----------------------------------------------------------------------------
// Name: DEC_Model_ABC::ReadFragOrder
// Created: ABL 2007-07-26
// -----------------------------------------------------------------------------
void DEC_Model_ABC::ReadFragOrder( xml::xistream& xis, const MIL_MissionType_ABC& missionType )
{
	std::string strOrdreConduite;
	xis >> xml::attribute( "name", strOrdreConduite );

	const MIL_FragOrderType* pType = MIL_FragOrderType::Find( strOrdreConduite );
	if( !pType )
		xis.error( "Unknown orderConduite type" );

	availableFragOrdersPerMission_[ &missionType ].insert( pType ) ; 
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Model_ABC::IsMissionAvailable
// Created: NLD 2003-11-24
// -----------------------------------------------------------------------------
bool DEC_Model_ABC::IsMissionAvailable( const MIL_MissionType_ABC& missionType ) const
{
	return availableMissions_.find( &missionType ) !=  availableMissions_.end(); 
}

// -----------------------------------------------------------------------------
// Name: DEC_Model_ABC::IsFragOrderAvailableForMission
// Created: NLD 2003-11-24
// -----------------------------------------------------------------------------
bool DEC_Model_ABC::IsFragOrderAvailableForMission( const MIL_MissionType_ABC& missionType, const MIL_FragOrderType& fragOrderType ) const
{
	if( fragOrderType.IsAvailableForAllMissions() )
        return true;

    CIT_FragOrderPerMissionMap it = availableFragOrdersPerMission_.find( &missionType );
    if( it == availableFragOrdersPerMission_.end() )
        return false;

	return it->second.find( &fragOrderType ) != it->second.end();
}

// -----------------------------------------------------------------------------
// Name: DEC_Model_ABC::GetName
// Created: NLD 2003-11-24
// -----------------------------------------------------------------------------
const std::string& DEC_Model_ABC::GetName() const
{
    return strModel_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Model_ABC::GetDIAModel
// Created: NLD 2003-11-24
// -----------------------------------------------------------------------------
DIA_Model& DEC_Model_ABC::GetDIAModel() const
{
    assert( pDIAModel_ );
    return *pDIAModel_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Model_ABC::GetScriptName
// Created: AGN 2004-03-23
// -----------------------------------------------------------------------------
const std::string& DEC_Model_ABC::GetScriptName() const
{
    return strScript_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Model_ABC::GetDIAType
// Created: AGN 2004-03-23
// -----------------------------------------------------------------------------
const DIA_Type& DEC_Model_ABC::GetDIAType() const
{
    assert( pDIAType_ != 0 );
    return *pDIAType_;
}


