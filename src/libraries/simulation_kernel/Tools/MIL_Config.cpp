// *****************************************************************************
//
// $Created: NLD 2003-11-21 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Tools/MIL_Config.cpp $
// $Author: Nld $
// $Modtime: 15/04/05 17:05 $
// $Revision: 6 $
// $Workfile: MIL_Config.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_Config.h"
#include "MIL_Tools.h"

// -----------------------------------------------------------------------------
// Name: MIL_Config constructor
// Created: NLD 2003-12-04
// -----------------------------------------------------------------------------
MIL_Config::MIL_Config()
    : nExerciceID_                 ( 0 )
    , bUseOnlyDIAArchive_          ( false )
    , nNbrPathFindThreads_         ( 1 )
    , bForceODBAutomateComposition_( false )
    , strCheckPointFileName_       ()
    , strCheckPointODBFileName_    ()
    , bUseDecDebug_                ( false )
    , bUseDiaDebugServer_          ( false )
    , bProfilingEnabled_           ( false )
    , bUseCheckPointCRC_           ( true )
    , bUsePathDebug_               ( false )
    , strConfFile_                 ( "scipio.xml" )
    , strRootDirectory_            ( MT_GetCurrentDir() )
    , bDataTestMode_               ( false )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_Config destructor
// Created: NLD 2003-12-04
// -----------------------------------------------------------------------------
MIL_Config::~MIL_Config()
{
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::AddFileToCRC
// Created: JVT 2005-04-07
// -----------------------------------------------------------------------------
void MIL_Config::AddFileToCRC( const std::string& fileName )
{
    if ( CRCMap_.find( fileName ) == CRCMap_.end() )
        CRCMap_[ fileName ] = MIL_Tools::ComputeCRC( fileName );
}

// -----------------------------------------------------------------------------
// Name: MIL_Config::serialize
// Created: JVT 2005-03-22
// -----------------------------------------------------------------------------
boost::crc_32_type::value_type MIL_Config::serialize( const std::string& strFileName ) const
{
    MT_XXmlOutputArchive archive;
    
    if ( !archive.Section( "Fichiers" ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Can't writer section 'Fichiers'", archive.RetrieveLastError()->GetInfo() );

    for ( CIT_CRCMap it = CRCMap_.begin(); it != CRCMap_.end(); ++it )
        if ( !archive.Section( "Fichier" ) 
          || !archive.WriteAttribute( "nom", it->first ) 
          || !archive.Write( it->second ) 
          || !archive.EndSection()
           )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Can't create section '%s'", it->first.c_str() ), archive.RetrieveLastError()->GetInfo() );

    archive.EndSection(); // Fichiers
    
    if ( !archive.WriteToFile( strFileName ) )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Can't create file '%s'", strFileName.c_str() ), archive.RetrieveLastError()->GetInfo() );
    
    return MIL_Tools::ComputeCRC( strFileName );
}