// *****************************************************************************
//
// $Created: NLD 2003-11-24 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/DEC_Model_ABC.inl $
// $Author: Nld $
// $Modtime: 11/04/05 14:37 $
// $Revision: 2 $
// $Workfile: DEC_Model_ABC.inl $
//
// *****************************************************************************

#include "Decision/DEC_Workspace.h"
#include "Entities/Orders/MIL_FragOrderType.h"
#include "DIA/DIA_Tool_Archive_lib.h"
#include "DIA/DIA_Archive_Exception.h"

#include <direct.h>
#include <sys/types.h>
#include <sys/stat.h>



// -----------------------------------------------------------------------------
// Name: DEC_Model_ABC constructor
// Created: NLD 2003-11-24
// -----------------------------------------------------------------------------
template< typename T >
DEC_Model_ABC< T >::DEC_Model_ABC( const DEC_Workspace& decWorkspace, const std::string& strModel, xml::xistream& xis, bool bNeedParsing, bool bUseOnlyArchive, const std::string& strBinaryPath, const std::string& strPrefix )
    : strModel_            ( strModel )
    , pDIAModel_           ( 0 )
    , pDIAType_            ( 0 )
    , strScript_           ()
    , missionBitset_       ()
    , fragOrdersPerMission_()
{
    std::string strDIAType;
    xis >> xml::attribute( "dia-type", strDIAType )
        >> xml::attribute( "file", strScript_ );

    pDIAType_ = DIA_TypeManager::Instance().GetType( strDIAType );
    if( !pDIAType_ )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, std::string( "Unknown DIA Type : " ) + strDIAType );

    InitializeModel   ( decWorkspace, bNeedParsing, bUseOnlyArchive, strBinaryPath, strPrefix );
    InitializeMissions( xis );
}

// -----------------------------------------------------------------------------
// Name: DEC_Model_ABC destructor
// Created: NLD 2003-11-24
// -----------------------------------------------------------------------------
template< typename T >
DEC_Model_ABC< T >::~DEC_Model_ABC()
{
    // NB : The DIA function tables are deleted in DEC_AgentXXX::TerminateDIA()
    DIA_Workspace::Instance().UnregisterModel( *pDIAModel_ );
    delete pDIAModel_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Model_ABC< T >::NeedScriptParsing
// Created: NLD 2003-11-24
// -----------------------------------------------------------------------------
template< typename T >
bool DEC_Model_ABC< T >::NeedScriptParsing( bool bNeedParsing, const std::string& strArchiveName, const std::string& strFileName, const std::string& strOpenedFileArchiveName ) const
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
// Name: DEC_Model_ABC< T >::FileChanged
// Created: AGE 2005-03-22
// -----------------------------------------------------------------------------
template< typename T >
bool DEC_Model_ABC< T >::FileChanged( const std::string& strFileName, time_t since )
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
// Name: DEC_Model_ABC< T >::InitializeModel
// Created: NLD 2006-12-13
// -----------------------------------------------------------------------------
template< typename T >
void DEC_Model_ABC< T >::InitializeModel( const DEC_Workspace& decWorkspace, bool bNeedParsing, bool bUseOnlyArchive, const std::string& strBinaryPath, const std::string& strPrefix )
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
// Name: DEC_Model_ABC< T >::InitializeMissions
// Created: NLD 2003-11-24
// -----------------------------------------------------------------------------
template< typename T >
void DEC_Model_ABC< T >::InitializeMissions( xml::xistream& xis )
{
    missionBitset_.clear();
    fragOrdersPerMission_.clear();

    xis >> xml::optional() >> xml::start( "missions" )
                           >> xml::list( "mission", *this, &DEC_Model_ABC::ReadMission )
                      >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: DEC_Model_ABC::ReadMission
// Created: ABL 2007-07-26
// -----------------------------------------------------------------------------
template< typename T >
void DEC_Model_ABC< T >::ReadMission( xml::xistream& xis )
{
    std::string strMission;
    xis >> xml::attribute( "name", strMission );

    const T* pType = T::Find( strMission );
    if ( !pType )
        xis.error( "Invalid mission name '" + strMission + "' for model '" + GetName() + "'" );

    if ( missionBitset_.size() < pType->GetID() + 1 )
        missionBitset_.resize( pType->GetID() + 1, false );
    missionBitset_[ pType->GetID() ] = true;

    // Check if the DIA behavior is present
    if( !IsMissionAvailable( *pType ) )
        xis.error( "Missing behaviors for mission" );

    InitializeMissionFragOrders( xis, *pType );
}

// -----------------------------------------------------------------------------
// Name: DEC_Model_ABC< T >::InitializeMissionFragOrders
// Created: NLD 2003-11-24
// -----------------------------------------------------------------------------
template< typename T >
void DEC_Model_ABC< T >::InitializeMissionFragOrders( xml::xistream& xis, const T& missionType )
{
    xis >> xml::list( "fragorder", *this, &DEC_Model_ABC::ReadFragorder, missionType );
}
// -----------------------------------------------------------------------------
// Name: DEC_Model_ABC::ReadFragorder
// Created: ABL 2007-07-26
// -----------------------------------------------------------------------------
template< typename T >
void DEC_Model_ABC< T >::ReadFragorder( xml::xistream& xis, const T& missionType )
{
        std::string strOrdreConduite;
        xis >> xml::attribute( "name", strOrdreConduite );

        const MIL_FragOrderType* pType = MIL_FragOrderType::Find( strOrdreConduite );
        if( !pType )
            xis.error( "Unknown orderConduite type" );

        T_IDBitVector& bitVector = fragOrdersPerMission_[ &missionType ];
        if( bitVector.size() < pType->GetID() + 1 )
            bitVector.resize( pType->GetID() + 1, false );
        bitVector[ pType->GetID() ] = true;  
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Model_ABC< T >::IsMissionAvailable
// Created: NLD 2003-11-24
// -----------------------------------------------------------------------------
template< typename T >
bool DEC_Model_ABC< T >::IsMissionAvailable( const MIL_MissionType_ABC& missionType ) const
{
    if( missionType.GetID() >= missionBitset_.size() || !missionBitset_[ missionType.GetID() ] )
        return false;
    return missionType.IsAvailableForModel( GetDIAModel() );
}

// -----------------------------------------------------------------------------
// Name: DEC_Model_ABC< T >::IsFragOrderAvailableForMission
// Created: NLD 2003-11-24
// -----------------------------------------------------------------------------
template< typename T >
bool DEC_Model_ABC< T >::IsFragOrderAvailableForMission( const MIL_MissionType_ABC& missionType, const MIL_FragOrderType& fragOrderType ) const
{
    if( fragOrderType.IsAvailableForAllMissions() )
        return true;

    CIT_FragOrderPerMissionMap it = fragOrdersPerMission_.find( &missionType );
    if( it == fragOrdersPerMission_.end() )
        return false;

    return fragOrderType.GetID() < it->second.size() && (it->second)[ fragOrderType.GetID() ];    
}

// -----------------------------------------------------------------------------
// Name: DEC_Model_ABC< T >::GetName
// Created: NLD 2003-11-24
// -----------------------------------------------------------------------------
template< typename T > inline
const std::string& DEC_Model_ABC< T >::GetName() const
{
    return strModel_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Model_ABC< T >::GetDIAModel
// Created: NLD 2003-11-24
// -----------------------------------------------------------------------------
template< typename T > inline
DIA_Model& DEC_Model_ABC< T >::GetDIAModel() const
{
    assert( pDIAModel_ );
    return *pDIAModel_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Model_ABC< T >::GetScriptName
// Created: AGN 2004-03-23
// -----------------------------------------------------------------------------
template< typename T > inline
const std::string& DEC_Model_ABC< T >::GetScriptName() const
{
    return strScript_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Model_ABC< T >::GetDIAType
// Created: AGN 2004-03-23
// -----------------------------------------------------------------------------
template< typename T > inline
const DIA_Type& DEC_Model_ABC< T >::GetDIAType() const
{
    assert( pDIAType_ != 0 );
    return *pDIAType_;
}
